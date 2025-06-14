#include "supplimentaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

int est_date_valide(const char* date) {
    int year, month, day;
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3) {
        return 0;
    }

    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }
    if (month == 2) {
        int bissextile = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if ((bissextile && day > 29) || (!bissextile && day > 28)) {
            return 0;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return 0;
        }
    }
    return 1;
}

int comparer_dates(const char* date1, const char* date2) {
    int year1, month1, day1;
    int year2, month2, day2;
    if (sscanf(date1, "%4d-%2d-%2d", &year1, &month1, &day1) != 3 ||
        sscanf(date2, "%4d-%2d-%2d", &year2, &month2, &day2) != 3) {
        return -1;
    }

    if (year1 < year2)
        return -1;
    if (year1 > year2)
        return 1;
    if (month1 < month2)
        return -1;
    if (month1 > month2)
        return 1;
    if (day1 < day2)
        return -1;
    if (day1 > day2)
        return 1;

    return 0;
}

int valider_periode(const char* date_debut, const char* date_fin) {
    if (!est_date_valide(date_debut)) {
        printf("Erreur : Date debut invalide (%s)\n", date_debut);
        return 0;
    }

    if (!est_date_valide(date_fin)) {
        printf("Erreur : Date fin invalide (%s)\n", date_fin);
        return 0;
    }

    int result = comparer_dates(date_debut, date_fin);

    if (result == 1) {
        printf("Erreur : Incoherence temporelle (%s > %s)\n", date_debut, date_fin);
        return 0;
    }

    return 1;
}

void nettoyer_chaine(char* chaine) {
    chaine[strcspn(chaine, "\n")] = '\0';
    char* debut = chaine;
    while (*debut == ' ')
        debut++;
    char* fin = chaine + strlen(chaine) - 1;
    while (fin > debut && *fin == ' ')
        fin--;
    *(fin + 1) = '\0';
    if (debut != chaine)
        memmove(chaine, debut, strlen(debut) + 1);
}

int lire_dernier_id(const char* fichier) {
    FILE* f = fopen(fichier, "r");
    if (f == NULL) return 0;

    int max_id = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f)) {
        int id;
        if (sscanf(ligne, "%d;", &id) == 1) {
            if (id > max_id)
                max_id = id;
        }
    }

    fclose(f);
    return max_id;
}

bool verifier_password(const char* password) {
    if (strlen(password) < 8) {
        return false;
    }

    bool contientMaj = false, contientMini = false, contientNmbr = false, contientCaractereSpecial = false;

    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') {
            contientMaj = true;
        } else if (password[i] >= 'a' && password[i] <= 'z') {
            contientMini = true;
        } else if (password[i] >= '0' && password[i] <= '9') {
            contientNmbr = true;
        } else if (strchr("!@#$%^&*", password[i]) != NULL) {
            contientCaractereSpecial = true;
        }
    }

    return contientMaj && contientMini && contientNmbr && contientCaractereSpecial;
}

bool verifier_email(const char* email) {
    const char* at = strchr(email, '@');
    const char* dot = strchr(email, '.');

    if (at == NULL || dot == NULL || at > dot) {
        return false;
    }

    if (dot == at + 1) {
        return false;
    }

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
        perror("Erreur allocation memoire");
        exit(1);
    }

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        chaine[i] = caractere;
        i++;

        chaine = (char*) realloc(chaine, (i + 1) * sizeof(char));
        if (chaine == NULL) {
            perror("Erreur reallocation memoire");
            exit(1);
        }
    }

    chaine[i] = '\0';

    return chaine;
}

