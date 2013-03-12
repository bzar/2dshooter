#include "spritesheet.h"
#include <fstream>

SpriteSheet::Frame const& SpriteSheet::Animation::getFrame(float const time) const
{
  int f = static_cast<int>(time * fps);
  if(loop)
  {
    f = f % frames.size();
  }
  else if(f >= frames.size())
  {
    f = frames.size() - 1;
  }

  return frames.at(f);
}

SpriteSheet::Animation const& SpriteSheet::Sprite::getAnimation(std::string const& name) const
{
  for(int i = 0; i < animations.size(); ++i)
  {
    if(animations.at(i).getName() == name)
    {
      return animations.at(i);
    }
  }
  
  throw std::runtime_error("Error Unknown animation!");
}

void SpriteSheet::Sprite::addAnimation(const SpriteSheet::Animation& value)
{
  animations.push_back(value);
  animations.back().setId(animations.size() - 1);
}

void SpriteSheet::initialize(qmlon::Value::Reference value)
{
  initialize(*this, value);
}

void SpriteSheet::initialize(SpriteSheet& spriteSheet, qmlon::Value::Reference value)
{
  qmlon::Initializer<Frame::Position> initPosition({
    {"x", qmlon::set(&Frame::Position::x)},
    {"y", qmlon::set(&Frame::Position::y)}
  });

  qmlon::Initializer<Frame::Size> initSize({
    {"width", qmlon::set(&Frame::Size::width)},
    {"height", qmlon::set(&Frame::Size::height)}
  });

  qmlon::Initializer<Frame> initFrame({
    {"position", qmlon::createSet(initPosition, &Frame::setPosition)},
    {"hotspot", qmlon::createSet(initPosition, &Frame::setHotspot)},
    {"size", qmlon::createSet(initSize, &Frame::setSize)}
  });

  qmlon::Initializer<Animation> initAnimation({
    {"id", qmlon::set(&Animation::setName)},
    {"fps", qmlon::set(&Animation::setFps)},
    {"loop", qmlon::set(&Animation::setLoop)}
  }, {
    {"Frame", qmlon::createAdd(initFrame, &Animation::addFrame)},
    {"Frames", [&](Animation& a, qmlon::Object& o) {
      int count = o.hasProperty("count") ? o.getProperty("count")->asInteger() : 1;
      int dx = 0;
      int dy = 0;

      if(o.hasProperty("delta"))
      {
        qmlon::Object& d = o.getProperty("delta")->asObject();
        dx = d.hasProperty("x") ? d.getProperty("x")->asInteger() : 0;
        dy = d.hasProperty("y") ? d.getProperty("y")->asInteger() : 0;
      }

      for(int i = 0; i < count; ++i)
      {
        Frame f = qmlon::create(o, initFrame);
        Frame::Position p = f.getPosition();
        p.x += i * dx;
        p.y += i * dy;
        f.setPosition(p);
        a.addFrame(f);
      }
    }}
  });

  qmlon::Initializer<Sprite> initSprite({
    {"id", qmlon::set(&Sprite::setName)}
  }, {
    {"Animation", qmlon::createAdd(initAnimation, &Sprite::addAnimation)}
  });

  qmlon::Initializer<SpriteSheet> initSheet({
    {"image", qmlon::set(&SpriteSheet::setImage)}
  }, {
    {"Sprite", qmlon::createAdd(initSprite, &SpriteSheet::addSprite)}
  });

  initSheet.init(spriteSheet, value);
}

SpriteSheet::Sprite const& SpriteSheet::getSprite(const std::string& name) const
{
  for(int i = 0; i < sprites.size(); ++i)
  {
    if(sprites.at(i).getName() == name)
    {
      return sprites.at(i);
    }
  }
  
  throw std::runtime_error("Error Unknown sprite " + name + "!");
}

void SpriteSheet::addSprite(const SpriteSheet::Sprite& value)
{
  sprites.push_back(value);
  sprites.back().setId(sprites.size() - 1);
}
