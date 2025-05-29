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
 * @file geometric.hpp
 * @brief ged::Geometric class declaration.
 */

#ifndef SRC_EDIT_COSTS_GEOMETRIC_HPP_
#define SRC_EDIT_COSTS_GEOMETRIC_HPP_

#include "edit_costs.hpp"
#include "../util/metric.hpp"
#include "../util/misc.hpp"
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <any>

namespace ged {

/*!
 * @brief Edit costs for graphs containing mixed node and edge attributes.
 * @details This cost is designed for graphs containing mixed node and edge attributes. 
 * It is suitable for graphs representing n-dimensional geometric data, such as point clouds, handwriting image graphs, diagram graphs, etc.
 * Examples of such datasets include: 
 * - Unipen Offline Handwriting Database: - todo: complete the link -
 * - Histograph Handwriting Database: - http://www.histograph.ch/ - todo: add link to the new Washington handwriting database created by Linlin Jia -
 * 
 * Nodes are attributed with the following attributes:
 * - "label": std::string
 *     symbolic labels (e.g., character labels, class labels, etc.)
 * - "coord": std::vector<double>
 *     coordinates in n-dimensional space (e.g., 2D or 3D coordinates)
 * - "embed": std::vector<double>
 *     embeddings in n-dimensional space (e.g., embeddings from a neural network)
 * All of them are used to compute node replace costs.
 *
 * Edges are attributed with the following attributes:
 * - "label": std::string
 *     symbolic labels (e.g., character labels, class labels, etc.)
 * - "weight": std::vector<double>
 *     weight of the edge (e.g., distance between two points in n-dimensional space). Notice that this should be a vector of doubles, not a single double value.
 * - "embed": std::vector<double>
 *     embeddings in n-dimensional space (e.g., embeddings from a neural network)
 * All of them are used to compute edge replace costs.
 *
 * Implements of the edit costs suggested in:
 * - Linlin Jia, - todo: add the paper -
 */
template<class UserNodeLabel, class UserEdgeLabel>
class Geometric : public EditCosts<UserNodeLabel, UserEdgeLabel> {
public:

	virtual ~Geometric();


	Geometric();

	/*!
	 * @brief Constructor.
	 * @param[in] node_ins_cost
	 * @param[in] node_del_cost
	 * @param[in] node_rel_cost Cost of replacing a node. <tt>node_label_cost</tt>, <tt>node_coord_cost</tt>, and <tt>node_embed_cost</tt> will all be set to <tt>node_rel_cost</tt>.
	 * @param[in] edge_ins_cost
	 * @param[in] edge_del_cost
	 * @param[in] edge_del_cost Cost of replacing an edge. <tt>edge_label_cost</tt>, <tt>edge_weight_cost</tt>, and <tt>edge_embed_cost</tt> will all be set to <tt>edge_rel_cost</tt>.
	 * @note When attributes of nodes and/or edges do not exist, set <tt>node_rel_cost = 0</tt> and/or <tt>edge_rel_cost = 0</tt> respectively, 
	 * the corresponding function will return 0 accordingly. The default values of all edit cost constants are set to constant 1.
	 */
	Geometric(double node_ins_cost, double node_del_cost, double node_rel_cost, double edge_ins_cost, double edge_del_cost, double edge_rel_cost);

	/*!
	 * @brief Constructor.
	 * @param[in] node_ins_cost Cost of inserting a node.
	 * @param[in] node_del_cost Cost of deleting a node.
	 * @param[in] edge_ins_cost Cost of inserting an edge.	
	 * @param[in] edge_del_cost Cost of deleting an edge.
	 * @param[in] node_label_cost Cost of replacing a node label.
	 * @param[in] node_coord_cost Cost of replacing a node coordinate.
	 * @param[in] node_embed_cost Cost of replacing a node embedding.
	 * @param[in] edge_label_cost Cost of replacing an edge label.
	 * @param[in] edge_weight_cost Cost of replacing an edge weight.
	 * @param[in] edge_embed_cost Cost of replacing an edge embedding.
	 * @note - When attributes of nodes and/or edges do not exist, set <tt>node_label_cost = 0</tt>, <tt>node_coord_cost = 0</tt>, <tt>node_embed_cost = 0</tt>, 
	 * <tt>edge_label_cost = 0</tt>, <tt>edge_weight_cost = 0</tt>, and/or <tt>edge_embed_cost = 0</tt> respectively, the corresponding function will return 0 accordingly. 
	 * The default values of all edit cost constants are set to constant 1.
	 * - Be careful, the replacement costs are at the end of the arguments, which is different from the 6 constants version!!
	 */
	Geometric(
		double node_ins_cost, double node_del_cost, double edge_ins_cost, double edge_del_cost, 
		double node_label_cost, double node_coord_cost, double node_embed_cost, 
		double edge_label_cost, double edge_weight_cost, double edge_embed_cost
	);

	/*!
	 * @brief Sets the configuration of the edit costs.
	 * @param[in] config A map containing the configuration options.
	 * @details The following options are supported:
	 * - "node_coord_metric": Metric for node coordinates (default: "euclidean").
	 * - "node_embed_metric": Metric for node embeddings (default: "cosine_distance").
	 * - "edge_weight_metric": Metric for edge weights (default: "euclidean").
	 * - "edge_embed_metric": Metric for edge embeddings (default: "cosine_distance").
	 */
	virtual void set_config(const std::unordered_map<std::string, std::any> & config) final;

	virtual double node_ins_cost_fun(const UserNodeLabel & node_label) const final;

	virtual double node_del_cost_fun(const UserNodeLabel & node_label) const final;

	virtual double node_rel_cost_fun(const UserNodeLabel & node_label_1, const UserNodeLabel & node_label_2) const final;

	// virtual UserNodeLabel median_node_label(const std::vector<UserNodeLabel> & node_labels) const final;

	// virtual UserEdgeLabel median_edge_label(const std::vector<UserEdgeLabel> & edge_labels) const final;

	virtual double edge_ins_cost_fun(const UserEdgeLabel & edge_label) const final;

	virtual double edge_del_cost_fun(const UserEdgeLabel & edge_label) const final;

	virtual double edge_rel_cost_fun(const UserEdgeLabel & edge_label_1, const UserEdgeLabel & edge_label_2) const final;

	virtual std::string safe_get_metric_name(const std::unordered_map<std::string, std::any> & name_map, const std::string & name_key) const final;
	
private:

	double node_ins_cost_;

	double node_del_cost_;

	double edge_ins_cost_;

	double edge_del_cost_;

	double node_label_cost_;

	double node_coord_cost_;

	double node_embed_cost_;

	double edge_label_cost_;

	double edge_weight_cost_;

	double edge_embed_cost_;

	bool initialized_ = false;

	std::unordered_map<std::string, std::any> config_ = {
		{"node_coord_metric", "euclidean"},
		{"node_embed_metric", "cosine_distance"},
		{"edge_weight_metric", "euclidean"},
		{"edge_embed_metric", "cosine_distance"}
	};

	std::function<double(const std::vector<double> &, const std::vector<double> &)> node_coord_metric_;
	std::function<double(const std::vector<double> &, const std::vector<double> &)> node_embed_metric_;
	std::function<double(const std::vector<double> &, const std::vector<double> &)> edge_weight_metric_;
	std::function<double(const std::vector<double> &, const std::vector<double> &)> edge_embed_metric_;
};

}

#include "geometric.ipp"

#endif /* SRC_EDIT_COSTS_GEOMETRIC_HPP_ */
