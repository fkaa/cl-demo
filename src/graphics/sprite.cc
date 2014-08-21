#include "graphics/sprite.h"

//
//  ███████╗██████╗ ██████╗ ██╗████████╗███████╗
//  ██╔════╝██╔══██╗██╔══██╗██║╚══██╔══╝██╔════╝
//  ███████╗██████╔╝██████╔╝██║   ██║   █████╗
//  ╚════██║██╔═══╝ ██╔══██╗██║   ██║   ██╔══╝
//  ███████║██║     ██║  ██║██║   ██║   ███████╗
//  ╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝   ╚═╝   ╚══════╝
//

Sprite::Sprite(std::shared_ptr<SpriteAtlas> atlas, std::string id) {  
  if (atlas->data.isMember(id)) {
    Json::Value info = atlas->data[id];

    if (info.isMember("texture")) {
      Json::Value tex = info["texture"];
      texture = Asset::load<Texture>(tex.asString());
    } else if (atlas->data.isMember("texture")) {
      Log::v("No texture specified for sprite '%s', using default.", id.c_str());
      Json::Value tex = atlas->data["texture"];
      texture = Asset::load<Texture>(tex.asString());
    } else {
      Log::e("No texture specified for sprite '%s'.", id.c_str());
    }

    if (info.isMember("source")) {
      Json::Value src = info["source"];
      source = glm::vec4(
        src[0].asFloat() / texture->width,
        src[1].asFloat() / texture->height,
        (src[0].asFloat() + src[2].asFloat()) / texture->width,
        (src[1].asFloat() + src[3].asFloat()) / texture->height
      );
    } else {
      Log::e("No source coordinates specified for sprite '%s'", id.c_str());
    }
  } else {
    Log::e("Unable to locate sprite data for '%s'", id.c_str());
  }
}

Sprite::~Sprite() {

}

const glm::vec4& Sprite::rect() {
  return source;
}

SpriteAtlas::SpriteAtlas(std::string path) {
  std::ifstream fs(path, std::ios::in);
  if (fs.bad()) {
    Log::e("Bad file path '%s'", path.c_str());
  }
  Json::Reader reader;
  if (reader.parse(fs, data)) {
    Log::d("Created sprite atlas '%s' [size=%i]", path.c_str(), data.size());
  } else {
    Log::e("Failed to read json file at '%s'", path.c_str());
  }
}

SpriteAtlas::~SpriteAtlas() {
  Log::d("Deleting sprite atlas [size=%i]", data.size());
}

SpriteSequence::SpriteSequence(std::shared_ptr<SpriteAtlas> atlas, std::string id) : idx(0), curr(0) {
  if (atlas->data.isMember(id)) {
    Json::Value info = atlas->data[id];

    if (info.isMember("texture")) {
      Json::Value tex = info["texture"];
      texture = Asset::load<Texture>(tex.asString());
    } else if (atlas->data.isMember("texture")) {
      Log::v("No texture specified for sprite sequence '%s', using default.", id.c_str());
      Json::Value tex = atlas->data["texture"];
      texture = Asset::load<Texture>(tex.asString());
    } else {
      Log::e("No texture specified for sprite sequence '%s'.", id.c_str());
    }

    if (info.isMember("frames")) {
      Json::Value frames = info["frames"];
      int len = frames.size();
      
      for (int i = 0; i < len; i++) {
        Json::Value frame = frames[i];
        Json::Value time = frame["time"];
        Json::Value src = frame["source"];
        SequenceData seq;
        seq.time = time.asFloat();
        seq.source = glm::vec4(
          src[0].asFloat() / texture->width,
          src[1].asFloat() / texture->height,
          (src[0].asFloat() + src[2].asFloat()) / texture->width,
          (src[1].asFloat() + src[3].asFloat()) / texture->height
        );

        data.push_back(seq);
      }
    } else {
      Log::e("No frames specified for sprite sequence '%s'.", id.c_str());
    }
  } else {
    Log::e("Unable to locate sprite sequence data for '%s'", id.c_str());
  }

  limit = data.size() > 0 ? data[0].time : 0.f;
}

SpriteSequence::~SpriteSequence() {
  /*for (int i = 0; i < data.size(); ++i) {
    delete data[i];
  }*/
}

const glm::vec4& SpriteSequence::rect() {
  return data[idx].source;
}

void SpriteSequence::update(double delta) {
  curr += delta;
  if (curr > limit) {
    if (++idx > data.size() - 1) {
      idx = 0;
    }
    curr -= limit;
    limit = data[idx].time;
  }
}

void SpriteSequence::reset() {
  curr = 0;
  idx = 0;
  limit = data[idx].time;
}
