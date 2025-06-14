cmake_minimum_required(VERSION 3.20)
project(CV_Theque C)

# Options de compilation
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

# Liste des fichiers sources .c
file(GLOB SOURCES
        "*.c"
        "main.c"
        "certificat.c"
        "education.c"
        "experience.c"
        "langue.c"
        "loisir.c"
        "skill.c"
        "supplimentaire.c"
        "utilisateur.c"
)

# Ajout de l'exécutable
add_executable(cv_theque ${SOURCES})

# Copie des fichiers .txt dans le dossier de build
file(COPY
        "certificats.txt"
        "educations.txt"
        "experiences.txt"
        "langues.txt"
        "loisirs.txt"
        "skills.txt"
        "utilisateurs.txt"
        DESTINATION ${CMAKE_BINARY_DIR}
)
