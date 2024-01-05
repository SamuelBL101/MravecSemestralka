cmake_minimum_required(VERSION 3.18)
add_subdirectory(server)

project(MravecSemestralka)

set(CMAKE_CXX_STANDARD 11)

add_executable(MravecSemestralka
        main.cpp
        my_socket.cpp
        my_socket.h
        BlockType.h
        Block.cpp
        Block.h
        World.cpp
        World.h
        Ant.cpp
        Ant.h
        Button.cpp
        Button.h
)

if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
    find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
    include_directories(c:/SFML-2.6.1/include)
    target_link_libraries(MravecSemestralka sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

    find_package(Threads REQUIRED)
    target_link_libraries(MravecSemestralka Threads::Threads)
endif ()
