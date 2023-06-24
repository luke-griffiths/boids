#include <math.h>
#include <random>
#include <iostream>
#include <thread>

#include "boids.hpp"

double avoid_factor{ 0.60 };
double visual_range{ 200.00 };
double visual_range_squared{ visual_range * visual_range };
double centering_factor{ 0.002 };
double matching_factor{ 0.02 };
double protected_range{ 150.000 };
double turn_factor{ 0.150 };
double minspeed{ 2.20 };
double maxspeed{ 6.000 };

double top = 200.0;
double bottom = top + 200.0;
double left = 300.0;
double right = left + 200.0;


Boid::Boid(double x, double y, double xv, double yv, std::string c) : xcoord{ x }, ycoord{ y }, xvel{ xv }, yvel{ yv }, color{c} {}


Boid::~Boid() {
  //used to ensure memory management policies are followed after python takes ownership
  //std::cout << "Boid being destroyed." << std::endl;
}

std::vector<Boid*> spawn(const int n, const int num_threads, const std::vector<std::string> colors) {
  if(n < 1 || num_threads < 1 || n < num_threads){
    throw std::invalid_argument("Number of threads cannot exceed number of boids");
  }
  std::vector<Boid*> flock;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> x_distribution(0.0, 1280.0);
  std::uniform_real_distribution<double> y_distribution(0.0, 720.0);
  std::uniform_real_distribution<double> vel_distribution(0.0, maxspeed);
  std::vector<std::vector<int>> partitions = partition(n, num_threads);
  for(int p = 0; p < partitions.size(); p++){
    std::string color = colors[p % colors.size()];
    for(int i = partitions[p][0]; i < partitions[p][1]; i++){
      flock.push_back(new Boid(x_distribution(generator), y_distribution(generator), vel_distribution(generator), vel_distribution(generator), color));
    }
  }
  return flock;
}


void delete_flock(std::vector<Boid*>& flock){
  for(Boid* b : flock) delete b;
}


void update_boid(Boid& boid, std::vector<Boid*>& flock) {
  double dx{ 0 };
  double dy{ 0 };
  double xvel_avg = 0, yvel_avg = 0, neighbors = 0;
  double xpos_avg = 0, ypos_avg = 0;
  double close_dx = 0, close_dy = 0;
  //separation
  for (Boid* b : flock) {
    dx += boid.xcoord - b->xcoord;
    dy += boid.ycoord - b->ycoord;
    if (std::abs(dx) < visual_range && std::abs(dy) < visual_range) {
      double dist = pow(dx, 2) + pow(dy, 2);
      if (dist < protected_range) {
        close_dx += boid.xcoord - b->xcoord;
        close_dy += boid.ycoord - b->ycoord;
      }
      else if (dist < visual_range_squared) {
        xpos_avg += b->xcoord;
        ypos_avg += b->ycoord;
        xvel_avg += b->xvel;
        yvel_avg += b->yvel;
        neighbors++;
      }
    }
  }
  if (neighbors) {
    xpos_avg /= neighbors;
    ypos_avg /= neighbors;
    xvel_avg /= neighbors;
    yvel_avg /= neighbors;
    boid.xvel += (xpos_avg - boid.xcoord) * centering_factor + (xvel_avg - boid.xvel) * matching_factor;
    boid.yvel += (ypos_avg - boid.ycoord) * centering_factor + (yvel_avg - boid.yvel) * matching_factor;
  }
  boid.xvel += close_dx * avoid_factor;
  boid.yvel += close_dy * avoid_factor;
  if (boid.ycoord < top) boid.yvel += turn_factor;
  if (boid.ycoord > bottom) boid.yvel -= turn_factor;
  if (boid.xcoord > right)boid.xvel -= turn_factor;
  if (boid.xcoord < left) boid.xvel += turn_factor;
  //enforce speed limitations
  double speed = hypot(boid.xvel, boid.yvel);
  if (speed < minspeed) {
    boid.xvel *= (minspeed / speed);
    boid.yvel *= (minspeed / speed);
  }
  else if (speed > maxspeed) {
    boid.xvel *= (maxspeed / speed);
    boid.yvel *= (maxspeed / speed);
  }
  boid.xcoord += boid.xvel;
  boid.ycoord += boid.yvel;
}


void update_partition(std::vector<Boid*>& flock, const int start, const int end){
  for(int i = start; i < end; i++){
    update_boid(*flock[i], flock);
  }
}


void update_flock(std::vector<Boid*>& flock, const int num_threads){
  std::vector<std::vector<int>> partitions = partition(flock.size(), num_threads);
  for(int t = 0; t < num_threads; t++){
    //flock is passed by reference, so must be wrapped with std::ref to be passed to a thread
    std::thread worker(update_partition, std::ref(flock), partitions[t][0], partitions[t][1]);
    worker.join();
  }
}


std::vector<std::vector<int>> partition(const int num_elements, const int num_partitions){
  int partition_size{num_elements / num_partitions};
  int leftover{num_elements - partition_size * num_partitions};
  int start{0};
  int end{partition_size};
  std::vector<std::vector<int>> partition_indices;
  while(end <= num_elements){
    partition_indices.push_back({start, end});
    if(leftover){
      partition_indices.back()[1]++;
      leftover--;
    } 
    start = partition_indices.back()[1];
    end = start + partition_size;
  }
  return partition_indices;
}


void setAvoidFactor(const double v) {
  avoid_factor = v;
  std::cout << "avoidFactor is now " << v << std::endl;
}


void setVisualRange(const double v) {
  visual_range = v;
  visual_range_squared = pow(visual_range, 2);
  std::cout << "visualRange is now " << v << std::endl;
}


void setCenteringFactor(const double v) {
  centering_factor = v;
  std::cout << "centeringFactor is now " << v << std::endl;
}


void setMatchingFactor(const double v) {
  matching_factor = v;
  std::cout << "matchingFactor is now " << v << std::endl;
}


void setProtectedRange(const double v) {
  protected_range = v;
  std::cout << "protectedRange is now " << v << std::endl;
}


void setTurnFactor(const double v) {
  turn_factor = v;
  std::cout << "turnFactor is now " << v << std::endl;
}


void setMinSpeed(const double v) {
  minspeed = v;
  std::cout << "minSpeed is now " << v << std::endl;
}


void setMaxSpeed(const double v) {
  maxspeed = v;
  std::cout << "maxSpeed is now " << v << std::endl;
}


void setBoundaries(const double width, const double height, const double x_offset, const double y_offset){
  top = y_offset;
  bottom = top + height;
  left = x_offset;
  right = left + width;
  std::cout << "Box is now specified by (" << left << ", " << top << ") and (" << right << ", " << bottom << ")" << std::endl;
}


std::vector<double> getBoundaries(){
  return {top, bottom, left, right};
}
