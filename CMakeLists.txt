cmake_minimum_required(VERSION 3.5.1)
project(phtrie)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=core-avx2 -O0  -Wno-deprecated -g")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS filesystem regex thread)

file(GLOB SRC
    "src/*.h"
    "src/*.cpp"
    "src/*/*.h"
    "src/*/*.cpp"
    "src/*/*/*.h"
    "src/*/*/*.cpp"
)

message(${SRC})

add_executable(phtrie ${SRC})
target_link_libraries(phtrie pthread Boost::regex Boost::filesystem Boost::thread)

list(REMOVE_ITEM SRC "main.cpp")
add_library(PH SHARED ${SRC})
target_link_libraries(PH pthread Boost::regex Boost::filesystem Boost::thread)


function (add_tx_test file_name )
    add_executable(${file_name} test/${file_name}.cpp)
    target_link_libraries(${file_name} PH gtest)
    add_test(NAME ${file_name} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${file_name})
endfunction()

add_tx_test(PH_TEST)