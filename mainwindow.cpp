#include "mainwindow.h"
#include "geosearch.h"
#include <QMenuBar>
#include <QMenu>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Droit au B.U.T.");

    _file_menu = menuBar()->addMenu(tr("&File"));
    _help_menu = menuBar()->addMenu(tr("&Help"));

    _quit_action = _file_menu->addAction("Quit");
    connect(_quit_action, &QAction::triggered, this, &MainWindow::onQuitClicked);

    _about_action = _help_menu->addAction("About");
    connect(_about_action, &QAction::triggered, this, &MainWindow::onAboutClicked);

    _main_widget.reset(new QGroupBox(this));
    setCentralWidget(_main_widget.get());

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *leftLayout = new QVBoxLayout;

    _text_edit.reset(new QLineEdit(_main_widget.get()));
    _text_edit->setPlaceholderText("Entrez un lieu...");
    leftLayout->addWidget(_text_edit.get());

    _button.reset(new QPushButton("Rechercher", _main_widget.get()));
    leftLayout->addWidget(_button.get());

    connect(_text_edit.get(), &QLineEdit::returnPressed, this, &MainWindow::onButtonClicked);

    _list.reset(new QListWidget(_main_widget.get()));
    leftLayout->addWidget(_list.get());

    mainLayout->addLayout(leftLayout);

    _tileMap.reset(new TileMap(_main_widget.get()));
    _tileMap->setMinimumSize(600, 400);
    mainLayout->addWidget(_tileMap.get());

    _main_widget->setLayout(mainLayout);

    _coordLabel = new QLabel("Coordonnées : ", _main_widget.get());
    leftLayout->addWidget(_coordLabel);

    connect(_tileMap.get(), &TileMap::mouseCoordinatesChanged, this, &MainWindow::updateCoordinates);

    _geoSearch.reset(new GeoSearch(this));
    connect(_button.get(), &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(_geoSearch.get(), &GeoSearch::searchFinished, this, &MainWindow::onSearchFinished);
    connect(_list.get(), &QListWidget::itemClicked, this, &MainWindow::onLocationSelected);
}

MainWindow::~MainWindow() {}

void MainWindow::updateCoordinates(double lat, double lon) {
    _coordLabel->setText(QString("Coordonnées : %1, %2").arg(lat, 0, 'f', 6).arg(lon, 0, 'f', 6));
}

void MainWindow::onButtonClicked() {
    QString location = _text_edit->text();
    if (!location.isEmpty()) {
        _geoSearch->searchLocation(location);
    }
}

void MainWindow::onSearchFinished(const QList<QPair<QString, QPair<double, double>>> &results) {
    _list->clear();
    _locationMap.clear();

    for (const auto &result : results) {
        _list->addItem(result.first);
        _locationMap[result.first] = result.second;
    }
}

void MainWindow::onLocationSelected() {
    QString selectedLocation = _list->currentItem()->text();
    if (_locationMap.contains(selectedLocation)) {
        auto coords = _locationMap[selectedLocation];
        _tileMap->setCenter(coords.first, coords.second, 12);
    }
}

void MainWindow::onQuitClicked() {
    close();
}

void MainWindow::onAboutClicked() {
    QMessageBox::about(this, "À propos",
                       "Nom: Droit au B.U.T.\n"
                       "Auteur: Floryan Bornet\n"
                       "Application de recherche géographique avec Qt\n"
                       "Date: 2025");
}
