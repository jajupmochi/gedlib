/*!
 * @file letter.ipp
 * @brief ged::Letter class definition.
 */

#ifndef SRC_EDIT_COSTS_LETTER_IPP_
#define SRC_EDIT_COSTS_LETTER_IPP_

namespace ged {

template<>
Letter<GXLLabel, GXLLabel>::
~Letter() {}

template<>
Letter<GXLLabel, GXLLabel>::
Letter(double node_ins_del_cost, double edge_ins_del_cost, double alpha) :
node_ins_del_cost_{node_ins_del_cost},
edge_ins_del_cost_{edge_ins_del_cost},
alpha_{alpha} {}

template<>
double
Letter<GXLLabel, GXLLabel>::
node_ins_cost_fun(const GXLLabel & node_label) const {
	return alpha_ * node_ins_del_cost_;
}

template<>
double
Letter<GXLLabel, GXLLabel>::
node_del_cost_fun(const GXLLabel & node_label) const {
	return alpha_ * node_ins_del_cost_;
}

template<>
double
Letter<GXLLabel, GXLLabel>::
node_rel_cost_fun(const GXLLabel & node_label_1, const GXLLabel & node_label_2) const {
	double x_l_minus_x_r(std::stod(node_label_1.at("x")) - std::stod(node_label_2.at("x")));
	double y_l_minus_y_r(std::stod(node_label_1.at("y")) - std::stod(node_label_2.at("y")));
	return alpha_ * std::sqrt(std::pow(x_l_minus_x_r, 2) + std::pow(y_l_minus_y_r, 2));
}

template<>
double
Letter<GXLLabel, GXLLabel>::
edge_ins_cost_fun(const GXLLabel & edge_label) const {
	return (1 - alpha_) * edge_ins_del_cost_;
}

template<>
double
Letter<GXLLabel, GXLLabel>::
edge_del_cost_fun(const GXLLabel & edge_label) const {
	return (1 - alpha_) * edge_ins_del_cost_;
}

template<>
double
Letter<GXLLabel, GXLLabel>::
edge_rel_cost_fun(const GXLLabel & edge_label_1, const GXLLabel & edge_label_2) const {
	return 0.0;
}

}

#endif /* SRC_EDIT_COSTS_LETTER_IPP_ */
