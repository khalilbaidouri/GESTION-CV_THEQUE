#ifndef SKILL_H
#define SKILL_H

typedef struct Skill {
    int id_skill;
    int id_utilisateur;
    char skill_name[255];
    struct Skill* suivant;
} Skill;

extern int dernier_id_skill;
void ajouter_et_sauvegarder_skill(const char* fichier, int id_utilisateur, const char* skill_name);
void gestion_skills(Skill** skills, int id_utilisateur);
void afficher_skills_par_utilisateur(int id_utilisateur);
int supprimer_skills_utilisateur(int id_utilisateur, int id_skill);
int skill_existe(const char* fichier, int id_utilisateur, const char* skill_name);
Skill* creer_skill(int id_utilisateur, const char* skill_name);
void ajouter_skill(Skill** tete, Skill* nouveau_skill);
void liberer_skills(Skill* skill);
int mettre_a_jour_skill(int id_utilisateur, int id_skill, const char* skill_name);
void afficher_skills(const char* fichier, int id_utilisateur);
#endif
