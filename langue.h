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
// Validation
bool valider_langue(char* langue);

// Vérification existence
bool langue_existe(const char* fichier, int id_utilisateur, const char* langue);

// Ajout + sauvegarde dans fichier (paramètre tete ignoré)
void ajouter_et_sauvegarder_langue(Langue** tete, const char* fichier, int id_utilisateur, char* langue);

// Affichage des langues d'un utilisateur
void afficher_langues_par_utilisateur(const char* fichier, int id_utilisateur);

// Suppression d'une langue (par id)
int supprimer_langues_utilisateur(int id_utilisateur, int id_langue);

// Mise à jour d'une langue
int mettre_a_jour_langue(int id_utilisateur, int id_langue, const char* langue);

// Menu de gestion des langues (paramètre tete ignoré)
void gestion_langues(Langue** langues, int id_utilisateur);


#endif // LANGUE_H