bool valider_telephone(const char* telephone) {
    if (telephone == NULL || strlen(telephone) == 0) {
        printf("Erreur : numero de telephone vide.\n");
        return false;
    }

    int len = strlen(telephone);
    if (len < 8 || len > 15) {
        printf("Erreur : numero de telephone doit avoir entre 8 et 15 chiffres.\n");
        return false;
    }

    int start = 0;
    if (telephone[0] == '+') {
        start = 1;
        if (len < 9) {
            printf("Erreur : numero avec indicatif doit avoir au moins 8 chiffres apres '+'.\n");
            return false;
        }
    }

    for (int i = start; i < len; i++) {
        if (!isdigit(telephone[i])) {
            printf("Erreur : numero de telephone invalide.\n");
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


int supprimer_element_par_ids(const char* filename, int id_utilisateur, int id_element) {
    FILE *original = fopen(filename, "r");
    if (!original) {
        printf("Erreur : ouverture %s\n", filename);
        return -1;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(original);
        printf("Erreur : creation fichier temporaire\n");
        return -1;
    }

    char ligne[MAX_LINE];
    int pos_id_element = 0;
    int deleted = 0;
    int pos_id_utilisateur = 1;


    if (strcmp(filename, "utilisateurs.txt") == 0)
        pos_id_utilisateur = 0;
    else if (strcmp(filename, "certificats.txt") == 0)
        pos_id_utilisateur = 4;
    else if (strcmp(filename, "langues.txt") == 0)
        pos_id_utilisateur = 2;

    while (fgets(ligne, sizeof(ligne), original)) {
        char copie[MAX_LINE];
        strcpy(copie, ligne);

        int current_id = -1, current_user = -1;
        char *token, *saveptr;
        int NumeroChamp = 0;

        token = strtok_r(copie, ";", &saveptr);
        while (token != NULL) {
            if (NumeroChamp == pos_id_element)
                current_id = atoi(token);
            if (NumeroChamp == pos_id_utilisateur) {
                current_user = atoi(token);
                break;
            }
            token = strtok_r(NULL, ";", &saveptr); // continue a tokeniser la meme chaine que precedemment
            NumeroChamp++;
        }

        if (!(current_user == id_utilisateur && (id_element == -1 || current_id == id_element))) {
            fputs(ligne, temp);  // enregistrer les lignes non concerner
        } else {
            deleted++;
            printf("Suppression : %s", ligne); //supprimer la ligne concerner
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
        printf("-> %d elements supprimes dans %s\n", deleted, filename);
    } else {
        remove("temp.txt");
        printf("-> Aucun element supprime dans %s\n", filename);
    }

    return deleted;
}

int mettre_a_jour_element(const char* filename, int id_utilisateur, int id_element, const char* nouveaux_donnees) {
    FILE *original = fopen(filename, "r");
    if (!original) {
        printf("Erreur : ouverture %s\n", filename);
        return -1;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(original);
        printf("Erreur : creation fichier temporaire\n");
        return -1;
    }

    char ligne[MAX_LINE];
    int updated = 0;

    int pos_id_utilisateur = 1;
    if (strcmp(filename, "utilisateurs.txt") == 0)
        pos_id_utilisateur = 0;
    else if (strcmp(filename, "certificats.txt") == 0)
        pos_id_utilisateur = 4;
    else if (strcmp(filename, "langues.txt") == 0)
        pos_id_utilisateur = 2;

    int est_utilisateurs = (strcmp(filename, "utilisateurs.txt") == 0);

    while (fgets(ligne, sizeof(ligne), original)) {
        char copie[MAX_LINE];
        strcpy(copie, ligne);

        int current_id = -1, current_user = -1;
        int numeroChamp = 0;
        char *token, *saveptr;

        token = strtok_r(copie, ";", &saveptr);
        while (token) {
            nettoyer_chaine(token);  // Nettoyage complet en une ligne

            if (numeroChamp == 0)
                current_id = atoi(token);
            if (numeroChamp == pos_id_utilisateur)
                current_user = atoi(token);

            token = strtok_r(NULL, ";", &saveptr);
            numeroChamp++;
        }

        if ((est_utilisateurs && current_id == id_element) ||
            (!est_utilisateurs && current_user == id_utilisateur && (id_element == -1 || current_id == id_element))) {

            char buffer[MAX_LINE];
            strncpy(buffer, nouveaux_donnees, MAX_LINE);
            buffer[MAX_LINE - 1] = '\0';
            nettoyer_chaine(buffer);

            fprintf(temp, "%s\n", buffer);
            updated++;
            printf("Mise a jour : %s -> %s\n", ligne, buffer);
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
        printf("-> %d elements mis a jour dans %s\n", updated, filename);
    } else {
        remove("temp.txt");
        printf("-> Aucun element mis a jour dans %s\n", filename);
    }

    return updated;
}