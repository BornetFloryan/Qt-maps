#ifndef GEOSEARCH_H
#define GEOSEARCH_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class GeoSearch : public QObject {
    Q_OBJECT

public:
    explicit GeoSearch(QObject *parent = nullptr);
    void searchLocation(const QString &location);

signals:
    void searchFinished(const QList<QPair<QString, QPair<double, double>>> &results);

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager _networkManager;
};

#endif // GEOSEARCH_H
