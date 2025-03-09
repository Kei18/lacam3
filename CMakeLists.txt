cmake_minimum_required(VERSION 3.16)
project(lacam CXX)

find_package(pybind11 REQUIRED)

add_subdirectory(./lacam3)
add_subdirectory(./third_party/argparse)
add_subdirectory(./bindings)
