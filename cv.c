#include "cv.h"
#include "supplimentaire.h"
#include "utilisateur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_cv = 0;

CV* creer_cv(int id_utilisateur, const char* photo_url, const char* titre) {
    CV* nouveau = (CV*)malloc(sizeof(CV));
    if (nouveau == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    nouveau->id_cv = ++dernier_id_cv;
    nouveau->id_utilisateur = id_utilisateur;
    
    // Copier photo_url (peut être vide)
    if (photo_url != NULL) {
        strncpy(nouveau->photo_url, photo_url, sizeof(nouveau->photo_url) - 1);
        nouveau->photo_url[sizeof(nouveau->photo_url) - 1] = '\0';
    } else {
        nouveau->photo_url[0] = '\0';
    }
    
    // Copier titre (avec valeur par défaut si vide)
    if (titre != NULL && strlen(titre) > 0) {
        strncpy(nouveau->titre, titre, sizeof(nouveau->titre) - 1);
        nouveau->titre[sizeof(nouveau->titre) - 1] = '\0';
    } else {
        strncpy(nouveau->titre, "Mon CV", sizeof(nouveau->titre) - 1);
        nouveau->titre[sizeof(nouveau->titre) - 1] = '\0';
    }

    nouveau->suivant = NULL;
    return nouveau;
}

void ajouter_cv(CV** tete, CV* nouveau) {
    if (*tete == NULL) {
        *tete = nouveau;
    } else {
        CV* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }
}


void ajouter_et_sauvegarder_cv(CV** tete, const char* fichier, int id_utilisateur, const char* photo_url,  char* titre) {
    // Vérification des paramètres
    nettoyer_chaine(titre);
    if(strlen(titre)==0)
    {
    	printf("titre invalid!");
    	return;
	}
    
    
    if (!tete || !fichier || !titre) {
        fprintf(stderr, "Paramètres invalides\n");
        return;
    }

    CV* nouveau = (CV*)malloc(sizeof(CV));
    if (!nouveau) {
        perror("Erreur d'allocation");
        return;
    }

    // Initialisation
    nouveau->id_cv = ++dernier_id_cv;
    nouveau->id_utilisateur = id_utilisateur;
    
    // Gestion photo URL
    if (photo_url) {
        strncpy(nouveau->photo_url, photo_url, sizeof(nouveau->photo_url)-1);
        nouveau->photo_url[sizeof(nouveau->photo_url)-1] = '\0';
    } else {
        nouveau->photo_url[0] = '\0';
    }

    // Titre obligatoire
    strncpy(nouveau->titre, titre, sizeof(nouveau->titre)-1);
    nouveau->titre[sizeof(nouveau->titre)-1] = '\0';
    nouveau->suivant = NULL;

    // Ajout à la liste
    if (!*tete) {
        *tete = nouveau;
    } else {
        CV* courant = *tete;
        while (courant->suivant) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }

    // Sauvegarde fichier
    FILE* f = fopen(fichier, "a");
    if (!f) {
        perror("Erreur ouverture fichier");
        free(nouveau);
        return;
    }

    fprintf(f, "%d;%d;%s;%s\n", nouveau->id_cv, nouveau->id_utilisateur, 
           nouveau->photo_url, nouveau->titre);
    fclose(f);
}
void afficher_cvs_par_utilisateur(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        printf("\n--- Aucun CV enregistré ---\n");
        return;
    }

    char ligne[512];
    int trouve = 0;

    printf("\n--- CVs de l'utilisateur ID %d ---\n", id_utilisateur);

    while (fgets(ligne, sizeof(ligne), f)) {  // <-- Correction ici
        int id_cv, id_user;
        char photo_url[500] = {0};
        char titre[255] = {0};
        char* token;
        char* saveptr;

        // Parsing robuste avec strtok_r
        token = strtok_r(ligne, ";", &saveptr);
        if (!token) continue;
        id_cv = atoi(token);

        token = strtok_r(NULL, ";", &saveptr);
        if (!token) continue;
        id_user = atoi(token);

        token = strtok_r(NULL, ";", &saveptr);
        if (token) strncpy(photo_url, token, sizeof(photo_url)-1);

        token = strtok_r(NULL, "\n", &saveptr);
        if (token) strncpy(titre, token, sizeof(titre)-1);

        if (id_user == id_utilisateur) {
            trouve = 1;
            printf("\nID CV: %d", id_cv);
            printf("\nTitre: %s", titre);
            if (strlen(photo_url) > 0) {
                printf("\nPhoto: %s", (strlen(photo_url) > 20 ? "[URL]" : photo_url));
            }
            printf("\n---------------------");
        }
    }

    if (!trouve) {
        printf("\nAucun CV trouvé.\n");
    } else {
        printf("\nAffichage terminé.\n");
    }

    fclose(f);
}
void liberer_cvs(CV* cv) {
    while (cv != NULL) {
        CV* temp = cv;
        cv = cv->suivant;
        free(temp);
    }
}

