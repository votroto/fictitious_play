cmake_minimum_required(VERSION 3.0.0)
project(fictitious_play VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(fictitious_play main.cpp)
target_include_directories(fictitious_play PRIVATE cxxopts)
target_compile_options(fictitious_play PRIVATE -Wall -Wextra -pedantic -Werror -Ofast)

target_link_libraries(fictitious_play Eigen3::Eigen)
