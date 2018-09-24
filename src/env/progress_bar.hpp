/*!
 * @file progress_bar.hpp
 * @brief ged::ProgressBar class declaration.
 */

#ifndef SRC_ENV_PROGRESS_BAR_HPP_
#define SRC_ENV_PROGRESS_BAR_HPP_

#include <iostream>
#include <iomanip>
#include <chrono>

namespace ged {

/*!
 * @brief A progress bar class.
 */
class ProgressBar {

public:

	friend std::ostream & operator<<(std::ostream &, const ProgressBar &);

	/*!
	 * @brief Constructs a progress bar for given number of tasks.
	 * @param[in] num_tasks The number of tasks.
	 */
	ProgressBar(std::size_t num_tasks);

	/*!
	 * @brief Increments the number of solved tasks.
	 */
	void increment();

	/*!
	 * @brief Sets the number of solved tasks to 0.
	 */
	void reset();

private:

	std::size_t num_solved_tasks_;

	const std::size_t num_tasks_;

	const std::chrono::high_resolution_clock::time_point start_time_;
};

/*!
 * @brief Streams the current progress in percent and the estimated remaining runtime.
 * @param[in,out] os Output stream.
 * @param[in] progress_bar The progress bar whose current progress should be streamed.
 * @return The output stream @p os.
 * @relates ged::ProgressBar
 */
std::ostream &
operator<<(std::ostream & os, const ProgressBar & progress_bar);

}

#include "progress_bar.ipp"

#endif /* SRC_ENV_PROGRESS_BAR_HPP_ */
