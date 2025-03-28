#include "education.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "experience.h"
#include "education.h"
#include "utilisateur.h"
#include <stdbool.h>

int dernier_id_education = 0;

// Fonction pour créer une nouvelle éducation
Education* creer_education(int id_utilisateur, const char* diplome, const char* institution, const char* date_debut, const char* date_fin) {
    Education* nouvelle_edu = (Education*)malloc(sizeof(Education));
    if (nouvelle_edu == NULL) {
        printf("Erreur : echec de l'allocation memoire.\n");
        return NULL;
    }

    // Attribuer un nouvel ID en utilisant dernier_id_education
    nouvelle_edu->id = ++dernier_id_education; // Incrémenter dernier_id_education
    nouvelle_edu->id_utilisateur = id_utilisateur;  // Assigner l'ID utilisateur

    // Copier les autres champs avec vérification de la longueur
    strncpy(nouvelle_edu->diplome, diplome, sizeof(nouvelle_edu->diplome) - 1);
    nouvelle_edu->diplome[sizeof(nouvelle_edu->diplome) - 1] = '\0'; // Assurer la null-termination

    strncpy(nouvelle_edu->institution, institution, sizeof(nouvelle_edu->institution) - 1);
    nouvelle_edu->institution[sizeof(nouvelle_edu->institution) - 1] = '\0';

    strncpy(nouvelle_edu->date_debut, date_debut, sizeof(nouvelle_edu->date_debut) - 1);
    nouvelle_edu->date_debut[sizeof(nouvelle_edu->date_debut) - 1] = '\0';

    strncpy(nouvelle_edu->date_fin, date_fin, sizeof(nouvelle_edu->date_fin) - 1);
    nouvelle_edu->date_fin[sizeof(nouvelle_edu->date_fin) - 1] = '\0';

    nouvelle_edu->suivant = NULL;
    return nouvelle_edu;
}

// Fonction pour ajouter une éducation à la liste
void ajouter_education(Education** tete, Education* nouvelle_edu) {
    if (*tete == NULL) {
        *tete = nouvelle_edu; // Si la liste est vide, la nouvelle éducation devient la tête
    } else {
        Education* current = *tete;
        while (current->suivant != NULL) {
            current = current->suivant; // Parcourir jusqu'à la fin de la liste
        }
        current->suivant = nouvelle_edu; // Ajouter à la fin
    }
}

// Fonction pour ajouter une éducation et la sauvegarder dans un fichier
void ajouter_et_sauvegarder_education(Education** tete,  char* fichier, int id_utilisateur,  char* diplome,  char* institution,  char* date_debut,  char* date_fin) {
    // Valider les données
    if (!valider_education(diplome, institution, date_debut, date_fin)) {
        printf("Erreur : les donnees saisies sont invalides.\n");
        return;
    }

    // Vérifier si l'éducation existe déjà
    if (education_existe(fichier, id_utilisateur, diplome, institution, date_debut, date_fin)) {
        printf("Erreur : cette éducation existe deje.\n");
        return;
    }
    
  

    // Créer une nouvelle éducation
    Education* nouvelle_edu = creer_education(id_utilisateur, diplome, institution, date_debut, date_fin);
    if (nouvelle_edu == NULL) {
        printf("Erreur : impossible de créer l'education.\n");
        return;
    }
    


    // Ajouter l'éducation à la liste
    ajouter_education(tete, nouvelle_edu);

    // Sauvegarder dans le fichier
    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }
    fprintf(f, "%d;%d;%s;%s;%s;%s\n", nouvelle_edu->id, nouvelle_edu->id_utilisateur, nouvelle_edu->diplome, nouvelle_edu->institution, nouvelle_edu->date_debut, nouvelle_edu->date_fin);
    fclose(f);

    printf("Education ajoutee avec succes.\n");
}

