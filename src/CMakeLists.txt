project(pybind-example-exec VERSION 0.1.0 LANGUAGES C CXX)
find_package(Python3 REQUIRED COMPONENTS Development Interpreter)

# add the executable
add_executable(main
    main.cpp
)

add_subdirectory(foo)
add_subdirectory(bar)

target_link_libraries(main 
    PUBLIC main_foo
    PUBLIC main_bar)

target_include_directories(main
  PUBLIC src)

# install module init file
install(FILES __init__.py DESTINATION python/pybindpy)

# install binary and libs
install(TARGETS main DESTINATION bin)
install(TARGETS main_foo main_bar LIBRARY DESTINATION lib)
install(TARGETS main_foo_py main_bar_py LIBRARY DESTINATION python/pybindpy)