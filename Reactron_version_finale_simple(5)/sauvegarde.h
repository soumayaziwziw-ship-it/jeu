#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

typedef struct {
    char nom[50];
    int niveau;
    int surcharge;
} Sauvegarde;

void sauvegarder(Sauvegarde s);
int charger(Sauvegarde *s);

#endif
