// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gauge/gauge.hpp>

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

struct option_benchmark : public gauge::time_benchmark
{
    void store_run(tables::table& results)
    {
        if (!results.has_column("magic"))
            results.add_column("magic");

        results.set_value("magic", (uint32_t)m_delay.count());
    }

    double measurement()
    {
        // Get the time spent sleeping
        double time = gauge::time_benchmark::measurement();
        // This should be higher than 99% of the requested time
        // The sleep period might end a little earlier on Windows
        EXPECT_GE(time, m_delay.count() * 0.99);
        return time;
    }

    void get_options(cxxopts::ParseResult& options)
    {
        auto symbols = options["symbols"].as<std::vector<uint32_t>>();
        auto symbol_size = options["symbol_size"].as<std::vector<uint32_t>>();
        auto types = options["type"].as<std::vector<std::string>>();

        assert(symbols.size() > 0);
        assert(symbol_size.size() > 0);
        assert(types.size() > 0);

        for (uint32_t i = 0; i < symbols.size(); ++i)
        {
            for (uint32_t j = 0; j < symbol_size.size(); ++j)
            {
                for (uint32_t u = 0; u < types.size(); ++u)
                {
                    gauge::config_set cs;
                    cs.set_value<uint32_t>("symbols", symbols[i]);
                    cs.set_value<uint32_t>("symbol_size", symbol_size[j]);
                    cs.set_value<std::string>("type", types[u]);

                    add_configuration(cs);
                }
            }
        }
    }

    void test_body()
    {
        gauge::config_set cs = get_current_configuration();

        uint32_t symbols = cs.get_value<uint32_t>("symbols");

        m_delay = std::chrono::milliseconds(symbols);

        RUN
        {
            std::this_thread::sleep_for(m_delay);
        }
    }

protected:

    std::chrono::microseconds m_delay;
};

BENCHMARK_OPTION(basic_options)
{
    auto& parser = gauge::runner::instance().option_parser();

    auto default_symbols =
        cxxopts::value<std::vector<uint32_t>>()->default_value("16,32");

    auto default_symbol_size =
        cxxopts::value<std::vector<uint32_t>>()->default_value("1600");

    auto default_types =
        cxxopts::value<std::vector<std::string>>()->
        default_value("encoder,decoder");

    parser.add_options()
    ("symbols", "Set the number of symbols", default_symbols);

    parser.add_options()
    ("symbol_size", "Set the symbol size in bytes", default_symbol_size);

    parser.add_options()
    ("type", "Set type [encoder|decoder]", default_types);
}

BENCHMARK_F(option_benchmark, options, basic, 1);
