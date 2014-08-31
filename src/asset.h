#ifndef ASSET_H
#define ASSET_H

#include <assert.h>
#include <typeinfo>
#include <map>
#include <string>
#include <iostream>
#include <memory>

#include "log.h"

class Asset {
 private:
  Asset() {};
  ~Asset() {};
  static std::map<std::string, std::shared_ptr<void> > cache;
 public:
  template <class T, typename... Args>
  static std::shared_ptr<T> load(std::string path, Args&&... args) {
    std::shared_ptr<T> asset = cache.count(path) > 0 ?
      std::static_pointer_cast<T>(cache.at(path)) :
      nullptr;
    if (!asset) {
      Log::d("Caching %s asset '%s'", typeid(T).name(), path.c_str());
      asset = std::make_shared<T>(path, args...);
      if (asset) {
        cache[path] = asset;
      } else {
        Log::e("Failed to create %s asset '%s'", typeid(T).name(), path.c_str());
      }
    } else {
      Log::v("Found cached asset for '%s'", path.c_str());
    }
    return asset;
  }
  template <class T>
  static void unload(std::shared_ptr<T> asset) {
    auto it = cache.begin();
    while (it != cache.end()) {
      if (it->second == asset) {
        cache.erase(it++);
      } else {
        it++;
      }
    }
  }
  static void clear();
};

#endif // ASSET_H
