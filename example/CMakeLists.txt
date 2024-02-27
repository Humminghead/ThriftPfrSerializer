cmake_minimum_required(VERSION 3.5)

project(tpfrserializer_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE tpfrserializer thrift benchmark)

install(TARGETS tpfrserializer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