bool valider_cv(const char* photo_url, const char* titre) {
    // Vérifier que le titre n'est pas vide après nettoyage
    char titre_copy[255];
    if (titre != NULL) {
        strncpy(titre_copy, titre, sizeof(titre_copy) - 1);
        titre_copy[sizeof(titre_copy) - 1] = '\0';
        nettoyer_chaine(titre_copy);
        if (strlen(titre_copy) == 0) {
            return false;
        }
    }
    
    // Si photo_url est fournie, vérifier qu'elle n'est pas trop longue
    if (photo_url != NULL && strlen(photo_url) >= 500) {
        return false;
    }
    
    return true;
}

bool cv_existe(const char* fichier, int id_utilisateur, const char* titre) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) return false;

    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_u;
        char titre_f[255], photo_url[500];
        
        if (sscanf(ligne, "%d;%d;%[^;];%[^\n]", &id, &id_u, photo_url, titre_f) == 4) {
            if (id_utilisateur == id_u && strcmp(titre, titre_f) == 0) {
                fclose(f);
                return true;
            }
        }
    }
    fclose(f);
    return false;
}

void gestion_cvs(CV** cvs, int id_utilisateur) {
    char choix;
    do {
        printf("\n=== Gestion des CVs ===\n");
        printf("1. Créer un nouveau CV\n");
        printf("2. Afficher mes CVs\n");
        printf("3. Modifier un CV\n");
        printf("4. Supprimer un CV\n");
        printf("5. Afficher un CV complet\n");  // Nouvelle option
        printf("6. Retour au menu principal\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                char photo_url[500];
                char titre[255];
                printf("Entrez l'URL de la photo : ");
                scanf("%s", photo_url);
                printf("Entrez le titre du CV : ");
                scanf(" %[^\n]", titre);
                ajouter_et_sauvegarder_cv(cvs, "cvs.txt", id_utilisateur, photo_url, titre);
                break;
            }
            case '2':
                afficher_cvs_par_utilisateur("cvs.txt", id_utilisateur);
                break;
            case '3': {
                int id_cv;
                char photo_url[500];
                char titre[255];
                printf("Entrez l'ID du CV à modifier : ");
                scanf("%d", &id_cv);
                printf("Entrez la nouvelle URL de la photo : ");
                scanf("%s", photo_url);
                printf("Entrez le nouveau titre : ");
                scanf(" %[^\n]", titre);
                if (mettre_a_jour_cv(id_utilisateur, id_cv, photo_url, titre)) {
                    printf("CV mis à jour avec succès\n");
                } else {
                    printf("Erreur lors de la mise à jour du CV\n");
                }
                break;
            }
            case '4': {
                int id_cv;
                printf("Entrez l'ID du CV à supprimer : ");
                scanf("%d", &id_cv);
                if (supprimer_cvs_utilisateur(id_utilisateur, id_cv)) {
                    printf("CV supprimé avec succès\n");
                } else {
                    printf("Erreur lors de la suppression du CV\n");
                }
                break;
            }
            case '5': {  // Nouveau cas
                // int id_cv;
                // printf("Entrez l'ID du CV à afficher : ");
                // scanf("%d", &id_cv);
                    afficher_infos_utilisateur(id_utilisateur);
                break;
            }
            case '6':
                printf("Retour au menu principal\n");
                break;
            default:
                printf("Choix invalide!\n");
        }
    } while (choix != '6');
}

int supprimer_cvs_utilisateur(int id_utilisateur, int id_cv) {
    return supprimer_element_par_ids("cvs.txt", id_utilisateur, id_cv);
}

int mettre_a_jour_cv(int id_utilisateur, int id_cv,  const char* photo_url, const char* titre) {
    // Validation
    if (strlen(titre) == 0) {
        printf("Erreur: Titre vide\n");
        return -1;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%d;%s;%s", 
             id_cv, id_utilisateur, photo_url ? photo_url : "", titre);

    return mettre_a_jour_element("cvs.txt", id_utilisateur, id_cv, nouveaux_donnees);
}

