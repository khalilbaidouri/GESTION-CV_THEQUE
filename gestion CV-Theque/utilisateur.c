#include "utilisateur.h"
#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "experience.h"
#include "education.h"
#include "education.h"
#include "experience.h"
#include "certificat.h"
#include "skill.h"
#include "langue.h"
#include "loisir.h"
#include "cv.h"
#include "education.h"
#include "experience.h"
#include "certificat.h"
#include "skill.h"
#include "langue.h"
#include "loisir.h"
#include "cv.h"

int dernier_id_utilisateur = 0;



// Fonction pour créer un nouvel utilisateur
Utilisateur* creer_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* role) {
    Utilisateur* nouvel_utilisateur = (Utilisateur*)malloc(sizeof(Utilisateur));
    if (nouvel_utilisateur == NULL) {
        printf("Erreur : échec de l'allocation mémoire.\n");
        return NULL;
    }

    // Attribuer un nouvel ID
    nouvel_utilisateur->id = ++dernier_id_utilisateur;

    // Copier les autres champs avec vérification de la longueur
    strncpy(nouvel_utilisateur->nom, nom, sizeof(nouvel_utilisateur->nom) - 1);
    nouvel_utilisateur->nom[sizeof(nouvel_utilisateur->nom) - 1] = '\0';

    strncpy(nouvel_utilisateur->prenom, prenom, sizeof(nouvel_utilisateur->prenom) - 1);
    nouvel_utilisateur->prenom[sizeof(nouvel_utilisateur->prenom) - 1] = '\0';

    strncpy(nouvel_utilisateur->email, email, sizeof(nouvel_utilisateur->email) - 1);
    nouvel_utilisateur->email[sizeof(nouvel_utilisateur->email) - 1] = '\0';

    strncpy(nouvel_utilisateur->password, password, sizeof(nouvel_utilisateur->password) - 1);
    nouvel_utilisateur->password[sizeof(nouvel_utilisateur->password) - 1] = '\0';

    strncpy(nouvel_utilisateur->role, role, sizeof(nouvel_utilisateur->role) - 1);
    nouvel_utilisateur->role[sizeof(nouvel_utilisateur->role) - 1] = '\0';

    // Générer la date de création
	// Générer la date de création sans l'heure
	time_t now = time(NULL);
	strftime(nouvel_utilisateur->date_creation, 20, "%Y-%m-%d", localtime(&now));


    nouvel_utilisateur->suivant = NULL;
    return nouvel_utilisateur;
}

// Fonction pour ajouter un utilisateur à la liste
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

// Fonction pour ajouter un utilisateur et le sauvegarder dans un fichier
void ajouter_et_sauvegarder_utilisateur(Utilisateur** tete, const char* fichier, const char* nom, const char* prenom, const char* email, const char* password, const char* role) {
    // Valider les données
    if (!valider_utilisateur(nom, prenom, email, password, role)) {
        printf("Erreur : les donnees saisies sont invalides.\n");
        return;
    }

    // Vérifier si l'utilisateur existe déjà
    if (utilisateur_existe(fichier, email)) {
        printf("Erreur : cet utilisateur existe deja.\n");
        return;
    }

    // Créer un nouvel utilisateur
    Utilisateur* nouvel_utilisateur = creer_utilisateur(nom, prenom, email, password, role);
    if (nouvel_utilisateur == NULL) {
        printf("Erreur : impossible de creer l'utilisateur.\n");
        return;
    }

    // Ajouter l'utilisateur à la liste
    ajouter_utilisateur(tete, nouvel_utilisateur);

    // Sauvegarder dans le fichier
    FILE* f = fopen(fichier, "a"); // Mode "append" pour ajouter à la fin du fichier
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return;
    }
    fprintf(f, "%d;%s;%s;%s;%s;%s;%s\n", nouvel_utilisateur->id, nouvel_utilisateur->nom, nouvel_utilisateur->prenom, nouvel_utilisateur->email, nouvel_utilisateur->password, nouvel_utilisateur->date_creation, nouvel_utilisateur->role);
    fclose(f);

    printf("Utilisateur ajoute avec succes.\n");
}
// Fonction pour afficher tous les utilisateurs
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
        printf("Mot de passe: %s\n", utilisateur->password);
        printf("Date de creation: %s\n", utilisateur->date_creation);
        printf("Role: %s\n", utilisateur->role);
        printf("----------------------------\n");
        utilisateur = utilisateur->suivant;
    }
}

