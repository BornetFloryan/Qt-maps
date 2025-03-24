#include "geocontroller.h"

GeoController::GeoController(TileMap *tileMap, QObject *parent)
    : QObject(parent), _tileMap(tileMap) {
    _geoSearch.reset(new GeoSearch(this));

    connect(_geoSearch.get(), &GeoSearch::searchFinished, this, &GeoController::handleSearchResults);
}

void GeoController::searchLocation(const QString &location) {
    if (!location.isEmpty()) {
        _geoSearch->searchLocation(location);
    }
}


void GeoController::handleSearchResults(const QList<QPair<QString, QPair<double, double>>> &results) {
    _locationMap.clear();

    for (const auto &result : results) {
        _locationMap[result.first] = result.second;
    }

    emit searchResultsReady(results);
}

void GeoController::handleLocationSelection(const QString &location) {
    if (_locationMap.contains(location)) {
        auto coords = _locationMap[location];
        _tileMap->setCenter(coords.first, coords.second, 12);
    }
}

