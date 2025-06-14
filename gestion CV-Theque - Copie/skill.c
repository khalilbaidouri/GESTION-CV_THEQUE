#include "skill.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_skill = 0;
void ajouter_et_sauvegarder_skill(const char* fichier, int id_utilisateur, const char* skill_name) {
    if (skill_name == NULL || strlen(skill_name) == 0) {
        printf("Erreur : la compétence est invalide.\n");
        return;
    }

    if (skill_existe(fichier, id_utilisateur, skill_name)) {
        printf("Cette compétence existe déjà pour cet utilisateur.\n");
        return;
    }

    int max_id = 0;
    FILE* f_read = fopen(fichier, "r");
    if (f_read) {
        char ligne[256];
        int id_temp, uid;
        char nom[100];
        while (fgets(ligne, sizeof(ligne), f_read)) {
            if (sscanf(ligne, "%d;%d;%99[^\n]", &id_temp, &uid, nom) == 3) {
                if (id_temp > max_id) max_id = id_temp;
            }
        }
        fclose(f_read);
    }

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }

    fprintf(f, "%d;%d;%s\n", max_id + 1, id_utilisateur, skill_name);
    fclose(f);

    printf("Compétence ajoutée avec succès.\n");
}

void afficher_skills_par_utilisateur(int id_utilisateur) {
    FILE* fichier = fopen("skills.txt", "r");
    if (fichier == NULL) {
        printf("Aucune comp�tence enregistr�e.\n");
        return;
    }

    char ligne[256];
    int trouve = 0;

    printf("\n--- Comp�tences de l'utilisateur ID %d ---\n", id_utilisateur);

    while (fgets(ligne, sizeof(ligne), fichier)) {
        int id_skill, id_user;
        char skill_name[255];

        if (sscanf(ligne, "%d;%d;%[^\n]", &id_skill, &id_user, skill_name) == 3) {
            if (id_user == id_utilisateur) {
                trouve = 1;
                printf("ID: %d - Comp�tence: %s\n", id_skill, skill_name);
            }
        }
    }

    if (!trouve) {
        printf("Aucune comp�tence trouv�e.\n");
    }

    fclose(fichier);
}



void liberer_skills(Skill* skill) {
    while (skill != NULL) {
        Skill* temp = skill;
        skill = skill->suivant;
        free(temp);
    }
}

void gestion_skills(Skill** skills, int id_utilisateur) {
    char choix;
    do {
        printf("\n--- GESTION COMP�TENCES ---\n");
        printf("1. Ajouter une comp�tence\n");
        printf("2. Afficher mes comp�tences\n");
        printf("3. Supprimer une comp�tence\n");
        printf("4. Modifier une comp�tence\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                char *skill_name = NULL;
                printf("Comp�tence : ");
                skill_name = saisir_chaine();
                
                ajouter_et_sauvegarder_skill("skills.txt", id_utilisateur, skill_name);
                free(skill_name);
                break;
            }
            case '2':
                afficher_skills_par_utilisateur(id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID de la competence a supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_skills_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {
                int id_modif;
                printf("ID de la compatence a modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouvelle comp�tence : ");
                char *skill_name = saisir_chaine();
                
                int result = mettre_a_jour_skill(id_utilisateur, id_modif, skill_name);
                if (result > 0) {
                    printf("Competence mise a jour avec succes!\n");
                } else {
                    printf("echec de la mise a jour de la competence.\n");
                }
                
                free(skill_name);
                break;
            }
        }
    } while (choix != '5');
}


int supprimer_skills_utilisateur(int id_utilisateur, int id_skill) {
    return supprimer_element_par_ids("skills.txt", id_utilisateur, id_skill);
}

int mettre_a_jour_skill(int id_utilisateur, int id_skill, const char* skill_name) {
    if (strlen(skill_name) == 0) {
        printf("Erreur: Nom de comp�tence vide\n");
        return -1;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%d;%s", 
             id_skill, id_utilisateur, skill_name);

    return mettre_a_jour_element("skills.txt", id_utilisateur, id_skill, nouveaux_donnees);
}

void afficher_skills(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (!f) return;

    char ligne[256];
    int trouve = 0;

    printf("\n--- Competences ---\n");

    while (fgets(ligne, sizeof(ligne), f)) {
        int id, id_user;
        char nom_skill[100];
        char* token;
        char* saveptr;

        token = strtok_r(ligne, ";", &saveptr);
        if (!token) continue;
        id = atoi(token);

        token = strtok_r(NULL, ";", &saveptr);
        if (!token) continue;
        id_user = atoi(token);

        token = strtok_r(NULL, "\n", &saveptr);
        if (!token) continue;
        strncpy(nom_skill, token, sizeof(nom_skill)-1);

        if (id_user == id_utilisateur) {
            trouve = 1;
            printf(" %s\n", nom_skill);
            printf("--------------------------\n");
        }
    }

    if (!trouve) {
        printf("Aucune competence trouvee.\n");
    }

    fclose(f);
}

int skill_existe(const char* fichier, int id_utilisateur, const char* skill_name) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return 0;
    }

    char ligne[256];
    int id_skill, uid;
    char nom[100];

    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d;%d;%99[^\n]", &id_skill, &uid, nom) == 3) {
            if (uid == id_utilisateur && strcmp(nom, skill_name) == 0) {
                fclose(f);
                return 1;
            }
        }
    }

    fclose(f);
    return 0;
}


