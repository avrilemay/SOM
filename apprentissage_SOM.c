/*# nom ........ : apprentissage_SOM.c
 * rôle......... : Module en charge de l'entraînement du SOM
 * auteur ...... : Avrile Floro
 * version ..... : v1 du 27/09/2024
 * licence ..... : réalisé dans le cadre du cours d'IA
 * usage ....... :  make        */

#include "elements.h"

/* mélange les indices du tableau dans un ordre aléatoire */
void melanger_indices(int* indices_a_mixer, int taille_du_tableau) {
    // algorithme de Fisher-Yates pour mélanger le tableau
    for (int i = taille_du_tableau - 1; i > 0; i--) {
        int j = random() % (i + 1);
        // échange les éléments i et j
        int temp = indices_a_mixer[i];
        indices_a_mixer[i] = indices_a_mixer[j];
        indices_a_mixer[j] = temp;
    }
}

/* calcule la distance euclidienne entre deux vecteurs */
float distance_euclidienne(const float* vect_entree, const float* vect_neurone,
                           int taille_vecteur) {
    float somme = 0; // initialisation de la somme

    // calcule la somme des carrés des différences
    for (int i = 0; i < taille_vecteur; i++) {
        float diff = vect_entree[i] - vect_neurone[i];
        somme += diff * diff;
    }
    // retourne la racine carrée de la somme = la distance euclidienne
    return sqrtf(somme);
}

/* sélectionne le neurone le plus proche du vecteur d'entrée (= le BMU) */
Neurone* choix_BMU(float* v_entree, ReseauNeurones* reseau) {
    float distance_min = INFINITY;
    Neurone* bmu = NULL;

    // mélange les indices des lignes et colonnes de la carte
    // permet d'assurer que le BMU est choisi aléatoirement (si plusieurs)

    // initialise shuffle_order d'après seq_order avec memcpy(source, dest, taille)
    memcpy(reseau->carte.shuffle_order_lignes, reseau->carte.sequentiel_order_lignes,
           reseau->carte.lignes * sizeof(int));
    // mélange les indices de shuffle_order d'après l'algo de Fisher-Yates
    melanger_indices(reseau->carte.shuffle_order_lignes, reseau->carte.lignes);

    memcpy(reseau->carte.shuffle_order_col, reseau->carte.sequentiel_order_col,
           reseau->carte.colonnes * sizeof(int));
    melanger_indices(reseau->carte.shuffle_order_col, reseau->carte.colonnes);

    // parcourt tous les neurones de la carte pour déterminer le BMU
    for (int i = 0; i < reseau->carte.lignes; i++) {
        int idx_ligne = reseau->carte.shuffle_order_lignes[i];
        for (int j = 0; j < reseau->carte.colonnes; j++) {
            int idx_col = reseau->carte.shuffle_order_col[j];
            // récupère l'adresse du neurone
            Neurone* neurone = &reseau->carte.neurones[idx_ligne][idx_col];

            // calcule distance entre le vecteur d'entrée (dataset) et le neurone évalué
            float distance = distance_euclidienne(v_entree, neurone->poids, reseau->taille_vecteurs);

            // met à jour le BMU si une distance euclidienne plus petite est trouvée
            if (distance < distance_min) {
                distance_min = distance;
                bmu = neurone;
            }
        }
    }
    return bmu; // renvoie le neurone BMU
}

/* met à jour les poids du neurone en utilisant la règle d'apprentissage */
void regle_apprentissage(const float* v_entree, Neurone* neurone, Neurone* bmu,
                         ReseauNeurones* reseau) {
    // variables pour stocker les positions du neurone et du BMU
    int x_neurone = 0, y_neurone = 0, x_bmu = 0, y_bmu = 0;

    // trouve les positions du neurone courant et du BMU dans la carte
    for (int i = 0; i < reseau->carte.lignes; i++) {
        for (int j = 0; j < reseau->carte.colonnes; j++) {
            // compare les adresses (et non les valeurs)
            if (&reseau->carte.neurones[i][j] == neurone) {
                x_neurone = i;
                y_neurone = j;
            }
            if (&reseau->carte.neurones[i][j] == bmu) {
                x_bmu = i;
                y_bmu = j;
            }
        }
    }

    // calcule la distance PHYSIQUE entre le neurone et le BMU sur la carte
    float distance_physique = sqrt(pow(x_neurone - x_bmu, 2) + pow(y_neurone - y_bmu, 2));

    // vérifie si le neurone est dans le voisinage actif du BMU
    if (distance_physique < reseau->taille_voisinage_actuelle) {
        // met à jour les poids du neurone si c'est le cas (reste à l'identique sinon)
        for (int i = 0; i < reseau->taille_vecteurs; i++) {
            // ajuste poids vers valeurs du vecteur d'entrée selon taux d'apprentissage
            neurone->poids[i] += reseau->taux_apprentissage_alpha_actuel * (v_entree[i]
                    - neurone->poids[i]);
        }
    }
}

