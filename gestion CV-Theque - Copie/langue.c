#include "langue.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_langue = 0;
bool valider_langue(char* langue) {
    nettoyer_chaine(langue);
    return strlen(langue) > 0 && strlen(langue) < 100;
}

bool langue_existe(const char* fichier, int id_utilisateur, const char* langue) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) return false;

    char ligne[MAX_LINE];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_u;
        char langue_f[100];

        if (sscanf(ligne, "%d;%99[^;];%d", &id, langue_f, &id_u) == 3) {
            if (id_utilisateur == id_u && strcmp(langue, langue_f) == 0) {
                fclose(f);
                return true;
            }
        }
    }
    fclose(f);
    return false;
}

void ajouter_et_sauvegarder_langue(Langue** tete, const char* fichier, int id_utilisateur, char* langue) {
    (void)tete;

    if (!valider_langue(langue)) {
        printf("Langue invalide\n");
        return;
    }

    if (langue_existe(fichier, id_utilisateur, langue)) {
        printf("Langue déjà existante pour cet utilisateur.\n");
        return;
    }

    int max_id = 0;
    FILE* f_read = fopen(fichier, "r");
    if (f_read) {
        char ligne[MAX_LINE];
        int id_temp, uid;
        char lg[100];
        while (fgets(ligne, sizeof(ligne), f_read)) {
            if (sscanf(ligne, "%d;%99[^;];%d", &id_temp, lg, &uid) == 3) {
                if (id_temp > max_id) max_id = id_temp;
            }
        }
        fclose(f_read);
    }

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }
    fprintf(f, "%d;%s;%d\n", max_id + 1, langue, id_utilisateur);
    fclose(f);

    printf("Langue ajoutée avec succès.\n");
}

void afficher_langues_par_utilisateur(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", fichier);
        return;
    }

    char ligne[MAX_LINE];
    int id_langue, id_user;
    char langue[100];
    int trouve = 0;

    printf("\n--- Langues ---\n");

    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d;%99[^;];%d", &id_langue, langue, &id_user) == 3) {
            if (id_user == id_utilisateur) {
                trouve = 1;
                printf("ID: %d - %s\n", id_langue, langue);
                printf("--------------------------\n");
            }
        }
    }

    if (!trouve) {
        printf("Aucune langue trouvee pour cet utilisateur.\n");
    }

    fclose(f);
}



int supprimer_langues_utilisateur(int id_utilisateur, int id_langue) {
    return supprimer_element_par_ids("langues.txt", id_utilisateur, id_langue);
}

int mettre_a_jour_langue(int id_utilisateur, int id_langue, const char* langue) {
    char langue_clean[100];
    strncpy(langue_clean, langue, sizeof(langue_clean) - 1);
    langue_clean[sizeof(langue_clean) - 1] = '\0';
    nettoyer_chaine(langue_clean);

    if (strlen(langue_clean) == 0) {
        printf("Erreur: Langue invalide\n");
        return -1;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees), "%d;%s;%d", id_langue, langue_clean, id_utilisateur);

    int res = mettre_a_jour_element("langues.txt", id_utilisateur, id_langue, nouveaux_donnees);
    if (res)
        printf("Langue mise à jour avec succès.\n");
    else
        printf("Échec de la mise à jour de la langue.\n");

    return res;
}

void gestion_langues(Langue** langues, int id_utilisateur) {
    (void)langues; // paramètre inutilisé
    char choix;
    do {
        printf("\n--- GESTION LANGUES ---\n");
        printf("1. Ajouter une langue\n");
        printf("2. Afficher mes langues\n");
        printf("3. Supprimer une langue\n");
        printf("4. Modifier une langue\n");
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Langue : ");
                char* langue = saisir_chaine();
                ajouter_et_sauvegarder_langue(NULL, "langues.txt", id_utilisateur, langue);
                free(langue);
                break;
            }
            case '2':
                afficher_langues_par_utilisateur("langues.txt", id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID de la langue à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_langues_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {
                int id_modif;
                printf("ID de la langue à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');

                printf("Nouvelle langue : ");
                char* langue = saisir_chaine();

                mettre_a_jour_langue(id_utilisateur, id_modif, langue);
                free(langue);
                break;
            }
            case '5':
                printf("Retour au menu précédent.\n");
                break;
            default:
                printf("Choix invalide.\n");
                break;
        }
    } while (choix != '5');
}
