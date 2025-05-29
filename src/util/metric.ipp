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
 * 	@file  metric.ipp
 *  @brief Definition of metric functions for double vectors.
 */

#ifndef SRC_UTIL_METRIC_IPP
#define SRC_UTIL_METRIC_IPP

namespace ged {

Eigen::VectorXd
Metric::double_vector_to_eigen(const std::vector<double>& vec) {
    return Eigen::VectorXd::Map(vec.data(), vec.size());
}

std::function<double(const std::vector<double> &, const std::vector<double> &)>
Metric::metric_string_to_function(const std::string & metric_name, const std::unordered_map<std::string, std::any> & config) {
    if (metric_name == "euclidean") {
        return Metric::euclidean;
    } else if (metric_name == "cosine") {
        return Metric::cosine;
    } else if (metric_name == "cosine_distance") {
        auto it = config.find("normalize_cosine");
        bool normalized = (it != config.end() && std::any_cast<bool>(it->second));
        return [normalized](const std::vector<double> & vec1, const std::vector<double> & vec2) {
            return Metric::cosine_distance(vec1, vec2, normalized);
        };
    } else {
        throw std::invalid_argument("Unknown metric name: " + metric_name);
    }
}

double
Metric::euclidean(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size()) {
        throw std::invalid_argument("Vectors must be of the same size for Euclidean distance.");
    }
    Eigen::VectorXd v1 = double_vector_to_eigen(vec1);
    Eigen::VectorXd v2 = double_vector_to_eigen(vec2);
    return (v1 - v2).norm();
}

double
Metric::cosine(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size()) {
        throw std::invalid_argument("Vectors must be of the same size for Cosine similarity.");
    }
    Eigen::VectorXd v1 = double_vector_to_eigen(vec1);
    Eigen::VectorXd v2 = double_vector_to_eigen(vec2);
    double denom = v1.norm() * v2.norm();
    if (denom == 0) {
        throw std::invalid_argument("One of the vectors is zero, cannot compute Cosine similarity.");
    }
    return v1.dot(v2) / denom;
}

double
Metric::cosine_distance(const std::vector<double>& vec1, const std::vector<double>& vec2, bool normalized) {
    double cosine_dis = 1.0 - cosine(vec1, vec2);
    return normalized ? cosine_dis / 2.0 : cosine_dis;
}

} // namespace ged

#endif /* SRC_UTIL_METRIC_IPP */
