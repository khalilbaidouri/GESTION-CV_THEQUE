#include "skill.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dernier_id_skill = 0;

Skill* creer_skill(int id_utilisateur, const char* skill_name) {
    Skill* nouveau_skill = (Skill*)malloc(sizeof(Skill));
    if (nouveau_skill == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    nouveau_skill->id_skill = ++dernier_id_skill;
    nouveau_skill->id_utilisateur = id_utilisateur;
    strncpy(nouveau_skill->skill_name, skill_name, sizeof(nouveau_skill->skill_name) - 1);
    nouveau_skill->skill_name[sizeof(nouveau_skill->skill_name) - 1] = '\0';
    nouveau_skill->suivant = NULL;

    return nouveau_skill;
}

void ajouter_skill(Skill** tete, Skill* nouveau_skill) {
    if (*tete == NULL) {
        *tete = nouveau_skill;
    } else {
        Skill* current = *tete;
        while (current->suivant != NULL) {
            current = current->suivant;
        }
        current->suivant = nouveau_skill;
    }
}

void ajouter_et_sauvegarder_skill(Skill** tete, const char* fichier, int id_utilisateur, const char* skill_name) {
    if (strlen(skill_name) == 0) {
        printf("Erreur : la compétence est invalide.\n");
        return;
    }

    Skill* nouveau_skill = creer_skill(id_utilisateur, skill_name);
    if (nouveau_skill == NULL) {
        printf("Erreur : impossible de créer la compétence.\n");
        return;
    }

    ajouter_skill(tete, nouveau_skill);

    FILE* f = fopen(fichier, "a");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }
    fprintf(f, "%d;%d;%s\n", nouveau_skill->id_skill, nouveau_skill->id_utilisateur, nouveau_skill->skill_name);
    fclose(f);
}

void afficher_skills_par_utilisateur(int id_utilisateur) {
    FILE* fichier = fopen("skills.txt", "r");
    if (fichier == NULL) {
        printf("Aucune compétence enregistrée.\n");
        return;
    }

    char ligne[256];
    int trouve = 0;

    printf("\n--- Compétences de l'utilisateur ID %d ---\n", id_utilisateur);

    while (fgets(ligne, sizeof(ligne), fichier)) {
        int id_skill, id_user;
        char skill_name[255];

        if (sscanf(ligne, "%d;%d;%[^\n]", &id_skill, &id_user, skill_name) == 3) {
            if (id_user == id_utilisateur) {
                trouve = 1;
                printf("ID: %d - Compétence: %s\n", id_skill, skill_name);
            }
        }
    }

    if (!trouve) {
        printf("Aucune compétence trouvée.\n");
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
        printf("\n--- GESTION COMPÉTENCES ---\n");
        printf("1. Ajouter une compétence\n");
        printf("2. Afficher mes compétences\n");
        printf("3. Supprimer une compétence\n");
        printf("4. Modifier une compétence\n");  // Nouvelle option
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                char *skill_name = NULL;
                printf("Compétence : ");
                skill_name = saisir_chaine();
                
                ajouter_et_sauvegarder_skill(skills, "skills.txt", id_utilisateur, skill_name);
                free(skill_name);
                break;
            }
            case '2':
                afficher_skills_par_utilisateur(id_utilisateur);
                break;
            case '3': {
                int id_suppr;
                printf("ID de la compétence à supprimer : ");
                scanf("%d", &id_suppr);
                supprimer_skills_utilisateur(id_utilisateur, id_suppr);
                break;
            }
            case '4': {  // Nouveau cas pour la modification
                int id_modif;
                printf("ID de la compétence à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                printf("Nouvelle compétence : ");
                char *skill_name = saisir_chaine();
                
                int result = mettre_a_jour_skill(id_utilisateur, id_modif, skill_name);
                if (result > 0) {
                    printf("Compétence mise à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour de la compétence.\n");
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
        printf("Erreur: Nom de compétence vide\n");
        return -1;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%d;%s", 
             id_skill, id_utilisateur, skill_name);

    return mettre_a_jour_element("skills.txt", id_utilisateur, id_skill, nouveaux_donnees);
}
