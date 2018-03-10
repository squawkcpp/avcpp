cmake_minimum_required(VERSION 3.3)

project(avcpp)

SET(MAJOR_VERSION "0")
SET(MINOR_VERSION "1")
SET(PATCH_VERSION "1" CACHE PATCH_VERSION "The package build number." )

# -----------------------   Options and Parameters  -----------------------

option(build_tests "Build all unit tests." OFF)
option(build_samples "Build samples." OFF)
option(build_documentation "Build the project documentation." OFF)

SET(DOCFILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/" CACHE PATH "The path to the documentation files.")
SET(TESTFILES "/testfiles" CACHE TESTFILES "The path to the testfiles.")

SET(LIBAVCPP_VERSION "master" CACHE STRING "The tagged version.")

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

if(!BUILD_SHARED_LIBS)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(CMAKE_EXE_LINKER_FLAGS "-static")
endif(BUILD_SHARED_LIBS)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

add_definitions(-DTESTFILES="${TESTFILES}/")


# ---------------------   External Dependencies       ---------------------
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory ( av )
if( build_samples )
    add_subdirectory ( samples )
endif()
if( build_tests )
    add_subdirectory ( test )
endif()

# -----------------------   Doxygen                 -----------------------
if (build_documentation)
    add_custom_target(cppcheck /usr/bin/cppcheck -DHTTP_BUFFER_SIZE=${HTTP_BUFFER_SIZE} -DHTTP_CLIENT_TIMEOUT=${HTTP_CLIENT_TIMEOUT} -DHTTP_SERVER_THREAD_POOL_SIZE=${HTTP_SERVER_THREAD_POOL_SIZE} --enable=all --inconclusive --xml --xml-version=2 ${HeaderFiles} 2> cppcheck.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating code analysis with CppCheck" VERBATIM
    )
    find_package(Doxygen)
    if( DOXYGEN_FOUND )
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
#        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxy-boot.js ${DOCFILES}/docs/doxy-boot.js COPYONLY)
        add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
    endif(DOXYGEN_FOUND)
endif(build_documentation)
