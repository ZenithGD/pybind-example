#include <iostream>

#include <foo/foo.hpp>
#include <bar/bar.hpp>

int main() {

    std::cout << "Hello PyBind11" << std::endl;

    Printer p("pybind example");
    p.print_int(add(1, 2));
    return 0;
}