#sgn
cmake_minimum_required(VERSION 3.1)
project(ISOReviewAsst)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(SQLiteCpp REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_executable(ISOReviewAsst src/SGNMain.cpp src/RESTful.cpp src/DBInterface.cpp)

target_link_libraries(ISOReviewAsst SQLiteCpp -lpthread -lpistache)
