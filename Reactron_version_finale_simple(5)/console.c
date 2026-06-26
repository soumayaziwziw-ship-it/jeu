#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "console.h"

void effacer_ecran() {
    system("cls");
}

void couleur(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void couleur_normale() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

int lire_touche() {
    int t;

    t = _getch();

    if (t == 0 || t == 224) {
        t = _getch();
        if (t == 72) return TOUCHE_HAUT;
        if (t == 80) return TOUCHE_BAS;
        if (t == 75) return TOUCHE_GAUCHE;
        if (t == 77) return TOUCHE_DROITE;
    }

    return t;
}