// Fonction pour afficher toutes les éducations
void afficher_educations_par_utilisateur(int id_utilisateur) {
    FILE* fichier = fopen("educations.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char ligne[256];
    int id_edu, id_user;
    int trouve = 0; // Indicateur si on trouve des formations

    printf("\n--- Formations de l'utilisateur ID %d ---\n", id_utilisateur);
    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *token;
        char diplome[100], institution[100], date_debut[20], date_fin[20];

        // Lire l'ID de l'éducation
        token = strtok(ligne, ";");
        if (token == NULL) continue;
        id_edu = atoi(token);

        // Lire l'ID utilisateur
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        id_user = atoi(token);

        // Vérifier si l'ID utilisateur correspond
        if (id_user == id_utilisateur) {
            trouve = 1;

            token = strtok(NULL, ";");
            if (token == NULL) continue;
            strncpy(diplome, token, sizeof(diplome));

            token = strtok(NULL, ";");
            if (token == NULL) continue;
            strncpy(institution, token, sizeof(institution));

            token = strtok(NULL, ";");
            if (token == NULL) continue;
            strncpy(date_debut, token, sizeof(date_debut));

            token = strtok(NULL, ";");
            if (token == NULL) continue;
            strncpy(date_fin, token, sizeof(date_fin));

            // Affichage formaté
            printf("ID Éducation: %d\n", id_edu);
            printf("Diplôme: %s\n", diplome);
            printf("Institution: %s\n", institution);
            printf("Début: %s\n", date_debut);
            printf("Fin: %s\n", date_fin);
            printf("--------------------------\n");
        }
    }

    if (!trouve) {
        printf("Aucune formation trouvée pour cet utilisateur.\n");
    }

    fclose(fichier);
}



// Fonction pour libérer la mémoire de la liste des éducations
void liberer_educations(Education* edu) {
    while (edu != NULL) {
        Education* temp = edu;
        edu = edu->suivant;
        free(temp);
    }
}

// Fonction pour valider les données d'une éducation
bool valider_education( char* diplome,  char* institution,  char* date_debut,  char* date_fin) {
	 nettoyer_chaine(diplome);
	 nettoyer_chaine(institution);
	 nettoyer_chaine(date_debut);
	 nettoyer_chaine(date_fin);
    // Vérifier que les champs ne sont pas vides
    if (strlen(diplome) == 0 || strlen(institution) == 0 || strlen(date_debut) == 0 || strlen(date_fin) == 0) {
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

// Fonction pour vérifier si une éducation existe déjà dans le fichier
bool education_existe(const char* fichier, int id_utilisateur, const char* diplome, const char* institution, const char* date_debut, const char* date_fin) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false; // Si le fichier n'existe pas, l'éducation ne peut pas exister
    }
          if (!utilisateur_existe_par_id(fichier, id_utilisateur)) {
		  
                    printf("Erreur : l'utilisateur avec l'ID %d n'existe pas.\n", id_utilisateur);
                }
     
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_user;
        char diplome_fichier[100], institution_fichier[100], date_debut_fichier[20], date_fin_fichier[20];
        sscanf(ligne, "%d;%d;%[^;];%[^;];%[^;];%[^\n]", &id, &id_user, diplome_fichier, institution_fichier, date_debut_fichier, date_fin_fichier);

        if (id_utilisateur == id_user &&
            strcmp(diplome, diplome_fichier) == 0 &&
            strcmp(institution, institution_fichier) == 0 &&
            strcmp(date_debut, date_debut_fichier) == 0 &&
            strcmp(date_fin, date_fin_fichier) == 0) {
            fclose(f);
            return true; // L'éducation existe déjà
        }
    }

    fclose(f);
    return false; // L'éducation n'existe pas
}

void gestion_educations(Education** educations, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION EDUCATIONS ---\n");
        printf("1. Ajouter une education\n");
        printf("2. Afficher mes educations\n");
        printf("3. Supprimer une education\n");
        printf("4. Modifier une education\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Diplome :");
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
                printf("ID de l'éducation à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_educations_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {  // Nouveau cas pour la modification
                int id_modif;
                printf("ID de l'éducation à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouveau diplôme : ");
                char *diplome = saisir_chaine();
                printf("Nouvelle institution : ");
                char *institution = saisir_chaine();
                printf("Nouvelle date début (YYYY-MM-DD) : ");
                char *date_debut = saisir_chaine();
                printf("Nouvelle date fin (YYYY-MM-DD) : ");
                char *date_fin = saisir_chaine();
                
                int result = mettre_a_jour_education(id_utilisateur, id_modif, 
                                                    diplome, institution, 
                                                    date_debut, date_fin);
                if (result > 0) {
                    printf("Éducation mise à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour de l'éducation.\n");
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
    // Validation de la période
    if (!valider_periode(date_debut, date_fin)) {
        return -1;
    }

    // Construction de la nouvelle ligne
    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%d;%s;%s;%s;%s", 
             id_education, id_utilisateur, diplome, institution, date_debut, date_fin);

    return mettre_a_jour_element("educations.txt", id_utilisateur, id_education, nouveaux_donnees);
}
