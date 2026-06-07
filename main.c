#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "sauvegarde.h"
#include "console.h"

int main() {
    Jeu jeu;
    Sauvegarde save;
    int choix;
    int ancien_niveau;

    srand(time(NULL));

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
        printf("Niveau : %d\n", save.niveau);
        printf("Surcharge : %d/5\n\n", save.surcharge);

        printf("1. Jouer\n");
        printf("2. Nouvelle sauvegarde\n");
        printf("3. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        if (choix == 1) {
            ancien_niveau = save.niveau;

            initialiser_jeu(&jeu, save.niveau, save.surcharge);
            boucle_jeu(&jeu);

            if (niveau_reussi(jeu) == 1) {
                save.niveau++;
                save.surcharge = 0;
            } else {
                save.surcharge++;
            }

            if (save.surcharge >= 5) {
                printf("\nSurcharge maximale. Retour au niveau 1.\n");
                save.niveau = 1;
                save.surcharge = 0;
                lire_touche();
            }

            sauvegarder(save);
        } else if (choix == 2) {
            printf("Nouveau nom : ");
            scanf("%49s", save.nom);
            save.niveau = 1;
            save.surcharge = 0;
            sauvegarder(save);
        }

    } while (choix != 3);

    couleur_normale();
    return 0;
}
