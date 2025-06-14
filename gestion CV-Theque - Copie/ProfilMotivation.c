#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ProfilMotivation.h"
#include "supplimentaire.h"

int dernier_id_profilmotivation = 0;

bool valider_texte(const char* texte) {
    if (!texte || strlen(texte) == 0) return false;
    while (*texte) {
        if (*texte == ';' || *texte == '\n' || *texte == '\r') return false;
        texte++;
    }
    return true;
}

bool profil_motivation_existe(const char* fichier, const char* profil, const char* motivation) {
    FILE* f = fopen(fichier, "r");
    if (!f) return false;

    char ligne[2 * MAX_TEXTE + 20];
    int id;
    char p[MAX_TEXTE], m[MAX_TEXTE];
    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d;%254[^;];%254[^\n]", &id, p, m) == 3) {
            if (strcmp(p, profil) == 0 && strcmp(m, motivation) == 0) {
                fclose(f);
                return true;
            }
            if (id > dernier_id_profilmotivation)
                dernier_id_profilmotivation = id;
        }
    }
    fclose(f);
    return false;
}

void ajouter_et_sauvegarder_profil_motivation(const char* fichier, const char* profil, const char* motivation) {
    if (!valider_texte(profil) || !valider_texte(motivation)) {
        printf("Profil ou motivation invalide.\n");
        return;
    }

    if (profil_motivation_existe(fichier, profil, motivation)) {
        printf("Ce profil/motivation existe déjà.\n");
        return;
    }

    FILE* f = fopen(fichier, "a");
    if (!f) {
        perror("Erreur ouverture fichier");
        return;
    }

    ++dernier_id_profilmotivation;
    fprintf(f, "%d;%s;%s\n", dernier_id_profilmotivation, profil, motivation);
    fclose(f);
}

void afficher_profils_motivation(const char* fichier) {
    FILE* f = fopen(fichier, "r");
    if (!f) {
        printf("Aucun profil/motivation trouve.\n");
        return;
    }

    char ligne[2 * MAX_TEXTE + 20];
    int id;
    char profil[MAX_TEXTE], motivation[MAX_TEXTE];
    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d;%254[^;];%254[^\n]", &id, profil, motivation) == 3) {
            //printf("ID : %d\n", id);
            printf("Profil : %s\n", profil);
            printf("Motivation : %s\n", motivation);
            printf("-----------------------\n");

            if (id > dernier_id_profilmotivation)
                dernier_id_profilmotivation = id;
        }
    }
    fclose(f);
}

int supprimer_profil_motivation(const char* fichier, int idMotivProfil) {
    FILE* f = fopen(fichier, "r");
    if (!f) return 0;

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(f);
        return 0;
    }

    char ligne[2 * MAX_TEXTE + 20];
    int id;
    char profil[MAX_TEXTE], motivation[MAX_TEXTE];
    bool trouve = false;

    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d;%254[^;];%254[^\n]", &id, profil, motivation) == 3) {
            if (id != idMotivProfil) {
                fprintf(temp, "%d;%s;%s\n", id, profil, motivation);
            } else {
                trouve = true;
            }
        }
    }

    fclose(f);
    fclose(temp);

    remove(fichier);
    rename("temp.txt", fichier);
    return trouve ? 1 : 0;
}

int mettre_a_jour_profil_motivation(const char* fichier, int idMotivProfil, const char* nouveau_profil, const char* nouvelle_motivation) {
    if (!valider_texte(nouveau_profil) || !valider_texte(nouvelle_motivation)) {
        printf("Profil ou motivation invalide.\n");
        return 0;
    }

    FILE* f = fopen(fichier, "r");
    if (!f) return 0;

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(f);
        return 0;
    }

    char ligne[2 * MAX_TEXTE + 20];
    int id;
    char profil[MAX_TEXTE], motivation[MAX_TEXTE];
    bool trouve = false;

    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d;%254[^;];%254[^\n]", &id, profil, motivation) == 3) {
            if (id == idMotivProfil) {
                fprintf(temp, "%d;%s;%s\n", id, nouveau_profil, nouvelle_motivation);
                trouve = true;
            } else {
                fprintf(temp, "%d;%s;%s\n", id, profil, motivation);
            }
        }
    }

    fclose(f);
    fclose(temp);

    remove(fichier);
    rename("temp.txt", fichier);
    return trouve ? 1 : 0;
}

void gestion_profils_motivation(const char* fichier) {
    char choix;
    do {
        printf("\n--- GESTION PROFILS MOTIVATION ---\n");
        printf("1. Ajouter un profil/motivation\n");
        printf("2. Afficher tous les profils/motivations\n");
        printf("3. Supprimer un profil/motivation\n");
        printf("4. Modifier un profil/motivation\n");
        printf("5. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Profil : ");
                char* profil = saisir_chaine();
                printf("Motivation : ");
                char* motivation = saisir_chaine();

                if (profil && motivation) {
                    ajouter_et_sauvegarder_profil_motivation(fichier, profil, motivation);
                } else {
                    printf("Erreur de saisie.\n");
                }

                free(profil);
                free(motivation);
                break;
            }
            case '2':
                afficher_profils_motivation(fichier);
                break;

            case '3': {
                int id_suppr;
                printf("ID du profil/motivation à supprimer : ");
                scanf("%d", &id_suppr);
                while (getchar() != '\n');

                if (supprimer_profil_motivation(fichier, id_suppr)) {
                    printf("Suppression réussie.\n");
                } else {
                    printf("ID non trouvé.\n");
                }
                break;
            }
            case '4': {
                int id_modif;
                printf("ID du profil/motivation à modifier : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');

                printf("Nouveau profil : ");
                char* nouveau_profil = saisir_chaine();
                printf("Nouvelle motivation : ");
                char* nouvelle_motivation = saisir_chaine();

                if (nouveau_profil && nouvelle_motivation) {
                    if (mettre_a_jour_profil_motivation(fichier, id_modif, nouveau_profil, nouvelle_motivation)) {
                        printf("Mise à jour réussie.\n");
                    } else {
                        printf("ID non trouvé ou mise à jour impossible.\n");
                    }
                } else {
                    printf("Erreur de saisie.\n");
                }
                free(nouveau_profil);
                free(nouvelle_motivation);
                break;
            }
        }
    } while (choix != '5');
}
