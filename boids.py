from build import pybindings as pb
import pygame 

BOID_SIZE = 6
NUM_BOIDS = 30
SCREEN_WIDTH = 1280
SCREEN_HEIGHT = 720

def drawBoid(x : float, y : float, color : str) -> None:
  """
  Screen must already exist globally in order for this to not return an error
  """
  pygame.draw.circle(screen, color, (x, y), BOID_SIZE)
  return


def drawArena() -> None:
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
dt = 0


pb.setBoundaries(500.0, 300.0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
flock = pb.spawn(NUM_BOIDS)

while running:
    # poll for events
    # pygame.QUIT event means the user clicked X to close your window
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # fill the screen with a color to wipe away anything from last frame
    screen.fill("white")

    drawArena() # for now, later get this from the cpp file 


    for boid in flock:
      pb.update_boid(boid, flock)
      

    for boid in flock:
      drawBoid(boid.x, boid.y, "blue")

    #updates the screen
    pygame.display.flip()

    # limits FPS to 60
    # dt is delta time in seconds since last frame, used for framerate-
    # independent physics.
    #this is good for now, but later I will want to print out the spare time and throw an error if the frame rate doesn't 
    #keep up with the 30fps requirement
    dt = clock.tick(30) / 1000

pygame.quit()
