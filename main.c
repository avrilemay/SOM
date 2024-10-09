/*# nom ......... : main.c
 * rôle........ : Self Organizing Map (SOM)
 *                Dataset utilisé : wines
 *                Fonction principale (main)
 * auteur ...... : Avrile Floro
 * version ..... : v1.2 du 26/09/2024
 * licence ..... : réalisé dans le cadre du cours d'IA
 * usage : pour compiler : gcc -Wall -Wextra main.c utils.c resultats.c
 *                          apprentissage_SOM.c neural_net.c dataset.c -o SOM
 *         pour compiler : make
 *         pour exécuter : ./SOM (compris dans make)
 */

#include "elements.h" // structures utilisées et définitions des fonctions


int main(int argc, char *argv[]) {

    (void)argv; // pas d'argument - élimination de l'avertissement

    // vérifie qu'aucun argument n'est passé au programme
    if (argc > 1) {
        usage("Ce programme ne prend pas d'arguments.\n");
    }

    // initialise la graine du générateur aléatoire avec le PID du processus
    srandom(time(NULL));

    // déclarations des structures de données
    JeuDeDonnees data;
    ReseauNeurones reseau;

    // initialise les paramètres du réseau et des données
    data.taille_vecteur = 13; // nombre d'attributs dans le jeu de données des vins
    reseau.taille_vecteurs = data.taille_vecteur; // utilisation pour le réseau
    reseau.min = 0.1f; // bornes pour la génération des vecteurs aléatoires
    reseau.max = 0.5f;
    reseau.nb_iterations = 1500; // nombre total d'itérations pour l'apprentissage
    reseau.taille_voisinage_init = 5.7f; // taille initiale du voisinage
    reseau.taux_apprentissage_alpha_initial = 0.865f; // taux d'apprentissage initial

    // taille de la carte SOM (10x10)
    reseau.carte.lignes = 10;
    reseau.carte.colonnes = 10;

    // lit le jeu de données à partir du fichier et détermine le nombre d'exemples
    lire_jeu_de_donnees(&data, "wine.data");

    // normalise le jeu de données pour que chaque attribut soit entre 0 et 1
    normaliser_dataset(&data);

    // crée la matrice de poids initiale pour le réseau de neurones
    init_carte_neurones(&reseau);

    // initialise les tableaux auxiliaires
    initialisation_tableaux_annexes(&data, &reseau);

    // effectue l'apprentissage du réseau
    apprentissage(&data, &reseau);

    // affiche les résultats après l'apprentissage
    afficher_resultats(&reseau);

    // calcule et affiche le pourcentage de données correctement classifiées
    calculer_pourcentage_justesse(&data, &reseau);

    // libère la mémoire du réseau de neurones et des données
    free_memoire(&reseau, &data);

    return 0;
}
