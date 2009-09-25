# a sample with ruby-opengl and Ruby/SDL
require 'sdl'
require 'gl'

include Gl

# initialize SDL and opengl
SDL.init SDL::INIT_VIDEO
SDL::GL.set_attr SDL::GL_RED_SIZE,5
SDL::GL.set_attr SDL::GL_GREEN_SIZE,5
SDL::GL.set_attr SDL::GL_BLUE_SIZE,5
SDL::GL.set_attr SDL::GL_DEPTH_SIZE,16
SDL::GL.set_attr SDL::GL_DOUBLEBUFFER,1
SDL::Screen.open 640,400,16,SDL::OPENGL

glViewport( 0, 0, 640, 400 );
glMatrixMode( GL_PROJECTION );
glLoadIdentity( );

glMatrixMode( GL_MODELVIEW );
glLoadIdentity( );

glEnable(GL_DEPTH_TEST);

glDepthFunc(GL_LESS);

glShadeModel(GL_SMOOTH);

shadedCube=true

color =
  [[ 1.0,  1.0,  0.0], 
  [ 1.0,  0.0,  0.0],
  [ 0.0,  0.0,  0.0],
  [ 0.0,  1.0,  0.0],
  [ 0.0,  1.0,  1.0],
  [ 1.0,  1.0,  1.0],
  [ 1.0,  0.0,  1.0],
  [ 0.0,  0.0,  1.0]]

cube =
  [[ 0.5,  0.5, -0.5], 
  [ 0.5, -0.5, -0.5],
  [-0.5, -0.5, -0.5],
  [-0.5,  0.5, -0.5],
  [-0.5,  0.5,  0.5],
  [ 0.5,  0.5,  0.5],
  [ 0.5, -0.5,  0.5],
  [-0.5, -0.5,  0.5]]


loop do

  while event = SDL::Event2.poll
    case event
    when SDL::Event2::Quit, SDL::Event2::KeyDown
      exit
    end
  end

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


  glBegin(GL_QUADS) 

  if shadedCube then
    glColor(color[0]);
    glVertex(cube[0]);
    glColor(color[1]);
    glVertex(cube[1]);
    glColor(color[2]);
    glVertex(cube[2]);
    glColor(color[3]);
    glVertex(cube[3]);
    
    glColor(color[3]);
    glVertex(cube[3]);
    glColor(color[4]);
    glVertex(cube[4]);
    glColor(color[7]);
    glVertex(cube[7]);
    glColor(color[2]);
    glVertex(cube[2]);
    
    glColor(color[0]);
    glVertex(cube[0]);
    glColor(color[5]);
    glVertex(cube[5]);
    glColor(color[6]);
    glVertex(cube[6]);
    glColor(color[1]);
    glVertex(cube[1]);
    
    glColor(color[5]);
    glVertex(cube[5]);
    glColor(color[4]);
    glVertex(cube[4]);
    glColor(color[7]);
    glVertex(cube[7]);
    glColor(color[6]);
    glVertex(cube[6]);
    
    glColor(color[5]);
    glVertex(cube[5]);
    glColor(color[0]);
    glVertex(cube[0]);
    glColor(color[3]);
    glVertex(cube[3]);
    glColor(color[4]);
    glVertex(cube[4]);
    
    glColor(color[6]);
    glVertex(cube[6]);
    glColor(color[1]);
    glVertex(cube[1]);
    glColor(color[2]);
    glVertex(cube[2]);
    glColor(color[7]);
    glVertex(cube[7]);
    
  else
    glColor(1.0, 0.0, 0.0);
    glVertex(cube[0]);
    glVertex(cube[1]);
    glVertex(cube[2]);
    glVertex(cube[3]);
    
    glColor(0.0, 1.0, 0.0);
    glVertex(cube[3]);
    glVertex(cube[4]);
    glVertex(cube[7]);
    glVertex(cube[2]);
    
    glColor(0.0, 0.0, 1.0);
    glVertex(cube[0]);
    glVertex(cube[5]);
    glVertex(cube[6]);
    glVertex(cube[1]);
    
    glColor(0.0, 1.0, 1.0);
    glVertex(cube[5]);
    glVertex(cube[4]);
    glVertex(cube[7]);
    glVertex(cube[6]);
    
    glColor(1.0, 1.0, 0.0);
    glVertex(cube[5]);
    glVertex(cube[0]);
    glVertex(cube[3]);
    glVertex(cube[4]);
    
    glColor(1.0, 0.0, 1.0);
    glVertex(cube[6]);
    glVertex(cube[1]);
    glVertex(cube[2]);
    glVertex(cube[7]);
    
  end

  glEnd()
  
  glMatrixMode(GL_MODELVIEW);
  glRotate(5.0, 1.0, 1.0, 1.0);
  
  SDL::GL.swap_buffers

end
