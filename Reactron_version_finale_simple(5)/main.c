#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "sauvegarde.h"
#include "console.h"

void pause_menu() {
    printf("\nAppuyez sur une touche pour revenir au menu.\n");
    lire_touche();
}

void afficher_aide() {
    effacer_ecran();
    printf("===== AIDE REACTRON =====\n\n");
    printf("But : atteindre les objectifs d'energie du niveau.\n");
    printf("Deplacement : Z Q S D ou les fleches.\n");
    printf("Selection : ESPACE sur une cellule.\n");
    printf("Annulation : deuxieme ESPACE sur la meme cellule.\n");
    printf("Permutation : selectionner une cellule puis aller sur une case voisine et ESPACE.\n");
    printf("Regle : la permutation est acceptee seulement si elle cree un alignement de 3 ou plus.\n");
    printf("Alignement de 5 ou plus : toutes les cellules du meme type sont absorbees.\n");
    pause_menu();
}

int main() {
    Jeu jeu;
    Sauvegarde save;
    int choix;

    srand((unsigned int)time(NULL));

    if (charger(&save) == 0) {
        printf("Nom du joueur : ");
        scanf("%49s", save.nom);
        save.niveau = 1;
        save.surcharge = 0;
        sauvegarder(save);
    }

    do {
        effacer_ecran();

        printf("===== MENU REACTRON =====\n");
        printf("Joueur : %s\n", save.nom);
        printf("Niveau sauvegarde : %d\n", save.niveau);
        printf("Surcharge : %d/%d\n\n", save.surcharge, SURCHARGE_MAX);

        printf("1. Jouer\n");
        printf("2. Nouvelle sauvegarde\n");
        printf("3. Aide / regles\n");
        printf("4. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        if (choix == 1) {
            if (save.niveau > 3) {
                effacer_ecran();
                printf("Bravo, les 3 niveaux obligatoires sont termines !\n");
                printf("Une nouvelle partie recommence au niveau 1.\n");
                save.niveau = 1;
                save.surcharge = 0;
                sauvegarder(save);
                pause_menu();
            } else {
                initialiser_jeu(&jeu, save.niveau, save.surcharge);
                boucle_jeu(&jeu);

                if (niveau_reussi(jeu) == 1) {
                    save.niveau++;
                    save.surcharge = 0;
                } else {
                    save.surcharge++;
                }

                if (save.surcharge >= SURCHARGE_MAX) {
                    effacer_ecran();
                    printf("Surcharge maximale : retour au niveau 1.\n");
                    save.niveau = 1;
                    save.surcharge = 0;
                    pause_menu();
                }

                sauvegarder(save);
            }
        } else if (choix == 2) {
            printf("Nouveau nom : ");
            scanf("%49s", save.nom);
            save.niveau = 1;
            save.surcharge = 0;
            sauvegarder(save);
            pause_menu();
        } else if (choix == 3) {
            afficher_aide();
        }

    } while (choix != 4);

    couleur_normale();
    return 0;
}
