#include "GL/glfw3.h"
#include "glhck/glhck.h"

#include "ew/engine.h"
#include "ew/integration/glfwcontrolcontext.h"
#include "ew/integration/glfwtimecontext.h"
#include "ew/integration/glhckglfwrendercontext.h"
#include "gamestate.h"

#include <cstdlib>
#include <random>

int const WIDTH = 800;
int const HEIGHT = 480;

int windowCloseCallback(GLFWwindow* window);
void windowResizeCallback(GLFWwindow* window, int width, int height);
int gameloop(GLFWwindow* window);

int main(int argc, char** argv)
{
  if (!glfwInit())
    return EXIT_FAILURE;

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2dshooter", NULL, NULL);
  glfwMakeContextCurrent(window);

  if(!window)
    return EXIT_FAILURE;

  glfwSwapInterval(1);
  glfwSetWindowCloseCallback(window, windowCloseCallback);

  if(!glhckContextCreate(argc, argv))
    return EXIT_FAILURE;

  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_AUTO))
    return EXIT_FAILURE;

  int retval = gameloop(window);

  glhckContextTerminate();
  glfwTerminate();

  return retval;
}


int windowCloseCallback(GLFWwindow* window)
{
  ew::Engine* engine = static_cast<ew::Engine*>(glfwGetWindowUserPointer(window));
  engine->quit();
  return GL_FALSE;
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
  glhckDisplayResize(width, height);
}

int gameloop(GLFWwindow* window)
{
  GLFWControlContext controlContext(window);
  GlhckGLFWRenderContext renderContext(window);
  GLFWTimeContext timeContext;
  ew::Engine engine(&controlContext, &renderContext, &timeContext);

  glfwSetWindowUserPointer(window, &engine);
  glfwSetWindowSizeCallback(window, windowResizeCallback);

  GameState game(&engine);
  engine.addState(0, &game);
  engine.setState(0);

  glhckMemoryGraph();

  engine.run();

  return EXIT_SUCCESS;
}
