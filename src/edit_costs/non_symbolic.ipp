/***************************************************************************
 *                                                                          *
 *   Copyright (C) 2020 by David B. Blumenthal and Linlin Jia               *
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
 * @file non_symbolic.ipp
 * @brief ged::NonSymbolic class definition.
 */

#ifndef SRC_EDIT_COSTS_NON_SYMBOLIC_IPP_
#define SRC_EDIT_COSTS_NON_SYMBOLIC_IPP_

namespace ged {

template<>
NonSymbolic<GXLLabel, GXLLabel>::
~NonSymbolic() {}

template<>
NonSymbolic<GXLLabel, GXLLabel>::
NonSymbolic(double node_ins_cost, double node_del_cost, double node_rel_cost, double edge_ins_cost, double edge_del_cost, double edge_rel_cost) :
node_ins_cost_{node_ins_cost},
node_del_cost_{node_del_cost},
node_rel_cost_{node_rel_cost},
edge_ins_cost_{edge_ins_cost},
edge_del_cost_{edge_del_cost},
edge_rel_cost_{edge_rel_cost} {}

template<>
double
NonSymbolic<GXLLabel, GXLLabel>::
node_ins_cost_fun(const GXLLabel & node_label) const {
	return node_ins_cost_;
}

template<>
double
NonSymbolic<GXLLabel, GXLLabel>::
node_del_cost_fun(const GXLLabel & node_label) const {
	return node_del_cost_;
}

template<>
double
NonSymbolic<GXLLabel, GXLLabel>::
node_rel_cost_fun(const GXLLabel & node_label_1, const GXLLabel & node_label_2) const {
	if (node_rel_cost_ == 0) {
		return 0.0;
	} else {
		double l_minus_r;
		double sum_square{0};
		for (const auto & kv : node_label_1) {
			l_minus_r = std::stod(kv.second) - std::stod(node_label_2.at(kv.first));
			sum_square += std::pow(l_minus_r, 2);
		}
		return node_rel_cost_ * std::sqrt(sum_square);
	}
}

template<>
GXLLabel
NonSymbolic<GXLLabel, GXLLabel>::
median_node_label(const std::vector<GXLLabel> & node_labels) const {
	// std::cout << "size:" << node_labels.size() << endl;
	// int i = 0;
	// for (const auto & node_label : node_labels) {
	// 	i += 1;
	// 	std::cout << i << endl;
	// 	for (const auto & kv : node_label) {
	// 		std::cout << "key:" << kv.first << ", value:" << kv.second << endl;
	// 	}
	// }

	if (static_cast<int>(node_labels.size()) == 0) {
		ged::GXLLabel median_label;
		return median_label;
	} else {
		// Transform the labels into coordinates and compute mean label as initial solution.
		std::vector<std::map<std::string, double>> node_labels_as_coords;
		std::map<std::string, double> sums;
		for (const auto & kv : node_labels.at(0)) {
			sums[kv.first] = 0;
		}
		for (const auto & node_label : node_labels) {
			std::map<std::string, double> coords;
			for (const auto & kv : node_label) {
				double label = std::stod(kv.second);
				sums[kv.first] += label;
				coords[kv.first] = label;
			}
			node_labels_as_coords.emplace_back(coords);
		}
		std::map<std::string, double> median;
		for (const auto & kv : sums) {
			median[kv.first] = kv.second / static_cast<double>(node_labels.size());
		}

		// Run main loop of Weiszfeld's Algorithm.
		double epsilon{0.0001};
		double delta{1.0};
		std::size_t num_itrs{0};
		bool all_equal{false};
		while ((delta > epsilon) and (num_itrs++ < 100) and (not all_equal)) {
			std::map<std::string, double> numerator;
			for (const auto & kv : sums) {
				numerator[kv.first] = 0;
			}
			double denominator{0};
			for (const auto & node_label_as_coord : node_labels_as_coords) {
				double norm{0};
				for (const auto & kv : node_label_as_coord) {
					norm += (kv.second - median.at(kv.first)) * (kv.second - median.at(kv.first));
				}
				norm = std::sqrt(norm);
				if (norm > 0) {
					for (const auto & kv : node_label_as_coord) {
						numerator[kv.first] += kv.second / norm;
					}
					denominator += 1.0 / norm;
				}
			}
			if (denominator == 0) {
				all_equal = true;
			}
			else {
				std::map<std::string, double> new_median;
				delta = 0.0;
				for (const auto & kv : numerator) {
					double this_median = kv.second / denominator;
					new_median[kv.first] = this_median;
					delta += std::abs(median.at(kv.first) - this_median);
				}
				median = new_median;
			}
		}

		//  Transform the solution to ged::GXLLabel and return it.
		ged::GXLLabel median_label;
		for (const auto & kv : median) {
			median_label[kv.first] = std::to_string(kv.second);
		}
		return median_label;
	}
}

template<>
double
NonSymbolic<GXLLabel, GXLLabel>::
edge_ins_cost_fun(const GXLLabel & edge_label) const {
	return edge_ins_cost_;
}

template<>
double
NonSymbolic<GXLLabel, GXLLabel>::
edge_del_cost_fun(const GXLLabel & edge_label) const {
	return edge_del_cost_;
}

template<>
double
NonSymbolic<GXLLabel, GXLLabel>::
edge_rel_cost_fun(const GXLLabel & edge_label_1, const GXLLabel & edge_label_2) const {
	if (edge_rel_cost_ == 0) {
		return 0.0;
	} else {
		double l_minus_r;
		double sum_square{0};
		for (const auto & kv : edge_label_1) {
			l_minus_r = std::stod(kv.second) - std::stod(edge_label_2.at(kv.first));
			sum_square += std::pow(l_minus_r, 2);
		}
		return edge_rel_cost_ * std::sqrt(sum_square);
	}
}

template<>
GXLLabel
NonSymbolic<GXLLabel, GXLLabel>::
median_edge_label(const std::vector<GXLLabel> & edge_labels) const {
	if (static_cast<int>(edge_labels.size()) == 0) {
		ged::GXLLabel median_label;
		return median_label;
	} else {
		// Transform the labels into coordinates and compute mean label as initial solution.
		std::vector<std::map<std::string, double>> edge_labels_as_coords;
		std::map<std::string, double> sums;
		for (const auto & kv : edge_labels.at(0)) {
			sums[kv.first] = 0;
		}
		for (const auto & edge_label : edge_labels) {
			std::map<std::string, double> coords;
			for (const auto & kv : edge_label) {
				double label = std::stod(kv.second);
				sums[kv.first] += label;
				coords[kv.first] = label;
			}
			edge_labels_as_coords.emplace_back(coords);
		}
		std::map<std::string, double> median;
		for (const auto & kv : sums) {
			median[kv.first] = kv.second / static_cast<double>(edge_labels.size());
		}

		// Run main loop of Weiszfeld's Algorithm.
		double epsilon{0.0001};
		double delta{1.0};
		std::size_t num_itrs{0};
		bool all_equal{false};
		while ((delta > epsilon) and (num_itrs++ < 100) and (not all_equal)) {
			std::map<std::string, double> numerator;
			for (const auto & kv : sums) {
				numerator[kv.first] = 0;
			}
			double denominator{0};
			for (const auto & edge_label_as_coord : edge_labels_as_coords) {
				double norm{0};
				for (const auto & kv : edge_label_as_coord) {
					norm += (kv.second - median.at(kv.first)) * (kv.second - median.at(kv.first));
				}
				norm = std::sqrt(norm);
				if (norm > 0) {
					for (const auto & kv : edge_label_as_coord) {
						numerator[kv.first] += kv.second / norm;
					}
					denominator += 1.0 / norm;
				}
			}
			if (denominator == 0) {
				all_equal = true;
			}
			else {
				std::map<std::string, double> new_median;
				delta = 0.0;
				for (const auto & kv : numerator) {
					double this_median = kv.second / denominator;
					new_median[kv.first] = this_median;
					delta += std::abs(median.at(kv.first) - this_median);
				}
				median = new_median;
			}
		}

		//  Transform the solution to ged::GXLLabel and return it.
		ged::GXLLabel median_label;
		for (const auto & kv : median) {
			median_label[kv.first] = std::to_string(kv.second);
		}
		return median_label;
	}
}

}

#endif /* SRC_EDIT_COSTS_NON_SYMBOLIC_IPP_ */
