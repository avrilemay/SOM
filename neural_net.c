/*# nom ........ : neural_net.c
 * rôle......... : Module en charge de l'initialisation du réseau de neurones
 * auteur ...... : Avrile Floro
 * version ..... : v1 du 27/09/2024
 * licence ..... : réalisé dans le cadre du cours d'IA
 * usage ....... :  make        */

#include "elements.h"

/* génère un float aléatoire entre deux bornes */
float float_generator(float min, float max) {
    // génère un float aléatoire entre min et max
    float rd_num = ((float)random() / (float)RAND_MAX) * (max - min) + min;
    return rd_num; // retourne le float généré
}

/* génère un vecteur de floats aléatoires */
float* vect_generateur(ReseauNeurones* reseau) {
    int taille = reseau->taille_vecteurs;

    // alloue de la mémoire pour le vecteur
    float* vecteur = malloc(taille * sizeof(float));
    if (!vecteur) {
        usage("Erreur d'allocation pour le vecteur");
        exit(EXIT_FAILURE);
    }

    // génère des valeurs aléatoires pour chaque composante du vecteur
    for (int i = 0; i < taille; i++) {
        vecteur[i] = float_generator(reseau->min, reseau->max);
    }
    return vecteur;
}

/* normalise un vecteur pour qu'il ait une norme de 1 */
void vect_normalizer(float* vecteur, int taille_vecteur) {
    float norme = 0;

    // calcule la norme du vecteur
    for (int i = 0; i < taille_vecteur; i++) {
        norme += vecteur[i] * vecteur[i];
    }
    norme = sqrtf(norme);

    if (norme == 0) {
        usage("Erreur lors de la normalisation du vecteur");
    }

    // divise chaque composante du vecteur par la norme
    for (int i = 0; i < taille_vecteur; i++) {
        vecteur[i] /= norme;
    }
}

/* génère la matrice de poids initiale pour le réseau de neurones */
void init_carte_neurones(ReseauNeurones* reseau) {
    // récupère les infos sur la carte (nb de lignes, colonnes et taille des vecteurs)
    int nb_lignes = reseau->carte.lignes;
    int nb_colonnes = reseau->carte.colonnes;
    int taille_vecteurs = reseau->taille_vecteurs;

    // alloc. mémoire pour la matrice de neurones - lignes (tableau de pointeurs)
    reseau->carte.neurones = malloc(nb_lignes * sizeof(Neurone*));
    if (!reseau->carte.neurones) {
        usage("Erreur d'allocation pour la carte de neurones");
    }

    // parcourt chaque ligne de la carte (qu'on vient d'allouer)
    for (int i = 0; i < nb_lignes; i++) {
        // alloue de la mémoire pour les neurones de la ligne
        reseau->carte.neurones[i] = malloc(nb_colonnes * sizeof(Neurone));
        if (!reseau->carte.neurones[i]) {
            usage("Erreur d'allocation pour les neurones");
        }

        // parcourt chaque neurone de la ligne (qu'on vient d'allouer)
        for (int j = 0; j < nb_colonnes; j++) {
            // alloue de la mémoire pour les poids du neurone (qui sont des floats)
            reseau->carte.neurones[i][j].poids = malloc(taille_vecteurs * sizeof(float));
            if (!reseau->carte.neurones[i][j].poids) {
                usage("Erreur d'allocation pour les poids du neurone");
            }

            // génère un vecteur de poids aléatoires (random float)
            float* vecteur_random_genere = vect_generateur(reseau);

            // normalise le vecteur de poids pour avoir une norme de 1
            vect_normalizer(vecteur_random_genere, reseau->taille_vecteurs);

            // copie le vecteur normalisé dans les poids du neurone
            memcpy(reseau->carte.neurones[i][j].poids, vecteur_random_genere,
                   taille_vecteurs * sizeof(float));

            // libère la mémoire du vecteur temporaire
            free(vecteur_random_genere);

            // initialise le label du neurone à 0
            reseau->carte.neurones[i][j].label = 0;
        }
    }
}