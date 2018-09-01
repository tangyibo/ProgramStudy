cmake_minimum_required(VERSION 2.8)
find_package(Boost COMPONENTS iostreams REQUIRED)

add_executable(memory-map main.cpp)
target_link_libraries(memory-map ${Boost_LIBRARIES})
