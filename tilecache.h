#ifndef TILECACHE_H
#define TILECACHE_H

#include <QMap>
#include <QPixmap>
#include <QString>

class TileCache {
public:
    TileCache(int maxSize = 100);
    ~TileCache();

    bool contains(const QString &key) const;
    void insert(const QString &key, const QPixmap &tile);
    QPixmap get(const QString &key) const;

private:
    int _maxSize;
    QMap<QString, QPixmap> _cache;
};

#endif // TILECACHE_H
