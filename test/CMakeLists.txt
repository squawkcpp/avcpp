# -------------------------------------------------------------------------
# Build LibAvCpp tests
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.3)

project( avcpp-test )

# -------------------------------------------------------------------------
# -----------------------   Manage Test Cases       -----------------------
# -------------------------------------------------------------------------
aux_source_directory( ${PROJECT_SOURCE_DIR} TEST_SOURCES )
enable_testing()
include_directories(${ROOT} ${IMPORTED_INCLUDES})
add_executable(${PROJECT_NAME} ${TEST_SOURCES} )
target_link_libraries(${PROJECT_NAME} av ${CONAN_LIBS} -ldl)
add_test(${PROJECT_NAME} ${PROJECT_NAME})
