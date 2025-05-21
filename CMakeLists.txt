cmake_minimum_required(VERSION 3.30)
project(cpp_hw)

set(CMAKE_CXX_STANDARD 20)

add_executable(task_1
        "task_1/User.cpp"
        "task_1/task_1.cpp"
        "task_1/User.h"
        "task_1/Group.cpp"
        "task_1/Group.h"
        "task_1/Observer.cpp"
        "task_1/Observer.h")

add_executable(task_2
        "task_2/task_2.cpp"
        "task_2/TypeList.hpp")

add_executable(task_3
        "task_3/TypeMap.hpp"
        "task_3/task_3.cpp")

add_executable(task_4
        "task_4/task_4.cpp"
        "task_4/lcs.hpp"
        "task_4/counter.hpp")
add_executable(task_5
        "task_5/task_5.cpp"
        "task_5/Log.cpp"
        "task_5/Log.h")
add_executable(task_6
        "task_6/task_6.cpp"
)
add_executable(task_7
        "task_7/task_7.cpp"
)
add_executable(task_8
        "task_8/task_8.cpp"
)