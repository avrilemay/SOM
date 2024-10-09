/*# nom ........ : elements.h
 * rôle......... : Header pour les structures, les bibliothèques et les déclarations
 * auteur ...... : Avrile Floro
 * version ..... : v1 du 27/09/2024
 * licence ..... : réalisé dans le cadre du cours d'IA
 * usage ....... :  make        */


#ifndef SOM_ELEMENTS_H
#define SOM_ELEMENTS_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>     // pour sqrt
#include <float.h>    // pour FLT_MAX et FLT_MIN
#include <string.h>   // pour strtok
#include <time.h>     // pour time


// structure pour un vecteur de données
typedef struct {
    float* attributs; // tableau des attributs du vecteur
    int label;   // label associé (la classe du vin)
} VecteurDonnees;

// structure pour le jeu de données entier
typedef struct {
    int nb_exemples;      // nombre total d'exemples dans le dataset
    int taille_vecteur;    // nombre d'attributs par vecteur
    VecteurDonnees* exemples;   // tableau des vecteurs de données
    int* sequence_ordre_lignes;    // indices séquentiels des exemples
    int* shuffled_order_lignes; // indices mélangés des exemples
} JeuDeDonnees;

// structure pour un neurone
typedef struct {
    float* poids;  // tableau des poids du neurone
    int label;    // étiquette associée au neurone
} Neurone;

// structure pour une carte de neurones (SOM)
typedef struct {
    int lignes;    // nombre de lignes dans la carte
    int colonnes;   // nombre de colonnes dans la carte
    Neurone** neurones;   // grille 2D des neurones
    int* sequentiel_order_lignes;   // indices séquentiels pour les lignes
    int* sequentiel_order_col;    // indices séquentiels pour les colonnes
    int* shuffle_order_lignes; // indices mélangés pour les lignes
    int* shuffle_order_col;    // indices mélangés pour les colonnes
} CarteNeurones;

// structure pour le réseau de neurones
typedef struct {
    int nb_iterations;  // nombre total d'itérations pour l'apprentissage
    float taux_apprentissage_alpha_initial; // taux d'apprentissage initial
    float taux_apprentissage_alpha_actuel; // taux d'apprentissage courant
    float taille_voisinage_init;  // taille initiale du voisinage
    float taille_voisinage_actuelle;    // taille actuelle du voisinage
    CarteNeurones carte;   // la carte de neurones
    float min;  // borne inférieure pour la génération aléatoire
    float max;  // borne supérieure pour la génération aléatoire
    int taille_vecteurs;  // taille des vecteurs du réseau de neurones
} ReseauNeurones;

/* déclaration des fonctions */

// initialise les tableaux auxiliaires nécessaires au fonctionnement du programme
void initialisation_tableaux_annexes(JeuDeDonnees* data, ReseauNeurones* reseau);

// génère un float aléatoire entre deux bornes
float float_generator(float min, float max);

// génère la matrice de poids initiale pour le réseau de neurones
void init_carte_neurones(ReseauNeurones* reseau);

// génère un vecteur de floats aléatoires
float* vect_generateur(ReseauNeurones* reseau);

// normalise un vecteur pour qu'il ait une norme de 1
void vect_normalizer(float* vecteur, int taille_vecteur);

// calcule la distance euclidienne entre deux vecteurs
float distance_euclidienne(const float* vect_entree, const float* vect_neurone, int
taille_vecteur);

// sélectionne le neurone le plus proche du vecteur d'entrée
Neurone* choix_BMU(float* v_entree, ReseauNeurones* reseau);

// mélange les indices du tableau dans un ordre aléatoire
void melanger_indices(int* indices_a_mixer, int taille_du_tableau);

// fonction principale d'apprentissage
void apprentissage(JeuDeDonnees* data, ReseauNeurones* reseau);

// met à jour les poids du neurone en utilisant la règle d'apprentissage
void regle_apprentissage(const float* v_entree, Neurone* neurone, Neurone* bmu,
                         ReseauNeurones* reseau);

// met à jour le taux d'apprentissage en fonction de l'itération courante
void mettre_a_jour_apprentissage_alpha(float iteration, ReseauNeurones* reseau);

// met à jour le rayon du voisinage en fonction de l'itération courante
void taille_voisinage_actif(float iteration, ReseauNeurones* reseau);

// lit le fichier de données et stocke les exemples et leurs labels
void lire_jeu_de_donnees(JeuDeDonnees* data, const char* nom_fichier);

// normalise chaque attribut des données entre 0 et 1
void normaliser_dataset(JeuDeDonnees* data);

// affiche les labels associés à chaque neurone de la carte
void afficher_resultats(ReseauNeurones* reseau);

// calcule le pourcentage de données correctement classifiées par le SOM
void calculer_pourcentage_justesse(JeuDeDonnees* data, ReseauNeurones* reseau);

// affiche un message d'erreur et termine le programme
void usage(const char* message);

// libère la mémoire du réseau de neurones et des données
void free_memoire(ReseauNeurones* reseau, JeuDeDonnees* data);


#endif //SOM_ELEMENTS_H
