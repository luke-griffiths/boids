#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "boids.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pybindings, m) {
  m.doc() = "Module providing an interface to the Boid objects and functions necessary to update the flock of Boids";
  
  py::class_<Boid>(m, "Boid")
    .def(py::init<double, double, double, double, std::string>()) //this is the constructor
    .def_readwrite("x", &Boid::xcoord) //note that I'm renaming xcoord to x in python
    .def_readwrite("y", &Boid::ycoord)
    .def_readwrite("xvel", &Boid::xvel)
    .def_readwrite("yvel", &Boid::yvel)
    .def_readwrite("color", &Boid::color);

  m.def("spawn", &spawn, py::return_value_policy::take_ownership, "Spawns a flock of boids");

  m.def("delete_flock", &delete_flock, "Frees memory by deleting the flock of boids");

  m.def("update_boid", &update_boid, "Updates the values of one boid");

  m.def("update_flock", &update_flock, "updates the flock using multiple threads");

  m.def("setAvoidFactor", &setAvoidFactor, "");
  m.def("setVisualRange", &setVisualRange, "");
  m.def("setCenteringFactor", &setCenteringFactor, "");
  m.def("setMatchingFactor", &setMatchingFactor, "");
  m.def("setProtectedRange", &setProtectedRange, "");
  m.def("setTurnFactor", &setTurnFactor, "");
  m.def("setMinSpeed", &setMinSpeed, "");
  m.def("setMaxSpeed", &setMaxSpeed, "");
  m.def("setBoundaries", &setBoundaries, "Sets the box size given W,H, and the screen offset");
  m.def("getBoundaries", &getBoundaries, "Gets the values of the bounding box");
}