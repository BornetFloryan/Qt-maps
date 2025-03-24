#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("À propos");

    QLabel *infoLabel = new QLabel(
        "Nom: Droit au B.U.T.\n"
        "Auteur: Floryan BORNET & Corentin BRENDLÉ\n"
        "Application de recherche géographique avec Qt\n"
        "Fonctionnalités Optionnelles : \n"
        "- Barre de statut avec les coordonnées pointées par la souris\n"
        "- Recherche en appuyant \"Entrée\" au lieu du bouton\n"
        "- Gestion des tuiles déjà disponibles dans le cache de l'application\n"
        "Date: 2025", this
        );

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(infoLabel);
    setLayout(layout);
}
