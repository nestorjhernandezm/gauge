#ifndef GAUGE_RUNNER_HPP
#define GAUGE_RUNNER_HPP

#include <vector>
#include <memory>

#include "benchmark.hpp"
#include "result.hpp"
#include "printer.hpp"

namespace gauge
{
    /// The gauge benchmark runner. Responsible for
    /// invoking the registered benchmarks.
    class runner
    {
    public:

        /// Pointer to a benchmark
        typedef std::shared_ptr<benchmark> benchmark_ptr;

        /// Pointer to a result printer
        typedef std::shared_ptr<printer> printer_ptr;

        /// Benchmark container
        typedef std::map<std::string, benchmark_ptr> benchmark_map;

        /// Test case container
        typedef std::map<std::string, benchmark_map> testcase_map;

        /// Creates a new runner with a default printer
        runner();

        /// Destructor
        ~runner();

        /// Access to the gauge benchmark runner instance. Even though we
        /// don't like singletons we use it here since it reduces complexity
        /// of registering and running benchmarks
        /// @return instance of the gauge benchmark runner
        static runner& instance();

        /// Run all the benchmarks registered with the gauge benchmark runner
        static void run_benchmarks(int argc = 0, const char* argv[] = 0);

        /// Registers an unique id for each benchmark type. All benchmarks call
        /// this function once to get their id.
        /// @return the unique id for a benchmark.
        static uint32_t register_id();

        /// Creates the desired benchmark with the runner
        template<class T>
        void register_benchmark()
            {
                uint32_t id = T::benchmark_id();
                benchmark_ptr bench = std::make_shared<T>();
                bench->set_id(id);
                add_benchmark(bench);
            }

        /// Adds a new benchmark
        void add_benchmark(benchmark_ptr bench);

        /// Returns the benchmark with the specific id
        /// @param id of the desired benchmark
        /// @return pointer to the benchmark
        benchmark_ptr get_benchmark(uint32_t id);

        /// Returns the id of the currently active benchmark
        /// @return id of benchmark
        benchmark_ptr current_benchmark();

        /// Start a new benchmark runner using the commandline
        /// parameters specified.
        /// @param argc for the program
        /// @param argv for the program
        void run(int argc = 0, const char *argv[] = 0);

        /// Run all the benchmarks
        void run_all();

        /// Run the benchmarks matching the specified filter
        /// @param filter name e.g. MyTest.*
        void run_filtered(const std::string &filter);

        /// Runs the benchmark with the specified id
        void run_benchmark(benchmark_ptr bench);

        /// Runs the specified benchmark running through the available
        /// configurations
        void run_benchmark_configurations(benchmark_ptr bench);

        /// @return access to the runners printers
        std::vector<printer_ptr>& printers();

    private: // @todo move to pimpl

        struct impl;
        std::unique_ptr<impl> m_impl;
    };
}

#endif
