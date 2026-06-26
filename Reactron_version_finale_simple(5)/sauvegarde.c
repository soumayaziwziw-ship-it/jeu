#include <stdio.h>
#include "sauvegarde.h"

void sauvegarder(Sauvegarde s) {
    FILE *f;

    f = fopen("save_reactron.txt", "w");

    if (f == NULL) {
        printf("Erreur sauvegarde.\n");
        return;
    }

    fprintf(f, "%s\n%d\n%d\n", s.nom, s.niveau, s.surcharge);

    fclose(f);
}

int charger(Sauvegarde *s) {
    FILE *f;

    f = fopen("save_reactron.txt", "r");

    if (f == NULL) {
        return 0;
    }

    if (fscanf(f, "%49s %d %d", s->nom, &s->niveau, &s->surcharge) != 3) {
        fclose(f);
        return 0;
    }

    fclose(f);
    return 1;
}
