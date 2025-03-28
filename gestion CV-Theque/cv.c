#include "cv.h"
#include "supplimentaire.h"
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
        printf("\n--- GESTION CVs ---\n");
        printf("1. Créer un nouveau CV\n");
        printf("2. Afficher mes CVs\n");
        printf("3. Supprimer un CV\n");
        printf("4. Modifier un CV\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
            	printf("URL de la photo (optionnel) : ");
                char *photo_url = saisir_chaine();
                printf("Titre du CV : ");
                char *titre = saisir_chaine();
                ajouter_et_sauvegarder_cv(cvs, "cvs.txt", id_utilisateur, photo_url, titre);
                free(photo_url);
                free(titre);
                break;
            }
            case '2':
                afficher_cvs_par_utilisateur("cvs.txt", id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID du CV à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_cvs_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {  // Nouveau cas pour la modification
                int id_modif;
                printf("ID du CV à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouvelle URL de photo (optionnel) : ");
                char *photo_url = saisir_chaine();
                printf("Nouveau titre : ");
                char *titre = saisir_chaine();
                
                int result = mettre_a_jour_cv(id_utilisateur, id_modif, photo_url, titre);
                if (result > 0) {
                    printf("CV mis à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour du CV.\n");
                }
                
                free(photo_url);
                free(titre);
                break;
            }
        }
    } while (choix != '5');
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
