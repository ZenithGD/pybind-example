#include <bar/bar.hpp>
#include <iostream>

Printer::Printer(const std::string& prefix) 
    : m_prefix(prefix) {}

void Printer::print_int(int v) {
    std::cout << "[ " << m_prefix << " ] " << v << std::endl;
}