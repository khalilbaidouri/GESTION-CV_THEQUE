#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <stdbool.h>

typedef struct Utilisateur {
    int id;
    char nom[100];
    char prenom[100];
    char email[100];
    char password[100];
    char date_creation[20];
    char role[50];
    struct Utilisateur* suivant;
} Utilisateur;

extern int dernier_id_utilisateur;  // Déclarer comme extern

// Déclarations de fonctions
Utilisateur* creer_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* role);
void ajouter_utilisateur(Utilisateur** tete, Utilisateur* nouvel_utilisateur);
void ajouter_et_sauvegarder_utilisateur(Utilisateur** tete, const char* fichier, const char* nom, const char* prenom, const char* email, const char* password, const char* role);
void afficher_utilisateurs(const Utilisateur* utilisateur);
void liberer_utilisateurs(Utilisateur* utilisateur);
bool utilisateur_existe_par_id(const char* fichier, int id_utilisateur);
bool valider_utilisateur(const char* nom, const char* prenom, const char* email, const char* password, const char* role);
bool utilisateur_existe_par_id(const char* fichier, int id_utilisateur);
int authentifier_utilisateur(const char* email, const char* password);
void gestion_utilisateurs(Utilisateur** utilisateurs, int* id_utilisateur_courant);
int supprimer_utilisateur_complet(int id_utilisateur);
bool utilisateur_existe(const char* fichier, const char* email);
// Vérifie si un utilisateur est admin
bool est_admin(int id_utilisateur);
int mettre_a_jour_utilisateur(int id_utilisateur, const char* nom, const char* prenom, const char* email, const char* password, const char* role);

// Fonction d'authentification


// Déclarations des fonctions de suppression
// Ajoutez ces déclarations si elles n'existent pas
void supprimer_toutes_educations_utilisateur(int id_utilisateur);
void supprimer_toutes_experiences_utilisateur(int id_utilisateur);
void supprimer_tous_certificats_utilisateur(int id_utilisateur);
void supprimer_tous_skills_utilisateur(int id_utilisateur);
void supprimer_toutes_langues_utilisateur(int id_utilisateur);
void supprimer_tous_loisirs_utilisateur(int id_utilisateur);
void supprimer_tous_cvs_utilisateur(int id_utilisateur);
#endif // UTILISATEUR_H
