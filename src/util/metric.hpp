/***************************************************************************
*                                                                          *
*   Copyright (C) 2025 by Linlin Jia                              *
*                                                                          *
*   This file is part of GEDLIB.                                           *
*                                                                          *
*   GEDLIB is free software: you can redistribute it and/or modify it      *
*   under the terms of the GNU Lesser General Public License as published  *
*   by the Free Software Foundation, either version 3 of the License, or   *
*   (at your option) any later version.                                    *
*                                                                          *
*   GEDLIB is distributed in the hope that it will be useful,              *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
*   GNU Lesser General Public License for more details.                    *
*                                                                          *
*   You should have received a copy of the GNU Lesser General Public       *
*   License along with GEDLIB. If not, see <http://www.gnu.org/licenses/>. *
*                                                                          *
***************************************************************************/

/*!
 * 	@file  metric.hpp
 *  @brief Declaration of metric functions for double vectors.
 */

#ifndef SRC_UTIL_METRIC_HPP
#define SRC_UTIL_METRIC_HPP

// #include <Eigen/Dense>
#include <any>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <cmath>
#include <functional>

namespace ged {

/*!
 * @brief Class containing metric functions for comparing vectors of doubles.
 * @details This class provides static methods to compute distances between vectors of doubles
 *          using e.g., Euclidean distance and Cosine similarity, with a feature map to convert std::vector to Eigen::VectorXd.
 */
class Metric {

public:
    /*!
     * @brief Converts a std::vector of doubles to an Eigen::VectorXd.
     * @param vec The input vector of doubles.
     * @return An Eigen::VectorXd representation of the input vector.
     */
    static Eigen::VectorXd double_vector_to_eigen(const std::vector<double>& vec);

    /*!
     * @brief Converts a string representing a metric name to a corresponding function.
     * @param metric_name The name of the metric as a string.
     * @return A function that computes the specified metric between two vectors of doubles.
     */
    static std::function<double(const std::vector<double> &, const std::vector<double> &)> metric_string_to_function(
        const std::string & metric_name, const std::unordered_map<std::string, std::any> & config = {}
    );
    
    /*!
     * @brief Computes the Euclidean distance between two vectors of doubles.
     * @param vec1 The first vector of doubles.
     * @param vec2 The second vector of doubles.
     * @return The Euclidean distance between the two vectors.
     */
    static double euclidean(const std::vector<double>& vec1, const std::vector<double>& vec2);

    /*!
     * @brief Computes the Cosine similarity between two vectors of doubles. The result is in the range [-1, 1].
     * @param vec1 The first vector of doubles.
     * @param vec2 The second vector of doubles.
     * @return The Cosine similarity between the two vectors.
     */
    static double cosine(const std::vector<double>& vec1, const std::vector<double>& vec2);

    /*!
     * @brief Computes the Cosine distance between two vectors of doubles.
     * @param vec1 The first vector of doubles.
     * @param vec2 The second vector of doubles.
     * @param normalize If true, normalizes the vectors to range [0, 1] after computing Cosine similarity. The default is true.
     * @return The Cosine distance between the two vectors.
     */
    static double cosine_distance(const std::vector<double>& vec1, const std::vector<double>& vec2, bool normalize = false);

};

} // namespace ged

#include "metric.ipp"

#endif // SRC_UTIL_METRIC_HPP