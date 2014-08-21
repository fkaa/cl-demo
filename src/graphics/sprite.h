#ifndef SPRITE_H
#define SPRITE_H

#include <json/json.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "graphics/texture.h"
#include "asset.h"

class Sprite;
class SpriteSequence;
class SpriteAtlas;

typedef std::shared_ptr<Sprite> SpritePtr;
typedef std::shared_ptr<SpriteSequence> SpriteSequencePtr;

class Sprite {
 public:
  Sprite(std::shared_ptr<SpriteAtlas> atlas, std::string id);
  Sprite() {};
  virtual ~Sprite();
  virtual void update(double delta) {}
  virtual void reset() {}
  virtual const glm::vec4& rect();
  TextureHandle texture;
 private:
  glm::vec4 source;
};

class SpriteAtlas {
 public:
  SpriteAtlas(std::string path);
  ~SpriteAtlas();

  friend class Sprite;
  friend class SpriteSequence;
 private:
  Json::Value data;
};

struct SequenceData {
  glm::vec4 source;
  double time;
};

class SpriteSequence : public Sprite {
 public:
  SpriteSequence(std::shared_ptr<SpriteAtlas> atlas, std::string id);
  ~SpriteSequence();
  virtual void update(double delta);
  virtual void reset();
  const glm::vec4& rect();
 private:
  int idx;
  double curr, limit;
  std::vector<SequenceData> data;
};

#endif
