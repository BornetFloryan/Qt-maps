#include "mainwindow.h"
#include "aboutdialog.h"

#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Droit au B.U.T.");

    _file_menu = menuBar()->addMenu(tr("&Fichier"));
    _help_menu = menuBar()->addMenu(tr("&Aide"));

    _quit_action = _file_menu->addAction("Quitter");
    connect(_quit_action, &QAction::triggered, this, &MainWindow::quitRequested);

    _about_action = _help_menu->addAction("À propos");
    connect(_about_action, &QAction::triggered, this, &MainWindow::aboutRequested);

    _main_widget.reset(new QGroupBox(this));
    setCentralWidget(_main_widget.get());

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *leftLayout = new QVBoxLayout;

    _text_edit.reset(new QLineEdit(_main_widget.get()));
    _text_edit->setPlaceholderText("Entrez un lieu...");
    leftLayout->addWidget(_text_edit.get());

    _button.reset(new QPushButton("Rechercher", _main_widget.get()));
    leftLayout->addWidget(_button.get());

    _list.reset(new QListWidget(_main_widget.get()));
    leftLayout->addWidget(_list.get());

    mainLayout->addLayout(leftLayout);

    _tileMap.reset(new TileMap(_main_widget.get()));
    _tileMap->setMinimumSize(600, 400);
    mainLayout->addWidget(_tileMap.get());

    _main_widget->setLayout(mainLayout);

    _coordLabel.reset(new QLabel("Coordonnées : ", _main_widget.get()));
    leftLayout->addWidget(_coordLabel.get());

    _geoController.reset(new GeoController(_tileMap.get(), this));

    connect(_tileMap.get(), &TileMap::mouseCoordinatesChanged, this, &MainWindow::updateCoordinates);

    connect(this, &MainWindow::locationChosen, _geoController.get(), &GeoController::handleLocationSelection);

    connect(this, &MainWindow::searchRequested, _geoController.get(), &GeoController::searchLocation);

    connect(_button.get(), &QPushButton::clicked, this, [this]() {
        emit searchRequested(_text_edit->text());
    });

    connect(_text_edit.get(), &QLineEdit::returnPressed, this, [this]() {
        emit searchRequested(_text_edit->text());
    });

    connect(_list.get(), &QListWidget::itemClicked, this, [this]() {
        if (_list->currentItem()) {
            emit locationChosen(_list->currentItem()->text());
        }
    });

    connect(_geoController.get(), &GeoController::searchResultsReady, this, &MainWindow::displaySearchResults);
    connect(this, &MainWindow::quitRequested, this, &QMainWindow::close);
    connect(this, &MainWindow::aboutRequested, this, [this]() {
        AboutDialog aboutDialog(this);
        aboutDialog.exec();
    });

}

MainWindow::~MainWindow() {}

void MainWindow::updateCoordinates(double lat, double lon) {
    _coordLabel->setText(QString("Coordonnées : %1, %2").arg(lat, 0, 'f', 6).arg(lon, 0, 'f', 6));
}

void MainWindow::displaySearchResults(const QList<QPair<QString, QPair<double, double>>> &results) {
    _list->clear();
    _locationMap.clear();

    for (const auto &result : results) {
        _list->addItem(result.first);
        _locationMap[result.first] = result.second;
    }
}
