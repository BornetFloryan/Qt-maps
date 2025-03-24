#ifndef TILEMAP_H
#define TILEMAP_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QMap>
#include <QCache>

class TileMap : public QWidget {
    Q_OBJECT

public:
    explicit TileMap(QWidget *parent = nullptr);
    void setCenter(double lon, double lat, int zoom);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void tileDownloaded(QNetworkReply *reply);

private:
    void loadTiles();
    QString tileUrl(int x, int y, int zoom);
    std::pair<int, int> geoToTile(double lon, double lat, int zoom);

    QNetworkAccessManager _networkManager;
    QMap<QString, QPixmap> _tiles;
    double _centerLon, _centerLat;
    int _zoom;
    int _tileSize = 256;
    QString _tileDir;
    bool _dragging = false;
    QPoint _lastMousePos;
    QSet<QString> _loadingTiles;
    QCache<QString, QPixmap> _tileCache;

signals:
    void mouseCoordinatesChanged(double lat, double lon);

};

#endif // TILEMAP_H
