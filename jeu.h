#ifndef JEU_H
#define JEU_H

#include <time.h>

#define LIGNES 12
#define COLONNES 20
#define NB_TYPES 5
#define VIDE ' '
#define SURCHARGE_MAX 5

typedef struct {
    char grille[LIGNES][COLONNES];

    int curseur_ligne;
    int curseur_colonne;

    int selection_active;
    int selection_ligne;
    int selection_colonne;

    int niveau;
    int coups;
    int coups_max;
    int temps_max;
    time_t debut;
    int surcharge;

    int r_absorbe;
    int b_absorbe;
    int g_absorbe;
    int y_absorbe;
    int v_absorbe;
} Jeu;

void initialiser_jeu(Jeu *j, int niveau, int surcharge);
void boucle_jeu(Jeu *j);
int niveau_reussi(Jeu j);
int niveau_echoue(Jeu j);

#endif
