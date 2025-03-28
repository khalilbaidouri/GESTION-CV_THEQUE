#ifndef LANGUE_H
#define LANGUE_H

#include <stdbool.h>

typedef struct Langue {
    int id_langue;
    char langue[100];
    int id_utilisateur;
    struct Langue* suivant;
} Langue;

extern int dernier_id_langue;

// Prototypes des fonctions
Langue* creer_langue(int id_utilisateur, const char* langue);
void ajouter_langue(Langue** tete, Langue* nouvelle);
void ajouter_et_sauvegarder_langue(Langue** tete, const char* fichier, int id_utilisateur,  char* langue);
void afficher_langues(const Langue* l);
void afficher_langues_par_utilisateur(const char* fichier, int id_utilisateur);
bool valider_langue( char* langue);
bool langue_existe(const char* fichier, int id_utilisateur, const char* langue);
void gestion_langues(Langue** langues, int id_utilisateur);
void liberer_langues(Langue* l);
int supprimer_langues_utilisateur(int id_utilisateur, int id_langue);
int mettre_a_jour_langue(int id_utilisateur, int id_langue, const char* langue);

#endif // LANGUE_H
