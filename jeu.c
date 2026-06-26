#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "console.h"

char energie_aleatoire() {
    char types[NB_TYPES] = {'R', 'B', 'G', 'Y', 'V'};
    return types[rand() % NB_TYPES];
}

void compter(Jeu *j, char c) {
    if (c == 'R') j->r_absorbe++;
    else if (c == 'B') j->b_absorbe++;
    else if (c == 'G') j->g_absorbe++;
    else if (c == 'Y') j->y_absorbe++;
    else if (c == 'V') j->v_absorbe++;
}

int objectif_r(Jeu j) {
    if (j.niveau == 1) return 8;
    if (j.niveau == 2) return 12;
    return 15;
}

int objectif_g(Jeu j) {
    if (j.niveau == 1) return 8;
    if (j.niveau == 2) return 12;
    return 15;
}

int objectif_y(Jeu j) {
    if (j.niveau == 1) return 0;
    if (j.niveau == 2) return 8;
    return 10;
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

void echanger(char *a, char *b) {
    char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int permutation_possible(Jeu *j, int l1, int c1, int l2, int c2) {
    int ok;

    echanger(&j->grille[l1][c1], &j->grille[l2][c2]);
    ok = existe_alignement(*j);
    echanger(&j->grille[l1][c1], &j->grille[l2][c2]);

    return ok;
}

int existe_coup_valide(Jeu *j) {
    int i, k;

    for (i = 0; i < LIGNES; i++) {
        for (k = 0; k < COLONNES; k++) {
            if (k + 1 < COLONNES && permutation_possible(j, i, k, i, k + 1)) return 1;
            if (i + 1 < LIGNES && permutation_possible(j, i, k, i + 1, k)) return 1;
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
    } while (existe_alignement(*j) || existe_coup_valide(j) == 0);
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
    j->debut = time(NULL);

    if (niveau == 1) {
        j->coups_max = 25;
        j->temps_max = 180;
    } else if (niveau == 2) {
        j->coups_max = 22;
        j->temps_max = 150;
    } else {
        j->coups_max = 20;
        j->temps_max = 120;
    }

    j->r_absorbe = 0;
    j->b_absorbe = 0;
    j->g_absorbe = 0;
    j->y_absorbe = 0;
    j->v_absorbe = 0;

    remplir_grille_sans_alignement(j);
}

int temps_restant(Jeu j) {
    int ecoule;
    ecoule = (int)(time(NULL) - j.debut);
    return j.temps_max - ecoule;
}

void afficher_objectifs(Jeu j) {
    printf("Objectifs : R >= %d | G >= %d", objectif_r(j), objectif_g(j));
    if (objectif_y(j) > 0) printf(" | Y >= %d", objectif_y(j));
    printf("\n");
}

void afficher_jeu(Jeu j) {
    int i, k;

    effacer_ecran();

    printf("===== REACTRON =====\n");
    printf("Niveau : %d | Coups : %d/%d | Temps : %d s | Surcharge : %d/%d\n",
           j.niveau, j.coups, j.coups_max, temps_restant(j), j.surcharge, SURCHARGE_MAX);

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

    printf("\nTouches : ZQSD ou fleches | ESPACE selection / annulation | E quitter\n");
}

void marquer_reaction_globale(Jeu *j, int marque[LIGNES][COLONNES], char type) {
    int i, k;

    for (i = 0; i < LIGNES; i++) {
        for (k = 0; k < COLONNES; k++) {
            if (j->grille[i][k] == type) {
                marque[i][k] = 1;
            }
        }
    }
}

int supprimer_alignements(Jeu *j) {
    int marque[LIGNES][COLONNES];
    int i, k, fin, taille;
    char type;
    int trouve;

    trouve = 0;

    for (i = 0; i < LIGNES; i++) {
        for (k = 0; k < COLONNES; k++) marque[i][k] = 0;
    }

    for (i = 0; i < LIGNES; i++) {
        k = 0;
        while (k < COLONNES) {
            type = j->grille[i][k];
            fin = k + 1;
            while (fin < COLONNES && j->grille[i][fin] == type && type != VIDE) fin++;
            taille = fin - k;

            if (type != VIDE && taille >= 3) {
                trouve = 1;
                if (taille >= 5) {
                    marquer_reaction_globale(j, marque, type);
                } else {
                    while (k < fin) {
                        marque[i][k] = 1;
                        k++;
                    }
                    k--;
                }
            }
            k = fin;
        }
    }

    for (k = 0; k < COLONNES; k++) {
        i = 0;
        while (i < LIGNES) {
            type = j->grille[i][k];
            fin = i + 1;
            while (fin < LIGNES && j->grille[fin][k] == type && type != VIDE) fin++;
            taille = fin - i;

            if (type != VIDE && taille >= 3) {
                trouve = 1;
                if (taille >= 5) {
                    marquer_reaction_globale(j, marque, type);
                } else {
                    while (i < fin) {
                        marque[i][k] = 1;
                        i++;
                    }
                    i--;
                }
            }
            i = fin;
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
    int col, ligne, cherche;

    for (col = 0; col < COLONNES; col++) {
        for (ligne = LIGNES - 1; ligne >= 0; ligne--) {
            if (j->grille[ligne][col] == VIDE) {
                cherche = ligne - 1;

                while (cherche >= 0 && j->grille[cherche][col] == VIDE) cherche--;

                if (cherche >= 0) {
                    j->grille[ligne][col] = j->grille[cherche][col];
                    j->grille[cherche][col] = VIDE;
                } else {
                    j->grille[ligne][col] = energie_aleatoire();
                }
            }
        }
    }
}

void stabiliser(Jeu *j) {
    while (supprimer_alignements(j) == 1) {
        afficher_jeu(*j);
        gravite(j);
    }
}

int sont_adjacentes(int l1, int c1, int l2, int c2) {
    return abs(l1 - l2) + abs(c1 - c2) == 1;
}

int essayer_echange(Jeu *j, int l1, int c1, int l2, int c2) {
    if (sont_adjacentes(l1, c1, l2, c2) == 0) return 0;

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
    return j.r_absorbe >= objectif_r(j) &&
           j.g_absorbe >= objectif_g(j) &&
           j.y_absorbe >= objectif_y(j);
}

int niveau_echoue(Jeu j) {
    if (j.coups >= j.coups_max) return 1;
    if (temps_restant(j) <= 0) return 1;
    return 0;
}

void deplacer_curseur(Jeu *j, int touche) {
    if ((touche == 'z' || touche == 'Z' || touche == TOUCHE_HAUT) && j->curseur_ligne > 0) {
        j->curseur_ligne--;
    } else if ((touche == 's' || touche == 'S' || touche == TOUCHE_BAS) && j->curseur_ligne < LIGNES - 1) {
        j->curseur_ligne++;
    } else if ((touche == 'q' || touche == 'Q' || touche == TOUCHE_GAUCHE) && j->curseur_colonne > 0) {
        j->curseur_colonne--;
    } else if ((touche == 'd' || touche == 'D' || touche == TOUCHE_DROITE) && j->curseur_colonne < COLONNES - 1) {
        j->curseur_colonne++;
    }
}

void gerer_espace(Jeu *j) {
    int ok;

    if (j->selection_active == 0) {
        j->selection_active = 1;
        j->selection_ligne = j->curseur_ligne;
        j->selection_colonne = j->curseur_colonne;
    } else if (j->selection_ligne == j->curseur_ligne && j->selection_colonne == j->curseur_colonne) {
        j->selection_active = 0;
    } else {
        ok = essayer_echange(j,
                             j->selection_ligne,
                             j->selection_colonne,
                             j->curseur_ligne,
                             j->curseur_colonne);

        j->selection_active = 0;

        if (ok == 0) {
            printf("\nPermutation refusee : elle ne cree pas d'alignement. Appuyez sur une touche.\n");
            lire_touche();
        }
    }
}

void boucle_jeu(Jeu *j) {
    int touche;

    while (1) {
        afficher_jeu(*j);

        if (niveau_reussi(*j) == 1) {
            printf("\nBravo ! Niveau reussi. Appuyez sur une touche.\n");
            lire_touche();
            return;
        }

        if (niveau_echoue(*j) == 1) {
            printf("\nEchec : limite atteinte. Appuyez sur une touche.\n");
            lire_touche();
            return;
        }

        if (existe_coup_valide(j) == 0) {
            printf("\nPlus aucun coup valide : nouvelle grille. Appuyez sur une touche.\n");
            lire_touche();
            remplir_grille_sans_alignement(j);
        }

        touche = lire_touche();

        if (touche == 'e' || touche == 'E') return;
        else if (touche == ' ') gerer_espace(j);
        else deplacer_curseur(j, touche);
    }
}
