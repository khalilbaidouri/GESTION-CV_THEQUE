#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <stdbool.h>

typedef struct Experience {
    int id;
    int id_utilisateur;
    char titre[100];
    char entreprise[100];
    char date_debut[20];
    char date_fin[20];
    struct Experience* suivant;
} Experience;

extern int dernier_id_experience;

void ajouter_et_sauvegarder_experience(Experience** tete, const char* fichier, int id_utilisateur, char* titre, char* entreprise, char* date_debut, char* date_fin);

void afficher_experiences(const char* fichier, int id_utilisateur);

bool valider_experience(char* titre, char* entreprise, char* date_debut, char* date_fin);
bool experience_existe(const char* fichier, int id_utilisateur, const char* titre, const char* entreprise, const char* date_debut, const char* date_fin);

int supprimer_experiences_utilisateur(int id_utilisateur, int id_experience);
int mettre_a_jour_experience(int id_utilisateur, int id_experience, const char* titre, const char* entreprise, const char* date_debut, const char* date_fin);

void gestion_experiences(Experience** experiences, int id_utilisateur);

void liberer_experiences(Experience* exp);
#endif