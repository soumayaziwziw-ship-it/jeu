#ifndef CONSOLE_H
#define CONSOLE_H

#define TOUCHE_HAUT 1001
#define TOUCHE_BAS 1002
#define TOUCHE_GAUCHE 1003
#define TOUCHE_DROITE 1004

void effacer_ecran();
void couleur(int c);
void couleur_normale();
int lire_touche();

#endif
