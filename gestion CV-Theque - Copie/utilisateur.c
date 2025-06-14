#include "utilisateur.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "experience.h"
#include "education.h"
#include "skill.h"
#include "langue.h"
#include "loisir.h"
#include "certificat.h"

int dernier_id_utilisateur = 0;

Utilisateur* creer_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* telephone) {
    Utilisateur* nouvel_utilisateur = (Utilisateur*)malloc(sizeof(Utilisateur));
    if (nouvel_utilisateur == NULL) {
        printf("Erreur : echec de l'allocation memoire.\n");
        return NULL;
    }

    nouvel_utilisateur->id = ++dernier_id_utilisateur;

    strncpy(nouvel_utilisateur->nom, nom, sizeof(nouvel_utilisateur->nom) - 1);
    nouvel_utilisateur->nom[sizeof(nouvel_utilisateur->nom) - 1] = '\0';

    strncpy(nouvel_utilisateur->prenom, prenom, sizeof(nouvel_utilisateur->prenom) - 1);
    nouvel_utilisateur->prenom[sizeof(nouvel_utilisateur->prenom) - 1] = '\0';

    strncpy(nouvel_utilisateur->email, email, sizeof(nouvel_utilisateur->email) - 1);
    nouvel_utilisateur->email[sizeof(nouvel_utilisateur->email) - 1] = '\0';

    strncpy(nouvel_utilisateur->password, password, sizeof(nouvel_utilisateur->password) - 1);
    nouvel_utilisateur->password[sizeof(nouvel_utilisateur->password) - 1] = '\0';

    strncpy(nouvel_utilisateur->telephone, telephone, sizeof(nouvel_utilisateur->telephone) - 1);
    nouvel_utilisateur->telephone[sizeof(nouvel_utilisateur->telephone) - 1] = '\0';

    strcpy(nouvel_utilisateur->role, "user");

    time_t now = time(NULL);
    strftime(nouvel_utilisateur->date_creation, 20, "%Y-%m-%d", localtime(&now));

    nouvel_utilisateur->suivant = NULL;
    return nouvel_utilisateur;
}

void ajouter_utilisateur(Utilisateur** tete, Utilisateur* nouvel_utilisateur) {
    if (*tete == NULL) {
        *tete = nouvel_utilisateur;
    } else {
        Utilisateur* current = *tete;
        while (current->suivant != NULL) {
            current = current->suivant;
        }
        current->suivant = nouvel_utilisateur;
    }
}

void ajouter_et_sauvegarder_utilisateur(Utilisateur** tete, const char* fichier, const char* nom, const char* prenom, const char* email, const char* password, const char* telephone) {
    if (!valider_utilisateur(nom, prenom, email, password, telephone)) {
        printf("Erreur : les donnees saisies sont invalides.\n");
        return;
    }

    if (utilisateur_existe(fichier, email)) {
        printf("Erreur : cet utilisateur existe deja.\n");
        return;
    }

    Utilisateur* nouvel_utilisateur = creer_utilisateur(nom, prenom, email, password, telephone);
    if (nouvel_utilisateur == NULL) {
        printf("Erreur : impossible de creer l'utilisateur.\n");
        return;
    }

    ajouter_utilisateur(tete, nouvel_utilisateur);

    FILE* f = fopen(fichier, "a"); // Mode  pour ajouter à la fin du fichier
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }
    fprintf(f, "%d;%s;%s;%s;%s;%s;%s;%s\n",
            nouvel_utilisateur->id,
            nouvel_utilisateur->nom,
            nouvel_utilisateur->prenom,
            nouvel_utilisateur->email,
            nouvel_utilisateur->password,
            nouvel_utilisateur->telephone,
            nouvel_utilisateur->date_creation,
            nouvel_utilisateur->role);
    fclose(f);

    printf("Utilisateur ajoute avec succes avec le role 'user'.\n");
}

