#sgn
cmake_minimum_required(VERSION 3.1)
project(ISOReviewAsst)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/include")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/sqlite3")

add_executable(ISOReviewAsst src/SGNMain.cpp src/RESTful.cpp src/DBInterface.cpp src/Logger.cpp)

target_link_libraries(ISOReviewAsst SQLiteCpp sqlite3 -lpthread -lpistache)

#add_subdirectory(SQLiteCpp)
