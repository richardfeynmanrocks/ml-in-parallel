//
//  pybind.cpp
//  Jacobian
//
//  Created by David Freifeld
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/eigen.h>

#include "bpnn.hpp"
#include "utils.hpp"
namespace py = pybind11;

PYBIND11_MODULE(_jacobian, m)
{
	m.doc() = "Fast machine learning in C++"; // optional module docstring
	py::enum_<Regularization>(m, "Regularization")
		.value("L1", Regularization::L1)
		.value("L2", Regularization::L2)
		.export_values();
	py::class_<Layer>(m, "Layer")
		.def(py::init<int, int>())
		.def("get_contents", &Layer::get_contents)
		.def("get_weights", &Layer::get_weights)
		.def("get_bias", &Layer::get_bias)
		.def("get_v", &Layer::get_v)
		.def("get_m", &Layer::get_m)
		.def("get_dZ", &Layer::get_dZ)
		.def_readonly("activation", &Layer::activation)
		.def_readonly("activation_deriv", &Layer::activation);
	py::class_<Network>(m, "Network")
		.def(py::init<char *, int, float, float, Regularization, float,
				  float, bool, float>(),
			 py::arg("path"), py::arg("batch"), py::arg("learn_rate"),
			 py::arg("bias_rate"), py::arg("regularization"),
			 py::arg("lambda"), py::arg("ratio"),
			 py::arg("early_exit") = true, py::arg("cutoff") = 0)
		.def("add_layer", &Network::add_layer, py::arg("nodes"),
			 py::arg("activation"), py::arg("activation_deriv"))
		.def("initialize", &Network::initialize)
		.def("init_optimizer", &Network::init_optimizer, py::arg("optimizer"))
		.def("init_decay", &Network::init_decay, py::arg("decay"))
		.def("set_activation", &Network::set_activation,
			 py::arg("index"), py::arg("custom"),
			 py::arg("custom_deriv"))
		.def("feedforward", &Network::feedforward)
		.def("backpropagate", &Network::backpropagate)
		.def("list_net", &Network::list_net)
		.def("next_batch", &Network::interactive_next_batch)
		.def("cost", &Network::cost)
		.def("accuracy", &Network::accuracy)
		.def("train", &Network::train)
		.def("get_cost", &Network::get_cost)
		.def("get_acc", &Network::get_acc)
		.def("get_val_cost", &Network::get_val_cost)
		.def("get_val_acc", &Network::get_val_acc)
		.def_readonly("layers", &Network::layers);
	auto a = m.def_submodule("activations", "Submodule supplying built-in activation functions.");
	a.def("linear", &linear, py::arg("x"));
	a.def("linear_deriv", &linear_deriv, py::arg("x"));
	a.def("sigmoid", &sigmoid, py::arg("x"));
	a.def("sigmoid_deriv", &sigmoid_deriv, py::arg("x"));
	a.def("lecun_tanh", &lecun_tanh, py::arg("x"));
	a.def("lecun_tanh_deriv", &lecun_tanh_deriv, py::arg("x"));
	a.def("softplus", &softplus, py::arg("x"));
	a.def("softplus_deriv", &softplus_deriv, py::arg("x"));
	a.def("inverse_logit", &inverse_logit, py::arg("x"));
	a.def("inverse_logit_deriv", &inverse_logit_deriv, py::arg("x"));
	a.def("cloglog", &cloglog, py::arg("x"));
	a.def("cloglog_deriv", &cloglog_deriv, py::arg("x"));
	a.def("bipolar", &bipolar, py::arg("x"));
	a.def("bipolar_deriv", &bipolar_deriv, py::arg("x"));
	a.def("step", &step, py::arg("x"));
	a.def("step_deriv", &step_deriv, py::arg("x"));
	a.def("hard_tanh", &hard_tanh, py::arg("x"));
	a.def("hard_tanh_deriv", &hard_tanh_deriv, py::arg("x"));
	a.def("leaky_relu", &leaky_relu, py::arg("x"));
	a.def("leaky_relu_deriv", &leaky_relu_deriv, py::arg("x"));
	a.def("relu", (rectifier(linear)), py::arg("x"));
	a.def("relu_deriv", (rectifier(linear_deriv)), py::arg("x"));
	auto o = m.def_submodule("optimizers", "Submodule supplying built-in gradient descent optimizers.");
	o.def("momentum", &optimizers::momentum, py::arg("beta"));
	o.def("demon", &optimizers::demon, py::arg("beta"), py::arg("max_ep"));
	o.def("adam", &optimizers::adam, py::arg("beta1"), py::arg("beta2"), py::arg("epsilon"));
	o.def("adamax", &optimizers::adamax, py::arg("beta1"), py::arg("beta2"), py::arg("epsilon"));
	auto d = m.def_submodule("decays", "Submodule supplying built-in learning rate annealing functions.");
	d.def("step", &decays::step, py::arg("a_0"), py::arg("k"));
	d.def("exponential", &decays::exponential, py::arg("a_0"), py::arg("k"));
	d.def("fractional", &decays::fractional, py::arg("a_0"), py::arg("k"));
	d.def("linear", &decays::linear, py::arg("max_ep"));
}
