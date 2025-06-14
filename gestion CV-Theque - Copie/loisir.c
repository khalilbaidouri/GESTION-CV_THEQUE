#include "loisir.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_loisir = 0;
void ajouter_et_sauvegarder_loisir(Loisir** tete, const char* fichier, int id_utilisateur, char* loisir) {
    (void)tete;

    if (!valider_loisir(loisir)) {
        printf("Loisir invalide\n");
        return;
    }

    if (loisir_existe(fichier, id_utilisateur, loisir)) {
        printf("Loisir déjà existant pour cet utilisateur.\n");
        return;
    }

    int max_id = 0;
    FILE* f_read = fopen(fichier, "r");
    if (f_read) {
        char ligne[256];
        int id_temp, uid;
        char loisir_temp[100];

        while (fgets(ligne, sizeof(ligne), f_read)) {
            if (sscanf(ligne, "%d;%d;%[^\n]", &id_temp, &uid, loisir_temp) == 3) {
                if (id_temp > max_id) {
                    max_id = id_temp;
                }
            }
        }
        fclose(f_read);
    }

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }

    fprintf(f, "%d;%d;%s\n", max_id + 1, id_utilisateur, loisir);
    fclose(f);

    printf("Loisir ajouté avec succès.\n");
}
void afficher_loisirs_par_utilisateur(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (!f) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", fichier);
        return;
    }

    char ligne[256];
    int trouve = 0;

    printf("\n--- Loisirs --- \n");

    while (fgets(ligne, sizeof(ligne), f)) {
        int id_loisir = -1, id_user = -1;
        char nom_loisir[100] = "";

        char copie[256];
        strncpy(copie, ligne, sizeof(copie));
        copie[sizeof(copie)-1] = '\0';

        char* saveptr;
        char* token = strtok_r(copie, ";", &saveptr);
        if (!token) continue;
        id_loisir = atoi(token);

        token = strtok_r(NULL, ";", &saveptr);
        if (!token) continue;
        id_user = atoi(token);

        token = strtok_r(NULL, ";\n", &saveptr);
        if (!token) continue;
        strncpy(nom_loisir, token, sizeof(nom_loisir)-1);

        if (id_user == id_utilisateur) {
            trouve = 1;
            printf("loisir est : %s\n", nom_loisir);
            printf("--------------------------\n");
        }
    }

    if (!trouve) {
        printf("Aucun loisir trouve pour cet utilisateur.\n");
    }

    fclose(f);
}


void liberer_loisirs(Loisir* l) {
    while (l != NULL) {
        Loisir* temp = l;
        l = l->suivant;
        free(temp);
    }
}

bool valider_loisir( char* loisir) {
	nettoyer_chaine(loisir);
    return strlen(loisir) > 0 && strlen(loisir) < 255;
}

bool loisir_existe(const char* fichier, int id_utilisateur, const char* loisir) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) return false;

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_u;
        char loisir_f[255];
        
        if (sscanf(ligne, "%d;%d;%[^\n]", &id, &id_u, loisir_f) == 3) {
            if (id_utilisateur == id_u && strcmp(loisir, loisir_f) == 0) {
                fclose(f);
                return true;
            }
        }
    }
    fclose(f);
    return false;
}

void gestion_loisirs(Loisir** loisirs, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION LOISIRS ---\n");
        printf("1. Ajouter un loisir\n");
        printf("2. Afficher mes loisirs\n");
        printf("3. Supprimer un loisir\n");
        printf("4. Modifier un loisir\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Loisir :");
                char *loisir = saisir_chaine();
                ajouter_et_sauvegarder_loisir(loisirs, "loisirs.txt", id_utilisateur, loisir);
                free(loisir);
                break;
            }
            case '2':
                afficher_loisirs_par_utilisateur("loisirs.txt", id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID du loisir a supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_loisirs_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {
                int id_modif;
                printf("ID du loisir a modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouveau loisir : ");
                char *loisir = saisir_chaine();
                
                int result = mettre_a_jour_loisir(id_utilisateur, id_modif, loisir);
                if (result > 0) {
                    printf("Loisir mis a jour avec succas!\n");
                } else {
                    printf("echec de la mise a jour du loisir.\n");
                }
                
                free(loisir);
                break;
            }
        }
    } while (choix != '5');
}
int supprimer_loisirs_utilisateur(int id_utilisateur, int id_loisir) {
    return supprimer_element_par_ids("loisirs.txt", id_utilisateur, id_loisir);
}


int mettre_a_jour_loisir(int id_utilisateur, int id_loisir, const char* nouveau_nom) {
    printf("\n=== Mise a jour Loisir ===\n");
    
    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees), 
             "%d;%d;%s", id_loisir, id_utilisateur, nouveau_nom);
    
    int res = mettre_a_jour_element("loisirs.txt", id_utilisateur, id_loisir, nouveaux_donnees);
    
    if (res > 0) {
        printf("Succes : loisir ID %d mis a jour\n", id_loisir);
    } else {
        printf("Aucun loisir correspondant trouve\n");
    }
    
    return res;
}
