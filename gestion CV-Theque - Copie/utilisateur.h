// utilisateur.h
#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <stdbool.h>

typedef struct Utilisateur {
    int id;
    char nom[100];
    char prenom[100];
    char email[100];
    char password[100];
    char telephone[20];      // Nouveau champ téléphone
    char date_creation[20];
    char role[50];
    struct Utilisateur* suivant;
} Utilisateur;


extern int dernier_id_utilisateur;  // D�clarer comme extern

// Déclarations des fonctions
Utilisateur* creer_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* telephone);
void ajouter_utilisateur(Utilisateur** tete, Utilisateur* nouvel_utilisateur);
void ajouter_et_sauvegarder_utilisateur(Utilisateur** tete, const char* fichier, const char* nom, const char* prenom, const char* email, const char* password, const char* telephone);
void afficher_utilisateurs(const Utilisateur* utilisateur);
void liberer_utilisateurs(Utilisateur* utilisateur);
bool utilisateur_existe_par_id(const char* fichier, int id_utilisateur);
bool valider_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* telephone);
bool utilisateur_existe(const char* fichier, const char* email);
int authentifier_utilisateur(const char* email, const char* password);
void gestion_utilisateurs(Utilisateur** utilisateurs, int* id_utilisateur_courant);
bool est_admin(int id_utilisateur);
int supprimer_utilisateur_complet(int id_utilisateur);
int mettre_a_jour_utilisateur(int id_utilisateur, const char* nom, const char* prenom, const char* email, const char* password, const char* telephone);
void afficher_infos_utilisateur(int id_utilisateur);
void afficher_utilisateur(const char* nomFichier, int idRecherche);

#endif