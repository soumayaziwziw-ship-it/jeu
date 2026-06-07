#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "console.h"

char types[NB_TYPES] = {'R', 'B', 'G', 'Y', 'V'};

char energie_aleatoire() {
    return types[rand() % NB_TYPES];
}

void compter(Jeu *j, char c) {
    if (c == 'R') j->r_absorbe++;
    else if (c == 'B') j->b_absorbe++;
    else if (c == 'G') j->g_absorbe++;
    else if (c == 'Y') j->y_absorbe++;
    else if (c == 'V') j->v_absorbe++;
}

void afficher_cellule(char c) {
    if (c == 'R') couleur(12);
    else if (c == 'B') couleur(9);
    else if (c == 'G') couleur(10);
    else if (c == 'Y') couleur(14);
    else if (c == 'V') couleur(13);
    else couleur(7);

    printf("%c", c);
    couleur_normale();
}

int existe_alignement(Jeu j) {
    int i, k;

    for (i = 0; i < LIGNES; i++) {
        for (k = 0; k < COLONNES - 2; k++) {
            if (j.grille[i][k] != VIDE &&
                j.grille[i][k] == j.grille[i][k + 1] &&
                j.grille[i][k] == j.grille[i][k + 2]) {
                return 1;
            }
        }
    }

    for (i = 0; i < LIGNES - 2; i++) {
        for (k = 0; k < COLONNES; k++) {
            if (j.grille[i][k] != VIDE &&
                j.grille[i][k] == j.grille[i + 1][k] &&
                j.grille[i][k] == j.grille[i + 2][k]) {
                return 1;
            }
        }
    }

    return 0;
}

void remplir_grille_sans_alignement(Jeu *j) {
    int i, k;

    do {
        for (i = 0; i < LIGNES; i++) {
            for (k = 0; k < COLONNES; k++) {
                j->grille[i][k] = energie_aleatoire();
            }
        }
    } while (existe_alignement(*j));
}

void initialiser_jeu(Jeu *j, int niveau, int surcharge) {
    j->curseur_ligne = 0;
    j->curseur_colonne = 0;

    j->selection_active = 0;
    j->selection_ligne = 0;
    j->selection_colonne = 0;

    j->niveau = niveau;
    j->coups = 0;
    j->surcharge = surcharge;

    if (niveau == 1) j->coups_max = 20;
    else if (niveau == 2) j->coups_max = 25;
    else j->coups_max = 30;

    j->r_absorbe = 0;
    j->b_absorbe = 0;
    j->g_absorbe = 0;
    j->y_absorbe = 0;
    j->v_absorbe = 0;

    remplir_grille_sans_alignement(j);
}

void afficher_objectifs(Jeu j) {
    if (j.niveau == 1) {
        printf("Objectif : R >= 8 et G >= 8\n");
    } else if (j.niveau == 2) {
        printf("Objectif : R >= 12, G >= 12 et Y >= 8\n");
    } else {
        printf("Objectif : R >= 15, G >= 15 et Y >= 10\n");
    }
}

