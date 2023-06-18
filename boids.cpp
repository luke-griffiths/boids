#include <math.h>
#include <random>

#include "boids.hpp"

/*TODO:
  1. make the boid class more interesting, add private member variables
  2. clean up hunter's algorithm, maybe?
  3. Add r/w locks on boid update
  4. add threading and flock partition stuff. 
  5. fix the destruction of Boid objects */

double avoid_factor{ 0.05 };
double visual_range{ 40.0 };
double visual_range_squared{ visual_range * visual_range };
double centering_factor{ 0.0005 };
double matching_factor{ 0.05 };
double protected_range{ 8.000 };
double turn_factor{ 3.000 };
double minspeed{ 1.000 };
double maxspeed{ 4.000 };

double top = 200.0;
double bottom = top + 200.0;
double left = 300.0;
double right = left + 200.0;

Boid::Boid(double x, double y, double xv, double yv) : xcoord{ x }, ycoord{ y }, xvel{ xv }, yvel{ yv } {}

Boid::~Boid() {
  //std::cout << "Boid being destroyed." << std::endl;  bring this back later
}

std::vector<Boid*> spawn(int n) {
  std::vector<Boid*> flock;

  std::default_random_engine generator;
  std::uniform_real_distribution<double> x_distribution(0.0, 1000.0); //change this from 1000
  std::uniform_real_distribution<double> y_distribution(0.0, 700.0); //change this from 700.0
  std::uniform_real_distribution<double> vel_distribution(0.0, maxspeed);

  for (int i = 0; i < n; i++) {
    flock.push_back(new Boid(x_distribution(generator), y_distribution(generator), vel_distribution(generator), vel_distribution(generator)));
  }
  return flock;
}


void update_boid(Boid& boid, std::vector<Boid>& flock) {
  double dx{ 0 };
  double dy{ 0 };
  double xvel_avg = 0, yvel_avg = 0, neighbors = 0;
  double xpos_avg = 0, ypos_avg = 0;
  double close_dx = 0, close_dy = 0;
  //separation
  for (Boid b : flock) {
    dx += boid.xcoord - b.xcoord;
    dy += boid.ycoord - b.ycoord;
    if (std::abs(dx) < visual_range && std::abs(dy) < visual_range) {
      double dist = pow(dx, 2) + pow(dy, 2);
      if (dist < protected_range) {
        close_dx += boid.xcoord - b.xcoord;
        close_dy += boid.ycoord - b.ycoord;
      }
      else if (dist < visual_range_squared) {
        xpos_avg += b.xcoord;
        ypos_avg += b.ycoord;
        xvel_avg += b.xvel;
        yvel_avg += b.yvel;
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

void setAvoidFactor(double v) {
  avoid_factor = v;
}


void setVisualRange(double v) {
  visual_range = v;
  visual_range_squared = pow(visual_range, 2);
}


void setCenteringFactor(double v) {
  centering_factor = v;
}


void setMatchingFactor(double v) {
  matching_factor = v;
}


void setProtectedRange(double v) {
  protected_range = v;
}


void setTurnFactor(double v) {
  turn_factor = v;
}


void setMinSpeed(double v) {
  minspeed = v;
}


void setMaxSpeed(double v) {
  maxspeed = v;
}


void setBoundaries(double width, double height, double x_offset, double y_offset){
  top = y_offset;
  bottom = top + height;
  left = x_offset;
  right = left + width;
}


std::vector<double> getBoundaries(){
  return {top, bottom, left, right};
}
