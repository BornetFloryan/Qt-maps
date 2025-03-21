#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QMap>
#include "geosearch.h"
#include "tilemap.h"

class QMenu;
class QAction;
class QGroupBox;
class QPushButton;
class QLineEdit;
class QListWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QMenu *_file_menu;
    QMenu *_help_menu;
    QAction *_quit_action;
    QAction *_about_action;
    QScopedPointer<QGroupBox> _main_widget;
    QScopedPointer<QPushButton> _button;
    QScopedPointer<QLineEdit> _text_edit;
    QScopedPointer<QListWidget> _list;
    QScopedPointer<TileMap> _tileMap;
    QScopedPointer<GeoSearch> _geoSearch;
    QMap<QString, QPair<double, double>> _locationMap;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();
    void onSearchFinished(const QList<QPair<QString, QPair<double, double>>> &results);
    void onLocationSelected();
    void onQuitClicked();
    void onAboutClicked();
};

#endif // MAINWINDOW_H
