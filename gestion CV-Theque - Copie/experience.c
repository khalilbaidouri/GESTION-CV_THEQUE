#include "experience.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int dernier_id_experience=0;
void ajouter_et_sauvegarder_experience(Experience** tete, const char* fichier, int id_utilisateur, char* titre, char* entreprise, char* date_debut, char* date_fin) {
    (void)tete;

    if (!valider_experience(titre, entreprise, date_debut, date_fin)) {
        printf("Erreur : les donnees saisies sont invalides.\n");
        return;
    }

    if (experience_existe(fichier, id_utilisateur, titre, entreprise, date_debut, date_fin)) {
        printf("Erreur : cette experience existe deja.\n");
        return;
    }

    int max_id = 0;
    FILE* f_read = fopen(fichier, "r");
    if (f_read) {
        char ligne[512];
        int id_temp, id_user;
        char titre_temp[100], entreprise_temp[100], date_debut_temp[20], date_fin_temp[20];

        while (fgets(ligne, sizeof(ligne), f_read)) {
            if (sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]",
                      &id_temp, &id_user, titre_temp, entreprise_temp, date_debut_temp, date_fin_temp) == 6) {
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
            titre,
            entreprise,
            date_debut,
            date_fin);
    fclose(f);

    printf("Experience ajoutee avec succes.\n");
}

void afficher_experiences(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", fichier);
        return;
    }

    char ligne[512];
    int trouve = 0;

    printf("\n--- Experiences ---\n");

    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_user;
        char titre[100], entreprise[100], date_debut[20], date_fin[20];

        if (sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]", &id, &id_user, titre, entreprise, date_debut, date_fin) == 6) {
            if (id_user == id_utilisateur) {
                trouve = 1;
                printf("ID: %d\n", id);
                printf("Titre: %s\n", titre);
                printf("Entreprise: %s\n", entreprise);
                printf("Date debut: %s\n", date_debut);
                printf("Date fin: %s\n", date_fin);
                printf("--------------------------\n");
            }
        }
    }

    if (!trouve) {
        printf("Aucune experience trouvee pour cet utilisateur.\n");
    }

    fclose(f);
}

void liberer_experiences(Experience* exp) {
    while (exp != NULL) {
        Experience* temp = exp;
        exp = exp->suivant;
        free(temp);
    }
}

bool valider_experience(char* titre, char* entreprise, char* date_debut, char* date_fin) {
    nettoyer_chaine(titre);
    nettoyer_chaine(entreprise);
    nettoyer_chaine(date_debut);
    nettoyer_chaine(date_fin);

    if (strlen(titre) == 0 || strlen(entreprise) == 0 || strlen(date_debut) == 0 || strlen(date_fin) == 0) {
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

bool experience_existe(const char* fichier, int id_utilisateur, const char* titre, const char* entreprise, const char* date_debut, const char* date_fin) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_user;
        char titre_fichier[100], entreprise_fichier[100], date_debut_fichier[20], date_fin_fichier[20];

        if (sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]", &id, &id_user, titre_fichier, entreprise_fichier, date_debut_fichier, date_fin_fichier) == 6) {
            if (id_utilisateur == id_user &&
                strcmp(titre, titre_fichier) == 0 &&
                strcmp(entreprise, entreprise_fichier) == 0 &&
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

void gestion_experiences(Experience** experiences, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION EXPERIENCES ---\n");
        printf("1. Ajouter une experience\n");
        printf("2. Afficher mes experiences\n");
        printf("3. Supprimer une experience\n");
        printf("4. Modifier une experience\n");
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Titre du poste : ");
                char *titre = saisir_chaine();
                printf("Entreprise : ");
                char *entreprise = saisir_chaine();
                printf("Date debut (YYYY-MM-DD) : ");
                char *date_debut = saisir_chaine();
                printf("Date fin (YYYY-MM-DD) : ");
                char *date_fin = saisir_chaine();

                ajouter_et_sauvegarder_experience(experiences, "experiences.txt",
                                                id_utilisateur, titre, entreprise,
                                                date_debut, date_fin);

                free(titre);
                free(entreprise);
                free(date_debut);
                free(date_fin);
                break;
            }
            case '2':
                afficher_experiences("experiences.txt", id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID de l'experience a supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_experiences_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {
                int id_modif;
                printf("ID de l'experience a modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');

                printf("Nouveau titre du poste : ");
                char *titre = saisir_chaine();
                printf("Nouvelle entreprise : ");
                char *entreprise = saisir_chaine();
                printf("Nouvelle date debut (YYYY-MM-DD) : ");
                char *date_debut = saisir_chaine();
                printf("Nouvelle date fin (YYYY-MM-DD) : ");
                char *date_fin = saisir_chaine();

                int result = mettre_a_jour_experience(id_utilisateur, id_modif,
                                                    titre, entreprise,
                                                    date_debut, date_fin);
                if (result > 0) {
                    printf("Experience mise a jour avec succes!\n");
                } else {
                    printf("Echec de la mise a jour de l'experience.\n");
                }

                free(titre);
                free(entreprise);
                free(date_debut);
                free(date_fin);
                break;
            }
        }
    } while (choix != '5');
}

int supprimer_experiences_utilisateur(int id_utilisateur, int id_experience) {
    return supprimer_element_par_ids("experiences.txt", id_utilisateur, id_experience);
}

int mettre_a_jour_experience(int id_utilisateur, int id_experience,
                            const char* titre, const char* entreprise,
                            const char* date_debut, const char* date_fin) {
    if (!valider_periode(date_debut, date_fin)) {
        return -1;
    }
    FILE *f = fopen("experiences.txt", "r");
    if (f) {
        char ligne[MAX_LINE];
        while (fgets(ligne, sizeof(ligne), f)) {
            int id, user_id;
            char exist_titre[100], exist_entreprise[100];
            char exist_debut[20], exist_fin[20];

            if (sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]",
                      &id, &user_id, exist_titre, exist_entreprise, exist_debut, exist_fin) == 6) {
                if (user_id == id_utilisateur && id != id_experience) {
                    if (comparer_dates(date_debut, exist_fin) <= 0 &&
                        comparer_dates(date_fin, exist_debut) >= 0) {
                        printf("Erreur : Chevauchement avec l'experience ID %d (%s - %s)\n",
                              id, exist_debut, exist_fin);
                        fclose(f);
                        return -1;
                    }
                }
            }
        }
        fclose(f);
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%d;%s;%s;%s;%s",
             id_experience, id_utilisateur, titre, entreprise, date_debut, date_fin);

    return mettre_a_jour_element("experiences.txt", id_utilisateur, id_experience, nouveaux_donnees);
}