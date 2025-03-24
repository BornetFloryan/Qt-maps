#ifndef GEOCONTROLLER_H
#define GEOCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QMap>
#include "geosearch.h"
#include "tilemap.h"

class GeoController : public QObject {
    Q_OBJECT

public:
    explicit GeoController(TileMap *tileMap, QObject *parent = nullptr);

public slots:
    void searchLocation(const QString &location);
    void handleSearchResults(const QList<QPair<QString, QPair<double, double>>> &results);
    void handleLocationSelection(const QString &location);

signals:
    void searchResultsReady(const QList<QPair<QString, QPair<double, double>>> &results);

private:
    QScopedPointer<GeoSearch> _geoSearch;
    TileMap *_tileMap;
    QMap<QString, QPair<double, double>> _locationMap;
};

#endif // GEOCONTROLLER_H
