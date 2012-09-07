#ifndef SPRITE_SHEET_HH
#define SPRITE_SHEET_HH

#include "qmlon.h"
#include <vector>
#include <string>
#include <map>

class SpriteSheet
{
public:
  class Frame
  {
  public:
    Frame() : position(), hotspot(), size() {}
    struct Position
    {
      Position() : x(0), y(0) {}
      int x;
      int y;
    };

    struct Size
    {
      Size() : width(0), height(0) {}
      int width;
      int height;
    };

    Position getPosition() const { return position; }
    Position getHotspot() const { return hotspot; }
    Size getSize() const { return size; }

    void setPosition(Position const value) { position = value; }
    void setHotspot(Position const value) { hotspot = value; }
    void setSize(Size const value) { size = value; }

  private:
    Position position;
    Position hotspot;
    Size size;
  };

  class Animation
  {
  public:
    Animation() : id(), fps(30), loop(false), frames() {}

    std::string const& getId() const { return id; }
    int getFps() const { return fps; }
    bool getLoop() const { return loop; }
    Frame const& getFrame(float const time) const;

    void setId(std::string const& value) { id = value; }
    void setFps(int const value) { fps = value; }
    void setLoop(bool const value) { loop = value; }
    void addFrame(Frame const& value) { frames.push_back(value); }

  private:
    std::string id;
    int fps;
    bool loop;
    std::vector<Frame> frames;
  };

  class Sprite
  {
  public:
    Sprite() : id(), animations() {}

    std::string const& getId() const { return id; }
    int getAnimationId(std::string const& name) const;
    Animation const& getAnimation(int const aid) const { return animations.at(aid).animation; };

    void setId(std::string const& value) { id = value; }
    void addAnimation(Animation const& value) { animations.push_back({value.getId(), value}); }

  private:
    struct NamedAnimation
    {
      std::string name;
      Animation animation;
    };
    std::string id;
    std::vector<NamedAnimation> animations;
  };

  SpriteSheet() : image(), sprites() {}

  std::string const& getImage() const { return image; }
  Sprite const& getSprite(std::string const& sid) const { return sprites.find(sid)->second; };

  void setImage(std::string const& value) { image = value; }
  void addSprite(Sprite const& value) { sprites[value.getId()] = value; }

  static SpriteSheet create(std::string const& filename);
  static SpriteSheet create(qmlon::Value::Reference value);
private:
  std::string image;
  std::map<std::string, Sprite> sprites;
};

#endif
