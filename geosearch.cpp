#include "geosearch.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

GeoSearch::GeoSearch(QObject *parent) : QObject(parent) {
    connect(&_networkManager, &QNetworkAccessManager::finished, this, &GeoSearch::handleNetworkReply);
}

void GeoSearch::searchLocation(const QString &location) {
    QUrl url("https://nominatim.openstreetmap.org/search");
    QUrlQuery query;
    query.addQueryItem("q", location);
    query.addQueryItem("format", "json");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "MyQtApp");
    _networkManager.get(request);
}

void GeoSearch::handleNetworkReply(QNetworkReply *reply) {
    if (reply->error()) {
        qWarning() << "Erreur réseau:" << reply->errorString();
    } else {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QList<QPair<QString, QPair<double, double>>> results;

        for (const QJsonValue &value : jsonDoc.array()) {
            QJsonObject obj = value.toObject();
            results.append({obj["display_name"].toString(), {obj["lat"].toString().toDouble(), obj["lon"].toString().toDouble()}});
        }

        emit searchFinished(results);
    }
    reply->deleteLater();
}
