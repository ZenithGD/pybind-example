#include <pybind11/pybind11.h>
#include <bar/bar.hpp>

namespace py = pybind11;

PYBIND11_MODULE(main_bar_py, m) {
    py::class_<Printer>(m, "Printer")
        .def(py::init<const std::string &>())
        .def("print_int", &Printer::print_int, "Print an integer")
        .def_property("prefix", 
            // Since the method has 2 overloads, we must statically cast to the specific overload so the
            // compiler knows which version to bind against
            static_cast<std::string&(Printer::*)()>(&Printer::get_prefix), 
            &Printer::set_prefix)
        .def("__repr__",
            [](const Printer &a) {
                return "<main_py.bar.Printer with prefix '" + a.get_prefix() + "'>";
            }
        );
}