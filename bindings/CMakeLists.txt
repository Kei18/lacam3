# Bindings-specific CMakeLists.txt

# Check if the pybind11.cpp file exists
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/pybind.cpp)
    message(FATAL_ERROR "pybind11.cpp file not found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# Target for PyBind11 bindings
pybind11_add_module(lacam MODULE
    ${CMAKE_CURRENT_SOURCE_DIR}/pybind.cpp
    # Include any other source files needed for bindings here
)
# Link against lacam3 library
target_link_libraries(lacam PRIVATE lacam3)
# Specify include directories (adjust paths as necessary)
target_include_directories(lacam PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${pybind11_INCLUDE_DIRS}
)
target_link_libraries(lacam PRIVATE lacam3)

# Link against Python libraries (handled by pybind11_add_module)