cmake_minimum_required(VERSION 3.22)
project(TI_301_PRJ_STUDENTS_master C)
set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PRJ_STUDENTS_master
        main.c
        graph.c
)

# (optionnel) si tu as un fichier de données à copier vers le dossier de build :
# file(COPY ${CMAKE_SOURCE_DIR}/data/exemple1.txt DESTINATION ${CMAKE_BINARY_DIR}/data)
