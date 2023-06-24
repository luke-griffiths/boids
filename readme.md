# Implementing 1,000,000 Boids

## Setup

In the boids folder, create a python environment using ```python3 -m venv env```. 
Activate the environment using ```source env/bin/activate``` (MacOS) or ```env\usr\bin\activate.bat``` (Windows).

Get the requirements from pip: ```pip install -r requirements.txt```

Then create your build folder:
```mkdir build
cd build
cmake ..
make```

Now return to the boids directory and run ```python3 boids.py```


Crazy cool observation! When running on MacOS near the frame rate limit with number of threads saturated, you should be able to see the impact of the MLFQ 
Moving the mouse should cause more (frame rate not maintained) warnings. the more the mouse moves the more you see it.
