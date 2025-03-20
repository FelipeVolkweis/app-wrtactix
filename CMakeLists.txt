cmake_minimum_required(VERSION 3.16.0)

# Setting project configs
project(WRTactix LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting Qt-specific configs
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# CMake policy: let AUTOMOC and AUTOUIC process header files that end with a .hh extension
cmake_policy(SET CMP0100 NEW)

find_package(Qt6 COMPONENTS Core Network Concurrent Test REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Setting `src_dir` as `src` folder absolute path
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

execute_process(COMMAND sh protoc.sh WORKING_DIRECTORY ${src_dir}/3rdparty/proto)
execute_process(COMMAND sh protoc.sh WORKING_DIRECTORY ${src_dir}/proto)

# Finding source and header files from disk
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${src_dir}/*.c ${src_dir}/*.cpp ${src_dir}/*.cc)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${src_dir}/*.h ${src_dir}/*.hpp ${src_dir}/*.hh)

# Adding the source, header and resource files to the target compilation
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Linking necessary libraries
target_link_libraries(
  ${PROJECT_NAME}
  Qt6::Network Qt6::Core Qt6::Concurrent Qt6::Test
  omniORB4 omnithread protobuf GEARSystem gRPC::grpc++ Eigen3::Eigen
)

# Adding `src` directory to the include path
target_include_directories(${PROJECT_NAME} PRIVATE ${src_dir})

target_compile_options(${PROJECT_NAME} PRIVATE "-isystem" "${src_dir}/3rdparty")
target_compile_options(${PROJECT_NAME} PRIVATE "-w" "-Wno-everything")

# Define Debug/Release specific flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(
      ${PROJECT_NAME} 
      PUBLIC QT_NO_DEBUG_OUTPUT QT_NO_INFO_OUTPUT QT_NO_WARNING_OUTPUT
    )
endif()

target_compile_definitions(
  ${PROJECT_NAME}
  PUBLIC DEFAULT_CONFIG_FILEPATH="${src_dir}/WRCoach/const/config/config.toml"
)
