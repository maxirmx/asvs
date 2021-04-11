# CMakeList.txt: проект CMake для server; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project (server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS -Wall)
set(BOOST_USE_STATIC_LIBS ON)
set(BOOST_USE_MULTITHREADED ON)
set(BOOST_USE_STATIC_RUNTIME OFF)

link_directories(/usr/local/lib)

find_package( Boost 1.74 COMPONENTS context             REQUIRED ) 
find_package( Threads                                   REQUIRED )


find_library(Z                  z                       REQUIRED )
find_library(SSL                ssl                     REQUIRED )
find_library(CRYPTO             crypto                  REQUIRED )
find_library(LZMA               lzma                    REQUIRED )
find_library(BZ2                bz2                     REQUIRED )
find_library(ZSTD               zstd                    REQUIRED )

find_library(D                  double-conversion       REQUIRED )
find_library(DL                 dl                      REQUIRED )
find_library(SODIUM             sodium                  REQUIRED )
find_library(EVENT              event                   REQUIRED )
find_library(JE                 jemalloc                REQUIRED )
find_library(SNAPPY             snappy                  REQUIRED )


find_library (GFLAGS            gflags                  REQUIRED )
find_library (GLOG              glog                    REQUIRED )

find_library(PROXYGEN           proxygen                REQUIRED )
find_library(PROXYGENHTTPSERVER proxygenhttpserver      REQUIRED )
find_library(WANGLE             wangle                  REQUIRED )
find_library(FOLLY              folly                   REQUIRED )
find_library(FIZZ               fizz                    REQUIRED )

find_library(PQ                 pq                      REQUIRED )
find_library(PQXX               pqxx                    REQUIRED )

set(LIBS
        ${Boost_LIBRARIES}
#		 ${Boost_CONTEXT_LIBRARY}
#        ${Boost_DATE_TIME_LIBRARY}
#        ${Boost_REGEX_LIBRARY}
# ---
        ${PQ}
        ${PQXX}
# ---
        ${PROXYGENHTTPSERVER}
        ${WANGLE}
        ${GLOG}
        ${GFLAGS}
        ${SSL}
        ${Z}
        ${EVENT}
        ${CRYPTO}
        ${CMAKE_THREAD_LIBS_INIT}
        ${D}
        ${SNAPPY}
        ${JE}
        ${DL}
        ${PROXYGEN}
        ${FIZZ}
        ${SODIUM}
        ${Z}
        ${FOLLY}
        ${LZMA}
        ${BZ2}
        ${ZSTD}
)

add_executable (${PROJECT_NAME} "server.cpp" "server.h" "ProvisioningAPI.cpp" "ProvisioningAPI.h" "CustomerDb.h" "CustomerDb.cpp" "AccountDb.cpp" "AccountDb.h" "AsVsHttpAPI.cpp" "Util.h" "DbConnection.h" "DbConnection.cpp")
target_link_libraries(${PROJECT_NAME} ${LIBS})

