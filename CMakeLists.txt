# 
# Stirshaken AS & VS
#
cmake_minimum_required (VERSION 3.8)

include(ExternalProject)

project ("stirshaken-asvs-server")

SET (__WITH_BOOST OFF)
SET (__WITH_PROXYGEN OFF)
SET (__WITH_PQXX OFF)

SET(LOAD_BOOST_SOURCES ON)
SET(LOAD_PROXYGEN_SOURCES ON)
SET(LOAD_PQXX_SOURCES ON)


# ********************** Boost *********************
	set(V_BOOST 1.74.0)
	set(V_BOOST_ 1_74_0)
	set(BOOST_PREFIX boost)

if (${LOAD_BOOST_SOURCES})
	ExternalProject_Add(boost
		PREFIX ${BOOST_PREFIX}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/${BOOST_PREFIX}
		URL https://dl.bintray.com/boostorg/release/${V_BOOST}/source/${BOOST_PREFIX}_${V_BOOST_}.tar.gz
		CONFIGURE_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${BOOST_PREFIX} && ./bootstrap.sh"
		BUILD_COMMAND ""
		INSTALL_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${BOOST_PREFIX} && ./b2 -j4 -d1 --without-python --without-graph_parallel --without-mpi --prefix=/usr/local install"
	)
	SET (__WITH_BOOST ON)
elseif (${BUILD_BOOST})
	ExternalProject_Add(boost
		PREFIX ${BOOST_PREFIX}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/${BOOST_PREFIX}
		CONFIGURE_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${BOOST_PREFIX} && ./bootstrap.sh"
		BUILD_COMMAND ""
		INSTALL_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${BOOST_PREFIX} && ./b2 -j4 -d1 --without-python --without-graph_parallel --without-mpi --prefix=/usr/local install"
	SET (__WITH_BOOST ON)
	)
endif()

# ********************** Proxygen *********************
	set(V_PROXYGEN 2020.11.16.00)
	set(PROXYGEN_PREFIX proxygen)

if (${LOAD_PROXYGEN_SOURCES})
	ExternalProject_Add(proxygen
		PREFIX ${PROXYGEN_PREFIX}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}
		URL https://github.com/facebook/proxygen/archive/v${V_PROXYGEN}.tar.gz 
		PATCH_COMMAND sed s/\-DCMAKE_INSTALL_PREFIX=\"\$DEPS_DIR\"/\-DCMAKE_INSTALL_PREFIX=\"\$PREFIX\"/ < ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen/build.sh > ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen/b.sh 
		CONFIGURE_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen && chmod +x b.sh && ./b.sh -j 4 --prefix /usr/local"
		BUILD_COMMAND ""
		INSTALL_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen && ./install.sh"
	)
	SET (__WITH_PROXYGEN ON)
elseif (${BUILD_PROXYGEN})
	ExternalProject_Add(proxygen
		PREFIX ${PROXYGEN_PREFIX}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}
		PATCH_COMMAND sed s/\-DCMAKE_INSTALL_PREFIX=\"\$DEPS_DIR\"/\-DCMAKE_INSTALL_PREFIX=\"\$PREFIX\"/ < ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen/build.sh > ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen/b.sh 
		CONFIGURE_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen && chmod +x b.sh && ./b.sh -j 4 --prefix /usr/local"
		BUILD_COMMAND ""
		INSTALL_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PROXYGEN_PREFIX}/proxygen && ./install.sh"
	)
	SET (__WITH_PROXYGEN ON)
endif()

# ********************** PQXX *********************
	set(V_PQXX 6.4.5)
	set(PQXX_PREFIX pqxx)

if (${LOAD_PQXX_SOURCES})
	ExternalProject_Add(pqxx
		PREFIX ${PQXX_PREFIX}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/${PQXX_PREFIX}
		URL https://github.com/jtv/libpqxx/archive/${V_PQXX}.tar.gz
		CONFIGURE_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PQXX_PREFIX} && mkdir -p release && cd release && cmake ../"
		BUILD_COMMAND ""
		INSTALL_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PQXX_PREFIX}/release && make -j4 install"
	)
	SET (__WITH_PQXX ON)
elseif (${BUILD_PQXX})
	ExternalProject_Add(pqxx
		PREFIX ${PQXX_PREFIX}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/${PQXX_PREFIX}
		CONFIGURE_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PQXX_PREFIX} && mkdir -p release && cd release && cmake ../"
		BUILD_COMMAND ""
		INSTALL_COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}/${PQXX_PREFIX}/release && make -j4 install"
	SET (__WITH_PQXX ON)
	)
endif()

add_subdirectory ("server")

if (${__WITH_BOOST})
	add_dependencies(server boost)
	if (${__WITH_PROXYGEN})
		add_dependencies(proxygen boost)
	endif()
endif()

if (${__WITH_PROXYGEN})
	add_dependencies(server proxygen)
endif()

if (${__WITH_PQXX})
	add_dependencies(server pqxx)
endif()

