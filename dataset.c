/*# nom ........ : dataset.c
 * rôle......... : Module en charge de la gestion du jeu de données (dataset: Wine)
 * auteur ...... : Avrile Floro
 * version ..... : v1 du 27/09/2024
 * licence ..... : réalisé dans le cadre du cours d'IA
 * usage ....... :  make  */

#include "elements.h"


/* lit le fichier de données et stocke les exemples et leurs labels */
void lire_jeu_de_donnees(JeuDeDonnees* data, const char* nom_fichier) {

    // ouvre le fichier en mode lecture
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        usage("Erreur lors de l'ouverture du fichier");
    }

    // initialise un buffer pour la lecture des lignes
    char buffer[1024];

    // initialise le compteur de lignes à 0
    data->nb_exemples = 0;

    // compte le nombre de lignes dans le fichier (chaque ligne est un exemple)
    while (fgets(buffer, sizeof(buffer), fichier)) {
        data->nb_exemples++;
    }

    // revient au début du fichier pour la lecture des données
    rewind(fichier);

    // alloue de la mémoire pour stocker tous les exemples
    data->exemples = malloc(data->nb_exemples * sizeof(VecteurDonnees));
    if (!data->exemples) {
        usage("Erreur d'allocation mémoire pour les exemples");
    }

    int indice_ligne = 0;

    // lit chaque ligne du fichier
    while (fgets(buffer, sizeof(buffer), fichier)) {
        // récupère l'exemple correspondant à l'indice actuel dans le dataset (adresse)
        VecteurDonnees* exemple = &data->exemples[indice_ligne];

        // alloue de la mémoire pour les attributs de l'exemple
        exemple->attributs = malloc(data->taille_vecteur * sizeof(float));
        if (!exemple->attributs) {
            usage("Erreur d'allocation pour les attributs");
        }

        // d'abord, extrait le label (classe) de l'exemple [1ère colonne]
        char* token = strtok(buffer, ",");
        exemple->label = atoi(token); // convertit le token en int

        // extrait chaque attribut de l'exemple [à partir de la 2ème colonne]
        for (int i = 0; i < data->taille_vecteur; i++) {
            token = strtok(NULL, ",");
            if (token != NULL) {
                exemple->attributs[i] = atof(token); // convertit le token en float
            } else {
                exemple->attributs[i] = 0.0f; // si le token est NULL, attribut = 0.0
            }
        }
        indice_ligne++; // passe à l'exemple suivant
    }
    // ferme le fichier
    fclose(fichier);
}

/* normalise chaque attribut des données entre 0 et 1 */
void normaliser_dataset(JeuDeDonnees* data) {

    // initialise les tableaux pour stocker les valeurs min et max de chaque attribut
    float* min = malloc(data->taille_vecteur * sizeof(float));
    float* max = malloc(data->taille_vecteur * sizeof(float));


    // initialise les min à FLT_MAX et les max à -FLT_MAX
    for (int i = 0; i < data->taille_vecteur; i++) {
        min[i] = FLT_MAX;
        max[i] = -FLT_MAX;
    }

    // parcourt tous les exemples pour trouver les min et max de chaque attribut
    for (int i = 0; i < data->nb_exemples; i++) {
        // isole l'exemple (avec son adresse)
        VecteurDonnees* exemple = &data->exemples[i];
        // parcourt chaque attribut de l'exemple
        for (int j = 0; j < data->taille_vecteur; j++) {
            if (exemple->attributs[j] < min[j]) {
                min[j] = exemple->attributs[j]; // met à jour le min si nécessaire
            }
            if (exemple->attributs[j] > max[j]) {
                max[j] = exemple->attributs[j]; // met à jour le max si nécessaire
            }
        }
    }

    // normalisation min-max des attributs [après avoir identifié min/max pour chaque]
    for (int i = 0; i < data->nb_exemples; i++) {
        // récupère chaque exemple
        VecteurDonnees* exemple = &data->exemples[i];
        // parcourt les attributs de chaque exemple
        for (int j = 0; j < data->taille_vecteur; j++) {
            if (max[j] - min[j] != 0) {
                // applique la formule de normalisation
                exemple->attributs[j] = (exemple->attributs[j] - min[j]) / (max[j] -
                        min[j]);
            } else { // (pour éviter une div par 0)
                exemple->attributs[j] = 0.0f; // si min == max, attribut normalisé à 0.0

            }
        }
    }

    // libération des tableaux utilisés
    free(min);
    free(max);
}