#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <vector>

class Boid {
public:
  Boid(double x, double y, double xv, double yv, std::string c);
  ~Boid();
  double xcoord;
  double ycoord;
  double xvel;
  double yvel;
  std::string color;
};

/* Spawns a flock of n Boids into equal partitions of the same color */
std::vector<Boid*> spawn(const int n, const int num_threads, const std::vector<std::string> colors);

/* Deletes the dynamically allocated flock of boids */
void delete_flock(std::vector<Boid*>& flock);

/* Updates the position and velocity of one Boid */
void update_boid(Boid& boid, std::vector<Boid*>& flock);

/* Function passed to each thread specifying the [start, end) indices of boids it must update */
void update_partition(std::vector<Boid*>& flock, const int start, const int end);

/* Helper function to partition a vector into num_partitions partitions of ~equal size*/
std::vector<std::vector<int>> partition(const int num_elements, const int num_partitions);

/* Creates num_threads worker threads to update the entire flock of boids.

Note that this function knowingly has a read/write race condition. Each thread writes to a separate 
subset of the flock, so there will never be a write/write collision; however, one thread could potentially
read the values of one boid while another thread is updating them. This would result in a garbage value for
the thread reading the boid. This isn't a problem because the garbage value gets filtered out when the speed
limits are enforced on the boid. Additionally, the alternative of using a r/w lock would have a huge performance 
cost on the algorithm which is less favorable than a boid moving slightly off course for one frame */
void update_flock(std::vector<Boid*>& flock, const int num_threads); //ref

//these will update the global variables
void setAvoidFactor(const double v);
void setVisualRange(const double v);
void setCenteringFactor(const double v);
void setMatchingFactor(const double v);
void setProtectedRange(const double v);
void setTurnFactor(const double v);
void setMinSpeed(const double v);
void setMaxSpeed(const double v);
void setBoundaries(const double width, const double height, const double x_offset, const double y_offset);
std::vector<double> getBoundaries();

#endif