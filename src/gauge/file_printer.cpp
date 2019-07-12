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

file_printer::file_printer(const std::string& name,
                           const std::string& default_filename) :
    printer(name), m_filename_option(name + "_file")
{
    // Add the filename option for this printer
    auto parser = gauge::runner::instance().option_parser();

    auto default_filename_value =
        cxxopts::value<std::string>()->default_value(default_filename);

    parser.add_options()
    (m_filename_option, "Set the output filename of the " + name + " printer",
     default_filename_value);
}

void file_printer::benchmark_result(const benchmark& info,
                                    const tables::table& results)
{
    tables::table output = results;
    if (info.has_configurations())
    {
        const auto& c = info.get_current_configuration();
        for (const auto& v : c)
        {
            output.add_const_column(v.first, v.second);
        }
    }

    m_tables.insert(m_tables.end(), output);
}

void file_printer::end()
{
    std::ofstream result_file;
    result_file.open(m_filename, std::ios::trunc);
    print_to_stream(result_file);
    result_file.close();
}

void file_printer::set_options(const cxxopts::ParseResult& options)
{
    printer::set_options(options);
    m_filename = options[m_filename_option].as<std::string>();
    assert(!m_filename.empty());
}
}
