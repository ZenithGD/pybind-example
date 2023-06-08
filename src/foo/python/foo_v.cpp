#include <pybind11/pybind11.h>
#include <foo/foo.hpp>

namespace py = pybind11;

PYBIND11_MODULE(main_foo_py, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");
}