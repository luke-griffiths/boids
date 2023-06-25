import pygame 
from threading import Thread 
from timeit import default_timer as timer

from build import pybindings as pb

BOID_SIZE = 3
NUM_BOIDS = 1500 #~320 is about the max single threaded cpp can support at 30fps
NUM_THREADS = 8
SCREEN_WIDTH = 1280
SCREEN_HEIGHT = 720
BOX_WIDTH = 300
BOX_HEIGHT = 500
FRAME_RATE = 30
PERIOD = 1 / FRAME_RATE #seconds

COLORS = ["blue", "red", "green"]

COMMANDS = {
  "setboundaries" : pb.setBoundaries,
  "setmaxspeed" : pb.setMaxSpeed,
  "setminspeed" : pb.setMinSpeed,
  "setturnfactor" : pb.setTurnFactor,
  "setprotectedrange" : pb.setProtectedRange,
  "setmatchingfactor" : pb.setMatchingFactor,
  "setcenteringfactor" : pb.setCenteringFactor,
  "setvisualrange" : pb.setVisualRange,
  "setavoidfactor" : pb.setAvoidFactor
}


### HELPER METHODS
def getUsrInput() -> None:
  while True:
    command = input(">>")
    parseInput(command)
  return


def parseInput(s : str) -> None:
  """
  Allows user to access C++ attributes controlling the boids behavior

  Input must be a valid command, and must contain the appropriate number of args
  """
  try:
    #Preprocess the string
    s = s.lower()
    args = s.split()
    cmd = args[0]
    args = [float(arg) for arg in args[1:]]
    COMMANDS[cmd](*args)
  except (KeyError, IndexError, ValueError) as err:
    print((f"KeyError: The entered command is not recognized or has the wrong number of arguments. {err}. \n"
    f"Valid commands: \n"
    f"{[key for key in COMMANDS]}"))
  return


def drawBoid(screen : int, x : float, y : float, color : str) -> None:
  pygame.draw.rect(screen, color, (x, y, BOID_SIZE, BOID_SIZE))
  return


def drawBox(screen) -> None:
  """
  Draws the region the boids stay within
  """
  bounds = pb.getBoundaries()
  pygame.draw.line(screen, "red", (bounds[2], bounds[0]), (bounds[3], bounds[0])) #Top line
  pygame.draw.line(screen, "red", (bounds[2], bounds[1]), (bounds[3], bounds[1])) #bottom line
  pygame.draw.line(screen, "red", (bounds[2], bounds[0]), (bounds[2], bounds[1])) #left line
  pygame.draw.line(screen, "red", (bounds[3], bounds[0]), (bounds[3], bounds[1])) #right line
  return


### END HELPER METHODS

#pygame setup
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
clock = pygame.time.Clock()
running = True
#setup the flock of boids
pb.setBoundaries(BOX_WIDTH, BOX_HEIGHT, SCREEN_WIDTH / 2 - BOX_WIDTH / 2, SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2)
flock = pb.spawn(NUM_BOIDS, NUM_THREADS, COLORS)
#run a thread to take input from the terminal. Will be abruptly killed when the pygame window is closed
usrInputThread = Thread(target=getUsrInput, daemon=True)
usrInputThread.start()

while running:
    start = timer()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False    
    #reset the screen
    screen.fill("white")
    drawBox(screen)
    #call multithreaded update algorithm
    pb.update_flock(flock, NUM_THREADS)

    for boid in flock:
      drawBoid(screen, boid.x, boid.y, boid.color)

    #updates the screen
    pygame.display.flip()

    frame_time = timer() - start
    if frame_time > PERIOD:
      #raise RuntimeError(..)
      print(f"Did not maintain {FRAME_RATE} fps. Frame took {round(frame_time, 4)} seconds but should have taken <{round(PERIOD, 4)}")
    clock.tick(FRAME_RATE)
#clean up C++ dynamically allocated memory
pb.delete_flock(flock)
pygame.quit()