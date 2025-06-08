cmake_minimum_required(VERSION 3.20)
project(CV_Theque C)  # Correction du nom du projet (et orthographe)

# Options de compilation
set(CMAKE_C_STANDARD 11)  # Correction : "CMAKE" (pas "CHAKE")
set(CMAKE_C_FLAGS "-Wall -Wextra")  # Activation des warnings

# Liste des fichiers sources .c
file(GLOB SOURCES
        "*.c"
        "certificat.c"
        "cv.c"
        "education.c"
        "experience.c"
        "langue.c"
        "loisir.c"  # Correction : "loisir.c" (pas "loisin.c")
        "skill.c"
        "supplimentaire.c"  # Attention à l'orthographe (supplémentaire ?)
        "utilisateur.c"
)

# Création de l'exécutable
add_executable(cv_theque ${SOURCES}
        ProfilMotivation.h)  # Correction : ${SOURCES} entre accolades

# Copie des fichiers .txt dans le dossier de build
file(COPY
        "certificats.txt"
        "cvs.txt"
        "educations.txt"
        "experiences.txt"
        "langues.txt"
        "loisirs.txt"
        "skills.txt"
        "utilisateurs.txt"
        "langues.txt"
        DESTINATION ${CMAKE_BINARY_DIR}
)