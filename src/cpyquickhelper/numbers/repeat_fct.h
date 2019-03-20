#pragma once

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <chrono>
#include <exception>
#include <vector>
#include <cmath>

#define REPEAT_INST10(INST) {\
    INST INST INST INST INST\
    INST INST INST INST INST\
    }

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

template<typename DTYPE>
class FunctionMeasureVector : FunctionMeasure
{
    protected:

    std::vector<DTYPE> values;

    public:

    FunctionMeasureVector(const std::vector<DTYPE> &v) : values(v)
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(this->th));
        if (this->values.size() == 0)
            throw std::runtime_error("Array to process must not be empty.");
    }
};

template<typename DTYPE>
class FunctionMeasureVectorThreshold : FunctionMeasure
{
    protected:

    std::vector<DTYPE> values;
    DTYPE th;

    public:

    FunctionMeasureVectorThreshold(const std::vector<DTYPE> &v, DTYPE t) : values(v), th(t)
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(this->th));
        if (this->values.size() == 0)
            throw std::runtime_error("Array to process must not be empty.");
    }
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
    std::vector<double> exec(repeat);
    int r = -1;
    for(auto it = exec.begin(); it != exec.end(); ++it)
    {
        start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < number; ++i)
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
    report.average /= (number * repeat);
    report.deviation /= (number * repeat);
    report.deviation -= report.average * report.average;
    report.deviation = sqrt(report.deviation > 0 ? report.deviation : 0);
}

#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
