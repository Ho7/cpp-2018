cmake_minimum_required(VERSION 3.13.1)
project(main)

find_package(Boost REQUIRED)

add_executable(main 
    main.cpp
    virtual-machine.cpp
    interpreter.cpp
)
target_link_libraries(main ${Boost_LIBRARIES})
set_target_properties(main PROPERTIES CXX_STANDARD 17)
