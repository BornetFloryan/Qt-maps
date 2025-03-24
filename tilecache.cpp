#include "tilecache.h"

TileCache::TileCache(int maxSize) : _maxSize(maxSize) {}

TileCache::~TileCache() {}

bool TileCache::contains(const QString &key) const {
    return _cache.contains(key);
}

void TileCache::insert(const QString &key, const QPixmap &tile) {
    if (_cache.size() >= _maxSize) {
        auto it = _cache.begin();
        _cache.remove(_cache.firstKey());
    }
    _cache.insert(key, tile);
}

QPixmap TileCache::get(const QString &key) const {
    return _cache.value(key);
}
