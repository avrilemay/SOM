/*# nom ......... : resultats.c
 * rôle........ : Module en charge des résultats et de l'évaluation
 * auteur ...... : Avrile Floro
 * version ..... : v1 du 27/09/2024
 * licence ..... : réalisé dans le cadre du cours d'IA
 * usage ....... :  make        */

#include "elements.h"

/* affiche les labels associés à chaque neurone de la carte */
void afficher_resultats(ReseauNeurones* reseau) {
    // affiche la légende des classes (ici des cépages)
    printf("[1] = Cépage 1\n[2] = Cépage 2\n[3] = Cépage 3\n\n");

    // parcourt chaque neurone de la carte et affiche son label
    for (int i = 0; i < reseau->carte.lignes; i++) {
        for (int j = 0; j < reseau->carte.colonnes; j++) {
            printf(" [%d] ", reseau->carte.neurones[i][j].label);
        }
        printf("\n"); // nouvelle ligne après chaque ligne de la carte
    }
}

/* calcule le pourcentage de données correctement classifiées par le SOM */
void calculer_pourcentage_justesse(JeuDeDonnees* data, ReseauNeurones* reseau) {
    int correct_matches = 0;

    // parcourt chaque exemple du dataset
    for (int i = 0; i < data->nb_exemples; i++) {
        VecteurDonnees* exemple = &data->exemples[i];

        // trouve le BMU pour l'exemple courant
        Neurone* bmu = choix_BMU(exemple->attributs, reseau);

        // compare le label de l'exemple avec celui du BMU
        if (exemple->label == bmu->label) {
            correct_matches++; // incrémente le compteur si les labels correspondent
        }
    }

    // calcule le pourcentage de justesse
    float pourcentage = ((float)correct_matches / (float)data->nb_exemples) * 100.0f;
    printf("\nPourcentage de justesse des catégories des vins: %.2f%%\n", pourcentage);
}