// Fonction pour libérer la mémoire de la liste des utilisateurs
void liberer_utilisateurs(Utilisateur* utilisateur) {
    while (utilisateur != NULL) {
        Utilisateur* temp = utilisateur;
        utilisateur = utilisateur->suivant;
        free(temp);
    }
}


// Fonction pour vérifier si un utilisateur existe déjà dans le fichier
bool utilisateur_existe_par_id(const char* fichier, int id_utilisateur) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false; // Si le fichier n'existe pas, l'utilisateur ne peut pas exister
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        char nom[100], prenom[100], email[100], password[100], date_creation[20], role[50];
        sscanf(ligne, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]", &id, nom, prenom, email, password, date_creation, role);

        if (id == id_utilisateur) {
            fclose(f);
            return true; // L'utilisateur existe
        }
    }

    fclose(f);
    return false; // L'utilisateur n'existe pas
}

bool valider_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* role) {
    // Vérifier que les champs ne sont pas vides
    if (strlen(nom) == 0 || strlen(prenom) == 0 || strlen(email) == 0 || strlen(password) == 0 || strlen(role) == 0) {
        return false;
    }

	// Créer une copie modifiable de 'role'
	char role_copy[100];  // Assurez-vous que la taille est suffisante
	strncpy(role_copy, role, sizeof(role_copy) - 1);
	role_copy[sizeof(role_copy) - 1] = '\0';  // Ajouter un caractère de fin
	
	// Convertir la copie en minuscules
	convertir_en_minuscules(role_copy);
	
	// Utiliser 'role_copy' pour la comparaison
	if (strcmp(role_copy, "user") != 0 && strcmp(role_copy, "admin") != 0 && strcmp(role_copy, "recruteur") != 0) {
	    printf("Erreur : le role \"%s\" n'existe pas !\n", role_copy);
	    return false;
	}


    // Vérifier l'e-mail
    if (!verifier_email(email)) {
        printf("Erreur : l'adresse e-mail est invalide.\n");
        return false;
    }

    // Vérifier le mot de passe
    if (!verifier_password(password)) {
        printf("Erreur : le mot de passe doit contenir au moins 8 caractères, une majuscule, une minuscule, un chiffre et un caractère special.\n");
        return false;
    }

    return true;
}

// Fonction pour vérifier si un utilisateur existe déjà dans le fichier
bool utilisateur_existe(const char* fichier, const char* email) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) {
        return false; // Si le fichier n'existe pas, l'utilisateur ne peut pas exister
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        char nom[100], prenom[100], email_fichier[100], password[100], date_creation[20], role[50];
        sscanf(ligne, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]", &id, nom, prenom, email_fichier, password, date_creation, role);

        if (strcmp(email, email_fichier) == 0) {
            fclose(f);
            return true; // L'utilisateur existe déjà
        }
    }

    fclose(f);
    return false; // L'utilisateur n'existe pas
}

// [Les implémentations suivent le même pattern que gestion_educations]

int authentifier_utilisateur(const char* email, const char* password) {
    FILE* fichier = fopen("utilisateurs.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1; // Retourner -1 pour signaler une erreur
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *token;
        int id;
        char fichier_nom[100], fichier_prenom[100], fichier_email[100], fichier_password[100], fichier_date[20], fichier_role[20];

        // Découpage de la ligne avec strtok
        token = strtok(ligne, ";");
        if (token == NULL) continue;
        id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(fichier_nom, token, sizeof(fichier_nom));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(fichier_prenom, token, sizeof(fichier_prenom));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(fichier_email, token, sizeof(fichier_email));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(fichier_password, token, sizeof(fichier_password));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(fichier_date, token, sizeof(fichier_date));

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(fichier_role, token, sizeof(fichier_role));

        // Vérifier si l'email et le mot de passe correspondent
        
        if (strcmp(fichier_email, email) == 0 && strcmp(fichier_password, password) == 0) {
            fclose(fichier);
            return id; // Retourne l'ID de l'utilisateur en cas de succès
        }
    }

    fclose(fichier);
    return -1; // Retourne -1 si l'utilisateur n'est pas trouvé
}

