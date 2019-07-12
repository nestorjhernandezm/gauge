// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <tables/table.hpp>

#include "benchmark.hpp"
#include "file_printer.hpp"
#include "runner.hpp"

namespace gauge
{
printer::printer(const std::string& name, bool enabled) :
    m_name(name),
    m_enabled(enabled)
{
    auto parser = gauge::runner::instance().option_parser();

    parser.add_options()
    ("use_" + m_name, "Use the " + m_name + " printer",
     cxxopts::value<bool>()->default_value(m_enabled ? "1" : "0"));
}

bool printer::is_enabled() const
{
    return m_enabled;
}

void printer::set_options(const cxxopts::ParseResult& options)
{
    m_enabled = options["use_" + m_name].as<bool>();
}
}