void afficher_jeu(Jeu j) {
    int i, k;

    effacer_ecran();

    printf("===== REACTRON =====\n");
    printf("Niveau : %d | Coups : %d/%d | Surcharge : %d/5\n",
           j.niveau, j.coups, j.coups_max, j.surcharge);

    afficher_objectifs(j);

    printf("Absorbe : R=%d  B=%d  G=%d  Y=%d  V=%d\n\n",
           j.r_absorbe, j.b_absorbe, j.g_absorbe, j.y_absorbe, j.v_absorbe);

    for (i = 0; i < LIGNES; i++) {
        for (k = 0; k < COLONNES; k++) {
            if (i == j.curseur_ligne && k == j.curseur_colonne) {
                couleur(240);
                printf("[");
                afficher_cellule(j.grille[i][k]);
                couleur(240);
                printf("]");
                couleur_normale();
            } else if (j.selection_active == 1 &&
                       i == j.selection_ligne &&
                       k == j.selection_colonne) {
                couleur(112);
                printf("(");
                afficher_cellule(j.grille[i][k]);
                couleur(112);
                printf(")");
                couleur_normale();
            } else {
                printf(" ");
                afficher_cellule(j.grille[i][k]);
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("\nTouches : Z haut | S bas | Q gauche | D droite");
    printf("\nESPACE : selectionner / echanger | E : quitter niveau\n");
}

int supprimer_alignements(Jeu *j) {
    int marque[LIGNES][COLONNES];
    int i, k;
    int trouve;

    trouve = 0;

    for (i = 0; i < LIGNES; i++) {
        for (k = 0; k < COLONNES; k++) {
            marque[i][k] = 0;
        }
    }

    for (i = 0; i < LIGNES; i++) {
        for (k = 0; k < COLONNES - 2; k++) {
            if (j->grille[i][k] != VIDE &&
                j->grille[i][k] == j->grille[i][k + 1] &&
                j->grille[i][k] == j->grille[i][k + 2]) {
                marque[i][k] = 1;
                marque[i][k + 1] = 1;
                marque[i][k + 2] = 1;
                trouve = 1;
            }
        }
    }

    for (i = 0; i < LIGNES - 2; i++) {
        for (k = 0; k < COLONNES; k++) {
            if (j->grille[i][k] != VIDE &&
                j->grille[i][k] == j->grille[i + 1][k] &&
                j->grille[i][k] == j->grille[i + 2][k]) {
                marque[i][k] = 1;
                marque[i + 1][k] = 1;
                marque[i + 2][k] = 1;
                trouve = 1;
            }
        }
    }

    if (trouve == 1) {
        for (i = 0; i < LIGNES; i++) {
            for (k = 0; k < COLONNES; k++) {
                if (marque[i][k] == 1) {
                    compter(j, j->grille[i][k]);
                    j->grille[i][k] = VIDE;
                }
            }
        }
    }

    return trouve;
}

void gravite(Jeu *j) {
    int col, ligne, k;

    for (col = 0; col < COLONNES; col++) {
        for (ligne = LIGNES - 1; ligne >= 0; ligne--) {
            if (j->grille[ligne][col] == VIDE) {
                k = ligne - 1;

                while (k >= 0 && j->grille[k][col] == VIDE) {
                    k--;
                }

                if (k >= 0) {
                    j->grille[ligne][col] = j->grille[k][col];
                    j->grille[k][col] = VIDE;
                } else {
                    j->grille[ligne][col] = energie_aleatoire();
                }
            }
        }
    }
}

void stabiliser(Jeu *j) {
    while (supprimer_alignements(j) == 1) {
        gravite(j);
    }
}

int sont_adjacentes(int l1, int c1, int l2, int c2) {
    if (abs(l1 - l2) + abs(c1 - c2) == 1) {
        return 1;
    }
    return 0;
}

void echanger(char *a, char *b) {
    char temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

int essayer_echange(Jeu *j, int l1, int c1, int l2, int c2) {
    if (sont_adjacentes(l1, c1, l2, c2) == 0) {
        return 0;
    }

    echanger(&j->grille[l1][c1], &j->grille[l2][c2]);

    if (existe_alignement(*j) == 1) {
        j->coups++;
        stabiliser(j);
        return 1;
    }

    echanger(&j->grille[l1][c1], &j->grille[l2][c2]);
    return 0;
}

int niveau_reussi(Jeu j) {
    if (j.niveau == 1) {
        return j.r_absorbe >= 8 && j.g_absorbe >= 8;
    } else if (j.niveau == 2) {
        return j.r_absorbe >= 12 && j.g_absorbe >= 12 && j.y_absorbe >= 8;
    }

    return j.r_absorbe >= 15 && j.g_absorbe >= 15 && j.y_absorbe >= 10;
}

void boucle_jeu(Jeu *j) {
    int touche;
    int fini;
    int ok;

    fini = 0;

    while (fini == 0) {
        afficher_jeu(*j);

        if (niveau_reussi(*j) == 1) {
            printf("\nBravo ! Niveau reussi. Appuyez sur une touche.\n");
            lire_touche();
            return;
        }

        if (j->coups >= j->coups_max) {
            printf("\nEchec : plus de coups. Appuyez sur une touche.\n");
            lire_touche();
            return;
        }

        touche = lire_touche();

        if (touche == 'z' || touche == 'Z') {
            if (j->curseur_ligne > 0) j->curseur_ligne--;
        } else if (touche == 's' || touche == 'S') {
            if (j->curseur_ligne < LIGNES - 1) j->curseur_ligne++;
        } else if (touche == 'q' || touche == 'Q') {
            if (j->curseur_colonne > 0) j->curseur_colonne--;
        } else if (touche == 'd' || touche == 'D') {
            if (j->curseur_colonne < COLONNES - 1) j->curseur_colonne++;
        } else if (touche == ' ') {
            if (j->selection_active == 0) {
                j->selection_active = 1;
                j->selection_ligne = j->curseur_ligne;
                j->selection_colonne = j->curseur_colonne;
            } else {
                ok = essayer_echange(j,
                                     j->selection_ligne,
                                     j->selection_colonne,
                                     j->curseur_ligne,
                                     j->curseur_colonne);

                j->selection_active = 0;

                if (ok == 0) {
                    printf("\nPermutation refusee. Appuyez sur une touche.\n");
                    lire_touche();
                }
            }
        } else if (touche == 'e' || touche == 'E') {
            fini = 1;
        }
    }
}
