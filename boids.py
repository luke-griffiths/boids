from build import pybindings as pb
import pygame 
from threading import Thread 
from timeit import default_timer as timer

BOID_SIZE = 3
NUM_BOIDS = 10 #320 is about the max single threaded cpp can support at 30fps
SCREEN_WIDTH = 1280
SCREEN_HEIGHT = 720
BOX_WIDTH = 300
BOX_HEIGHT = 500
FRAME_RATE = 30
PERIOD = 1 / FRAME_RATE #seconds
NUM_PERIODS_IN_AVG = 7


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


def getUsrInput() -> None:
  while True:
    command = input(">>")
    parseInput(command)
  return


def parseInput(s : str) -> None:
  try:
    #Preprocess the string
    s = s.lower()
    args = s.split()
    cmd = args[0]
    args = [float(arg) for arg in args[1:]]
    COMMANDS[cmd](*args)
  except KeyError as err:
    print(f"KeyError: The entered command is not recognized. {err}")
  except IndexError as err:
    print(f"IndexError: You must input a function with a valid number of args: {err}")
  except ValueError as err:
    print(f"Invalid argument type: {err}")
  return

  

def drawBoid(x : float, y : float, color : str) -> None:
  """
  Screen must already exist globally in order for this to not return an error
  """
  pygame.draw.rect(screen, color, (x, y, BOID_SIZE, BOID_SIZE))
  return


def drawBox() -> None:
  """
  Draws the region that the boids try to stay within
  """
  bounds = pb.getBoundaries()
  pygame.draw.line(screen, "red", (bounds[2], bounds[0]), (bounds[3], bounds[0])) #Top line
  pygame.draw.line(screen, "red", (bounds[2], bounds[1]), (bounds[3], bounds[1])) #bottom line
  pygame.draw.line(screen, "red", (bounds[2], bounds[0]), (bounds[2], bounds[1])) #left line
  pygame.draw.line(screen, "red", (bounds[3], bounds[0]), (bounds[3], bounds[1])) #right line
  return


# pygame setup
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
clock = pygame.time.Clock()
running = True

pb.setBoundaries(BOX_WIDTH, BOX_HEIGHT, SCREEN_WIDTH / 2 - BOX_WIDTH / 2, SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2)
flock = pb.spawn(NUM_BOIDS)

usrInputThread = Thread(target=getUsrInput, daemon=True)
usrInputThread.start()
periods = [0.0 for _ in range(NUM_PERIODS_IN_AVG)]
i = 0

while running:
    start = timer()
    # poll for events
    # pygame.QUIT event means the user clicked X to close your window
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
            
    # fill the screen with a color to wipe away anything from last frame
    screen.fill("white")

    drawBox()

    pb.update_flock(flock, 2)
    for boid in flock:
    #  pb.update_boid(boid, flock)
      drawBoid(boid.x, boid.y, "blue")

    #updates the screen
    pygame.display.flip()

    periods[i] = timer() - start
    i = (i + 1) % NUM_PERIODS_IN_AVG
    if sum(periods) / NUM_PERIODS_IN_AVG > PERIOD:
      msg = f'''Did not maintain {FRAME_RATE} fps. 
      Last {NUM_PERIODS_IN_AVG} frames took {[round(period, 3) for period in periods]} seconds.
      Should have averaged {round(PERIOD, 3)} s but instead averaged {round(sum(periods) / NUM_PERIODS_IN_AVG, 3)} s'''
      #raise RuntimeError(msg)
      print(msg)
    clock.tick(FRAME_RATE)

pb.delete_flock(flock)
pygame.quit()




