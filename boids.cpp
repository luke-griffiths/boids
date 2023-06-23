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


Boid::Boid(double x, double y, double xv, double yv, std::string c) : xcoord{ x }, ycoord{ y }, xvel{ xv }, yvel{ yv }, color{c} {
  std::cout << "boid being created" << std::endl;
}

Boid::~Boid() {
  std::cout << "Boid being destroyed." << std::endl;
}

std::vector<Boid*> spawn(int n) {
  std::vector<Boid*> flock;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> x_distribution(0.0, 1000.0); //change this from 1000
  std::uniform_real_distribution<double> y_distribution(0.0, 700.0); //change this from 700.0
  std::uniform_real_distribution<double> vel_distribution(0.0, maxspeed);
  for (int i = 0; i < n; i++) {
    flock.push_back(new Boid(x_distribution(generator), y_distribution(generator), vel_distribution(generator), vel_distribution(generator), "blue"));
  }
  return flock;
}

void delete_flock(std::vector<Boid*> flock){
  for(Boid* b : flock){
    delete b;
  }
  return;
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

std::vector<std::string> colors = {"blue", "red", "green", "black", "cyan", "magenta"};

void partition(std::vector<Boid*> flock, const int start_index, const int end_index, const std::string& color){
  for(int i = start_index; i < end_index; i++){
    update_boid(*flock[i], flock);
  }
  return;
}

void update_flock(std::vector<Boid*> flock, int num_threads){
  int partition_size = flock.size() / num_threads;
  //create a threadpool of num_threads
  //for(int t = 0; t < 1; t++){
    //std::thread worker(partition, flock, t * partition_size, t * partition_size + partition_size,  colors[num_threads % colors.size()]);
    std::thread worker(partition, std::ref(flock), 0, flock.size(),  colors[num_threads % colors.size()]);

    worker.join();
  //}
  return;
}























void setAvoidFactor(double v) {
  avoid_factor = v;
  std::cout << "avoidFactor is now " << v << std::endl;
}


void setVisualRange(double v) {
  visual_range = v;
  visual_range_squared = pow(visual_range, 2);
  std::cout << "visualRange is now " << v << std::endl;
}


void setCenteringFactor(double v) {
  centering_factor = v;
  std::cout << "centeringFactor is now " << v << std::endl;
}


void setMatchingFactor(double v) {
  matching_factor = v;
  std::cout << "matchingFactor is now " << v << std::endl;
}


void setProtectedRange(double v) {
  protected_range = v;
  std::cout << "protectedRange is now " << v << std::endl;
}


void setTurnFactor(double v) {
  turn_factor = v;
  std::cout << "turnFactor is now " << v << std::endl;
}


void setMinSpeed(double v) {
  minspeed = v;
  std::cout << "minSpeed is now " << v << std::endl;
}


void setMaxSpeed(double v) {
  maxspeed = v;
  std::cout << "maxSpeed is now " << v << std::endl;
}


void setBoundaries(double width, double height, double x_offset, double y_offset){
  top = y_offset;
  bottom = top + height;
  left = x_offset;
  right = left + width;
  std::cout << "Box is now specified by (" << left << ", " << top << ") and (" << right << ", " << bottom << ")" << std::endl;
}


std::vector<double> getBoundaries(){
  return {top, bottom, left, right};
}
