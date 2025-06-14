#ifndef LOISIR_H
#define LOISIR_H

#include <stdbool.h>

typedef struct Loisir {
    int id_loisir;
    int id_utilisateur;
    char loisir[255];
    struct Loisir* suivant;
} Loisir;

extern int dernier_id_loisir;

void ajouter_et_sauvegarder_loisir(Loisir** tete, const char* fichier, int id_utilisateur, char* loisir);
void afficher_loisirs_par_utilisateur(const char* fichier, int id_utilisateur);
void liberer_loisirs(Loisir* l);
bool valider_loisir( char* loisir);
bool loisir_existe(const char* fichier, int id_utilisateur, const char* loisir);
void gestion_loisirs(Loisir** loisirs, int id_utilisateur);
int supprimer_loisirs_utilisateur(int id_utilisateur, int id_loisir);
int mettre_a_jour_loisir(int id_utilisateur, int id_loisir, const char* nouveau_nom);
#endif
