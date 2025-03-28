#include "certificat.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_certificat = 0;

Certificat* creer_certificat(const char* nom, const char* organisme, const char* date, int id_utilisateur) {
    Certificat* nouveau = (Certificat*)malloc(sizeof(Certificat));
    if (nouveau == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    nouveau->id_certificat = ++dernier_id_certificat;
    strncpy(nouveau->nom_certificat, nom, sizeof(nouveau->nom_certificat) - 1);
    nouveau->nom_certificat[sizeof(nouveau->nom_certificat) - 1] = '\0';
    
    strncpy(nouveau->organisme, organisme, sizeof(nouveau->organisme) - 1);
    nouveau->organisme[sizeof(nouveau->organisme) - 1] = '\0';
    
    strncpy(nouveau->date_obtention, date, sizeof(nouveau->date_obtention) - 1);
    nouveau->date_obtention[sizeof(nouveau->date_obtention) - 1] = '\0';
    
    nouveau->id_utilisateur = id_utilisateur;
    nouveau->suivant = NULL;

    return nouveau;
}

void ajouter_certificat(Certificat** tete, Certificat* nouveau) {
    if (*tete == NULL) {
        *tete = nouveau;
    } else {
        Certificat* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }
}

void ajouter_et_sauvegarder_certificat(Certificat** tete, const char* fichier,  char* nom,  char* organisme,  char* date, int id_utilisateur) {
    if (!valider_certificat(nom, organisme, date)) {
        printf("Données du certificat invalides\n");
        return;
    }
    if(certificat_existe(fichier,  nom,  organisme,  date, id_utilisateur)){
	
    	printf("certificat deja exist!\n");
    	return;
    }
    Certificat* nouveau = creer_certificat(nom, organisme, date, id_utilisateur);
    if (nouveau == NULL) {
        printf("Erreur création certificat\n");
        return;
    }

    ajouter_certificat(tete, nouveau);

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }
    fprintf(f, "%d;%s;%s;%s;%d\n", nouveau->id_certificat, nouveau->nom_certificat, nouveau->organisme, nouveau->date_obtention, nouveau->id_utilisateur);
    fclose(f);
}

void afficher_certificats_par_utilisateur(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", fichier);
        return;
    }

    char ligne[256];
    int id_certificat, id_user;
    char nom_certificat[100], organisme[100], date_obtention[20];
    int trouve = 0;

    printf("\n--- Certificats de l'utilisateur ID %d ---\n", id_utilisateur);
    
    while (fgets(ligne, sizeof(ligne), f)) {
        char* token = strtok(ligne, ";");
        if (token == NULL) continue;
        id_certificat = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(nom_certificat, token, sizeof(nom_certificat));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(organisme, token, sizeof(organisme));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(date_obtention, token, sizeof(date_obtention));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        id_user = atoi(token);

        // Vérifier si l'ID utilisateur correspond
        if (id_user == id_utilisateur) {
            trouve = 1;
            printf("ID Certificat: %d\n", id_certificat);
            printf("Nom: %s\n", nom_certificat);
            printf("Organisme: %s\n", organisme);
            printf("Date d'obtention: %s\n", date_obtention);
            printf("--------------------------\n");
        }
    }

    if (!trouve) {
        printf("Aucun certificat trouvé pour cet utilisateur.\n");
    }

    fclose(f);
}

void liberer_certificats(Certificat* cert) {
    while (cert != NULL) {
        Certificat* temp = cert;
        cert = cert->suivant;
        free(temp);
    }
}

bool valider_certificat( char* nom,  char* organisme,  char* date) {
	
	nettoyer_chaine(nom);
	nettoyer_chaine(organisme);
	nettoyer_chaine(date);

    if (strlen(nom) == 0 || strlen(organisme) == 0 || strlen(date) == 0) {
        return false;
    }
    return est_date_valide(date);
}

bool certificat_existe(const char* fichier, const char* nom, const char* organisme, const char* date, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) return false;

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_u;
        char nom_f[255], org_f[255], date_f[20];
        
        if (sscanf(ligne, "%d;%[^;];%[^;];%[^;];%d", &id, nom_f, org_f, date_f, &id_u) == 5) {
            if (strcmp(nom, nom_f) == 0 && strcmp(organisme, org_f) == 0 && 
                strcmp(date, date_f) == 0 && id_utilisateur == id_u) {
                fclose(f);
                return true;
            }
        }
    }
    fclose(f);
    return false;
}

void gestion_certificats(Certificat** certificats, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION CERTIFICATS ---\n");
        printf("1. Ajouter un certificat\n");
        printf("2. Afficher mes certificats\n");
        printf("3. Supprimer un certificat\n");
        printf("4. Modifier un certificat\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Nom du certificat :");
                char *nom = saisir_chaine();
                printf("Organisme emetteur : ");
                char *organisme = saisir_chaine();
                printf("Date d'obtention (YYYY-MM-DD) :  ");
                char *date = saisir_chaine();
                
                ajouter_et_sauvegarder_certificat(certificats, "certificats.txt", 
                                                nom, organisme, date, id_utilisateur);
                
                free(nom);
                free(organisme);
                free(date);
                break;
            }
            case '2':
                afficher_certificats_par_utilisateur("certificats.txt", id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID du certificat à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_certificats_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {  // Nouveau cas pour la modification
                int id_modif;
                printf("ID du certificat à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouveau nom du certificat : ");
                char *nom = saisir_chaine();
                printf("Nouvel organisme emetteur : ");
                char *organisme = saisir_chaine();
                printf("Nouvelle date d'obtention (YYYY-MM-DD) : ");
                char *date = saisir_chaine();
                
                int result = mettre_a_jour_certificat(id_utilisateur, id_modif, nom, organisme, date);
                if (result > 0) {
                    printf("Certificat mis à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour du certificat.\n");
                }
                
                free(nom);
                free(organisme);
                free(date);
                break;
            }
        }
    } while (choix != '5');
}

// Dans chaque fichier (exemple pour certificat.c)

int supprimer_certificats_utilisateur(int id_utilisateur, int id_certificat) {
    return supprimer_element_par_ids("certificats.txt", id_utilisateur, id_certificat);
}

int mettre_a_jour_certificat(int id_utilisateur, int id_certificat,  const char* nom, const char* organisme,const char* date_obtention) {
    if (!est_date_valide(date_obtention)) {
        printf("Erreur: Date invalide\n");
        return -1;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%s;%s;%s;%d", 
             id_certificat, nom, organisme, date_obtention, id_utilisateur);

    return mettre_a_jour_element("certificats.txt", id_utilisateur, id_certificat, nouveaux_donnees);
}
