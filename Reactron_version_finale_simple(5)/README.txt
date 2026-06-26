REACTRON - VERSION FINALE SIMPLE

Compilation sous Windows :
gcc main.c jeu.c sauvegarde.c console.c -o reactron.exe

Execution :
reactron.exe

Touches :
Z / Q / S / D ou fleches : deplacer le curseur
ESPACE : selectionner une cellule, annuler si on rappuie sur la meme case, ou permuter avec une case voisine
E : quitter le niveau

Fichiers :
main.c          : menu, lancement des niveaux, retour automatique au menu
jeu.c / jeu.h   : matrice 12 x 20, objectifs, selection, permutation, alignements, reaction globale, gravite, cascades
console.c / .h  : couleurs, effacement ecran, lecture clavier
sauvegarde.c/.h : sauvegarde du nom, du niveau et de la surcharge

Notions de cours utilisees :
- variables, conditions, boucles
- tableaux 2D
- fonctions et prototypes
- pointeurs simples pour modifier la structure Jeu
- structures
- chaines de caracteres pour le nom du joueur
- fichiers textes avec FILE pour la sauvegarde
- modularite .h / .c

Choix important :
Le code reste volontairement simple pour etre explicable en soutenance.
Les algorithmes suivent ceux de la presentation : generation initiale, validation de permutation, cycle de stabilisation et verification de fin de niveau.
