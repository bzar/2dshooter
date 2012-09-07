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

int SpriteSheet::Sprite::getAnimationId(std::string const& name) const
{
  for(int i = 0; i < animations.size(); ++i)
  {
    if(animations.at(i).name == name)
    {
      return i;
    }
  }

  return -1;
}

SpriteSheet SpriteSheet::create(std::string const& filename)
{
  std::ifstream ifs(filename);
  auto value = qmlon::readValue(ifs);
  return create(value);
}

SpriteSheet SpriteSheet::create(qmlon::Value::Reference value)
{
  qmlon::Initializer<Frame::Position> initPosition({
    {"x", [](Frame::Position& p, qmlon::Value::Reference v) { p.x = v->asInteger(); }},
    {"y", [](Frame::Position& p, qmlon::Value::Reference v) { p.y = v->asInteger(); }}
  });

  qmlon::Initializer<Frame::Size> initSize({
    {"width", [](Frame::Size& s, qmlon::Value::Reference v) { s.width = v->asInteger(); }},
    {"height", [](Frame::Size& s, qmlon::Value::Reference v) { s.height = v->asInteger(); }}
  });

  qmlon::Initializer<Frame> initFrame({
    {"position", [&](Frame& f, qmlon::Value::Reference v) { f.setPosition(qmlon::create(v, initPosition)); }},
    {"hotspot", [&](Frame& f, qmlon::Value::Reference v) { f.setHotspot(qmlon::create(v, initPosition)); }},
    {"size", [&](Frame& f, qmlon::Value::Reference v) { f.setSize(qmlon::create(v, initSize)); }}
  });

  qmlon::Initializer<Animation> initAnimation({
    {"id", [](Animation& a, qmlon::Value::Reference v) { a.setId(v->asString()); }},
    {"fps", [](Animation& a, qmlon::Value::Reference v) { a.setFps(v->asInteger()); }},
    {"loop", [](Animation& a, qmlon::Value::Reference v) { a.setLoop(v->asBoolean()); }}
  }, {
    {"Frame", [&](Animation& a, qmlon::Object* o) { a.addFrame(qmlon::create(o, initFrame)); }},
    {"Frames", [&](Animation& a, qmlon::Object* o) {
      int count = o->hasProperty("count") ? o->getProperty("count")->asInteger() : 1;
      int dx = 0;
      int dy = 0;

      if(o->hasProperty("delta"))
      {
        qmlon::Object* d = o->getProperty("delta")->asObject();
        dx = d->hasProperty("x") ? d->getProperty("x")->asInteger() : 0;
        dy = d->hasProperty("y") ? d->getProperty("y")->asInteger() : 0;
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
    {"id", [](Sprite& s, qmlon::Value::Reference v) { s.setId(v->asString()); }}
  }, {
    {"Animation", [&](Sprite& s, qmlon::Object* o) { s.addAnimation(qmlon::create(o, initAnimation)); }}
  });

  qmlon::Initializer<SpriteSheet> initSheet({
    {"image", [](SpriteSheet& sheet, qmlon::Value::Reference value) { sheet.setImage(value->asString()); }}
  }, {
    {"Sprite", [&](SpriteSheet& sheet, qmlon::Object* obj) { sheet.addSprite(qmlon::create(obj, initSprite)); }}
  });

  return qmlon::create(value, initSheet);
}
