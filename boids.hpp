#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <vector>

class Boid {
public:
  Boid(double x, double y, double xv, double yv, std::string c);
  ~Boid();
  //make these private later
  double xcoord;
  double ycoord;
  double xvel;
  double yvel;
  std::string color;
};


/*Spawns a flock of n Boids */
std::vector<Boid*> spawn(int n);

void delete_flock(std::vector<Boid*> flock);

/* Updates the position and velocity of one Boid */
void update_boid(Boid& boid, std::vector<Boid*>& flock);

/* Updates the entire flock using multithreading */
void update_flock(std::vector<Boid*> flock, int num_threads);

//these will update the global variables
void setAvoidFactor(double v);
void setVisualRange(double v);
void setCenteringFactor(double v);
void setMatchingFactor(double v);
void setProtectedRange(double v);
void setTurnFactor(double v);
void setMinSpeed(double v);
void setMaxSpeed(double v);
void setBoundaries(double width, double height, double x_offset, double y_offset);
std::vector<double> getBoundaries();

#endif