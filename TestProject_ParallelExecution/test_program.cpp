#include "ParallelExecution.h"
#include <deque>

const size_t iterations = 100LL;

std::deque<double> serialExecution(const std::deque<double>& inputA, const std::deque<double>& inputB)
{
    assert(inputA.size() == inputB.size() && "Both input arrays need to have the same size");

    std::deque<double> result(inputA.size(), 0.0);
    for (size_t i = 0; i < iterations; ++i)
    {
        for (size_t j = 0; j < result.size(); ++j)
        {
            result[j] += (inputA[j] + inputB[j]) / (i + j + 1);
        }
    }

    return result;
}

std::deque<double> parallelExecution(const std::deque<double>& inputA, const std::deque<double>& inputB)
{
    /* Note: the code is just an example of the usage of the ParallelExecution class and does not really cover a useful scenario */

    assert(inputA.size() == inputB.size() && "Both input arrays need to have the same size");

    ParallelExecution pe(12);                                        // Number of threads to use, if not otherwise specified

    std::deque<double> result(inputA.size(), 0.0);
    pe.parallel_for(0, iterations - 1, [&](const size_t i)           // Outer for loop uses as much threads as cores are available on the system
    {
        pe.parallel_for(0, result.size() - 1, [&](const size_t j)    // Inner for loop uses 2 threads explicitly (the parallelisation at this point does not really make sense, just for demonstration purposes)
        {
            const double newValue = (inputA[j] + inputB[j]) / (i + j + 1);

            pe.setResult([&] ()
            {
                result[j] += newValue;                               // Stores the result value in a thread-safe way (in different iterations the same variable may be accessed at the same time)
            });
        }, 2);

        pe.cout() << "Iteration " << i << " done" << std::endl;      // Gives a threads-safe console output
        //pe.write("Iteration " + std::to_string(i) + " done");      // Alternatively, provide a string to be printed
    });

    return result;
}

int main()
{
    std::deque<double> inputA(10'000LLU, 42);
    std::deque<double> inputB(10'000LLU, 4711.1337);

    serialExecution(inputA, inputB);
    parallelExecution(inputA, inputB);
}
