cmake_minimum_required(VERSION 3.10.2)
project(main)

add_executable(main 
    main.cpp
    string.cpp
)
set_target_properties(main PROPERTIES CXX_STANDARD 17)
