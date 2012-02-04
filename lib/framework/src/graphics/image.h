#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "../platform/gl.h"
#include "../util/quad.h"

class Image {
public:
  Image(std::string const& filename, bool const loadImmediately = true);
  Image(Image const& other);
  ~Image();
  
  void load();
  void render(Quad const& target) const;
  Quad const& quad() const;
  unsigned int width() const;
  unsigned int height() const;
  
private:
  struct TextureInformation
  {
    TextureInformation() : id(0), width(0), height(0), refs(0), quad(0, 0, 0, 0), loaded(false) {}
    
    GLuint id;
    unsigned int width;
    unsigned int height;
    unsigned int refs;
    Quad quad;
    bool loaded;
    std::string filename;
  };
  
  TextureInformation* texture;
};
#endif