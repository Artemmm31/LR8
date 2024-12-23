cmake_minimum_required(VERSION 3.10)
project(EmployeeManager)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIR})
add_executable(main main.cpp check.cpp)
target_link_libraries(main ${CURL_LIBRARIES})
