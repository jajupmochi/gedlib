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
 * @file letter_2.hpp
 * @brief ged::Letter2 class declaration.
 */

#ifndef SRC_EDIT_COSTS_LETTER_2_HPP_
#define SRC_EDIT_COSTS_LETTER_2_HPP_

#include "edit_costs.hpp"

namespace ged {

/*!
 * @brief Edit costs for graphs contained in Letter datasets.
 * @details The graphs contained in the Letter datasets represent the capital letters A, E, F, H, I, K, L, M, N, T, V, W, X, Y, and Z which have been distorted by three different degrees (low, medium, high).
 * Nodes are attributed with Euclidean coordinates (named "x" and "y").
 * Edges are have no attributes.
 * The Letter datasets are contained in the IAM graph database repository which can be downloaded from http://www.fki.inf.unibe.ch/databases/iam-graph-database:
 * - K. Riesen, H. Bunke:
 *   IAM graph database repository for graph based pattern recognition and machine learning,
 *   https://doi.org/10.1007/978-3-540-89689-0_33
 *
 * Implements a generalized version of the edit costs suggested in:
 * - K. Riesen, H. Bunke:
 *   Graph data, in: *Graph Classification and Clustering Based on Vector Space Embedding*,
 *   https://doi.org/10.1142/9789814304726_0004
 * In this implementation, costs of node/edge ins/del and node rel are set seperatedly.
 */
template<class UserNodeLabel, class UserEdgeLabel>
class Letter2 : public EditCosts<UserNodeLabel, UserEdgeLabel> {
public:

	virtual ~Letter2();

	/*!
	 * @brief Constructor.
	 * @param[in] node_ins_cost
	 * @param[in] node_del_cost
	 * @param[in] node_rel_cost
	 * @param[in] edge_ins_cost
	 * @param[in] edge_del_cost
	 * @param[in] alpha Importance of node edit operations vs. importance of edge edit operations.
	 * @note Calling the constructor with the default arguments constructs the edit costs for Letter high suggested in https://doi.org/10.1142/9789814304726_0004.
	 * For Letter medium, the suggested arguments are <tt>node_ins_del_cost = 0.7</tt>, <tt>edge_ins_del_cost = 1.9</tt>, and <tt>alpha = 0.75</tt>.
	 * For Letter low, the suggested arguments are <tt>node_ins_del_cost = 0.3</tt>, <tt>edge_ins_del_cost = 0.1</tt>, and <tt>alpha = 0.25</tt>.
	 */
	Letter2(double node_ins_cost = 0.675, double node_del_cost = 0.675, double node_rel_cost = 0.75, double edge_ins_cost = 0.425, double edge_del_cost = 0.425);

	virtual double node_ins_cost_fun(const UserNodeLabel & node_label) const final;

	virtual double node_del_cost_fun(const UserNodeLabel & node_label) const final;

	virtual double node_rel_cost_fun(const UserNodeLabel & node_label_1, const UserNodeLabel & node_label_2) const final;

	virtual UserNodeLabel median_node_label(const std::vector<UserNodeLabel> & node_labels) const final;

	virtual double edge_ins_cost_fun(const UserEdgeLabel & edge_label) const final;

	virtual double edge_del_cost_fun(const UserEdgeLabel & edge_label) const final;

	virtual double edge_rel_cost_fun(const UserEdgeLabel & edge_label_1, const UserEdgeLabel & edge_label_2) const final;

private:

	double node_ins_cost_;

	double node_del_cost_;

	double node_rel_cost_;

	double edge_ins_cost_;

	double edge_del_cost_;
};

}

#include "letter_2.ipp"

#endif /* SRC_EDIT_COSTS_LETTER_2_HPP_ */
