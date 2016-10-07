#pragma once

#include <thread>
#include <deque>
#include <mutex>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

class ParallelExecution
{
public:
	/**
	 * @brief Provides simple methods to parallelise for loops including helper functions for critical sections.
	 * 
	 * @param numbThreads specifies the number of threads used for parallelisation (if not specified otherwise). Defaults to the number of cores available on the system (virtual + real cores)
	 */
	explicit ParallelExecution(const size_t numbThreads = std::thread::hardware_concurrency())
		: numbThreads(numbThreads)
	{
		assert(numbThreads > 0 && "At least one thread is necessary");
	}

	/**
	 * @brief Stores results in a thread-safe way.
	 * 
	 * A mutex will automatically be locked on entry and unlocked on exit of this function. This is useful after the parallel computation when a common variable is accessed containing all the results.
	 * 
	 * @param callback includes the code which shoud be executed in a thread-safe way
	 */
	void setResult(const std::function<void()>& callback)
	{
		std::lock_guard<std::mutex> lock(mutexResult);											// Only one thread at a time is allowed to change the result value (Unlocks automatically on destructor call)

		callback();
	}

	/**
	 * @brief Writes messages to the console in a thread-safe way.
	 * 
	 * Same mutex behaviour as in ParallelExecution::setResult(). Useful if you don't want your console output get messed up.
	 * 
	 * @param message to print to the console
	 */
	void write(const std::string& message)
	{
		std::lock_guard<std::mutex> lock(mutexConsole);

		std::cout << message << std::endl;
	}

	/**
	 * @brief Executes index-based containers in parallel.
	 * 
	 * @param idxBegin first index to start (including), e. g. <code>0</code>
	 * @param idxEnd last index to start (including), e. g. <code>container.size()</code>
	 * @param callback this function will be called from each thread multiple times. Each time an associated index will be passed to the function
	 * @param numbThreadsFor number of threads which should be used for parallelisation (only for the current specific loop)
	 */
	void parallel_for(const size_t idxBegin, const size_t idxEnd, const std::function<void(const size_t)>& callback, const size_t numbThreadsFor = CLASS_SETTING) const
	{
		/* If the user provides a thread number for this loop, use it. Otherwise use the thread number from the class */
		size_t _numbThreadsFor = numbThreadsFor == CLASS_SETTING ? numbThreads : numbThreadsFor;
		const size_t sizeThreads = std::min(idxEnd - idxBegin + 1, _numbThreadsFor);
		assert(sizeThreads > 0 && "No index range given");

		auto threadFunction = [&callback] (const std::deque<size_t>& indices)
		{
			for(const size_t i : indices)
			{
				callback(i);																	// The thread funtion executes the callback for every assigned index
			}
		};

		if(sizeThreads == 1)																	// There is no need to parallise when only one idx is given, just execute in main thread
		{
			std::deque<size_t> indices;
			for (size_t i = idxBegin; i <= idxEnd; ++i)
			{
				indices.push_back(i);
			}

			threadFunction(indices);
			return;
		}

		std::deque<std::deque<size_t>> indicesThreads(sizeThreads);								// Every thread gets its own indices
		std::deque<std::thread> threads(sizeThreads);

		for (size_t i = idxBegin; i <= idxEnd; ++i)
		{
			indicesThreads[i % sizeThreads].push_back(i);										// Assign every index to its corresponding thread (with the results of their modulo value)
		}

		/* Start every thread */
		for (size_t i = 0; i < threads.size(); i++)
		{
			threads[i] = std::thread(threadFunction, indicesThreads[i]);						// Execute the threadFunction which calls the callback from the user
		}

		/* And wait until they are finished with calculation */
		for (size_t i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}
	}

private:
	size_t numbThreads;
	std::mutex mutexResult;
	std::mutex mutexConsole;
	static const size_t CLASS_SETTING = 0;
};
