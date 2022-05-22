#
# Stirshaken AS & VS
#
cmake_minimum_required (VERSION 3.8)

include(ExternalProject)

project ("stirshaken-asvs-server")

# Various locations for external projects
set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/deps CACHE STRING "Dependencies' folder'")
set(DEPS_INCLUDE_DIR ${DEPS}/include)
set(DEPS_LIB_DIR ${DEPS}/lib)
set(DEPS_LIB_DIR_64 ${DEPS}/lib64)
set(DEPS_BIN_DIR ${DEPS}/bin)
set(DEPS_SBIN_DIR ${DEPS}/sbin)
set(DEPS_PKG_DIR ${DEPS_LIB_DIR}/pkgconfig:${DEPS_LIB_DIR_64}/pkgconfig:/usr/local/lib:/usr/local/lib64)

SET(V_PROXYGEN 2020.11.16.00)
# V_PQXX 7.7.3  		# HASH 11e147bbe2d3024d68d29b38eab5d75899dbb6131e421a2dbf9f88bac9bf4b0d
SET(V_PQXX 6.4.5)		# HASH 86921fdb0fe54495a79d5af2c96f2c771098c31e9b352d0834230fd2799ad362
SET(V_JWT 1.13.1)
SET(V_JANSSON 2.14)
SET(V_KS 1.8.0)

execute_process( COMMAND "awk" "-F=" "/^NAME/{print $2}" "/etc/os-release"
				 RESULT_VARIABLE OSTYPE_RES OUTPUT_VARIABLE OSTYPE_TXT )
if(OSTYPE_RES EQUAL 0)
	if("${OSTYPE_TXT}" MATCHES "\"Ubuntu\"")
		message(STATUS "Adding Ubuntu compile definitions")
		set(IS_UBUNTU ON)
	elseif("${OSTYPE_TXT}" MATCHES "\"CentOS Linux\"")
		message(STATUS "Adding CentOS compile definitions")
		set(IS_UBUNTU OFF)
	else()
		message(FATAL_ERROR "Unsupported OSTYPE: ${OSTYPE}")
	endif()
else(OSTYPE_RES EQUAL 0)
	message(FATAL_ERROR "Failed to detect OSTYPE: ${OSTYPE_TXT}")
endif(OSTYPE_RES EQUAL 0)


ExternalProject_Add(proxygen-prj
	PREFIX ${DEPS}
	URL https://github.com/facebook/proxygen/archive/v${V_PROXYGEN}.tar.gz
	URL_HASH SHA256=be617ce7ff8af12ab2008e5f49c2b0375b22919591cbae751c149d5de4c88200
	SOURCE_DIR ${DEPS}/src/proxygen-src
	DOWNLOAD_NO_PROGRESS true
	UPDATE_COMMAND ""
	PATCH_COMMAND sed s/\-DCMAKE_INSTALL_PREFIX=\"\$DEPS_DIR\"/\-DCMAKE_INSTALL_PREFIX=\"\$PREFIX\"/ < ${DEPS}/src/proxygen-src/proxygen/build.sh > ${DEPS}/src/proxygen-src/proxygen/b.sh
	CONFIGURE_COMMAND  chmod +x ${DEPS}/src/proxygen-src/proxygen/b.sh
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/proxygen-src/proxygen ${CMAKE_COMMAND} -E env BOOST_INCLUDEDIR=${DEPS_INCLUDE_DIR} BOOST_LIBRARYDIR=${DEPS_LIB_DIR} ./b.sh -j 4 --prefix ${DEPS}
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/proxygen-src/proxygen ./install.sh
)

ExternalProject_Add(pqxx-prj
	PREFIX ${DEPS}
	URL https://github.com/jtv/libpqxx/archive/${V_PQXX}.tar.gz
	URL_HASH SHA256=86921fdb0fe54495a79d5af2c96f2c771098c31e9b352d0834230fd2799ad362
	SOURCE_DIR ${DEPS}/src/pqxx-src
	DOWNLOAD_NO_PROGRESS true
	UPDATE_COMMAND ""
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS}
)

ExternalProject_Add(jansson-prj
	PREFIX ${DEPS}
	URL https://github.com/akheron/jansson/releases/download/v${V_JANSSON}/jansson-${V_JANSSON}.tar.gz
	URL_HASH SHA256=5798d010e41cf8d76b66236cfb2f2543c8d082181d16bc3085ab49538d4b9929
	SOURCE_DIR ${DEPS}/src/jansson-src
	DOWNLOAD_NO_PROGRESS true
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/jansson-src ./configure --prefix=${DEPS}
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/jansson-src make
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/jansson-src make install
)

ExternalProject_Add(jwt-prj
	PREFIX ${DEPS}
	URL https://github.com/benmcollins/libjwt/archive/refs/tags/v${V_JWT}.tar.gz
	URL_HASH SHA256=4df55ac89c6692adaf3badb43daf3241fd876612c9ab627e250dfc4bb59993d9
	SOURCE_DIR ${DEPS}/src/jwt-src
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/jwt-src autoreconf -i
	COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/jwt-src ${CMAKE_COMMAND} -E env PKG_CONFIG_PATH=${DEPS_PKG_DIR} ./configure --prefix=${DEPS} 
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/jwt-src make
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/jwt-src make install
)

ExternalProject_Add(ks-prj
	PREFIX ${DEPS}
	GIT_REPOSITORY https://github.com/signalwire/libks.git
	GIT_TAG v${V_KS}
	SOURCE_DIR ${DEPS}/src/ks-src
	UPDATE_COMMAND ""
	PATCH_COMMAND sed -i s/__PRETTY_FUNCTION__/__func__/g ${DEPS}/src/ks-src/src/include/libks/ks_debug.h
	      COMMAND sed -i s/__PRETTY_FUNCTION__/__func__/g ${DEPS}/src/ks-src/src/include/libks/ks_json.h
	      COMMAND sed -i s/add_subdirectory\(tests\)//g ${DEPS}/src/ks-src/CMakeLists.txt
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/ks-src cmake . -DCMAKE_INSTALL_PREFIX=${DEPS} -DKS_STATIC:BOOL=ON
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/ks-src make
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/ks-src make install
)

set(KS_LIBS "-L/home/maxirmx/Projects/asvs/deps/lib -l:libks.a -luuid")

ExternalProject_Add(stirshaken-prj
	PREFIX ${DEPS}
	GIT_REPOSITORY https://github.com/signalwire/libstirshaken.git
	GIT_TAG 85bf303
	SOURCE_DIR ${DEPS}/src/stirshaken-src
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/stirshaken-src autoreconf -i
	COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/stirshaken-src ${CMAKE_COMMAND} -E env PKG_CONFIG_PATH=${DEPS_PKG_DIR} CPPFLAGS=-I${DEPS_INCLUDE_DIR} KS_LIBS=${KS_LIBS} ./configure --prefix=${DEPS} 
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/stirshaken-src make
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/stirshaken-src make install
)

include_directories(BEFORE ${DEPS_INCLUDE_DIR})
add_subdirectory ("server")

add_dependencies(jwt-prj jansson-prj)
add_dependencies(stirshaken-prj ks-prj jwt-prj)
add_dependencies(server stirshaken-prj proxygen-prj pqxx-prj)
