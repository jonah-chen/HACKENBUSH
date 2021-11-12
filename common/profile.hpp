/**
 * @file profile.hpp
 * @author Jonah Chen
 * @brief basic profiler that uses the std::chrono library. The basic usage is:
 *  - use PROFILE_START only ONCE in a scope to begin a timer
 *  - use PROFILE_LOG to print to the console. 
 *  - use PROFILE_RESET to reset the counter.
 * 
 * @warning note that defining RELEASE will disable the profiler.
 * 
 * @version 0.1
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#ifndef RELEASE
#define PROFILE_START Timer __timer(__FILE__,__FUNCTION__,__LINE__)
#define PROFILE_RESET __timer(__FILE__,__FUNCTION__,__LINE__)
#define PROFILE_LOG __timer.log(__LINE__)
#else
#define PROFILE_START
#define PROFILE_RESET
#define PROFILE_LOG
#endif

#include <chrono>
#include <iostream>

class Timer
{
public:
	Timer(const char *file, const char *function, int start_line)
			: file_(file), function_(function), start_line_(start_line)
	{
		start_ = clock::now();
	}

	void log(int end_line)
	{
		timepoint end = clock::now();
		duration elapsed = end - start_;
		double exec_time = elapsed.count();

		std::cout << file_ << ":" << start_line_ << "-" << end_line
				  << "(" << function_ << ")@";

		if (exec_time >= 1.0)
			std::cout << exec_time << "s/it" << std::endl;
		else
			std::cout << 1.0 / exec_time << "it/s" << std::endl;
	}

private:
	using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<double>;
	timepoint start_;
	const char *file_;
	const char *function_;
	int start_line_;
};