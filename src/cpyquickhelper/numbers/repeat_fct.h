#pragma once
#include <chrono>

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

/**
* Pointer on a function which takes nothing and returns nothing.
*/
typedef void *void_function_void(void *);

/** Report on execution.
*/
typedef struct
{
    int number;
    int repeat;
    double average;
    double deviation;
    double min_exec;
    double max_exec;
} ExecutionStat;

/**
* Method *Run* is measured.
*/
class FunctionMeasure
{
    public:
        /**
        * Function to measure.
        */
        int run(int i) { throw std::runtime_error("It should be overwritten."); }
};

/**
* Measures the execution time of a function with no parameters.
* @param    fct     function wrapped in a class FunctionMeasure,
*                   it must contain method *run*
* @param    repeat  number of times to repeat the function,
*                   the function measures the average time
*                   for *repeat* execution
* @param    number  number of executions
* @param    verbose diplay information on each execution
* @param    report  results
*/
template <typename FCTTYPE>
void repeat_execution(FCTTYPE& fct, int repeat, int number, ExecutionStat& report, bool verbose=false)
{
    std::chrono::high_resolution_clock::time_point start, end;
    std::vector<double> exec(number);
    int r;
    for(auto it = exec.begin(); it != exec.end(); ++it)
    {
        start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < repeat; ++i)
            r = fct.run(i);
        end = std::chrono::high_resolution_clock::now();
        *it = std::chrono::duration<double>(end - start).count();
        if (verbose)
            std::cout << "[repeat_execution] "
                      << (std::distance(exec.begin(), it) + 1)
                      << "/" << exec.size() << ": "
                      << *it << "  --  " << r
                      //<< std::chrono::duration<double>(end).count()
                      << "\n";
        if (it == exec.begin())
        {
            report.average = *it;
            report.deviation = (*it)*(*it);
            report.min_exec = *it;
            report.max_exec = *it;
        }
        else 
        {
            report.average += *it;
            report.deviation += (*it)*(*it);
            report.min_exec = report.min_exec < *it ? report.min_exec : *it;
            report.max_exec = report.max_exec > *it ? report.max_exec : *it;
        }
    }
    report.number = number;
    report.repeat = repeat;
    report.average /= number;
    report.deviation /= number;
    report.deviation -= report.average * report.average;
    report.deviation = sqrt(report.deviation);
}

#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
