#include "experience.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "experience.h"
#include "education.h"



int dernier_id_experience = 0;


// Fonction pour créer une nouvelle expérience
Experience* creer_experience(int id_utilisateur, const char* titre, const char* entreprise, const char* date_debut, const char* date_fin) {
    Experience* nouvelle_exp = (Experience*)malloc(sizeof(Experience));
    if (nouvelle_exp == NULL) {
        printf("Erreur : echec de l'allocation memoire.\n");
        return NULL;
    }

    // Attribuer un nouvel ID en utilisant dernier_id_experience
    nouvelle_exp->id = ++dernier_id_experience; // Incrémenter dernier_id_experience
    nouvelle_exp->id_utilisateur = id_utilisateur;  // Assigner l'ID utilisateur

    // Copier les autres champs avec vérification de la longueur
    strncpy(nouvelle_exp->titre, titre, sizeof(nouvelle_exp->titre) - 1);
    nouvelle_exp->titre[sizeof(nouvelle_exp->titre) - 1] = '\0'; // Assurer la null-termination

    strncpy(nouvelle_exp->entreprise, entreprise, sizeof(nouvelle_exp->entreprise) - 1);
    nouvelle_exp->entreprise[sizeof(nouvelle_exp->entreprise) - 1] = '\0';

    strncpy(nouvelle_exp->date_debut, date_debut, sizeof(nouvelle_exp->date_debut) - 1);
    nouvelle_exp->date_debut[sizeof(nouvelle_exp->date_debut) - 1] = '\0';

    strncpy(nouvelle_exp->date_fin, date_fin, sizeof(nouvelle_exp->date_fin) - 1);
    nouvelle_exp->date_fin[sizeof(nouvelle_exp->date_fin) - 1] = '\0';

    nouvelle_exp->suivant = NULL;
    return nouvelle_exp;
}

// Fonction pour ajouter une expérience à la liste
void ajouter_experience(Experience** tete, Experience* nouvelle_exp) {
    if (*tete == NULL) {
        *tete = nouvelle_exp; // Si la liste est vide, la nouvelle expérience devient la tête
    } else {
        Experience* current = *tete;
        while (current->suivant != NULL) {
            current = current->suivant; // Parcourir jusqu'à la fin de la liste
        }
        current->suivant = nouvelle_exp; // Ajouter à la fin
    }
}

// Fonction pour ajouter une expérience et la sauvegarder dans un fichier
 void ajouter_et_sauvegarder_experience(Experience** tete,  char* fichier, int id_utilisateur,  char* titre,  char* entreprise,  char* date_debut,  char* date_fin) {
    // Valider les données
    if (!valider_experience(titre, entreprise, date_debut, date_fin)) {
        printf("Erreur : les donnees saisies sont invalides.\n");
        return;
    }

    // Vérifier si l'expérience existe déjà
    if (experience_existe(fichier, id_utilisateur, titre, entreprise, date_debut, date_fin)) {
        printf("Erreur : cette experience existe deja.\n");
        return;
    }

    // Créer une nouvelle expérience
    Experience* nouvelle_exp = creer_experience(id_utilisateur, titre, entreprise, date_debut, date_fin);
    if (nouvelle_exp == NULL) {
        printf("Erreur : impossible de creer l'experience.\n");
        return;
    }

    // Ajouter l'expérience à la liste
    ajouter_experience(tete, nouvelle_exp);

    // Sauvegarder dans le fichier
    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }
    fprintf(f, "%d;%d;%s;%s;%s;%s\n", nouvelle_exp->id, nouvelle_exp->id_utilisateur, nouvelle_exp->titre, nouvelle_exp->entreprise, nouvelle_exp->date_debut, nouvelle_exp->date_fin);
    fclose(f);

    printf("Expérience ajoutée avec succès.\n");
}

// Fonction pour afficher toutes les expériences
void afficher_experiences_par_utilisateur(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", fichier);
        return;
    }

    char ligne[256];
    int id_user;
    char titre[100], entreprise[100], date_debut[20], date_fin[20];
    int trouve = 0;

    printf("\n--- Expériences de l'utilisateur ID %d ---\n", id_utilisateur);

    while (fgets(ligne, sizeof(ligne), f)) {
        char* token = strtok(ligne, ";");
        if (token == NULL) continue;
        strncpy(titre, token, sizeof(titre));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(entreprise, token, sizeof(entreprise));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(date_debut, token, sizeof(date_debut));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(date_fin, token, sizeof(date_fin));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        id_user = atoi(token);

        // Vérifier si l'ID utilisateur correspond
        if (id_user == id_utilisateur) {
            trouve = 1;
            printf("Titre: %s\n", titre);
            printf("Entreprise: %s\n", entreprise);
            printf("Date début: %s\n", date_debut);
            printf("Date fin: %s\n", date_fin);
            printf("--------------------------\n");
        }
    }

    if (!trouve) {
        printf("Aucune expérience trouvée pour cet utilisateur.\n");
    }

    fclose(f);
}

// Fonction pour libérer la mémoire de la liste des expériences
void liberer_experiences(Experience* exp) {
    while (exp != NULL) {
        Experience* temp = exp;
        exp = exp->suivant;
        free(temp);
    }
}

