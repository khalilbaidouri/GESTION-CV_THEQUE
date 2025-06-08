#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // Pour tolower

// Fonction pour v�rifier si une date est valide au format YYYY-MM-DD
int est_date_valide(const char* date) {
    int year, month, day;
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3) {
        return 0; // Format invalide
    }

    // V�rifier que l'ann�e, le mois et le jour sont dans des plages valides
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0; // Date invalide
    }

    // V�rification des jours en fonction du mois
    if (month == 2) {
        // F�vrier : v�rifier si c'est une ann�e bissextile
        int bissextile = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if ((bissextile && day > 29) || (!bissextile && day > 28)) {
            return 0; // Jour invalide en f�vrier
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        // Mois avec 30 jours
        if (day > 30) {
            return 0; // Jour invalide
        }
    }

    return 1; // La date est valide
}

// Fonction pour comparer deux dates
int comparer_dates(const char* date1, const char* date2) {
    int year1, month1, day1;
    int year2, month2, day2;
    if (sscanf(date1, "%4d-%2d-%2d", &year1, &month1, &day1) != 3 ||
        sscanf(date2, "%4d-%2d-%2d", &year2, &month2, &day2) != 3) {
        return -1; // Erreur d'analyse
    }

    if (year1 < year2) return -1;
    if (year1 > year2) return 1;
    if (month1 < month2) return -1;
    if (month1 > month2) return 1;
    if (day1 < day2) return -1;
    if (day1 > day2) return 1;

    return 0; // Les dates sont �gales
}

int valider_periode(const char* date_debut, const char* date_fin) {
    // V�rification des formats
    if (!est_date_valide(date_debut)) {
        printf("Erreur : Date de d�but invalide (%s)\n", date_debut);
        return 0;
    }
    
    if (!est_date_valide(date_fin)) {
        printf("Erreur : Date de fin invalide (%s)\n", date_fin);
        return 0;
    }

    // Comparaison des dates
    int result = comparer_dates(date_debut, date_fin);
    
    if (result == 1) {
        printf("Erreur : Incoh�rence temporelle (%s > %s)\n", date_debut, date_fin);
        return 0;
    }
    
    return 1; // Validation OK
}
// Fonction pour nettoyer une cha�ne de caract�res
void nettoyer_chaine(char* chaine) {
    chaine[strcspn(chaine, "\n")] = '\0'; // Supprime le retour � la ligne
    char* debut = chaine;
    while (*debut == ' ') debut++; // Supprime les espaces au d�but
    char* fin = chaine + strlen(chaine) - 1;
    while (fin > debut && *fin == ' ') fin--; // Supprime les espaces � la fin
    *(fin + 1) = '\0';
    if (debut != chaine) memmove(chaine, debut, strlen(debut) + 1); // D�place la cha�ne nettoy�e
}

// Fonction pour lire le dernier id depuis un fichier
int lire_dernier_id(const char* fichier) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) return 0; // Si le fichier n'existe pas, commencer � 0

    int max_id = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        if (sscanf(ligne, "%d;", &id) == 1) { // Lire l'ID de la ligne
            if (id > max_id) max_id = id;
        }
    }

    fclose(f);
    return max_id; // Retourner le dernier ID trouv�
}

bool verifier_password(const char* password) {
    if (strlen(password) < 8) {
        return false; // Le mot de passe doit avoir au moins 8 caract�res
    }

    bool has_upper = false, has_lower = false, has_digit = false, has_special = false;

    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') {
            has_upper = true;
        } else if (password[i] >= 'a' && password[i] <= 'z') {
            has_lower = true;
        } else if (password[i] >= '0' && password[i] <= '9') {
            has_digit = true;
        } else if (strchr("!@#$%^&*", password[i]) != NULL) {
            has_special = true;
        }
    }

    return has_upper && has_lower && has_digit && has_special;
}
bool verifier_email(const char* email) {
    // V�rifier la pr�sence d'un '@' et d'un '.'
    const char* at = strchr(email, '@');
    const char* dot = strchr(email, '.');

    if (at == NULL || dot == NULL || at > dot) {
        return false; // Format d'e-mail invalide
    }

    // V�rifier que le '.' n'est pas juste apr�s le '@'
    if (dot == at + 1) {
        return false;
    }

    // V�rifier que l'e-mail ne commence ou ne se termine pas par un caract�re invalide
    if (email[0] == '.' || email[strlen(email) - 1] == '.') {
        return false;
    }

    return true;
}


