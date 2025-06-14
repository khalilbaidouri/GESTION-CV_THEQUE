#include "certificat.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_certificat = 0;
void ajouter_et_sauvegarder_certificat(const char* fichier, const char* nom, const char* organisme, const char* date, int id_utilisateur) {
    if (!valider_certificat(nom, organisme, date)) {
        printf("Données du certificat invalides\n");
        return;
    }

    if (certificat_existe(fichier, nom, organisme, date, id_utilisateur)) {
        printf("Certificat déjà existant pour cet utilisateur.\n");
        return;
    }

    int max_id = 0;
    FILE* f_read = fopen(fichier, "r");
    if (f_read) {
        char ligne[512];
        int id_temp, uid;
        char nom_cert[100], org[100], dt[100];
        while (fgets(ligne, sizeof(ligne), f_read)) {
            if (sscanf(ligne, "%d;%99[^;];%99[^;];%99[^;];%d", &id_temp, nom_cert, org, dt, &uid) == 5) {
                if (id_temp > max_id) max_id = id_temp;
            }
        }
        fclose(f_read);
    }

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    fprintf(f, "%d;%s;%s;%s;%d\n", max_id + 1, nom, organisme, date, id_utilisateur);
    fclose(f);

    printf("Certificat ajoute avec succès.\n");
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

    printf("\n--- Certificats  ---\n");
    
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
        printf("Aucun certificat trouve pour cet utilisateur.\n");
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

                ajouter_et_sauvegarder_certificat("certificats.txt",
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
                printf("ID du certificat � supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_certificats_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {
                int id_modif;
                printf("ID du certificat a modifier : ");
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
                    printf("Certificat mis a jour avec succes!\n");
                } else {
                    printf("echec de la mise a jour du certificat.\n");
                }

                free(nom);
                free(organisme);
                free(date);
                break;
            }
        }
    } while (choix != '5');
}


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
