#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QMap>
#include<QGroupBox>

#include "tilemap.h"
#include "geocontroller.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void searchRequested(const QString &location);
    void locationChosen(const QString &location);
    void quitRequested();
    void aboutRequested();

public slots:
    void updateCoordinates(double lat, double lon);
    void displaySearchResults(const QList<QPair<QString, QPair<double, double>>> &results);

private:
    QScopedPointer<GeoController> _geoController;

    QScopedPointer<QLineEdit> _text_edit;
    QScopedPointer<QPushButton> _button;
    QScopedPointer<QListWidget> _list;
    QScopedPointer<QLabel> _coordLabel;
    QScopedPointer<TileMap> _tileMap;
    QScopedPointer<QGroupBox> _main_widget;

    QMenu *_file_menu;
    QMenu *_help_menu;
    QAction *_quit_action;
    QAction *_about_action;

    QMap<QString, QPair<double, double>> _locationMap;
};

#endif // MAINWINDOW_H
