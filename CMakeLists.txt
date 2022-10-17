cmake_minimum_required(VERSION 3.11)

project(SceneGen LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(OpenImageIO REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_library(TINYXML2_LIB tinyxml2 REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-invalid-offsetof")

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Tags/Scene.cpp
    ${PROJECT_SOURCE_DIR}/src/Tags/Shape.cpp
    ${PROJECT_SOURCE_DIR}/src/Tags/Tag.cpp
    ${PROJECT_SOURCE_DIR}/src/Tags/Procedural.cpp
    ${PROJECT_SOURCE_DIR}/src/Tags/BSDF.cpp
    ${PROJECT_SOURCE_DIR}/src/Tags/Emitter.cpp
    ${PROJECT_SOURCE_DIR}/src/Tags/Texture.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Boost_INCLUDE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/extern/include")
target_link_libraries(${PROJECT_NAME} PRIVATE 
    OpenImageIO::OpenImageIO ${TINYXML2_LIB} Eigen3::Eigen)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
add_compile_options(-lstdc++fs)
