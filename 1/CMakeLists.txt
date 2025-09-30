cmake_minimum_required(VERSION 3.10)
project(DPLL_SAT_Solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dpll_solver src/dpll_solver.cpp)
set_target_properties(dpll_solver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)