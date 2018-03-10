# -------------------------------------------------------------------------
# Build AvCpp Samples
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.3)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
project( samples )

# -------------------------------------------------------------------------
# -----------------------   Compile Binary          -----------------------
# -------------------------------------------------------------------------

add_executable( metadata metadata.cpp )
target_link_libraries(metadata av ${CONAN_LIBS} -ldl)

add_executable(decode decode.cpp)
target_link_libraries(decode av ${CONAN_LIBS} -ldl)

add_executable(decode_audio decode_audio.cpp)
target_link_libraries(decode_audio av ${CONAN_LIBS} -ldl)

add_executable(encode encode.cpp)
target_link_libraries(encode av ${CONAN_LIBS} -ldl)

add_executable(encode_audio encode_audio.cpp)
target_link_libraries(encode_audio av ${CONAN_LIBS} -ldl)

add_executable(resample resample.cpp)
target_link_libraries(resample av ${CONAN_LIBS} -ldl)

add_executable(transcode_aac transcode_aac.cpp)
target_link_libraries(transcode_aac av ${CONAN_LIBS} -ldl)

#add_executable(dr_range dr_range.cpp)
#target_link_libraries(dr_range av ${CONAN_LIBS} -ldl)

find_package(SDL)
if( ${SDL_FOUND} )
    include_directories(${ROOT} ${INCLUDES} ${SDL_INCLUDE_DIR})
    add_executable(play play.cpp)
    target_link_libraries(play av ${CONAN_LIBS} ${SDL_LIBRARY})
endif()

