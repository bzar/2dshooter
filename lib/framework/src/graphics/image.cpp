#include "image.h"
#include "../util/log.h"
#include <SDL/SDL_image.h>

Image::Image(std::string const& filename, bool const loadImmediately) : texture(new TextureInformation())
{
  texture->filename = filename;
  if(loadImmediately)
  {
    load();
  }
}

Image::Image(Image const& other) : texture(other.texture)
{
  texture->refs += 1;
}

Image::~Image()
{
  texture->refs -= 1;
  if(texture->refs <= 0)
  {
    Log::info(std::string("Deleting texture ") + texture->filename);
    glDeleteTextures(1, &(texture->id));
    delete texture;
  }
}

void Image::load()
{
  if(!texture->loaded)
  {
    Log::info(std::string("Loading texture ") + texture->filename);
    SDL_Surface* img = IMG_Load(texture->filename.c_str());
    texture->width = img->w;
    texture->height = img->h;
    texture->quad = Quad(0, 0, img->w, img->h);
    glGenTextures(1, &(texture->id));
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(img);  
    texture->loaded = true;
  }
}

void Image::render(Quad const& target) const
{
  if(!texture->loaded || !target.boundingRect().intersectsWith(Rect(-1, -1, 2, 2)))
  {
    return;
  }
  
 
  int const QUAD_COORD_NUM = 8;
  GLfloat box[QUAD_COORD_NUM];
  target.coordArray(box);
  GLfloat tex[] = {0,1, 0,0, 1,1, 1,0};

  glBindTexture(GL_TEXTURE_2D, texture->id);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  glVertexPointer(2, GL_FLOAT, 0, box);
  glTexCoordPointer(2, GL_FLOAT, 0, tex);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Quad const& Image::quad() const
{
  return texture->quad;
}
unsigned int Image::width() const
{
  return texture->width;
}

unsigned int Image::height() const
{
  return texture->height;
}
