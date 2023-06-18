#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "boids.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pybindings, m) {
  m.doc() = "where does this get printed?";

  py::class_<Boid>(m, "Boid")
    .def(py::init<double, double, double, double>()) //this is the constructor
    .def_readwrite("x", &Boid::xcoord) //note that I'm renaming xcoord to x in python
    .def_readwrite("y", &Boid::ycoord)
    .def_readwrite("xvel", &Boid::xvel)
    .def_readwrite("yvel", &Boid::yvel);

  m.def("spawn", &spawn, "Spawns a flock of boids");

  m.def("update_boid", &update_boid, "Updates the values of one boid");

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