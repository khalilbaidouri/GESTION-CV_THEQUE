#include <stdio.h>
#include "education.h"
#include "experience.h"
#include "utilisateur.h"
#include "certificat.h"
#include "loisir.h"
#include "skill.h"
#include "langue.h"
#include "supplimentaire.h"

int main() {
    dernier_id_education = lire_dernier_id("educations.txt");
    dernier_id_experience = lire_dernier_id("experiences.txt");
    dernier_id_utilisateur = lire_dernier_id("utilisateurs.txt");
    dernier_id_certificat = lire_dernier_id("certificats.txt");
    dernier_id_loisir = lire_dernier_id("loisirs.txt");
    dernier_id_skill = lire_dernier_id("skills.txt");
    dernier_id_langue = lire_dernier_id("langues.txt");

    Education* educations = NULL;
    Experience* experiences = NULL;
    Utilisateur* utilisateurs = NULL;
    Certificat* certificats = NULL;
    Loisir* loisirs = NULL;
    Skill* skills = NULL;
    Langue* langues = NULL;

    char choix;
    int id_utilisateur_courant = 0;

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gestion des utilisateurs\n");
        printf("2. Gestion des educations\n");
        printf("3. Gestion des experiences\n");
        printf("4. Gestion des certificats\n");
        printf("5. Gestion des loisirs\n");
        printf("6. Gestion des skills\n");
        printf("7. Gestion des langues\n");
        printf("8. Afficher mon CV\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        while (getchar() != '\n');

        switch (choix) {
            case '1':
                gestion_utilisateurs(&utilisateurs, &id_utilisateur_courant);
                break;

            case '2':
                if (id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord\n");
                } else {
                    gestion_educations(&educations, id_utilisateur_courant);
                }
            break;

            case '3':
                if (id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord\n");
                } else {
                    gestion_experiences(&experiences, id_utilisateur_courant);
                }
            break;

            case '4':
                if (id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord\n");
                } else {
                    gestion_certificats(&certificats, id_utilisateur_courant);
                }
            break;


            case '5':

                if (id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord\n");
                } else {
                    gestion_loisirs(&loisirs, id_utilisateur_courant);
                }
            break;

            case '6':

                if (id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord\n");
                } else {
                    gestion_skills(&skills, id_utilisateur_courant);
                }
            break;

            case '7':
                if (id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord\n");
                } else {
                    gestion_langues(&langues, id_utilisateur_courant);
                }
            break;

            case '8':
                if (id_utilisateur_courant == 0) {
                    printf("Veuillez vous connecter d'abord\n");
                } else {
                    afficher_infos_utilisateur(id_utilisateur_courant);
                }
            break;
            case '0':
                printf("Au revoir!\n");
                break;

            default:
                printf("Choix invalide!\n");
        }

    } while (choix != '0');

    liberer_educations(educations);
    liberer_experiences(experiences);
    liberer_utilisateurs(utilisateurs);
    liberer_certificats(certificats);
    liberer_loisirs(loisirs);
    liberer_skills(skills);
    //liberer_langues(langues);
    //liberer_profils_motivation(profilsMotivation);

    return 0;
}