cmake_minimum_required(VERSION 3.16.0)

# Project setup
project(WRTactix LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
cmake_policy(SET CMP0100 NEW)  # Process .hh headers for Qt

# Dependency finding
find_package(Qt6 COMPONENTS Core Network Concurrent Test REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Path configuration
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(test_dir ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(data_dir ${CMAKE_CURRENT_SOURCE_DIR}/data)

# Protocol Buffer generation
execute_process(COMMAND sh protoc.sh WORKING_DIRECTORY ${src_dir}/3rdparty/proto)
execute_process(COMMAND sh protoc.sh WORKING_DIRECTORY ${src_dir}/proto)

# ------------------------------------------------------------
# Main Library (shared between app and tests)
# ------------------------------------------------------------
file(GLOB_RECURSE LIB_SOURCES
    CONFIGURE_DEPENDS
    ${src_dir}/*.c
    ${src_dir}/*.cpp
    ${src_dir}/*.cc
)
list(REMOVE_ITEM LIB_SOURCES "${src_dir}/main.cc")  # Remove main entry point

file(GLOB_RECURSE LIB_HEADERS
    CONFIGURE_DEPENDS
    ${src_dir}/*.h
    ${src_dir}/*.hpp
    ${src_dir}/*.hh
)

add_library(${PROJECT_NAME}_lib STATIC
    ${LIB_SOURCES}
    ${LIB_HEADERS}
)

target_include_directories(${PROJECT_NAME}_lib PRIVATE
    ${src_dir}
    ${src_dir}/3rdparty
)

target_link_libraries(${PROJECT_NAME}_lib
    Qt6::Network
    Qt6::Core
    Qt6::Concurrent
    Qt6::Test
    omniORB4
    omnithread
    protobuf
    GEARSystem
    gRPC::grpc++
    Eigen3::Eigen
)

target_compile_options(${PROJECT_NAME}_lib PRIVATE
    -isystem ${src_dir}/3rdparty
    -w
    -Wno-everything
)

# ------------------------------------------------------------
# Main Executable
# ------------------------------------------------------------
add_executable(${PROJECT_NAME}
    ${src_dir}/main.cc
)

target_link_libraries(${PROJECT_NAME}
    ${PROJECT_NAME}_lib
)

# ------------------------------------------------------------
# Test Executable
# ------------------------------------------------------------
enable_testing()

file(GLOB_RECURSE TEST_SOURCES
    CONFIGURE_DEPENDS
    ${test_dir}/*.cc
)

add_executable(${PROJECT_NAME}_tests
    ${TEST_SOURCES}
)

target_link_libraries(${PROJECT_NAME}_tests
    ${PROJECT_NAME}_lib
    Qt6::Test
)

target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ${src_dir}
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

# ------------------------------------------------------------
# Data Generators (Binaries for .cc files under the data folder)
# ------------------------------------------------------------
file(GLOB_RECURSE DATA_SOURCES
    CONFIGURE_DEPENDS
    ${data_dir}/*.cc
)

foreach(data_source ${DATA_SOURCES})
    get_filename_component(target_name ${data_source} NAME_WE)

    get_filename_component(data_source_dir ${data_source} DIRECTORY)

    add_executable(${target_name} ${data_source})

    target_link_libraries(${target_name} ${PROJECT_NAME}_lib)

    target_include_directories(${target_name} PRIVATE
        ${src_dir}
    )

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${data_source_dir}
        OUTPUT_NAME ${target_name}.bin 
    )
endforeach()

# ------------------------------------------------------------
# Build Configuration
# ------------------------------------------------------------
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME}_lib
        PUBLIC QT_NO_DEBUG_OUTPUT QT_NO_INFO_OUTPUT QT_NO_WARNING_OUTPUT
    )
endif()

target_compile_definitions(${PROJECT_NAME}_lib
    PUBLIC DEFAULT_CONFIG_FILEPATH="${src_dir}/WRCoach/const/config/config.toml"
)