#ifndef JEU_H
#define JEU_H

#define LIGNES 12
#define COLONNES 20
#define NB_TYPES 5
#define VIDE ' '

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

#endif
