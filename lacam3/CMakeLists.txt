cmake_minimum_required(VERSION 3.16)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
file(GLOB SRCS "./src/*.cpp")
project(lacam3)
add_library(${PROJECT_NAME} STATIC ${SRCS})
target_compile_options(${PROJECT_NAME} PUBLIC -O3 -Wall)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE ./include)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
