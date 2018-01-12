# TestProject_ParallelExecution
[Class](TestProject_ParallelExecution/ParallelExecution.h) for easy parallelization of `for` loops in C++. It also covers methods to write the result to a common variable or something to the console in a thread-safe way.

## Installation
Only the header [`ParallelExecution.h`](TestProject_ParallelExecution/ParallelExecution.h) is required. Simply download it and add it to your include path.

## Usage
To run a loop in parallel, just include the header, create an object of the class `ParallelExecution` and then use the `parallel_for()` function.
```cpp
#include "ParallelExecution.h"
//...
ParallelExecution pe;
pe.parallel_for(0, iterations - 1, [&](const size_t i)  // i = {0, 1, ..., iterations - 1}
{
  // Do stuff in parallel
}
```
More details can be found in the [test program](TestProject_ParallelExecution/test_program.cpp) which also covers the usage of the thread-safe functions.

## Note for this branch
The implementation in this branch includes extra methods to write something directly to the console via the `std::cout` or `std::cerr` objects (see also [this issue](https://github.com/Milania1/TestProject_ParallelExecution/issues/1)), e.g.
```cpp
pe.cout() << "Iteration " << i << " done" << std::endl;
```
But the implementation makes uses of [inline variables](https://stackoverflow.com/questions/38043442/how-do-inline-variables-work) and hence requires a C++17 compiler (e.g. by using the `/std:c++latest` switch in Visual Studio 2017).

## References
Details can be found in the cordesponding [blog article](https://milania.de/blog/C%2B%2B_class_for_easy_parallelization_of_for_loops).