void afficher_utilisateurs(const Utilisateur* utilisateur) {
    if (utilisateur == NULL) {
        printf("Aucun utilisateur a afficher.\n");
        return;
    }

    while (utilisateur != NULL) {
        printf("ID: %d\n", utilisateur->id);
        printf("Nom: %s\n", utilisateur->nom);
        printf("Prenom: %s\n", utilisateur->prenom);
        printf("Email: %s\n", utilisateur->email);
        printf("Telephone: %s\n", utilisateur->telephone);
        printf("Mot de passe: %s\n", utilisateur->password);
        printf("Date de creation: %s\n", utilisateur->date_creation);
        printf("Role: %s\n", utilisateur->role);
        printf("----------------------------\n");
        //utilisateur = utilisateur->suivant;
    }
}

void liberer_utilisateurs(Utilisateur* utilisateur) {
    while (utilisateur != NULL) {
        Utilisateur* temp = utilisateur;
        utilisateur = utilisateur->suivant;
        free(temp);
    }
}

bool utilisateur_existe_par_id(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        char nom[100], prenom[100], email[100], password[100], telephone[20], date_creation[20], role[50];
        sscanf(ligne, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
               &id, nom, prenom, email, password, telephone, date_creation, role);

        if (id == id_utilisateur) {
            fclose(f);
            return true;
        }
    }

    fclose(f);
    return false;
}

bool valider_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* telephone) {
    if (strlen(nom) == 0 || strlen(prenom) == 0 || strlen(email) == 0 || strlen(password) == 0 || strlen(telephone) == 0) {
        printf("Erreur : tous les champs sont obligatoires.\n");
        return false;
    }

    if (!verifier_email(email)) {
        printf("Erreur : l'adresse email est invalide.\n");
        return false;
    }

    if (!verifier_password(password)) {
        printf("Erreur : le mot de passe doit contenir au moins 8 caracteres, une majuscule, une minuscule, un chiffre et un caractere special.\n");
        return false;
    }

    if (!valider_telephone(telephone)) {
        return false;
    }

    return true;
}

bool utilisateur_existe(const char* fichier, const char* email) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        char nom[100], prenom[100], email_fichier[100], password[100], telephone[20], date_creation[20], role[50];
        sscanf(ligne, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
               &id, nom, prenom, email_fichier, password, telephone, date_creation, role);

        if (strcmp(email, email_fichier) == 0) {
            fclose(f);
            return true;
        }
    }

    fclose(f);
    return false;
}

int authentifier_utilisateur(const char* email, const char* password) {
    FILE* fichier = fopen("utilisateurs.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de louverture du fichier");
        return -1;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *token;
        int id;
        char fichier_nom[100], fichier_prenom[100], fichier_email[100],
             fichier_password[100], fichier_telephone[20], fichier_date[20], fichier_role[20];

        token = strtok(ligne, ";");
        if (token == NULL)
            continue;
        id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        strncpy(fichier_nom, token, sizeof(fichier_nom));

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        strncpy(fichier_prenom, token, sizeof(fichier_prenom));

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        strncpy(fichier_email, token, sizeof(fichier_email));

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        strncpy(fichier_password, token, sizeof(fichier_password));

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        strncpy(fichier_telephone, token, sizeof(fichier_telephone));

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        strncpy(fichier_date, token, sizeof(fichier_date));

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        strncpy(fichier_role, token, sizeof(fichier_role));

        if (strcmp(fichier_email, email) == 0 && strcmp(fichier_password, password) == 0) {
            fclose(fichier);
            return id;
        }
    }

    fclose(fichier);
    return -1;
}

