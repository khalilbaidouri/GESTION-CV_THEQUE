#include "langue.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_langue = 0;

Langue* creer_langue(int id_utilisateur, const char* langue) {
    Langue* nouvelle = (Langue*)malloc(sizeof(Langue));
    if (nouvelle == NULL) {
        printf("Erreur allocation mémoire\n");
        return NULL;
    }

    nouvelle->id_langue = ++dernier_id_langue;
    nouvelle->id_utilisateur = id_utilisateur;
    strncpy(nouvelle->langue, langue, sizeof(nouvelle->langue) - 1);
    nouvelle->langue[sizeof(nouvelle->langue) - 1] = '\0';
    nouvelle->suivant = NULL;

    return nouvelle;
}

void ajouter_langue(Langue** tete, Langue* nouvelle) {
    if (*tete == NULL) {
        *tete = nouvelle;
    } else {
        Langue* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouvelle;
    }
}

void ajouter_et_sauvegarder_langue(Langue** tete, const char* fichier, int id_utilisateur,  char* langue) {
    if (!valider_langue(langue)) {
        printf("Langue invalide\n");
        return;
    }

    Langue* nouvelle = creer_langue(id_utilisateur, langue);
    if (nouvelle == NULL) {
        printf("Erreur création langue\n");
        return;
    }

    ajouter_langue(tete, nouvelle);

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }
    fprintf(f, "%d;%s;%d\n", nouvelle->id_langue, nouvelle->langue, nouvelle->id_utilisateur);
    fclose(f);
}

void afficher_langues_par_utilisateur(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", fichier);
        return;
    }

    char ligne[256];
    int id_user;
    char langue[100];
    int trouve = 0;

    printf("\n--- Langues parlées par l'utilisateur ID %d ---\n", id_utilisateur);

    while (fgets(ligne, sizeof(ligne), f)) {
        char* token = strtok(ligne, ";");
        if (token == NULL) continue;
        strncpy(langue, token, sizeof(langue));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        id_user = atoi(token);

        if (id_user == id_utilisateur) {
            trouve = 1;
            printf("- %s\n", langue);
        }
    }

    if (!trouve) {
        printf("Aucune langue trouvée pour cet utilisateur.\n");
    }

    fclose(f);
}


void liberer_langues(Langue* l) {
    while (l != NULL) {
        Langue* temp = l;
        l = l->suivant;
        free(temp);
    }
}

bool valider_langue( char* langue) {
	
	nettoyer_chaine(langue);
    return strlen(langue) > 0 && strlen(langue) < 100;
}

bool langue_existe(const char* fichier, int id_utilisateur, const char* langue) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) return false;

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_u;
        char langue_f[100];
        
        if (sscanf(ligne, "%d;%[^;];%d", &id, langue_f, &id_u) == 3) {
            if (id_utilisateur == id_u && strcmp(langue, langue_f) == 0) {
                fclose(f);
                return true;
            }
        }
    }
    fclose(f);
    return false;
}

void gestion_langues(Langue** langues, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION LANGUES ---\n");
        printf("1. Ajouter une langue\n");
        printf("2. Afficher mes langues\n");
        printf("3. Supprimer une langue\n");
        printf("4. Modifier une langue\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Langue");
                char *langue = saisir_chaine();
                ajouter_et_sauvegarder_langue(langues, "langues.txt", id_utilisateur, langue);
                free(langue);
                break;
            }
            case '2':
                afficher_langues_par_utilisateur("langues.txt", id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID de la langue à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_langues_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {  // Nouveau cas pour la modification
                int id_modif;
                printf("ID de la langue à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouvelle langue : ");
                char *langue = saisir_chaine();
                
                int result = mettre_a_jour_langue(id_utilisateur, id_modif, langue);
                if (result > 0) {
                    printf("Langue mise à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour de la langue.\n");
                }
                
                free(langue);
                break;
            }
        }
    } while (choix != '5');
}
int supprimer_langues_utilisateur(int id_utilisateur, int id_langue) {
    return supprimer_element_par_ids("langues.txt", id_utilisateur, id_langue);
}


int mettre_a_jour_langue(int id_utilisateur, int id_langue, const char* langue) {
    // Nettoyer la chaîne
    char langue_clean[100];
    strncpy(langue_clean, langue, sizeof(langue_clean)-1);
    langue_clean[sizeof(langue_clean)-1] = '\0';
    nettoyer_chaine(langue_clean);

    if (strlen(langue_clean) == 0) {
        printf("Erreur: Langue invalide\n");
        return -1;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%s;%d", 
             id_langue, langue_clean, id_utilisateur);

    return mettre_a_jour_element("langues.txt", id_utilisateur, id_langue, nouveaux_donnees);
}
