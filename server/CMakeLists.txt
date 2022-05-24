cmake_minimum_required (VERSION 3.8)

project (server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS -Wall)
set(BOOST_USE_STATIC_LIBS ON)
set(BOOST_USE_MULTITHREADED ON)
set(BOOST_USE_STATIC_RUNTIME OFF)

link_directories(/usr/local/lib)

find_package( Boost 1.71 COMPONENTS context  date_time  REQUIRED )
find_package( Threads                                   REQUIRED )

find_library(Z                  z                       REQUIRED )
find_library(LZMA               lzma                    REQUIRED )
find_library(BZ2                bz2                     REQUIRED )
find_library(ZSTD               zstd                    REQUIRED )
find_library(LZ4                lz4                     REQUIRED )

find_library(D                  double-conversion       REQUIRED )
find_library(DL                 dl                      REQUIRED )
find_library(SODIUM             sodium                  REQUIRED )
find_library(EVENT              event                   REQUIRED )
find_library(JE                 jemalloc                REQUIRED )
find_library(SNAPPY             snappy                  REQUIRED )
find_library(IBERTY             iberty                  REQUIRED )

find_library (GFLAGS            gflags                  REQUIRED )
find_library (GLOG              glog                    REQUIRED )

find_library (UUID              uuid                    REQUIRED )
find_library (CURL              curl                    REQUIRED )


if(${IS_UBUNTU})
        find_library(SSL        ssl     REQUIRED )
        find_library(CRYPTO     crypto  REQUIRED )
        find_library(PQ         pq      REQUIRED)
else (${IS_UBUNTU})
        find_library(SSL        ssl     REQUIRED        HINTS /lib64)
        find_library(CRYPTO     crypto  REQUIRED        HINTS /lib64)
        find_library(PQ         pq      REQUIRED        HINTS /usr/pgsql-10/lib)
endif(${IS_UBUNTU})

set(LIBS
        ${Boost_LIBRARIES}
# ---
        ${PQ}
# ---
        ${GLOG} ${GFLAGS} ${SSL} ${Z} ${EVENT} ${CRYPTO} ${CMAKE_THREAD_LIBS_INIT}
        ${SSL} ${CRYPTO} ${D} ${SNAPPY} ${IBERTY} ${JE} ${DL} ${SODIUM} ${Z} ${LZ4} 
        ${LZMA} ${BZ2} ${ZSTD} ${UUID} ${CURL}
)

add_executable (${PROJECT_NAME}
                "server.cpp"
                "server.h"
                "ProvisioningAPI.cpp"
                "ProvisioningAPI.h"
                "spDataObject.h"
                "spDataObject.cpp"
                "AsVsHttpAPI.cpp"
                "Util.h"
                "DbConnection.h"
                "DbConnection.cpp"
                "spColumn.h"
                "spBaseObject.h"
                "spBaseObject.cpp"
                "StirShakenAPI.h"
                "StirShakenAPI.cpp"
)

function(add_external_library NAME)
	add_library(${NAME} STATIC IMPORTED)
       	set_target_properties(${NAME} PROPERTIES IMPORTED_LOCATION ${DEPS_LIB_DIR}/lib${NAME}.a)
	target_link_libraries(${PROJECT_NAME} ${NAME})
endfunction()

function(add_external_library_64 NAME)
	add_library(${NAME} STATIC IMPORTED)
        set_target_properties(${NAME} PROPERTIES IMPORTED_LOCATION ${DEPS_LIB_DIR_64}/lib${NAME}.a)
	target_link_libraries(${PROJECT_NAME} ${NAME})
endfunction()

add_external_library(proxygenhttpserver)
add_external_library(proxygen)
add_external_library(wangle)
add_external_library(fizz)
add_external_library(folly)

add_external_library(stirshaken)
add_external_library(jwt)
add_external_library(jansson)
add_external_library(ks)

if(${IS_UBUNTU})
        add_external_library(pqxx)
else(${IS_UBUNTU})
        add_external_library_64(pqxx)
#        add_external_library(ssl)
#        add_external_library(crypto)
endif(${IS_UBUNTU})

target_link_libraries(${PROJECT_NAME} ${LIBS})