void gestion_utilisateurs(Utilisateur** utilisateurs, int* id_utilisateur_courant) {
    char choix;
    do {
        printf("\n--- GESTION UTILISATEURS ---\n");
        printf("1. S'inscrire\n");
        printf("2. Se connecter\n");
        printf("3. Afficher mes information\n");
        printf("4. Supprimer un utilisateur\n");
        printf("5. Modifier un utilisateur\n");
        printf("6. Retour\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1': {
                printf("Nom : ");
                char *nom = saisir_chaine();
                printf("Prenom : ");
                char *prenom = saisir_chaine();
                printf("Email : ");
                char *email = saisir_chaine();
                printf("Mot de passe : ");
                char *password = saisir_chaine();
                printf("Telephone : ");
                char *telephone = saisir_chaine();

                printf("Inscription en cours avec le role 'user'...\n");
                ajouter_et_sauvegarder_utilisateur(utilisateurs, "utilisateurs.txt",
                                                  nom, prenom, email, password, telephone);
                free(nom);
                free(prenom);
                free(email);
                free(password);
                free(telephone);
                break;
            }
            case '2': {
                printf("Email : ");
                char *email = saisir_chaine();
                printf("Mot de passe : ");
                char *password = saisir_chaine();

                int id = authentifier_utilisateur(email, password);
                if (id != -1) {
                    *id_utilisateur_courant = id;
                    printf("Connexion reussie! ID: %d\n", id);
                } else {
                    printf("echec de connexion. Verifiez vos identifiants.\n");
                }

                free(email);
                free(password);
                break;
            }
            case '3':
                afficher_utilisateur("utilisateurs.txt", *id_utilisateur_courant);
                break;

            case '4': {
                if (*id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord.\n");
                    break;
                }

                int id_suppr;
                printf("ID utilisateur supprimer (0 pour soi-meme) : ");
                scanf("%d", &id_suppr);

                if (id_suppr == 0) {
                    id_suppr = *id_utilisateur_courant;
                }

                if (id_suppr != *id_utilisateur_courant && !est_admin(*id_utilisateur_courant)) {
                    printf("Permission refusee. Seuls les admins peuvent supprimer d'autres comptes.\n");
                    break;
                }

                supprimer_utilisateur_complet(id_suppr);

                if (id_suppr == *id_utilisateur_courant) {
                    *id_utilisateur_courant = 0;
                }
                break;
            }
            case '5': {
                    if (*id_utilisateur_courant == 0) {
                        printf("Veuillez vous connecter d'abord.\n");
                        break;
                    }

                    int id_modif = *id_utilisateur_courant;  // on prend directement l'ID de l'utilisateur connecté

                    printf("Modification du profil utilisateur (ID = %d)\n", id_modif);

                    printf("Nouveau nom : ");
                    char *nom = saisir_chaine();
                    printf("Nouveau prenom : ");
                    char *prenom = saisir_chaine();
                    printf("Nouvel email : ");
                    char *email = saisir_chaine();
                    printf("Nouveau mot de passe : ");
                    char *password = saisir_chaine();
                    printf("Nouveau telephone : ");
                    char *telephone = saisir_chaine();
                    int result = mettre_a_jour_utilisateur(id_modif, nom, prenom, email, password, telephone);
                    if (result > 0) {
                        printf("Utilisateur mis a jour avec succes !\n");
                    } else {
                        printf("Echec de la mise a jour de l'utilisateur.\n");
                    }

                    free(nom);
                    free(prenom);
                    free(email);
                    free(password);
                    free(telephone);

                    break;
            }
        }
    } while (choix != '6');
}

bool est_admin(int id_utilisateur) {
    FILE* fichier = fopen("utilisateurs.txt", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return false;
    }

    char ligne[MAX_LINE];
    int id;
    char nom[50], prenom[50], email[100], mdp[50], telephone[20], date_creation[20], role[20];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (sscanf(ligne, "%d;%49[^;];%49[^;];%99[^;];%49[^;];%19[^;];%19[^;];%19s",
                   &id, nom, prenom, email, mdp, telephone, date_creation, role) == 8) {
            if (id == id_utilisateur) {
                fclose(fichier);
                return strcmp(role, "admin") == 0;
            }
        }
    }

    fclose(fichier);
    return false;
}

