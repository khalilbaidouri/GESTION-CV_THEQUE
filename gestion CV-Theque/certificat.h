#ifndef CERTIFICAT_H
#define CERTIFICAT_H

#include <stdbool.h>

typedef struct Certificat {
    int id_certificat;
    char nom_certificat[255];
    char organisme[255];
    char date_obtention[20]; // Format YYYY-MM-DD
    int id_utilisateur;
    struct Certificat* suivant;
} Certificat;

// Déclaration du compteur d'ID
extern int dernier_id_certificat;

// Prototypes des fonctions
Certificat* creer_certificat(const char* nom, const char* organisme, const char* date, int id_utilisateur);
void ajouter_certificat(Certificat** tete, Certificat* nouveau);
void ajouter_et_sauvegarder_certificat(Certificat** tete, const char* fichier,  char* nom,  char* organisme,  char* date, int id_utilisateur);
void afficher_certificats_par_utilisateur(const char* fichier, int id_utilisateur);
void liberer_certificats(Certificat* cert);
bool valider_certificat( char* nom,  char* organisme,  char* date);
bool certificat_existe(const char* fichier, const char* nom, const char* organisme, const char* date, int id_utilisateur);
void gestion_certificats(Certificat** certificats, int id_utilisateur);
int supprimer_certificats_utilisateur(int id_utilisateur, int id_certificat);
int mettre_a_jour_certificat(int id_utilisateur, int id_certificat,  const char* nom, const char* organisme,const char* date_obtention);

#endif // CERTIFICAT_H
