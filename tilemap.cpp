#include "tilemap.h"
#include <QPainter>
#include <QtMath>
#include <QNetworkRequest>

TileMap::TileMap(QWidget *parent)
    : QWidget(parent), _centerLon(6.839349), _centerLat(47.64263), _zoom(12) {
    connect(&_networkManager, &QNetworkAccessManager::finished, this, &TileMap::tileDownloaded);
    loadTiles();
}

void TileMap::setCenter(double lat, double lon, int zoom) {
    _centerLon = lon;
    _centerLat = lat;
    _zoom = zoom;
    _tiles.clear();
    loadTiles();
    update();
}

std::pair<int, int> TileMap::geoToTile(double lon, double lat, int zoom) {
    int n = 1 << zoom;
    int x = (lon + 180.0) / 360.0 * n;
    int y = (1.0 - log(tan(qDegreesToRadians(lat)) + 1.0 / cos(qDegreesToRadians(lat))) / M_PI) / 2.0 * n;
    return {x, y};
}

QString TileMap::tileUrl(int x, int y, int zoom) {
    return QString("https://maptiles.p.rapidapi.com/en/map/v1/%1/%2/%3.png").arg(zoom).arg(x).arg(y);
}

void TileMap::loadTiles() {
    auto [centerX, centerY] = geoToTile(_centerLon, _centerLat, _zoom);
    int halfXTiles = width() / (2 * _tileSize) + 1;
    int halfYTiles = height() / (2 * _tileSize) + 1;

    for (int dx = -halfXTiles; dx <= halfXTiles; ++dx) {
        for (int dy = -halfYTiles; dy <= halfYTiles; ++dy) {
            int x = centerX + dx, y = centerY + dy;
            QString key = QString("%1_%2_%3").arg(_zoom).arg(x).arg(y);

            QUrl url = tileUrl(x, y, _zoom);
            QNetworkRequest request(url);
            request.setRawHeader("x-rapidapi-key", "36960ba187msh38ed93928107d9dp141345jsn49d778acf12a");
            request.setRawHeader("x-rapidapi-host", "maptiles.p.rapidapi.com");
            _networkManager.get(request);
        }
    }
}

void TileMap::tileDownloaded(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());

        QStringList parts = reply->url().path().split("/");
        if (parts.size() >= 4) {
            QString key = QString("%1_%2_%3").arg(parts[parts.size() - 3])
            .arg(parts[parts.size() - 2])
                .arg(parts[parts.size() - 1].replace(".png", ""));
            _tiles[key] = pixmap;
            update();
        }
    }
    reply->deleteLater();
}

void TileMap::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    if (_tiles.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Chargement des tuiles...");
        return;
    }

    auto [centerX, centerY] = geoToTile(_centerLon, _centerLat, _zoom);
    int halfXTiles = width() / (2 * _tileSize) + 1;
    int halfYTiles = height() / (2 * _tileSize) + 1;

    double tileXOffset = (_centerLon + 180.0) / 360.0 * (1 << _zoom);
    double tileYOffset = (1.0 - log(tan(qDegreesToRadians(_centerLat)) + 1.0 / cos(qDegreesToRadians(_centerLat))) / M_PI) / 2.0 * (1 << _zoom);

    int pixelOffsetX = (_tileSize * (tileXOffset - centerX));
    int pixelOffsetY = (_tileSize * (tileYOffset - centerY));

    for (int dx = -halfXTiles; dx <= halfXTiles; ++dx) {
        for (int dy = -halfYTiles; dy <= halfYTiles; ++dy) {
            int x = centerX + dx, y = centerY + dy;
            QString key = QString("%1_%2_%3").arg(_zoom).arg(x).arg(y);

            if (_tiles.contains(key)) {
                int screenX = dx * _tileSize + (width() / 2) - pixelOffsetX;
                int screenY = dy * _tileSize + (height() / 2) - pixelOffsetY;
                painter.drawPixmap(screenX, screenY, _tileSize, _tileSize, _tiles[key]);
            }
        }
    }
}
