cmake_minimum_required(VERSION 3.10)
project(task)

set(CMAKE_CXX_STANDARD 20)
add_executable(task 
  main.cpp 
  src/dns_cache.h 
  src/dns_cache_impl.h 
  src/dns_cache_impl.cpp 
  src/test.h 
  src/test.cpp
)
