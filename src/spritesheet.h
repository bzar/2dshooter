#ifndef SPRITE_SHEET_HH
#define SPRITE_SHEET_HH

#include "qmloninitializer.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

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
    Animation() : id(-1), name(), fps(30), loop(false), frames() {}

    int getId() const { return id; }
    std::string const& getName() const { return name; }
    int getFps() const { return fps; }
    bool getLoop() const { return loop; }
    Frame const& getFrame(float const time) const;

    void setId(int value) { id = value; }
    void setName(std::string const& value) { name = value; }
    void setFps(int const value) { fps = value; }
    void setLoop(bool const value) { loop = value; }
    void addFrame(Frame const& value) { frames.push_back(value); }

  private:
    int id;
    std::string name;
    int fps;
    bool loop;
    std::vector<Frame> frames;
  };

  class Sprite
  {
  public:
    Sprite() : id(), name(), animations() {}

    int getId() const { return id; }
    std::string const& getName() const { return name; }
    Animation const& getAnimation(std::string const& name) const;
    Animation const& getAnimation(int const aid) const { return animations.at(aid); };

    void setId(int value) { id = value; }
    void setName(std::string const& value) { name = value; }
    void addAnimation(Animation const& value);

  private:
    int id;
    std::string name;
    std::vector<Animation> animations;
  };

  void initialize(qmlon::Value::Reference value);
  static void initialize(SpriteSheet& spritesheet, qmlon::Value::Reference value);
  
  SpriteSheet() : image(), sprites() {}

  std::string const& getImage() const { return image; }
  Sprite const& getSprite(int sid) const { return sprites.at(sid); };
  Sprite const& getSprite(std::string const& name) const;

  void setImage(std::string const& value) { image = value; }
  void addSprite(Sprite const& value);

private:
  std::string image;
  std::vector<Sprite> sprites;
};

#endif