void afficher_cv_complet(int id_utilisateur, int id_cv) {
    // Afficher les informations du CV
    FILE* fichier = fopen("cvs.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier CVs\n");
        return;
    }

    CV cv;
    bool cv_trouve = false;
    while (fscanf(fichier, "%d,%d,%[^,],%[^\n]\n", 
           &cv.id_cv, &cv.id_utilisateur, cv.photo_url, cv.titre) == 4) {
        if (cv.id_cv == id_cv && cv.id_utilisateur == id_utilisateur) {
            cv_trouve = true;
            printf("\n=== CV ===\n");
            printf("Titre: %s\n", cv.titre);
            printf("Photo: %s\n", cv.photo_url);
            break;
        }
    }
    fclose(fichier);

    if (!cv_trouve) {
        printf("CV non trouvé\n");
        return;
    }

    // Afficher les informations de l'utilisateur
    printf("\n=== Informations Personnelles ===\n");
    FILE* fichier_utilisateur = fopen("utilisateurs.txt", "r");
    if (fichier_utilisateur != NULL) {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier_utilisateur)) {
            int id;
            char nom[100], prenom[100], email[100], telephone[20];
            if (sscanf(ligne, "%d,%[^,],%[^,],%[^,],%[^\n]", 
                &id, nom, prenom, email, telephone) == 5) {
                if (id == id_utilisateur) {
                    printf("Nom: %s\n", nom);
                    printf("Prénom: %s\n", prenom);
                    printf("Email: %s\n", email);
                    printf("Téléphone: %s\n", telephone);
                    break;
                }
            }
        }
        fclose(fichier_utilisateur);
    }

    // Afficher les éducations
    printf("\n=== Formation ===\n");
    FILE* fichier_education = fopen("educations.txt", "r");
    if (fichier_education != NULL) {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier_education)) {
            int id, id_user;
            char diplome[100], ecole[100], date_debut[20], date_fin[20];
            if (sscanf(ligne, "%d,%d,%[^,],%[^,],%[^,],%[^\n]", 
                &id, &id_user, diplome, ecole, date_debut, date_fin) == 6) {
                if (id_user == id_utilisateur) {
                    printf("Diplôme: %s\n", diplome);
                    printf("École: %s\n", ecole);
                    printf("Période: %s - %s\n", date_debut, date_fin);
                    printf("---\n");
                }
            }
        }
        fclose(fichier_education);
    }

    // Afficher les expériences
    printf("\n=== Expériences Professionnelles ===\n");
    FILE* fichier_experience = fopen("experiences.txt", "r");
    if (fichier_experience != NULL) {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier_experience)) {
            int id, id_user;
            char poste[100], entreprise[100], date_debut[20], date_fin[20], description[500];
            if (sscanf(ligne, "%d,%d,%[^,],%[^,],%[^,],%[^,],%[^\n]", 
                &id, &id_user, poste, entreprise, date_debut, date_fin, description) == 7) {
                if (id_user == id_utilisateur) {
                    printf("Poste: %s\n", poste);
                    printf("Entreprise: %s\n", entreprise);
                    printf("Période: %s - %s\n", date_debut, date_fin);
                    printf("Description: %s\n", description);
                    printf("---\n");
                }
            }
        }
        fclose(fichier_experience);
    }

    // Afficher les compétences
    printf("\n=== Compétences ===\n");
    FILE* fichier_skill = fopen("skills.txt", "r");
    if (fichier_skill != NULL) {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier_skill)) {
            int id, id_user;
            char nom[100], niveau[50];
            if (sscanf(ligne, "%d,%d,%[^,],%[^\n]", 
                &id, &id_user, nom, niveau) == 4) {
                if (id_user == id_utilisateur) {
                    printf("%s (%s)\n", nom, niveau);
                }
            }
        }
        fclose(fichier_skill);
    }

    // Afficher les langues
    printf("\n=== Langues ===\n");
    FILE* fichier_langue = fopen("langues.txt", "r");
    if (fichier_langue != NULL) {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier_langue)) {
            int id, id_user;
            char nom[100], niveau[50];
            if (sscanf(ligne, "%d,%d,%[^,],%[^\n]", 
                &id, &id_user, nom, niveau) == 4) {
                if (id_user == id_utilisateur) {
                    printf("%s (%s)\n", nom, niveau);
                }
            }
        }
        fclose(fichier_langue);
    }

    // Afficher les loisirs
    printf("\n=== Centres d'intérêt ===\n");
    FILE* fichier_loisir = fopen("loisirs.txt", "r");
    if (fichier_loisir != NULL) {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier_loisir)) {
            int id, id_user;
            char nom[100];
            if (sscanf(ligne, "%d,%d,%[^\n]", 
                &id, &id_user, nom) == 3) {
                if (id_user == id_utilisateur) {
                    printf("- %s\n", nom);
                }
            }
        }
        fclose(fichier_loisir);
    }

    printf("\n=== Certifications ===\n");
    FILE* fichier_certificat = fopen("certificats.txt", "r");
    if (fichier_certificat != NULL) {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier_certificat)) {
            int id, id_user;
            char nom[100], organisme[100], date[20];
            if (sscanf(ligne, "%d,%d,%[^,],%[^,],%[^\n]", 
                &id, &id_user, nom, organisme, date) == 5) {
                if (id_user == id_utilisateur) {
                    printf("Certificat: %s\n", nom);
                    printf("Organisme: %s\n", organisme);
                    printf("Date: %s\n", date);
                    printf("---\n");
                }
            }
        }
        fclose(fichier_certificat);
    }
}
