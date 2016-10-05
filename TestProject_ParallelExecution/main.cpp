#include "ParallelExecution.h"
#include <ctime>
#include <atomic>

std::deque<double> serialExecution(const std::deque<double>& input)
{
	std::deque<double> resultVariable;
	for (size_t i = 0; i < input.size(); i++)
	{
		double value = 0.0;
		for (size_t j = 0; j < input.size(); j++)
		{
			value += std::log10(std::sqrt(std::pow(input[i], 3)) * std::sin(j) + std::cos(j) * std::cos(i));
		}

		if (static_cast<int>(value) % 2 == 0)
		{
			resultVariable.push_back(value);
		}

		if (i % 1000 == 0)
		{
			std::cout << "Progess: " << static_cast<double>(i) / static_cast<double>(input.size() - 1) << std::endl;
		}
	}

	return resultVariable;
}

std::deque<double> parallelExecution(const std::deque<double>& input)
{
	/* Note: the code is just an example for the usage of the ParallelExecution class and does not really cover an useful scenario */
	
	ParallelExecution pe;

	std::deque<double> resultVariable;
	pe.parallel_for(0, input.size() - 1, [&](const size_t i)		// Outer for loop uses as much threads as cores are available on the system
	{
		std::atomic<double> value = 0.0;
		pe.parallel_for(0, input.size() - 1, [&](const size_t j)	// Inner for loop uses 2 threads explicitly (the parallelisation at this point does not really make sense, just for demonstration purposes)
		{
			value = value + std::log10(std::sqrt(std::pow(input[i], 3)) * std::sin(j) + std::cos(j) * std::cos(i));					// Do some (weird) computation
		}, 2);

		if (static_cast<int>(value) % 2 == 0)		// Store the value only if some condition holds (direct index access would not require thread-safe storage of the result value)
		{
			pe.setResult([&] ()
			{
				resultVariable.push_back(value);	// Store the result value in a thread-safe way
			});
		}

		if (i % 1000 == 0)							// Just to output not too much
		{
			pe.write(std::string("Progess: ") + std::to_string(static_cast<double>(i) / static_cast<double>(input.size() - 1)));	// Gives a threads-safe console output
		}
	});
	
	return resultVariable;
}

int main()
{
	/* Initialisation */
	std::deque<double> inputValues(10'000LLU);

	std::srand(static_cast<unsigned int>(std::time(0)));
	for (size_t i = 0; i < inputValues.size(); i++)
	{
		inputValues[i] = std::rand();
	}

	/* Benchmark */
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	serialExecution(inputValues);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Serial computation time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " s" << std::endl;

	begin = std::chrono::steady_clock::now();
	parallelExecution(inputValues);
	end = std::chrono::steady_clock::now();

	std::cout << "Parallel computation time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " s" << std::endl;
}
