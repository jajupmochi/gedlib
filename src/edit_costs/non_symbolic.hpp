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
 * @file non_symbolic.hpp
 * @brief ged::NonSymbolic class declaration.
 */

#ifndef SRC_EDIT_COSTS_NON_SYMBOLIC_HPP_
#define SRC_EDIT_COSTS_NON_SYMBOLIC_HPP_

#include "edit_costs.hpp"

namespace ged {

/*!
 * @brief Edit costs for graphs containing only non-symbolic labels.
 * @details This cost is designed for graphs containing only non-symbolic labels. Examples of such datasets include: COIL-RAG, COLORS-3, Fingerprint, FRANKENSTEIN, Letter-high, Letter-low, Letter-med, SYNTHETIC, SYNTHETICnew, Synthie, TWITTER-Real-Graph-Partial, which can be downloaded from https://ls11-www.cs.tu-dortmund.de/staff/morris/graphkerneldatasets:
 * - Kristian Kersting, Nils M. Kriege, Christopher Morris, Petra Mutzel, Marion Neumann:
 *   Benchmark Data Sets for Graph Kernels,
 *   http://graphkernels.cs.tu-dortmund.de
 * 
 * Nodes are attributed with non-symbolic labels, all of which are used to compute node replace costs.
 * Edges are attributed with non-symbolic labels, all of which are used to compute node replace costs.
 *
 * Implements a generalized version of the edit costs suggested in:
 * - K. Riesen, H. Bunke:
 *   Graph data, in: *Graph Classification and Clustering Based on Vector Space Embedding*,
 *   https://doi.org/10.1142/9789814304726_0004
 * In this implementation, costs of node/edge ins/del/rel are set separately.
 */
template<class UserNodeLabel, class UserEdgeLabel>
class NonSymbolic : public EditCosts<UserNodeLabel, UserEdgeLabel> {
public:

	virtual ~NonSymbolic();

	/*!
	 * @brief Constructor.
	 * @param[in] node_ins_cost
	 * @param[in] node_del_cost
	 * @param[in] node_rel_cost
	 * @param[in] edge_ins_cost
	 * @param[in] edge_del_cost
	 * @param[in] edge_del_cost
	 * @note When attributes of nodes and/or edges do not exist, set <tt>node_rel_cost = 0</tt> and/or <tt>edge_rel_cost = 0</tt> respectively, the corresponding function will return 0 accordingly. The default values of all edit cost constants are set to constant 1.
	 */
	NonSymbolic(double node_ins_cost = 1, double node_del_cost = 1, double node_rel_cost = 1, double edge_ins_cost = 1, double edge_del_cost = 1, double edge_rel_cost = 1);

	virtual double node_ins_cost_fun(const UserNodeLabel & node_label) const final;

	virtual double node_del_cost_fun(const UserNodeLabel & node_label) const final;

	virtual double node_rel_cost_fun(const UserNodeLabel & node_label_1, const UserNodeLabel & node_label_2) const final;

	virtual UserNodeLabel median_node_label(const std::vector<UserNodeLabel> & node_labels) const final;

	virtual UserEdgeLabel median_edge_label(const std::vector<UserEdgeLabel> & edge_labels) const final;

	virtual double edge_ins_cost_fun(const UserEdgeLabel & edge_label) const final;

	virtual double edge_del_cost_fun(const UserEdgeLabel & edge_label) const final;

	virtual double edge_rel_cost_fun(const UserEdgeLabel & edge_label_1, const UserEdgeLabel & edge_label_2) const final;

private:

	double node_ins_cost_;

	double node_del_cost_;

	double node_rel_cost_;

	double edge_ins_cost_;

	double edge_del_cost_;

	double edge_rel_cost_;
};

}

#include "non_symbolic.ipp"

#endif /* SRC_EDIT_COSTS_NON_SYMBOLIC_HPP_ */
