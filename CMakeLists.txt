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
set(DEPS_BIN_DIR ${DEPS}/bin)
set(DEPS_SBIN_DIR ${DEPS}/sbin)

SET(V_PROXYGEN 2020.11.16.00)
SET(V_PQXX 7.7.3)


ExternalProject_Add(proxygen-prj
	PREFIX ${DEPS}
	URL https://github.com/facebook/proxygen/archive/v${V_PROXYGEN}.tar.gz
	URL_HASH SHA256=be617ce7ff8af12ab2008e5f49c2b0375b22919591cbae751c149d5de4c88200
	SOURCE_DIR ${DEPS}/src/proxygen-src
	DOWNLOAD_NO_PROGRESS true
	UPDATE_COMMAND ""
	PATCH_COMMAND sed s/\-DCMAKE_INSTALL_PREFIX=\"\$DEPS_DIR\"/\-DCMAKE_INSTALL_PREFIX=\"\$PREFIX\"/ < ${DEPS}/src/proxygen-src/proxygen/build.sh > ${DEPS}/src/proxygen-src/proxygen/b.sh
	CONFIGURE_COMMAND  chmod +x ${DEPS}/src/proxygen-src/proxygen/b.sh
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/proxygen-src/proxygen ./b.sh -j 4 --prefix ${DEPS}
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${DEPS}/src/proxygen-src/proxygen ./install.sh
)

# ********************** PQXX *********************
ExternalProject_Add(pqxx-prj
	PREFIX ${DEPS}
	URL https://github.com/jtv/libpqxx/archive/${V_PQXX}.tar.gz
	URL_HASH SHA256=11e147bbe2d3024d68d29b38eab5d75899dbb6131e421a2dbf9f88bac9bf4b0d
	SOURCE_DIR ${DEPS}/src/pqxx-src
	DOWNLOAD_NO_PROGRESS true
	UPDATE_COMMAND ""
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS}
)

include_directories(BEFORE ${DEPS_INCLUDE_DIR})
add_subdirectory ("server")

add_dependencies(server proxygen-prj pqxx-prj)
