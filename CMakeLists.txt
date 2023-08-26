cmake_minimum_required(VERSION 3.10)
project(task)

set(CMAKE_CXX_STANDARD 20)
add_executable(task 
  main.cpp 
  dns_cache.h 
  dns_cache_impl.h 
  dns_cache_impl.cpp 
  test.h 
  test.cpp
)
