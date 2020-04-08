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
});
```
More details can be found in the [test program](TestProject_ParallelExecution/test_program.cpp) which also covers the usage of the thread-safe functions.

## References
Details can be found in the cordesponding [blog article](https://milania.de/blog/C%2B%2B_class_for_easy_parallelization_of_for_loops).