int supprimer_utilisateur_complet(int id_utilisateur) {
    supprimer_element_par_ids("educations.txt", id_utilisateur, -1);
    supprimer_element_par_ids("experiences.txt", id_utilisateur, -1);
    supprimer_element_par_ids("certificats.txt", id_utilisateur, -1);
    supprimer_element_par_ids("skills.txt", id_utilisateur, -1);
    supprimer_element_par_ids("langues.txt", id_utilisateur, -1);
    supprimer_element_par_ids("loisirs.txt", id_utilisateur, -1);

    return supprimer_element_par_ids("utilisateurs.txt", id_utilisateur, id_utilisateur);
}

int mettre_a_jour_utilisateur(int id_utilisateur, const char* nom, const char* prenom,
                             const char* email, const char* password, const char* telephone) {
    if (!valider_utilisateur(nom, prenom, email, password, telephone)) {
        printf("Erreur : les donnees saisies sont invalides.\n");
        return;
    }

    if (utilisateur_existe("utilisateur.txt", email)) {
        printf("Erreur : cet utilisateur existe deja.\n");
        return;
    }

    printf("\n=== Mise à jour Utilisateur ===\n");
    char date_modif[20];
    time_t now = time(NULL);
    strftime(date_modif, sizeof(date_modif), "%Y-%m-%d", localtime(&now));

    char role_existant[50] = {0};
    FILE* f = fopen("utilisateurs.txt", "r");
    if (!f) {
        perror("Erreur ouverture fichier utilisateurs.txt");
        return 0;
    }

    char ligne[MAX_LINE];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        char nom_f[50], prenom_f[50], email_f[100], password_f[50], telephone_f[20], date_f[20], role_f[50];
        if (sscanf(ligne, "%d;%49[^;];%49[^;];%99[^;];%49[^;];%19[^;];%19[^;];%49[^\n]",
                   &id, nom_f, prenom_f, email_f, password_f, telephone_f, date_f, role_f) == 8) {
            if (id == id_utilisateur) {
                strncpy(role_existant, role_f, sizeof(role_existant) - 1);
                break;
            }
                   }
    }
    fclose(f);

    if (role_existant[0] == '\0') {
        printf("Utilisateur ID %d non trouvé.\n", id_utilisateur);
        return 0;
    }

    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%s;%s;%s;%s;%s;%s;%s",
             id_utilisateur, nom, prenom, email, password, telephone, date_modif, role_existant);

    return mettre_a_jour_element("utilisateurs.txt", id_utilisateur, id_utilisateur, nouveaux_donnees);
}
void afficher_infos_utilisateur(int id_utilisateur) {
    printf("=====MON CV=====\n");
    afficher_utilisateur("utilisateurs.txt", id_utilisateur);
    afficher_skills("skills.txt", id_utilisateur);
    afficher_educations("educations.txt", id_utilisateur);
    afficher_experiences("experiences.txt", id_utilisateur);
    afficher_langues_par_utilisateur("langues.txt", id_utilisateur);
    afficher_loisirs_par_utilisateur("loisirs.txt", id_utilisateur);
    afficher_certificats_par_utilisateur("certificats.txt", id_utilisateur);
}

void afficher_utilisateur(const char* nomFichier, int idRecherche) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        int id;
        char nom[100], prenom[100], email[100], password[100], telephone[20], date_creation[20], role[50];

        if (sscanf(ligne, "%d;%99[^;];%99[^;];%99[^;];%99[^;];%19[^;];%19[^;];%49[^\n]",
                   &id, nom, prenom, email, password, telephone, date_creation, role) == 8) {
            if (id == idRecherche) {
                printf("Nom: %s\n", nom);
                printf("Prenom: %s\n", prenom);
                printf("Email: %s\n", email);
                printf("Telephone: %s\n", telephone);
                printf("Date de creation: %s\n", date_creation);
                fclose(fichier);
                return;
            }
        }
    }

    printf("Utilisateur avec ID %d non trouvé.\n", idRecherche);
    fclose(fichier);
}