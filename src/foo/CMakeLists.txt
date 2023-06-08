# Create the library target
add_library(main_foo
    foo.cpp
)

# Include the top level directory
target_include_directories(main_foo PUBLIC ${PROJECT_SOURCE_DIR})

# Create a new python module
pybind11_add_module (
    main_foo_py
    python/foo_v.cpp
)

# Link the C++ library against the python module 
target_link_libraries(main_foo_py PRIVATE main_foo)

# Include the top level directory too
target_include_directories(main_foo_py PUBLIC ${PROJECT_SOURCE_DIR})