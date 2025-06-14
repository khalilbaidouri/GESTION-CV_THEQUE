#include "education.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilisateur.h"
#include <stdbool.h>

int dernier_id_education = 0;
void ajouter_et_sauvegarder_education(Education** tete, const char* fichier, int id_utilisateur, char* diplome, char* institution, char* date_debut, char* date_fin) {
    (void)tete;

    if (!valider_education(diplome, institution, date_debut, date_fin)) {
        printf("Erreur : les donnees saisies sont invalides.\n");
        return;
    }

    if (education_existe(fichier, id_utilisateur, diplome, institution, date_debut, date_fin)) {
        printf("Erreur : cette education existe deja.\n");
        return;
    }

    int max_id = 0;
    FILE* f_read = fopen(fichier, "r");
    if (f_read) {
        char ligne[512];
        int id_temp, id_user;
        char diplome_temp[100], institution_temp[100], date_debut_temp[20], date_fin_temp[20];

        while (fgets(ligne, sizeof(ligne), f_read)) {
            if (sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]",
                      &id_temp, &id_user, diplome_temp, institution_temp, date_debut_temp, date_fin_temp) == 6) {
                if (id_temp > max_id) {
                    max_id = id_temp;
                }
                      }
        }
        fclose(f_read);
    }
    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }

    fprintf(f, "%d;%d;%s;%s;%s;%s\n",
            max_id + 1,
            id_utilisateur,
            diplome,
            institution,
            date_debut,
            date_fin);
    fclose(f);

    printf("Education ajoutee avec succes.\n");
}

void afficher_educations_par_utilisateur(int id_utilisateur) {
    FILE* fichier = fopen("educations.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char ligne[500];
    int id_edu, id_user;
    int trouve = 0;

    printf("\n--- Formations de l'utilisateur ---\n");

    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *token;
        char diplome[100], institution[100], date_debut[20], date_fin[20];

        token = strtok(ligne, ";");
        if (token == NULL)
            continue;
        id_edu = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        id_user = atoi(token);

        if (id_user == id_utilisateur) {
            trouve = 1;

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            strncpy(diplome, token, sizeof(diplome) - 1);
            diplome[sizeof(diplome) - 1] = '\0';

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            strncpy(institution, token, sizeof(institution) - 1);
            institution[sizeof(institution) - 1] = '\0';

            token = strtok(NULL, ";");
            if (token == NULL) continue;
            strncpy(date_debut, token, sizeof(date_debut) - 1);
            date_debut[sizeof(date_debut) - 1] = '\0';

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            char* newline = strchr(token, '\n');
            if (newline)
                *newline = '\0';
            strncpy(date_fin, token, sizeof(date_fin) - 1);
            date_fin[sizeof(date_fin) - 1] = '\0';

            printf("ID education: %d\n", id_edu);
            printf("Diplome: %s\n", diplome);
            printf("Institution: %s\n", institution);
            printf("Debut: %s\n", date_debut);
            printf("Fin: %s\n", date_fin);
            printf("--------------------------\n");
        }
    }

    if (!trouve) {
        printf("Aucune formation trouvee pour cet utilisateur.\n");
    }

    fclose(fichier);
}

void liberer_educations(Education* edu) {
    while (edu != NULL) {
        Education* temp = edu;
        edu = edu->suivant;
        free(temp);
    }
}

bool valider_education(char* diplome, char* institution, char* date_debut, char* date_fin) {
    nettoyer_chaine(diplome);
    nettoyer_chaine(institution);
    nettoyer_chaine(date_debut);
    nettoyer_chaine(date_fin);

    if (strlen(diplome) == 0 || strlen(institution) == 0 || strlen(date_debut) == 0 || strlen(date_fin) == 0) {
        return false;
    }

    if (!est_date_valide(date_debut) || !est_date_valide(date_fin)) {
        return false;
    }

    if (comparer_dates(date_fin, date_debut) < 0) {
        return false;
    }

    return true;
}

