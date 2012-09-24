#ifndef GAUGE_PRINTER_HPP
#define GAUGE_PRINTER_HPP

#include "benchmark.hpp"
#include "result.hpp"

namespace gauge
{
    /// Progress printer interface. To provide a custom
    /// Result printer create a sub-class of this interface
    /// and add it to gauge using:
    ///
    ///   gauge::runner::instance().printers().push_back(
    ///     std::make_shared<my_custom_printer>());
    ///
    class printer
    {
    public:

        /// Destructor
        ~printer()
            {}

        /// Called when the benchmark program is started
        virtual void start_benchmark(/*const gauge_info &info*/)
            {}

        /// Called when a result from a benchmark is ready
        /// @param result the benchmark results
        virtual void benchmark_result(const benchmark &/*info*/,
                                      const result &/*result*/)
            {}

        /// Called when the benchmark program is finished
        virtual void end_benchmark(/*const gauge_info &info*/)
            {}
    };
}

#endif
