#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "boids.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pybindings, m) {
  m.doc() = "Module providing an interface to the Boid objects and functions necessary to update the flock of Boids";
  
  py::class_<Boid>(m, "Boid")
    .def(py::init<double, double, double, double, std::string>()) //this is the constructor
    .def_readwrite("x", &Boid::xcoord) //renaming xcoord to x in python
    .def_readwrite("y", &Boid::ycoord)
    .def_readwrite("xvel", &Boid::xvel)
    .def_readwrite("yvel", &Boid::yvel)
    .def_readwrite("color", &Boid::color);

  //this return policy gives c++ responsibility for calling destructors
  m.def("spawn", &spawn, py::return_value_policy::reference, "Spawns a flock of boids");

  m.def("delete_flock", &delete_flock, "Frees memory by deleting the flock of boids");

  m.def("update_flock", &update_flock, "updates the flock using multiple threads");

  m.def("setAvoidFactor", &setAvoidFactor);
  m.def("setVisualRange", &setVisualRange);
  m.def("setCenteringFactor", &setCenteringFactor);
  m.def("setMatchingFactor", &setMatchingFactor);
  m.def("setProtectedRange", &setProtectedRange);
  m.def("setTurnFactor", &setTurnFactor);
  m.def("setMinSpeed", &setMinSpeed);
  m.def("setMaxSpeed", &setMaxSpeed);
  m.def("setBoundaries", &setBoundaries, "Sets the box size given W,H, and the screen offset");
  m.def("getBoundaries", &getBoundaries, "Gets the values of the bounding box");
}