// Fonctions de gestion pour chaque module
void gestion_utilisateurs(Utilisateur** utilisateurs, int* id_utilisateur_courant) {
    char choix;
    do {
        printf("\n--- GESTION UTILISATEURS ---\n");
        printf("1. S'inscrire\n");
        printf("2. Se connecter\n");
        printf("3. Afficher tous les utilisateurs\n");
        printf("4. Supprimer un utilisateur\n");
        printf("5. Modifier un utilisateur\n");  // Nouvelle option
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
                printf("Rôle (user/admin/recruteur) : ");
                char *role = saisir_chaine();

                convertir_en_minuscules(role);
                
                ajouter_et_sauvegarder_utilisateur(utilisateurs, "utilisateurs.txt", 
                                                  nom, prenom, email, password, role);
                
                free(nom);
                free(prenom);
                free(email);
                free(password);
                free(role);
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
                    printf("Échec de connexion. Vérifiez vos identifiants.\n");
                }
                
                free(email);
                free(password);
                break;
            }
            case '3':
                afficher_utilisateurs(*utilisateurs);
                break;
                
            case '4': {
                if (*id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord.\n");
                    break;
                }
                
                int id_suppr;
                printf("ID utilisateur à supprimer (0 pour soi-meme) : ");
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
            case '5': {  // Nouveau cas pour la modification
                if (*id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord.\n");
                    break;
                }
                
                int id_modif;
                printf("ID utilisateur à modifier (0 pour soi-meme) : ");
                scanf("%d", &id_modif);
                while (getchar() != '\n');
                
                if (id_modif == 0) {
                    id_modif = *id_utilisateur_courant;
                }
                
                if (id_modif != *id_utilisateur_courant && !est_admin(*id_utilisateur_courant)) {
                    printf("Permission refusee. Seuls les admins peuvent modifier d'autres comptes.\n");
                    break;
                }
                
                printf("Nouveau nom : ");
                char *nom = saisir_chaine();
                printf("Nouveau prénom : ");
                char *prenom = saisir_chaine();
                printf("Nouvel email : ");
                char *email = saisir_chaine();
                printf("Nouveau mot de passe : ");
                char *password = saisir_chaine();
                printf("Nouveau rôle (user/admin/recruteur) : ");
                char *role = saisir_chaine();
                
                int result = mettre_a_jour_utilisateur(id_modif, nom, prenom, email, password, role);
                if (result > 0) {
                    printf("Utilisateur mis à jour avec succès!\n");
                } else {
                    printf("Échec de la mise à jour de l'utilisateur.\n");
                }
                
                free(nom);
                free(prenom);
                free(email);
                free(password);
                free(role);
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
    char nom[50], prenom[50], email[100], mdp[50], date_creation[20], role[20];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Découper la ligne en ses 7 éléments
        if (sscanf(ligne, "%d;%49[^;];%49[^;];%99[^;];%49[^;];%19[^;];%19s", 
                   &id, nom, prenom, email, mdp, date_creation, role) == 7) {
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
    // 1. Suppression de toutes les données associées
    supprimer_element_par_ids("educations.txt", id_utilisateur, -1);
    supprimer_element_par_ids("experiences.txt", id_utilisateur, -1);
    supprimer_element_par_ids("certificats.txt", id_utilisateur, -1);
    supprimer_element_par_ids("skills.txt", id_utilisateur, -1);
    supprimer_element_par_ids("langues.txt", id_utilisateur, -1);
    supprimer_element_par_ids("loisirs.txt", id_utilisateur, -1);
    supprimer_element_par_ids("cvs.txt", id_utilisateur, -1);

    // 2. Suppression de l'utilisateur lui-même
    // Format de utilisateurs.txt : id;nom;prenom;email;password;date_creation;role
    return supprimer_element_par_ids("utilisateurs.txt", id_utilisateur, id_utilisateur);
}


int mettre_a_jour_utilisateur(int id_utilisateur, const char* nom, const char* prenom, const char* email, const char* password, const char* role) {
    printf("\n=== Mise à jour Utilisateur ===\n");
    
    // Obtenir la date actuelle pour la mise à jour
    char date_creation[20];
    time_t now = time(NULL);
    strftime(date_creation, 20, "%Y-%m-%d", localtime(&now));
    
    // Construire la nouvelle ligne
    char nouveaux_donnees[MAX_LINE];
    snprintf(nouveaux_donnees, sizeof(nouveaux_donnees),
             "%d;%s;%s;%s;%s;%s;%s", 
             id_utilisateur, nom, prenom, email, password, date_creation, role);
    
    return mettre_a_jour_element("utilisateurs.txt", id_utilisateur, id_utilisateur, nouveaux_donnees);
}