// Fonction pour valider les données d'une expérience
bool valider_experience( char* titre,  char* entreprise,  char* date_debut,  char* date_fin) {
    // Vérifier que les champs ne sont pas vides
    nettoyer_chaine(titre);
    nettoyer_chaine(entreprise);
    nettoyer_chaine(date_debut);
    nettoyer_chaine(date_fin);
    
    if (strlen(titre) == 0 || strlen(entreprise) == 0 || strlen(date_debut) == 0 || strlen(date_fin) == 0) {
        return false;
    }

    // Vérifier que les dates sont valides
    if (!est_date_valide(date_debut) || !est_date_valide(date_fin)) {
        return false;
    }

    // Vérifier que la date de fin est postérieure ou égale à la date de début
    if (comparer_dates(date_fin, date_debut) < 0) {
        return false;
    }

    return true;
}

// Fonction pour vérifier si une expérience existe déjà dans le fichier
bool experience_existe(const char* fichier, int id_utilisateur, const char* titre, const char* entreprise, const char* date_debut, const char* date_fin) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false; // Si le fichier n'existe pas, l'expérience ne peut pas exister
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_user;
        char titre_fichier[100], entreprise_fichier[100], date_debut_fichier[20], date_fin_fichier[20];
        sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]", &id, &id_user, titre_fichier, entreprise_fichier, date_debut_fichier, date_fin_fichier);

        if (id_utilisateur == id_user &&
            strcmp(titre, titre_fichier) == 0 &&
            strcmp(entreprise, entreprise_fichier) == 0 &&
            strcmp(date_debut, date_debut_fichier) == 0 &&
            strcmp(date_fin, date_fin_fichier) == 0) {
            fclose(f);
            return true; // L'expérience existe déjà
        }
    }

    fclose(f);
    return false; // L'expérience n'existe pas
}

void gestion_experiences(Experience** experiences, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION EXPÉRIENCES ---\n");
        printf("1. Ajouter une expérience\n");
        printf("2. Afficher mes expériences\n");
        printf("3. Supprimer une expérience\n");
        printf("4. Modifier une expérience\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Titre du poste :");
                char *titre = saisir_chaine();
                printf("Entreprise :");
                char *entreprise = saisir_chaine();
                printf("Date debut (YYYY-MM-DD) : ");
                char *date_debut = saisir_chaine();
                printf("Date fin (YYYY-MM-DD) :");
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
                afficher_experiences_par_utilisateur("experiences.txt", id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID de l'expérience à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_experiences_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {  // Nouveau cas pour la modification
                int id_modif;
                printf("ID de l'expérience à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouveau titre du poste : ");
                char *titre = saisir_chaine();
                printf("Nouvelle entreprise : ");
                char *entreprise = saisir_chaine();
                printf("Nouvelle date début (YYYY-MM-DD) : ");
                char *date_debut = saisir_chaine();
                printf("Nouvelle date fin (YYYY-MM-DD) : ");
                char *date_fin = saisir_chaine();
                
                int result = mettre_a_jour_experience(id_utilisateur, id_modif,
                                                    titre, entreprise,
                                                    date_debut, date_fin);
                if (result > 0) {
                    printf("Expérience mise à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour de l'expérience.\n");
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
/*void supprimer_experience_par_ids(const char* fichier, int id_utilisateur, int id_experience) {
    FILE *f = fopen(fichier, "r");
    FILE *temp = fopen("temp.txt", "w");
    char ligne[256];
    int trouve = 0;

    while (fgets(ligne, sizeof(ligne), f)) {
        int current_id, current_user;
        sscanf(ligne, "%d;%d;", &current_id, &current_user);

        if (!(current_id == id_experience && current_user == id_utilisateur)) {
            fputs(ligne, temp);
        } else {
            trouve = 1;
        }
    }

    fclose(f);
    fclose(temp);

    if (trouve) {
        remove(fichier);
        rename("temp.txt", fichier);
        printf("Expérience ID %d supprimée\n", id_experience);
    } else {
        remove("temp.txt");
        printf("Expérience non trouvée\n");
    }
}
*/

int supprimer_experiences_utilisateur(int id_utilisateur, int id_experience) {
    return supprimer_element_par_ids("experiences.txt", id_utilisateur, id_experience);
}

int mettre_a_jour_experience(int id_utilisateur, int id_experience,
                            const char* titre, const char* entreprise,
                            const char* date_debut, const char* date_fin) {
    // Validation de base
    if (!valider_periode(date_debut, date_fin)) {
        return -1;
    }

    // Vérification des chevauchements
    FILE *f = fopen("experiences.txt", "r");
    if (f) {
        char ligne[MAX_LINE];
        while (fgets(ligne, sizeof(ligne), f)) {
            int id, user_id;
            char exist_titre[100], exist_entreprise[100];
            char exist_debut[20], exist_fin[20];
            
            if (sscanf(ligne, "%d;%[^;];%[^;];%[^;];%[^;];%d",
                      &id, exist_titre, exist_entreprise, exist_debut, exist_fin, &user_id) == 6) {
                if (user_id == id_utilisateur && id != id_experience) {
                    // Vérification du chevauchement
                    if (comparer_dates(date_debut, exist_fin) <= 0 && 
                        comparer_dates(date_fin, exist_debut) >= 0) {
                        printf("Erreur : Chevauchement avec l'expérience ID %d (%s - %s)\n",
                              id, exist_debut, exist_fin);
                        fclose(f);
                        return -1;
                    }
                }
            }
        }
        fclose(f);
    }

    // Construction des données
    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%s;%s;%s;%s;%d", 
             id_experience, titre, entreprise, date_debut, date_fin, id_utilisateur);

    return mettre_a_jour_element("experiences.txt", id_utilisateur, id_experience, nouveaux_donnees);
}
