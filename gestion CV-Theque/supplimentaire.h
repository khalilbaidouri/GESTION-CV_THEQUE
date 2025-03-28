#ifndef SUPPLIMENTAIRE_H
#define SUPPLIMENTAIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE 1024

// Déclarations des fonctions existantes
int est_date_valide(const char* date);
int comparer_dates(const char* date1, const char* date2);
int valider_periode(const char* date_debut, const char* date_fin);
void nettoyer_chaine(char* chaine);
int lire_dernier_id(const char* fichier);

// Nouvelles fonctions
bool verifier_password(const char* password);
bool verifier_email(const char* email);
char* saisir_chaine();
void convertir_en_minuscules(char* chaine);
// Dans supplimentaire.h
int supprimer_element_par_ids(const char* filename, int id_utilisateur, int id_element);
int mettre_a_jour_element(const char* filename, int id_utilisateur, int id_element, const char* nouveaux_donnees);

// Prototypes des fonctions de gestion




#endif // SUPPLIMENTAIRE_H
