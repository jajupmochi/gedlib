/***************************************************************************
 *                                                                          *
 *   Copyright (C) 2025 by Linlin Jia               *
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
 * @file geometric.ipp
 * @brief ged::Geometric class definition.
 */

#ifndef SRC_EDIT_COSTS_GEOMETRIC_IPP_
#define SRC_EDIT_COSTS_GEOMETRIC_IPP_

#include <any>
#include <string>

namespace ged {

template<>
Geometric<ged::AttrLabel, ged::AttrLabel>::
~Geometric() {}

template<>
Geometric<ged::AttrLabel, ged::AttrLabel>::
Geometric() : 
node_ins_cost_{1.0}, node_del_cost_{1.0}, edge_ins_cost_{1.0}, edge_del_cost_{1.0},
node_label_cost_{1.0}, node_coord_cost_{1.0}, node_embed_cost_{1.0},
edge_label_cost_{1.0}, edge_weight_cost_{1.0}, edge_embed_cost_{1.0}
{}

template<>
Geometric<ged::AttrLabel, ged::AttrLabel>::
Geometric(double node_ins_cost, double node_del_cost, double node_rel_cost, double edge_ins_cost, double edge_del_cost, double edge_rel_cost) :
node_ins_cost_{node_ins_cost},
node_del_cost_{node_del_cost},
edge_ins_cost_{edge_ins_cost},
edge_del_cost_{edge_del_cost},
node_label_cost_{node_rel_cost},
node_coord_cost_{node_rel_cost},
node_embed_cost_{node_rel_cost},
edge_label_cost_{edge_rel_cost},
edge_weight_cost_{edge_rel_cost},
edge_embed_cost_{edge_rel_cost}
{}

template<>
Geometric<ged::AttrLabel, ged::AttrLabel>::
Geometric(
	double node_ins_cost, double node_del_cost, double edge_ins_cost, double edge_del_cost, 
	double node_label_cost, double node_coord_cost, double node_embed_cost, 
	double edge_label_cost, double edge_weight_cost, double edge_embed_cost
) :
node_ins_cost_{node_ins_cost},
node_del_cost_{node_del_cost},
edge_ins_cost_{edge_ins_cost},
edge_del_cost_{edge_del_cost},			
node_label_cost_{node_label_cost},
node_coord_cost_{node_coord_cost},
node_embed_cost_{node_embed_cost},
edge_label_cost_{edge_label_cost},
edge_weight_cost_{edge_weight_cost},
edge_embed_cost_{edge_embed_cost}
{}

template<>
std::string
Geometric<ged::AttrLabel, ged::AttrLabel>::
safe_get_metric_name(const std::unordered_map<std::string, std::any> & name_map, const std::string & name_key) const {
	auto it = name_map.find(name_key);
	if (it != name_map.end()) {
		try {
			// Check if any has a value:
			if (!it->second.has_value()) {
				throw std::invalid_argument("No value is given to the metric name for key '" + name_key + "' in the configuration map.");
			}
		
			std::string metric_name;

			if (it->second.type() == typeid(std::string)) {
				// If the value is a string, cast it to string:
				metric_name = std::any_cast<std::string>(it->second);
			} else if (it->second.type() == typeid(const char *)) {
				// If the value is a const char *, cast it to string:
				metric_name = std::string(std::any_cast<const char *>(it->second));
			} else {
				throw std::invalid_argument("The value for key '" + name_key + "' in the configuration map is not a string or const char *.");
			}

			if (metric_name.empty()) {
				throw std::invalid_argument("The metric name for key '" + name_key + "' is empty.");
			} 

			return metric_name;
		} catch (const std::bad_any_cast & e) {
			throw std::invalid_argument("Failed to cast the value for key '" + name_key + "' in the configuration map to a string: " + std::string(e.what()));
		} // end try
	} else {
		throw std::invalid_argument("Metric name for key '" + name_key + "' not found in the configuration map.");
	} // end if
}
		  
template<>
void
Geometric<ged::AttrLabel, ged::AttrLabel>::
set_config(const std::unordered_map<std::string, std::any> & config) {
	// Set the configuration map, replace the values of the metrics if they are provided in the config:
	for (const auto & kv : config) {
		config_[kv.first] = kv.second;
	}

	std::string node_coord_metric_name = safe_get_metric_name(config_, "node_coord_metric");
	node_coord_metric_ = ged::Metric::metric_string_to_function(node_coord_metric_name, config_);

	std::string node_embed_metric_name = safe_get_metric_name(config_, "node_embed_metric");
	node_embed_metric_ = ged::Metric::metric_string_to_function(node_embed_metric_name, config_);

	std::string edge_weight_metric_name = safe_get_metric_name(config_, "edge_weight_metric");
	edge_weight_metric_ = ged::Metric::metric_string_to_function(edge_weight_metric_name, config_);

	std::string edge_embed_metric_name = safe_get_metric_name(config_, "edge_embed_metric");
	edge_embed_metric_ = ged::Metric::metric_string_to_function(edge_embed_metric_name, config_);

	initialized_ = true;
}

template<>
double
Geometric<ged::AttrLabel, ged::AttrLabel>::
node_ins_cost_fun(const ged::AttrLabel & node_label) const {
	return node_ins_cost_;
}

template<>
double
Geometric<ged::AttrLabel, ged::AttrLabel>::
node_del_cost_fun(const ged::AttrLabel & node_label) const {
	return node_del_cost_;
}

template<>
double
Geometric<ged::AttrLabel, ged::AttrLabel>::
node_rel_cost_fun(const ged::AttrLabel & node_label_1, const ged::AttrLabel & node_label_2) const {
	double cost = 0.0;

	// Calculate the coordinates costs:
	if (node_coord_cost_ != 0) {
		auto coords_1_opt = ged::util::get_attr_label_value<std::vector<double>>(node_label_1, "coords");
		auto coords_2_opt = ged::util::get_attr_label_value<std::vector<double>>(node_label_2, "coords");

		if (coords_1_opt && coords_2_opt) {
			const std::vector<double> & coords_1 = coords_1_opt->get();
			const std::vector<double> & coords_2 = coords_2_opt->get();
			cost += node_coord_metric_(coords_1, coords_2) * node_coord_cost_;
		}
	}

	// Calculate the embedding costs:
	if (node_embed_cost_ != 0) {
		auto embedding_1_opt = ged::util::get_attr_label_value<std::vector<double>>(node_label_1, "embedding");
		auto embedding_2_opt = ged::util::get_attr_label_value<std::vector<double>>(node_label_2, "embedding");

		if (embedding_1_opt && embedding_2_opt) {
			const std::vector<double> & embedding_1 = embedding_1_opt->get();
			const std::vector<double> & embedding_2 = embedding_2_opt->get();
			cost += node_embed_metric_(embedding_1, embedding_2) * node_embed_cost_;
		}
	}

	// Calculate the label costs:
	if (node_label_cost_ != 0) {
		auto label_1_opt = ged::util::get_attr_label_value<std::string>(node_label_1, "label");
		auto label_2_opt = ged::util::get_attr_label_value<std::string>(node_label_2, "label");
		if (label_1_opt && label_2_opt) {
			const std::string & label_1 = label_1_opt->get();
			const std::string & label_2 = label_2_opt->get();
			if (label_1 != label_2) {
				// If the labels are different, add a cost:
				cost += node_label_cost_;
			}
		}
	}

	return cost;
}

// template<>
// ged::AttrLabel
// Geometric<ged::AttrLabel, ged::AttrLabel>::
// median_node_label(const std::vector<ged::AttrLabel> & node_labels) const {
// 	// std::cout << "size:" << node_labels.size() << endl;
// 	// int i = 0;
// 	// for (const auto & node_label : node_labels) {
// 	// 	i += 1;
// 	// 	std::cout << i << endl;
// 	// 	for (const auto & kv : node_label) {
// 	// 		std::cout << "key:" << kv.first << ", value:" << kv.second << endl;
// 	// 	}
// 	// }

// 	if (static_cast<int>(node_labels.size()) == 0) {
// 		ged::ged::AttrLabel median_label;
// 		return median_label;
// 	} else {
// 		// Transform the labels into coordinates and compute mean label as initial solution.
// 		std::vector<std::map<std::string, double>> node_labels_as_coords;
// 		std::map<std::string, double> sums;
// 		for (const auto & kv : node_labels.at(0)) {
// 			sums[kv.first] = 0;
// 		}
// 		for (const auto & node_label : node_labels) {
// 			std::map<std::string, double> coords;
// 			for (const auto & kv : node_label) {
// 				double label = std::stod(kv.second);
// 				sums[kv.first] += label;
// 				coords[kv.first] = label;
// 			}
// 			node_labels_as_coords.emplace_back(coords);
// 		}
// 		std::map<std::string, double> median;
// 		for (const auto & kv : sums) {
// 			median[kv.first] = kv.second / static_cast<double>(node_labels.size());
// 		}

// 		// Run main loop of Weiszfeld's Algorithm.
// 		double epsilon{0.0001};
// 		double delta{1.0};
// 		std::size_t num_itrs{0};
// 		bool all_equal{false};
// 		while ((delta > epsilon) and (num_itrs++ < 100) and (not all_equal)) {
// 			std::map<std::string, double> numerator;
// 			for (const auto & kv : sums) {
// 				numerator[kv.first] = 0;
// 			}
// 			double denominator{0};
// 			for (const auto & node_label_as_coord : node_labels_as_coords) {
// 				double norm{0};
// 				for (const auto & kv : node_label_as_coord) {
// 					norm += (kv.second - median.at(kv.first)) * (kv.second - median.at(kv.first));
// 				}
// 				norm = std::sqrt(norm);
// 				if (norm > 0) {
// 					for (const auto & kv : node_label_as_coord) {
// 						numerator[kv.first] += kv.second / norm;
// 					}
// 					denominator += 1.0 / norm;
// 				}
// 			}
// 			if (denominator == 0) {
// 				all_equal = true;
// 			}
// 			else {
// 				std::map<std::string, double> new_median;
// 				delta = 0.0;
// 				for (const auto & kv : numerator) {
// 					double this_median = kv.second / denominator;
// 					new_median[kv.first] = this_median;
// 					delta += std::abs(median.at(kv.first) - this_median);
// 				}
// 				median = new_median;
// 			}
// 		}

// 		//  Transform the solution to ged::AttrLabel and return it.
// 		ged::AttrLabel median_label;
// 		for (const auto & kv : median) {
// 			median_label[kv.first] = std::to_string(kv.second);
// 		}
// 		return median_label;
// 	}
// }

template<>
double
Geometric<ged::AttrLabel, ged::AttrLabel>::
edge_ins_cost_fun(const ged::AttrLabel & edge_label) const {
	return edge_ins_cost_;
}

template<>
double
Geometric<ged::AttrLabel, ged::AttrLabel>::
edge_del_cost_fun(const ged::AttrLabel & edge_label) const {
	return edge_del_cost_;
}

template<>
double
Geometric<ged::AttrLabel, ged::AttrLabel>::
edge_rel_cost_fun(const ged::AttrLabel & edge_label_1, const ged::AttrLabel & edge_label_2) const {
	double cost = 0.0;
	// Calculate the weights costs:
	if (edge_weight_cost_ != 0) {
		auto weight_1_opt = ged::util::get_attr_label_value<std::vector<double>>(edge_label_1, "weight");
		auto weight_2_opt = ged::util::get_attr_label_value<std::vector<double>>(edge_label_2, "weight");

		if (weight_1_opt && weight_2_opt) {
			const std::vector<double> & weight_1 = weight_1_opt->get();
			const std::vector<double> & weight_2 = weight_2_opt->get();
			cost += edge_weight_metric_(weight_1, weight_2) * edge_weight_cost_;
		}
	}

	// Calculate the embedding costs:
	if (edge_embed_cost_ != 0) {
		auto embedding_1_opt = ged::util::get_attr_label_value<std::vector<double>>(edge_label_1, "embedding");
		auto embedding_2_opt = ged::util::get_attr_label_value<std::vector<double>>(edge_label_2, "embedding");
		
		if (embedding_1_opt && embedding_2_opt) {
			const std::vector<double> & embedding_1 = embedding_1_opt->get();
			const std::vector<double> & embedding_2 = embedding_2_opt->get();
			cost += edge_embed_metric_(embedding_1, embedding_2) * edge_embed_cost_;
		}
	}

	// Calculate the label costs:
	if (edge_label_cost_ != 0) {
		auto label_1_opt = ged::util::get_attr_label_value<std::string>(edge_label_1, "label");
		auto label_2_opt = ged::util::get_attr_label_value<std::string>(edge_label_2, "label");
		if (label_1_opt && label_2_opt) {
			// If both labels have "label", use the label cost.
			const std::string & label_1 = label_1_opt->get();
			const std::string & label_2 = label_2_opt->get();
			if (label_1 != label_2) {
				// If the labels are different, add a cost.
				cost += edge_label_cost_;
			}
		}
	}

	return cost;
}

// template<>
// ged::AttrLabel
// Geometric<ged::AttrLabel, ged::AttrLabel>::
// median_edge_label(const std::vector<ged::AttrLabel> & edge_labels) const {
// 	if (static_cast<int>(edge_labels.size()) == 0) {
// 		ged::AttrLabel median_label;
// 		return median_label;
// 	} else {
// 		// Transform the labels into coordinates and compute mean label as initial solution.
// 		std::vector<std::map<std::string, double>> edge_labels_as_coords;
// 		std::map<std::string, double> sums;
// 		for (const auto & kv : edge_labels.at(0)) {
// 			sums[kv.first] = 0;
// 		}
// 		for (const auto & edge_label : edge_labels) {
// 			std::map<std::string, double> coords;
// 			for (const auto & kv : edge_label) {
// 				double label = std::stod(kv.second);
// 				sums[kv.first] += label;
// 				coords[kv.first] = label;
// 			}
// 			edge_labels_as_coords.emplace_back(coords);
// 		}
// 		std::map<std::string, double> median;
// 		for (const auto & kv : sums) {
// 			median[kv.first] = kv.second / static_cast<double>(edge_labels.size());
// 		}

// 		// Run main loop of Weiszfeld's Algorithm.
// 		double epsilon{0.0001};
// 		double delta{1.0};
// 		std::size_t num_itrs{0};
// 		bool all_equal{false};
// 		while ((delta > epsilon) and (num_itrs++ < 100) and (not all_equal)) {
// 			std::map<std::string, double> numerator;
// 			for (const auto & kv : sums) {
// 				numerator[kv.first] = 0;
// 			}
// 			double denominator{0};
// 			for (const auto & edge_label_as_coord : edge_labels_as_coords) {
// 				double norm{0};
// 				for (const auto & kv : edge_label_as_coord) {
// 					norm += (kv.second - median.at(kv.first)) * (kv.second - median.at(kv.first));
// 				}
// 				norm = std::sqrt(norm);
// 				if (norm > 0) {
// 					for (const auto & kv : edge_label_as_coord) {
// 						numerator[kv.first] += kv.second / norm;
// 					}
// 					denominator += 1.0 / norm;
// 				}
// 			}
// 			if (denominator == 0) {
// 				all_equal = true;
// 			}
// 			else {
// 				std::map<std::string, double> new_median;
// 				delta = 0.0;
// 				for (const auto & kv : numerator) {
// 					double this_median = kv.second / denominator;
// 					new_median[kv.first] = this_median;
// 					delta += std::abs(median.at(kv.first) - this_median);
// 				}
// 				median = new_median;
// 			}
// 		}

// 		//  Transform the solution to ged::AttrLabel and return it.
// 		ged::AttrLabel median_label;
// 		for (const auto & kv : median) {
// 			median_label[kv.first] = std::to_string(kv.second);
// 		}
// 		return median_label;
// 	}
// }

}

#endif /* SRC_EDIT_COSTS_GEOMETRIC_IPP_ */
