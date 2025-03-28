#include "loisir.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_loisir = 0;

Loisir* creer_loisir(int id_utilisateur, const char* loisir) {
    Loisir* nouveau = (Loisir*)malloc(sizeof(Loisir));
    if (nouveau == NULL) {
        printf("Erreur allocation mémoire\n");
        return NULL;
    }

    nouveau->id_loisir = ++dernier_id_loisir;
    nouveau->id_utilisateur = id_utilisateur;
    strncpy(nouveau->loisir, loisir, sizeof(nouveau->loisir) - 1);
    nouveau->loisir[sizeof(nouveau->loisir) - 1] = '\0';
    nouveau->suivant = NULL;

    return nouveau;
}

void ajouter_loisir(Loisir** tete, Loisir* nouveau) {
    if (*tete == NULL) {
        *tete = nouveau;
    } else {
        Loisir* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }
}

void ajouter_et_sauvegarder_loisir(Loisir** tete, const char* fichier, int id_utilisateur,  char* loisir) {
    if (!valider_loisir(loisir)) {
        printf("Loisir invalide\n");
        return;
    }

    Loisir* nouveau = creer_loisir(id_utilisateur, loisir);
    if (nouveau == NULL) {
        printf("Erreur création loisir\n");
        return;
    }

    ajouter_loisir(tete, nouveau);

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }
    fprintf(f, "%d;%d;%s\n", nouveau->id_loisir, nouveau->id_utilisateur, nouveau->loisir);
    fclose(f);
}

void afficher_loisirs_par_utilisateur(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", fichier);
        return;
    }

    char ligne[256];
    int id_user;
    char loisir[100];
    int trouve = 0;

    printf("\n--- Loisirs de l'utilisateur ID %d ---\n", id_utilisateur);

    while (fgets(ligne, sizeof(ligne), f)) {
        char* token = strtok(ligne, ";");
        if (token == NULL) continue;
        strncpy(loisir, token, sizeof(loisir));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        id_user = atoi(token);

        if (id_user == id_utilisateur) {
            trouve = 1;
            printf("- %s\n", loisir);
        }
    }

    if (!trouve) {
        printf("Aucun loisir trouvé pour cet utilisateur.\n");
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
                printf("ID du loisir à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_loisirs_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {  // Nouveau cas pour la modification
                int id_modif;
                printf("ID du loisir à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouveau loisir : ");
                char *loisir = saisir_chaine();
                
                int result = mettre_a_jour_loisir(id_utilisateur, id_modif, loisir);
                if (result > 0) {
                    printf("Loisir mis à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour du loisir.\n");
                }
                
                free(loisir);
                break;
            }
        }
    } while (choix != '5');
}
int supprimer_loisirs_utilisateur(int id_utilisateur, int id_loisir) {
    // Format de loisirs.txt : id;id_utilisateur;nom_loisir
    return supprimer_element_par_ids("loisirs.txt", id_utilisateur, id_loisir);
}


int mettre_a_jour_loisir(int id_utilisateur, int id_loisir, const char* nouveau_nom) {
    printf("\n=== Mise à jour Loisir ===\n");
    
    // Construire la nouvelle ligne selon le format
    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees), 
             "%d;%d;%s", id_loisir, id_utilisateur, nouveau_nom);
    
    int res = mettre_a_jour_element("loisirs.txt", id_utilisateur, id_loisir, nouveaux_donnees);
    
    if (res > 0) {
        printf("Succès : loisir ID %d mis à jour\n", id_loisir);
    } else {
        printf("Aucun loisir correspondant trouvé\n");
    }
    
    return res;
}
