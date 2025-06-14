#ifndef PROFIL_MOTIVATION_H
#define PROFIL_MOTIVATION_H

#include <stdbool.h>

#define MAX_TEXTE 255

extern int dernier_id_profilmotivation ;

typedef struct {
    int idMotivProfil;
    char profil[MAX_TEXTE];
    char motivation[MAX_TEXTE];
} ProfilMotivation;

void ajouter_et_sauvegarder_profil_motivation(const char* fichier, const char* profil, const char* motivation);
void afficher_profils_motivation(const char* fichier);
int supprimer_profil_motivation(const char* fichier, int idMotivProfil);
int mettre_a_jour_profil_motivation(const char* fichier, int idMotivProfil, const char* nouveau_profil, const char* nouvelle_motivation);
void gestion_profils_motivation(const char* fichier);
bool valider_texte(const char* texte);
bool profil_motivation_existe(const char* fichier, const char* profil, const char* motivation);

#endif
