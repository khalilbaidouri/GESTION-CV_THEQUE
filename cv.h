#ifndef CV_H
#define CV_H

#include <stdbool.h>

typedef struct CV {
    int id_cv;
    int id_utilisateur;
    char photo_url[500];
    char titre[255];
    struct CV* suivant;
} CV;

extern int dernier_id_cv;
// Prototypes des fonctions
CV* creer_cv(int id_utilisateur, const char* photo_url, const char* titre);
void ajouter_cv(CV** tete, CV* nouveau);
void ajouter_et_sauvegarder_cv(CV** tete, const char* fichier, int id_utilisateur, const char* photo_url,  char* titre);
void afficher_cvs_par_utilisateur(const char* fichier, int id_utilisateur);
void liberer_cvs(CV* cv);
bool valider_cv(const char* photo_url, const char* titre);
bool cv_existe(const char* fichier, int id_utilisateur, const char* titre);
void gestion_cvs(CV** cvs, int id_utilisateur);
int supprimer_cvs_utilisateur(int id_utilisateur, int id_cv);
int mettre_a_jour_cv(int id_utilisateur, int id_cv,  const char* photo_url, const char* titre);
void afficher_cv_complet(int id_utilisateur, int id_cv);

#endif
