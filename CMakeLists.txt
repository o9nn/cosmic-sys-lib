cmake_minimum_required(VERSION 3.14)
project(cosmic-system VERSION 1.2.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(COSMIC_BUILD_EXAMPLES "Build example programs" ON)
option(COSMIC_BUILD_TESTS "Build test programs" ON)
option(COSMIC_BUILD_SHARED "Build shared library" OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Library sources
set(COSMIC_SOURCES
    src/system.cpp
    src/geometry.cpp
    src/operations.cpp
    src/terms.cpp
)

# Library headers
set(COSMIC_HEADERS
    include/cosmic/cosmic.hpp
    include/cosmic/system.hpp
    include/cosmic/geometry.hpp
    include/cosmic/operations.hpp
    include/cosmic/terms.hpp
    include/cosmic/trees.hpp
    include/cosmic/system1.hpp
    include/cosmic/system2.hpp
)

# Create library
if(COSMIC_BUILD_SHARED)
    add_library(cosmic SHARED ${COSMIC_SOURCES})
else()
    add_library(cosmic STATIC ${COSMIC_SOURCES})
endif()

# Include directories
target_include_directories(cosmic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set library properties
set_target_properties(cosmic PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${COSMIC_HEADERS}"
)

# Examples
if(COSMIC_BUILD_EXAMPLES)
    add_executable(basic_usage examples/basic_usage.cpp)
    target_link_libraries(basic_usage PRIVATE cosmic)
    
    add_executable(terms_demo examples/terms_demo.cpp)
    target_link_libraries(terms_demo PRIVATE cosmic)
    
    add_executable(oeis_demo examples/oeis_demo.cpp)
    target_link_libraries(oeis_demo PRIVATE cosmic)
    
    add_executable(system1_system2_demo examples/system1_system2_demo.cpp)
    target_link_libraries(system1_system2_demo PRIVATE cosmic)
endif()

# Tests
if(COSMIC_BUILD_TESTS)
    enable_testing()
    
    add_executable(test_system tests/test_system.cpp)
    target_link_libraries(test_system PRIVATE cosmic)
    add_test(NAME SystemTests COMMAND test_system)
    
    add_executable(test_geometry tests/test_geometry.cpp)
    target_link_libraries(test_geometry PRIVATE cosmic)
    add_test(NAME GeometryTests COMMAND test_geometry)
    
    add_executable(test_operations tests/test_operations.cpp)
    target_link_libraries(test_operations PRIVATE cosmic)
    add_test(NAME OperationsTests COMMAND test_operations)
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS cosmic
    EXPORT cosmic-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cosmic
)

install(EXPORT cosmic-targets
    FILE cosmic-targets.cmake
    NAMESPACE cosmic::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cosmic
)

# Package configuration
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cosmic-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cosmic-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cosmic
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cosmic-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cosmic-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cosmic-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cosmic
)

# pkg-config
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cosmic.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/cosmic.pc
    @ONLY
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cosmic.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# Summary
message(STATUS "")
message(STATUS "Cosmic System Library ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build examples: ${COSMIC_BUILD_EXAMPLES}")
message(STATUS "  Build tests: ${COSMIC_BUILD_TESTS}")
message(STATUS "  Build shared: ${COSMIC_BUILD_SHARED}")
message(STATUS "")
