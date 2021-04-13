// server.h 

#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/uuid.hpp>

#include <chrono>
#include <sstream>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <folly/portability/GFlags.h>
#include <folly/portability/Unistd.h>

#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <pqxx/pqxx>

#include "Util.h"
#include "CustomerDb.h"
#include "AccountDb.h"
#include "DbConnection.h"

