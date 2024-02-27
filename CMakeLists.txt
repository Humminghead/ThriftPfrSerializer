cmake_minimum_required(VERSION 3.5)

project(tpfrserializer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##
## OPTIONS
##
option(TPFR_Install           "Install the library." ${TPFR_Install_INIT})
option(TPFR_BuildExamples     "Build the examples." ${TPFR_BuildExamples_INIT})

##
## COFIG
##
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(TPFR_CONFIG_INSTALL_DIR        "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(TPFR_CMAKE_VERSION_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(TPFR_INCLUDE_BUILD_DIR         "${PROJECT_SOURCE_DIR}/include/")
set(TPFR_INCLUDE_INSTALL_DIR       "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
set(TPFR_CMAKE_VERSION_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(TPFR_CMAKE_PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
if (TPFR_SystemInclude)
    set(TPFR_SYSTEM_INCLUDE "SYSTEM")
endif()
set(TPFR_PKGCONFIG_INSTALL_DIR     "${CMAKE_INSTALL_DATADIR}/pkgconfig")

# Create configuration files
configure_package_config_file( "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                               "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                               INSTALL_DESTINATION
                               ${TPFR_CONFIG_INSTALL_DIR}/cmake
                               PATH_VARS TPFR_INCLUDE_INSTALL_DIR TPFR_CONFIG_INSTALL_DIR
                               )

##
## TARGET
##
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} ${TPFR_SYSTEM_INCLUDE} INTERFACE
    $<BUILD_INTERFACE:${TPFR_INCLUDE_BUILD_DIR}>
    $<INSTALL_INTERFACE:${TPFR_INCLUDE_INSTALL_DIR}>
    )

# Install a pkg-config file, so other tools can find this.
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
)

##
## EXAMPLES
##
if (TPFR_BuildExamples)
    add_subdirectory(example)
endif()

##
## INSTALL
## install header files, generate and install cmake config files for find_package()
##
include(CMakePackageConfigHelpers)
# use a custom package version config file instead of
# write_basic_package_version_file to ensure that it's architecture-independent
configure_file(
    "cmake/${PROJECT_NAME}ConfigVersion.cmake.in"
    ${TPFR_CMAKE_VERSION_CONFIG_FILE}
    @ONLY
)

if(TPFR_Install)
    install(
        DIRECTORY ${TPFR_INCLUDE_BUILD_DIR}
        DESTINATION ${TPFR_INCLUDE_INSTALL_DIR}
    )
    install(
        FILES ${TPFR_CMAKE_PROJECT_CONFIG_FILE} ${TPFR_CMAKE_VERSION_CONFIG_FILE}
        DESTINATION ${TPFR_CONFIG_INSTALL_DIR}
    )
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION ${TPFR_PKGCONFIG_INSTALL_DIR}
    )
endif()
