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
    return _getch();
}