char* saisir_chaine() {
    char *chaine = NULL;
    char caractere;
    int i = 0;

    chaine = (char*) malloc(sizeof(char));
    if (chaine == NULL) {
        perror("Erreur d'allocation de m�moire");
        exit(1);
    }

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        chaine[i] = caractere;
        i++;

        chaine = (char*) realloc(chaine, (i + 1) * sizeof(char));
        if (chaine == NULL) {
            perror("Erreur de reallocation de memoire");
            exit(1);
        }
    }

    // Ajouter le caract�re nul � la fin de la cha�ne
    chaine[i] = '\0';

    return chaine;
}
bool valider_telephone(const char* telephone) {
    if (telephone == NULL || strlen(telephone) == 0) {
        printf("Erreur : le numéro de téléphone ne peut pas être vide.\n");
        return false;
    }

    int len = strlen(telephone);

    // Vérifier la longueur (entre 8 et 15 chiffres selon les standards internationaux)
    if (len < 8 || len > 15) {
        printf("Erreur : le numéro de téléphone doit contenir entre 8 et 15 chiffres.\n");
        return false;
    }

    // Vérifier que tous les caractères sont des chiffres (sauf le premier qui peut être +)
    int start = 0;
    if (telephone[0] == '+') {
        start = 1;
        if (len < 9) { // Au moins 8 chiffres après le +
            printf("Erreur : le numéro de téléphone avec indicatif international doit contenir au moins 8 chiffres après le '+'.\n");
            return false;
        }
    }

    for (int i = start; i < len; i++) {
        if (!isdigit(telephone[i])) {
            printf("Erreur : le numéro de téléphone ne doit contenir que des chiffres (et éventuellement un '+' au début).\n");
            return false;
        }
    }

    return true;
}


void convertir_en_minuscules(char* chaine) {
    for (int i = 0; chaine[i] != '\0'; i++) {
        chaine[i] = tolower(chaine[i]);
    }
}


