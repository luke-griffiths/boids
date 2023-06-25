# Boids
Boids is a simple flock simulation of bird-oid objects. You can find more info (and the algorithm) [here](https://vanhunteradams.com/Pico/Animal_Movement/Boids_Lab.html). I implemented an optimized version of the boids algorithm on an RP2040 microcontroller, but I can't post that code because it's currently still an assignment for [ECE 4760](https://ece4760.github.io/). So here's a (non-optimized) version of boids that can be run without any microcontroller/VGA hardware setup. I chose to handle input and visualization with python, but C++ performs the calculations for the boids algorithm. The optimized algorithm on the 2-core microcontroller was able to handle up to ~900 boids while maintaining 30 fps. This non-optimized version was still able to handle 2000 boids on an 8-core computer. 
## Setup

From the boids directory, create and activate a python environment. Get the required modules ```pip install -r requirements.txt```. Then create your build folder:
```
mkdir build
cd build
cmake ..
make
```
Add a blank ```__init__.py``` file to the build folder.
Now return to the boids directory and run boids.py

## Results
This video shows 2000 boids running at 30 fps. I added in color so you can see the boids each thread controls. YouTube doesn't display this very well, so if you really want to see it I recommend just cloning the repository and executing the steps above. It will look much smoother and clearer running in real time than it does on YouTube.

[![watch boids](https://img.youtube.com/vi/FF8Y4yHtews/0.jpg)](https://youtu.be/FF8Y4yHtews)

One cool observation I found when running on MacOS. If the flock you're updating is right at the limit of what your computer can support while maintaining your frame rate, you can actually see the impact of MacOS's scheduling algorithm on the boids program. MacOS uses a multi-level feedback queue to keep I/O from looking laggy, so any user input should steal priority from the boids algorithm threads. If you move the mouse around really fast you should see more messages printed to the terminal indicating that the boids program didn't maintain its timing requirements for the last frame. Pretty cool IMO. 
