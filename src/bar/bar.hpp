#pragma once

#include <iostream>

class Printer {
public:

    Printer(const std::string& prefix);

    void print_int(int v);

    inline void set_prefix(const std::string& prefix) { m_prefix = prefix; };

    inline std::string get_prefix() const { return m_prefix; };
    inline std::string& get_prefix() { return m_prefix; };

private:
    std::string m_prefix;
};