/**
 * Supprime un ou plusieurs �l�ments d'un fichier 
 * @param filename: Fichier � modifier
 * @param id_utilisateur: ID utilisateur concern�
 * @param id_element: ID �l�ment � supprimer (-1 pour tous)
 * @param element_type: Nom du type pour les messages
 * @return 0 si succ�s, -1 si erreur
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_LINE 1024

int supprimer_element_par_ids(const char* filename, int id_utilisateur, int id_element) {
    FILE *original = fopen(filename, "r");
    if (!original) {
        printf("Erreur : impossible d'ouvrir %s\n", filename);
        return -1;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(original);
        printf("Erreur : création fichier temporaire impossible\n");
        return -1;
    }

    char ligne[MAX_LINE];
    int deleted = 0;
    int user_field_pos = 1; // Position par défaut (2e champ)
    int id_field_pos = 0;   // Position ID élément

    if (strcmp(filename, "utilisateurs.txt") == 0) user_field_pos = 0;
    else if (strcmp(filename, "certificats.txt") == 0) user_field_pos = 4;
    else if (strcmp(filename, "experiences.txt") == 0) user_field_pos = 1;
    else if (strcmp(filename, "langues.txt") == 0) user_field_pos = 2;
    else if (strcmp(filename, "loisirs.txt") == 0) user_field_pos = 1;

    while (fgets(ligne, sizeof(ligne), original)) {
        char copie[MAX_LINE];
        strcpy(copie, ligne);

        int current_id = -1, current_user = -1;
        char *token, *saveptr;
        int field_idx = 0;

        token = strtok_r(copie, ";", &saveptr);
        while (token != NULL) {
            if (field_idx == id_field_pos) current_id = atoi(token);
            if (field_idx == user_field_pos) {
                current_user = atoi(token);
                break;
            }
            token = strtok_r(NULL, ";", &saveptr);
            field_idx++;
        }

        if (!(current_user == id_utilisateur && (id_element == -1 || current_id == id_element))) {
            fputs(ligne, temp);
        } else {
            deleted++;
            printf("Suppression : %s", ligne);
        }
    }

    fclose(original);
    fclose(temp);

    if (deleted > 0) {
        if (remove(filename) != 0) {
            printf("Erreur suppression fichier original\n");
            remove("temp.txt");
            return -1;
        }
        if (rename("temp.txt", filename) != 0) {
            printf("Erreur renommage fichier temporaire\n");
            return -1;
        }
        printf("-> %d éléments supprimés dans %s\n", deleted, filename);
    } else {
        remove("temp.txt");
        printf("-> Aucun élément à supprimer dans %s\n", filename);
    }

    return deleted;
}
//
// int mettre_a_jour_element(const char* filename, int id_utilisateur, int id_element, const char* nouveaux_donnees) {
//     FILE *original = fopen(filename, "r");
//     if (!original) {
//         printf("Erreur : impossible d'ouvrir %s\n", filename);
//         return -1;
//     }
//     FILE *temp = fopen("temp.txt", "w");
//     if (!temp) {
//         fclose(original);
//         printf("Erreur : création fichier temporaire\n");
//         return -1;
//     }
//
//     char ligne[MAX_LINE];
//     int updated = 0;
//     int user_field_pos = (strcmp(filename, "loisirs.txt") == 0) ? 1 :
//                          (strcmp(filename, "langues.txt") == 0) ? 2 : 1;
//
//     int est_utilisateurs = (strcmp(filename, "utilisateurs.txt") == 0);
//
//     while (fgets(ligne, sizeof(ligne), original)) {
//         char copie[MAX_LINE];
//         strcpy(copie, ligne);
//
//         int current_id = -1, current_user = -1;
//         int field_idx = 0;
//         char *token, *saveptr;
//
//         token = strtok_r(copie, ";", &saveptr);
//         while (token) {
//             // Nettoyer fin de token
//             char *end = token + strlen(token) - 1;
//             while (end > token && (*end == '\n' || *end == '\r' || *end == ' ')) end--;
//             *(end + 1) = '\0';
//
//             if (field_idx == 0) current_id = atoi(token);
//             if (field_idx == user_field_pos) current_user = atoi(token);
//
//             token = strtok_r(NULL, ";", &saveptr);
//             field_idx++;
//         }
//
//         if (current_user == id_utilisateur && (id_element == -1 || current_id == id_element)) {
//             // Nettoyer nouveaux_donnees de sa fin de ligne
//             char buffer[MAX_LINE];
//             strncpy(buffer, nouveaux_donnees, MAX_LINE);
//             buffer[MAX_LINE - 1] = '\0';
//
//             size_t len = strlen(buffer);
//             if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
//                 buffer[len - 1] = '\0';
//             }
//
//             fprintf(temp, "%s\n", buffer);
//             updated++;
//             printf("Mise à jour : %s -> %s\n", ligne, buffer);
//         } else {
//             fputs(ligne, temp);
//         }
//     }
//
//     fclose(original);
//     fclose(temp);
//
//     if (updated > 0) {
//         if (remove(filename) != 0) {
//             printf("Erreur suppression fichier original\n");
//             remove("temp.txt");
//             return -1;
//         }
//         if (rename("temp.txt", filename) != 0) {
//             printf("Erreur renommage fichier temporaire\n");
//             return -1;
//         }
//         printf("-> %d éléments mis à jour dans %s\n", updated, filename);
//     } else {
//         remove("temp.txt");
//         printf("-> Aucun élément à mettre à jour dans %s\n", filename);
//     }
//
//     return updated;
// }
//


#define MAX_LINE 512

int mettre_a_jour_element(const char* filename, int id_utilisateur, int id_element, const char* nouveaux_donnees) {
    FILE *original = fopen(filename, "r");
    if (!original) {
        printf("Erreur : impossible d'ouvrir %s\n", filename);
        return -1;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(original);
        printf("Erreur : création fichier temporaire\n");
        return -1;
    }

    char ligne[MAX_LINE];
    int updated = 0;

    // Position du champ user_id dans la ligne selon le fichier
    int user_field_pos = (strcmp(filename, "loisirs.txt") == 0) ? 1 :
                         (strcmp(filename, "langues.txt") == 0) ? 2 : 1;

    // Flag pour indiquer si on travaille sur utilisateurs.txt
    int est_utilisateurs = (strcmp(filename, "utilisateurs.txt") == 0);

    while (fgets(ligne, sizeof(ligne), original)) {
        char copie[MAX_LINE];
        strcpy(copie, ligne);

        int current_id = -1, current_user = -1;
        int field_idx = 0;
        char *token, *saveptr;

        token = strtok_r(copie, ";", &saveptr);
        while (token) {
            // Nettoyer fin de token
            char *end = token + strlen(token) - 1;
            while (end > token && (*end == '\n' || *end == '\r' || *end == ' ')) end--;
            *(end + 1) = '\0';

            if (field_idx == 0) current_id = atoi(token);
            if (field_idx == user_field_pos) current_user = atoi(token);

            token = strtok_r(NULL, ";", &saveptr);
            field_idx++;
        }

        if ((est_utilisateurs && current_id == id_element) ||
            (!est_utilisateurs && current_user == id_utilisateur && (id_element == -1 || current_id == id_element))) {
            // Nettoyer nouveaux_donnees de sa fin de ligne
            char buffer[MAX_LINE];
            strncpy(buffer, nouveaux_donnees, MAX_LINE);
            buffer[MAX_LINE - 1] = '\0';

            size_t len = strlen(buffer);
            if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
                buffer[len - 1] = '\0';
            }

            fprintf(temp, "%s\n", buffer);
            updated++;
            printf("Mise à jour : %s -> %s\n", ligne, buffer);
        } else {
            fputs(ligne, temp);
        }
    }

    fclose(original);
    fclose(temp);

    if (updated > 0) {
        if (remove(filename) != 0) {
            printf("Erreur suppression fichier original\n");
            remove("temp.txt");
            return -1;
        }
        if (rename("temp.txt", filename) != 0) {
            printf("Erreur renommage fichier temporaire\n");
            return -1;
        }
        printf("-> %d éléments mis à jour dans %s\n", updated, filename);
    } else {
        remove("temp.txt");
        printf("-> Aucun élément à mettre à jour dans %s\n", filename);
    }

    return updated;
}
