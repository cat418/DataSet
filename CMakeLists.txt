cmake_minimum_required(VERSION 3.17)

project(test)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE  SRC src/armor_detector/*.cpp src/Armor/*.cpp src/Light/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SRC})

find_package(OpenCV REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS} include)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