bool education_existe(const char* fichier, int id_utilisateur, const char* diplome, const char* institution, const char* date_debut, const char* date_fin) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false;
    }

    if (!utilisateur_existe_par_id("utilisateurs.txt", id_utilisateur)) {
        printf("Erreur : l'utilisateur avec l'ID %d n'existe pas.\n", id_utilisateur);
        fclose(f);
        return false;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_user;
        char diplome_fichier[100], institution_fichier[100], date_debut_fichier[20], date_fin_fichier[20];

        if (sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]", &id, &id_user, diplome_fichier, institution_fichier, date_debut_fichier, date_fin_fichier) == 6) {
            if (id_utilisateur == id_user &&
                strcmp(diplome, diplome_fichier) == 0 &&
                strcmp(institution, institution_fichier) == 0 &&
                strcmp(date_debut, date_debut_fichier) == 0 &&
                strcmp(date_fin, date_fin_fichier) == 0) {
                fclose(f);
                return true;
            }
        }
    }

    fclose(f);
    return false;
}

void gestion_educations(Education** educations, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION EDUCATIONS ---\n");
        printf("1. Ajouter une education\n");
        printf("2. Afficher mes educations\n");
        printf("3. Supprimer une education\n");
        printf("4. Modifier une education\n");
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Diplome : ");
                char *diplome = saisir_chaine();
                printf("Institution : ");
                char *institution = saisir_chaine();
                printf("Date debut (YYYY-MM-DD) : ");
                char *date_debut = saisir_chaine();
                printf("Date fin (YYYY-MM-DD) : ");
                char *date_fin = saisir_chaine();

                ajouter_et_sauvegarder_education(educations, "educations.txt",
                                               id_utilisateur, diplome, institution,
                                               date_debut, date_fin);

                free(diplome);
                free(institution);
                free(date_debut);
                free(date_fin);
                break;
            }
            case '2':
                afficher_educations_par_utilisateur(id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID de l'education a supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_educations_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {
                int id_modif;
                printf("ID de l'education a modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');

                printf("Nouveau diplome : ");
                char *diplome = saisir_chaine();
                printf("Nouvelle institution : ");
                char *institution = saisir_chaine();
                printf("Nouvelle date debut (YYYY-MM-DD) : ");
                char *date_debut = saisir_chaine();
                printf("Nouvelle date fin (YYYY-MM-DD) : ");
                char *date_fin = saisir_chaine();

                int result = mettre_a_jour_education(id_utilisateur, id_modif,
                                                    diplome, institution,
                                                    date_debut, date_fin);
                if (result > 0) {
                    printf("Education mise a jour avec succes!\n");
                } else {
                    printf("Echec de la mise a jour de l'education.\n");
                }

                free(diplome);
                free(institution);
                free(date_debut);
                free(date_fin);
                break;
            }
        }
    } while (choix != '5');
}

int supprimer_educations_utilisateur(int id_utilisateur, int id_education) {
    return supprimer_element_par_ids("educations.txt", id_utilisateur, id_education);
}

int mettre_a_jour_education(int id_utilisateur, int id_education,
                           const char* diplome, const char* institution,
                           const char* date_debut, const char* date_fin) {
    if (!valider_periode(date_debut, date_fin)) {
        return -1;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%d;%s;%s;%s;%s",
             id_education, id_utilisateur, diplome, institution, date_debut, date_fin);

    return mettre_a_jour_element("educations.txt", id_utilisateur, id_education, nouveaux_donnees);
}

void afficher_educations(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (!f) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }

    char ligne[512];
    int trouve = 0;

    printf("\n--- Formations ---\n");

    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_user;
        char diplome[100], etablissement[100], date_debut[20], date_fin[20];

        if (sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]", &id, &id_user, diplome, etablissement, date_debut, date_fin) == 6) {
            if (id_user == id_utilisateur) {
                trouve = 1;
                printf("ID: %d\n", id);
                printf("Diplome: %s\n", diplome);
                printf("Etablissement: %s\n", etablissement);
                printf("Date debut: %s\n", date_debut);
                printf("Date fin: %s\n", date_fin);
                printf("--------------------------\n");
            }
        }
    }

    if (!trouve) {
        printf("Aucune formation trouvee.\n");
    }

    fclose(f);
}