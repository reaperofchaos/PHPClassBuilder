cmake_minimum_required(VERSION 3.0)
set(PROJECTNAME "PhpClassBuilder")
project(PhpClassBuilder)
include(CheckCCompilerFlag)

set(CMAKE_HOST_SYSTEM_PROCESSOR "arm64")

add_executable(${PROJECTNAME} main.cpp PHPBuilder.cpp)