/* fonction de réduction du taux d'apprentissage avec deux phases distinctes */
void mettre_a_jour_apprentissage_alpha(float iteration, ReseauNeurones* reseau) {
    // taux d'apprentissage alpha initial conservé dans une variable
    float alpha_initial = reseau->taux_apprentissage_alpha_initial;

    // définition des phases de réduction du taux d'apprentissage
    // phase 1 : 20% des itérations
    float nb_iter_phase_1 = (float)reseau->nb_iterations * 0.2f;
    // phase 2 : 80% des itérations
    float nb_iter_phase_2 = (float)reseau->nb_iterations - nb_iter_phase_1;

    // phase 1 : réduction linéaire rapide d'alpha > alpha_initial / 10
    if (iteration < nb_iter_phase_1) {
        reseau->taux_apprentissage_alpha_actuel = alpha_initial * (1.0f - (iteration /
                nb_iter_phase_1) * 0.9f);

        // phase 2 : réduction linéaire lente d'alpha_initial / 10 > alpha_initial / 100
    } else {
        reseau->taux_apprentissage_alpha_actuel = (alpha_initial / 10.0f) * (1.0f - (
                (iteration - nb_iter_phase_1) / nb_iter_phase_2) * 0.9f);
    }
}

/* met à jour la taille du voisinage en fonction de l'itération courante */
void taille_voisinage_actif(float iteration, ReseauNeurones* reseau) {
    float voisinage_initial = reseau->taille_voisinage_init;
    float total_iter = (float)reseau->nb_iterations;

    // réduction linéaire de la taille du voisinage
    reseau->taille_voisinage_actuelle = voisinage_initial * (1 - (iteration /
            total_iter));

    // la taille du voisinage n'est jamais inférieure à 1.0
    if (reseau->taille_voisinage_actuelle < 1.0f) {
        reseau->taille_voisinage_actuelle = 1.0f;
    }
}

/* fonction principale d'apprentissage */
void apprentissage(JeuDeDonnees* data, ReseauNeurones* reseau) {

    // boucle principale sur le nombre d'itérations (EPOCHS)
    for (int iter = 0; iter < reseau->nb_iterations; iter++) {

        // mélange les exemples du dataset pour une présentation aléatoire
        // init. shuffle_order d'après seq_order grâce à memcpy(dest, source, taille)
        // puis mélange les indices du shuffle_order grâce à l'algorithme de Fisher-Yates
        memcpy(data->shuffled_order_lignes, data->sequence_ordre_lignes,
               data->nb_exemples * sizeof(int));
        melanger_indices(data->shuffled_order_lignes, data->nb_exemples);

        // met à jour le taux d'apprentissage alpha et la taille du voisinage
        // alpha: deux phases de baisse linéaire (rapide puis lente)
        mettre_a_jour_apprentissage_alpha((float)iter, reseau);
        taille_voisinage_actif((float)iter, reseau); // linéaire

        // parcourt chaque exemple du dataset
        for (int i = 0; i < data->nb_exemples; i++) {
                    // isole l'indice de l'exemple du dataset (numéro ligne)
            int indice_dataset = data->shuffled_order_lignes[i];
                    // récupère le vecteur associé à l'indice (adresse)
            VecteurDonnees* exemple = &data->exemples[indice_dataset];
            // récupère les attributs du vecteur exemple (et pas le label)
            float* entree = exemple->attributs;

            // trouve le BMU (neurone gagnant) pour l'exemple en cours du dataset
            Neurone* bmu = choix_BMU(entree, reseau);

            // associe le label de l'exemple du dataset courant au BMU
            bmu->label = exemple->label;

            // parcourt tous les neurones de la carte pour mettre à jour leurs poids
            for (int k = 0; k < reseau->carte.lignes; k++) {
                for (int l = 0; l < reseau->carte.colonnes; l++) {
                    // isole chaque neurone de la carte (grâce à son adresse)
                    Neurone* neurone = &reseau->carte.neurones[k][l];
                    // modification du poids du neurone
                    // SI distance physique avec BMU < taille du voisinage actuel
                    regle_apprentissage(entree, neurone, bmu, reseau);
                }
            }
        }
    }
}