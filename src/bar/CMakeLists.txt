set(BAR_SRCS
    bar.cpp
)

add_library(main_bar
    ${BAR_SRCS}
)

target_include_directories(main_bar PUBLIC ${PROJECT_SOURCE_DIR})

pybind11_add_module (
    main_bar_py
    python/bar_v.cpp
)

target_link_libraries(main_bar_py PRIVATE main_bar)
target_include_directories(main_bar_py PUBLIC ${PROJECT_SOURCE_DIR})
