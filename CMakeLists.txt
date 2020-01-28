cmake_minimum_required(VERSION 3.16.2)
project(cminus)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pedantic -Wall -Wextra -Wno-unused-parameter -O0 -g")

include_directories(${CMAKE_SOURCE_DIR}/include)

include(lib/CMakeLists.txt)

set(LEXICO_SRC
    src/lexico/main.cpp
    ${LIBCMINUS_SRC}
)

set(SINTATICO_SRC
    src/sintatico/main.cpp
    ${LIBCMINUS_SRC}
)

set(GERACODIGO_SRC
    src/geracodigo/main.cpp
    ${LIBCMINUS_SRC}
)

add_executable(lexico ${LEXICO_SRC})
add_executable(sintatico ${SINTATICO_SRC})
add_executable(geracodigo ${GERACODIGO_SRC})
