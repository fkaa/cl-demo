#include "asset.h"

std::map<std::string, std::shared_ptr<void> > Asset::cache;

void Asset::clear() {
  cache.clear();
}
