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
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Droit au B.U.T.");

    // Menus
    _file_menu = menuBar()->addMenu(tr("&File"));
    _help_menu = menuBar()->addMenu(tr("&Help"));

    _quit_action = _file_menu->addAction("Quit");
    connect(_quit_action, &QAction::triggered, this, &MainWindow::onQuitClicked);

    _about_action = _help_menu->addAction("About");
    connect(_about_action, &QAction::triggered, this, &MainWindow::onAboutClicked);

    // Création du widget central
    _main_widget.reset(new QGroupBox(this));
    setCentralWidget(_main_widget.get());

    // Layout principal (horizontal : recherche à gauche, carte à droite)
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // Layout vertical pour la recherche et la liste des résultats
    QVBoxLayout *leftLayout = new QVBoxLayout;

    // Champ de recherche
    _text_edit.reset(new QLineEdit(_main_widget.get()));
    _text_edit->setPlaceholderText("Entrez un lieu...");
    leftLayout->addWidget(_text_edit.get());

    // Bouton de recherche
    _button.reset(new QPushButton("Rechercher", _main_widget.get()));
    leftLayout->addWidget(_button.get());

    // Liste des résultats
    _list.reset(new QListWidget(_main_widget.get()));
    leftLayout->addWidget(_list.get());

    // Ajout du layout de gauche au layout principal
    mainLayout->addLayout(leftLayout);

    // Ajout de la carte (TileMap) à droite
    _tileMap.reset(new TileMap(_main_widget.get()));
    _tileMap->setMinimumSize(600, 400);
    mainLayout->addWidget(_tileMap.get());

    // Application du layout au widget principal
    _main_widget->setLayout(mainLayout);

    // Gestion du réseau
    _geoSearch.reset(new GeoSearch(this));
    connect(_button.get(), &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(_geoSearch.get(), &GeoSearch::searchFinished, this, &MainWindow::onSearchFinished);
    connect(_list.get(), &QListWidget::itemClicked, this, &MainWindow::onLocationSelected);
}

MainWindow::~MainWindow() {}

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
        qDebug() << "Mise à jour de la carte : " << coords.first << ", " << coords.second;
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
