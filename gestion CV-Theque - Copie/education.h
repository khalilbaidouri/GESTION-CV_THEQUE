#ifndef EDUCATION_H
#define EDUCATION_H

#include <stdbool.h>

typedef struct Education {
    int id;
    int id_utilisateur;
    char diplome[100];
    char institution[100];
    char date_debut[20];
    char date_fin[20];
    struct Education* suivant;
} Education;

extern int dernier_id_education;

void ajouter_et_sauvegarder_education(Education** tete, const char* fichier, int id_utilisateur, char* diplome, char* institution, char* date_debut, char* date_fin);
void afficher_educations_par_utilisateur(int id_utilisateur);
void afficher_educations(const char* fichier, int id_utilisateur);
bool valider_education(char* diplome, char* institution, char* date_debut, char* date_fin);
bool education_existe(const char* fichier, int id_utilisateur, const char* diplome, const char* institution, const char* date_debut, const char* date_fin);
int supprimer_educations_utilisateur(int id_utilisateur, int id_education);
int mettre_a_jour_education(int id_utilisateur, int id_education, const char* diplome, const char* institution, const char* date_debut, const char* date_fin);
void gestion_educations(Education** educations, int id_utilisateur);
void liberer_educations(Education* edu);
#endif