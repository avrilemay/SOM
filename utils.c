/*# nom ......... : utils.c
 * rôle........ : Fonctions utilitaires (initialisation mémoire, usage et free_memoire)
 * auteur ...... : Avrile Floro
 * version ..... : v1 du 27/09/2024
 * licence ..... : réalisé dans le cadre du cours d'IA
 * usage ....... :  make        */


#include "elements.h"


/* initialise les tableaux auxiliaires nécessaires au fonctionnement du programme */
void initialisation_tableaux_annexes(JeuDeDonnees* data, ReseauNeurones* reseau) {

    // alloc. mémoire: tab. indices séquentiels et mélangés des lignes (carte SOM)
    reseau->carte.sequentiel_order_lignes = malloc(reseau->carte.lignes * sizeof(int));
    reseau->carte.shuffle_order_lignes = malloc(reseau->carte.lignes * sizeof(int));

    // alloc. mémoire: tab. indices séquentiels et mélangés des colonnes (carte SOM)
    reseau->carte.sequentiel_order_col = malloc(reseau->carte.colonnes * sizeof(int));
    reseau->carte.shuffle_order_col = malloc(reseau->carte.colonnes * sizeof(int));

    // vérifie si les allocations ont réussi
    if (!reseau->carte.sequentiel_order_lignes || !reseau->carte.sequentiel_order_col ||
        !reseau->carte.shuffle_order_lignes || !reseau->carte.shuffle_order_col) {
        usage("Erreur d'allocation pour les indices de la carte");
    }

    // initialise les indices séquentiels pour les lignes (carte SOM)
    for (int i = 0; i < reseau->carte.lignes; i++) {
        reseau->carte.sequentiel_order_lignes[i] = i;
    }

    // initialise les indices séquentiels pour les colonnes (carte SOM)
    for (int i = 0; i < reseau->carte.colonnes; i++) {
        reseau->carte.sequentiel_order_col[i] = i;
    }

    // alloc. mémoire pour les indices séquentiels et mélangés (exemples du dataset)
    data->sequence_ordre_lignes = malloc(data->nb_exemples * sizeof(int));
    data->shuffled_order_lignes = malloc(data->nb_exemples * sizeof(int));

    // vérifie si les allocations ont réussi (exemples du dataset)
    if (!data->sequence_ordre_lignes || !data->shuffled_order_lignes) {
        usage("Erreur d'allocation pour les indices du dataset");
    }

    // initialise les indices séquentiels (exemples du dataset)
    for (int i = 0; i < data->nb_exemples; i++) {
        data->sequence_ordre_lignes[i] = i;
    }
}

/* affiche un message d'erreur et termine le programme */
void usage(const char* message) {
    fprintf(stderr, "Erreur : %s\n", message);
    exit(EXIT_FAILURE);
}

/* libère la mémoire du réseau de neurones et des données */
void free_memoire(ReseauNeurones* reseau, JeuDeDonnees* data) {
    // libère la mémoire de la carte de neurones
    for (int i = 0; i < reseau->carte.lignes; i++) {
        for (int j = 0; j < reseau->carte.colonnes; j++) {
            free(reseau->carte.neurones[i][j].poids); // libère les poids du neurone
        }
        free(reseau->carte.neurones[i]); // libère la ligne de neurones
    }
    free(reseau->carte.neurones); // libère le tableau de lignes
    free(reseau->carte.sequentiel_order_lignes);
    free(reseau->carte.sequentiel_order_col);
    free(reseau->carte.shuffle_order_lignes);
    free(reseau->carte.shuffle_order_col);

    // libère la mémoire des exemples du dataset
    for (int i = 0; i < data->nb_exemples; i++) {
        free(data->exemples[i].attributs); // libère les attributs de l'exemple
    }
    free(data->exemples); // libère le tableau d'exemples (de lignes)
    free(data->sequence_ordre_lignes);
    free(data->shuffled_order_lignes);
}