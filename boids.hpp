#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <vector>
//#include <iostream>
//#include <thread>
//#include <shared_mutex>
//#include <condition_variable>
class Boid {
public:
  Boid(double x, double y, double xv, double yv);
  ~Boid();
  //make these private later
  double xcoord;
  double ycoord;
  double xvel;
  double yvel;
};


/*Spawns a flock of n Boids */
std::vector<Boid*> spawn(int n);

/* Updates the position and velocity of one Boid */
void update_boid(Boid& boid, std::vector<Boid>& flock);

//these will update the global variables
void setAvoidFactor(double v);
void setVisualRange(double v); //note this also updates the squared
void setCenteringFactor(double v);
void setMatchingFactor(double v);
void setProtectedRange(double v);
void setTurnFactor(double v);
void setMinSpeed(double v);
void setMaxSpeed(double v);
void setBoundaries(double width, double height, double x_offset, double y_offset);
std::vector<double> getBoundaries();

#endif

