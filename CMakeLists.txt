cmake_minimum_required(VERSION 3.5)

project(tpfrserializer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tpfrserializer main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE thrift benchmark)

install(TARGETS tpfrserializer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
