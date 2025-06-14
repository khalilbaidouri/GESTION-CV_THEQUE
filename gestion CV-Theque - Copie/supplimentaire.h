#ifndef SUPPLIMENTAIRE_H
#define SUPPLIMENTAIRE_H
#include <stdbool.h>
#define MAX_LINE 1024

int est_date_valide(const char* date);
int comparer_dates(const char* date1, const char* date2);
int valider_periode(const char* date_debut, const char* date_fin);
void nettoyer_chaine(char* chaine);
int lire_dernier_id(const char* fichier);
bool verifier_password(const char* password);
bool verifier_email(const char* email);
char* saisir_chaine();
void convertir_en_minuscules(char* chaine);
bool valider_telephone(const char* telephone);
int supprimer_element_par_ids(const char* filename, int id_utilisateur, int id_element);
int mettre_a_jour_element(const char* filename, int id_utilisateur, int id_element, const char* nouveaux_donnees);





#endif
