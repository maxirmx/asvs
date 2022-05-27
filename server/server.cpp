// server.cpp: определяет точку входа для приложения.
//

#include "server.h"

using namespace std;
using namespace proxygen;

using folly::SocketAddress;

#include "ProvisioningAPI.h"
#include "AsVsHttpAPI.h"
#include "StirShakenAPI.h"

DEFINE_string(prov_api_http_ip, "localhost", "Provisioning API IP/Hostname");
DEFINE_int32(prov_api_http_port, 8080, "Provisioning API port");

DEFINE_string(asvs_api_http_ip, "localhost", "AS&VS HTTP API IP/Hostname");
DEFINE_int32(asvs_api_http_port, 8081, "AS&VS HTTP API port");

DEFINE_string(postgres, "", "PostgresSql connection string (\"user=postgres host=localhost port=5432 dbname=asvs password=postgres\")");

DEFINE_int32(threads,0, "Number of threads used by AS&VS HTTP API. Zero means to default to the number of cores on this machine.");

int main(int argc, char* argv[])
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  // .................... Database connection ....................

  if (FLAGS_postgres.empty())
      LOG(INFO) << "Database connection string has not been provided. We will try defaults but the result can be disappointing." << std::endl;

   try
  {
      DbConnection::d = make_unique<DbConnection>(FLAGS_postgres);
      LOG(INFO) << DbConnection::d->describe();
      DbInMemory::d = make_unique<DbInMemory>();
      DbInMemory::d->loadDb();
      StirShakenAPI::api->initialize();

   }
  catch (const StirShakenException& e)
  {
      LOG(ERROR) << "stirshaken initialization error " << e.which() << "'" << e.what() << "'" << endl;
      return -1;
  }
  catch (const pqxx::failure& e)
  {
      if (FLAGS_postgres.empty())
          LOG(ERROR) << "Database connection using default parameters failed." << endl;
      else
          LOG(ERROR) << "Database connection defined by --postgres \"" << FLAGS_postgres << "\" failed." << endl;
      LOG(ERROR) << "The engine said:" << endl << e.what() << endl;
      return -1;
  }
  catch (InternalError& e)
  {
      if (FLAGS_postgres.empty())
          LOG(ERROR) << "Database connection using default parameters failed." << endl;
      else
          LOG(ERROR) << "Database connection defined by --postgres \"" << FLAGS_postgres << "\" failed." << endl;
      LOG(ERROR) << e.what() << endl;
      return -1;
  }

  // ................. Provisioining API startup .................

  LOG(INFO) << "Binding provisioning API to: " << FLAGS_prov_api_http_ip << ":" << FLAGS_prov_api_http_port << endl;
  vector<HTTPServer::IPConfig> prov_api_defs =
  {
    {
          SocketAddress(FLAGS_prov_api_http_ip, FLAGS_prov_api_http_port, true),
          HTTPServer::Protocol::HTTP
    }
  };

  HTTPServerOptions prov_api_options;
  prov_api_options.threads = 1;
  prov_api_options.idleTimeout = chrono::milliseconds(60000);
  prov_api_options.shutdownOn = { SIGINT, SIGTERM };
  prov_api_options.enableContentCompression = false;
  prov_api_options.handlerFactories =
      RequestHandlerChain()
      .addThen<ProvApiHandlerFactory>()
      .build();
  // Increase the default flow control to 1MB/10MB
  prov_api_options.initialReceiveWindow = uint32_t(1 << 20);
  prov_api_options.receiveStreamWindowSize = uint32_t(1 << 20);
  prov_api_options.receiveSessionWindowSize = 10 * (1 << 20);
  prov_api_options.h2cEnabled = true;

  HTTPServer prov_api_server(std::move(prov_api_options));
  prov_api_server.bind(prov_api_defs);
  thread prov_api_thread([&]() { prov_api_server.start(); });
  LOG(INFO) << "Provisioning API started" << std::endl;

// ................. AS & VS HTTP API startup .................

  LOG(INFO) << "Binding AS&VS HTTP API to:   " << FLAGS_asvs_api_http_ip << ":" << FLAGS_asvs_api_http_port << endl;

  if (FLAGS_threads <= 0)
  {
      FLAGS_threads = sysconf(_SC_NPROCESSORS_ONLN);
      CHECK(FLAGS_threads > 0);
  }

  vector<HTTPServer::IPConfig> asvs_api_defs =
  {
    {
          SocketAddress(FLAGS_asvs_api_http_ip, FLAGS_asvs_api_http_port, true),
          HTTPServer::Protocol::HTTP
    }
  };

  HTTPServerOptions asvs_api_options;
  asvs_api_options.threads = static_cast<size_t>(FLAGS_threads);
  asvs_api_options.idleTimeout = chrono::milliseconds(60000);
  asvs_api_options.enableContentCompression = false;
  asvs_api_options.handlerFactories =
      RequestHandlerChain()
      .addThen<AsVsApiHandlerFactory>()
      .build();
  // Increase the default flow control to 1MB/10MB
  asvs_api_options.initialReceiveWindow = uint32_t(1 << 20);
  asvs_api_options.receiveStreamWindowSize = uint32_t(1 << 20);
  asvs_api_options.receiveSessionWindowSize = 10 * (1 << 20);
  asvs_api_options.h2cEnabled = true;

  HTTPServer asvs_api_server(std::move(asvs_api_options));
  asvs_api_server.bind(asvs_api_defs);
  thread asvs_api_thread([&]() { asvs_api_server.start(); });
  LOG(INFO) << "AS&VS HTTP API started using " << FLAGS_threads << " threads" << std::endl;

    // ....................... Shutdown .......................

  prov_api_thread.join();
  asvs_api_server.stop();
  asvs_api_thread.join();

  LOG(INFO) << "Server shutdown" << std::endl;
  return 0;
  }