# nom ......... : Makefile
# rôle........ : Compiler le projet et lancer l'exécutable SOM
# auteur ...... : Avrile Floro
# version ..... : v1 du 27/09/2024
# licence ..... : réalisé dans le cadre du cours d'IA
# usage : 		   pour compiler et exécuter : make
#				   pour nettoyer : make clean

all: compile run

compile:
	gcc -Wall -Wextra main.c utils.c resultats.c apprentissage_SOM.c neural_net.c dataset.c -o SOM -lm

run:
	./SOM

error:
	-./SOM arg_inutile

clean:
	rm -f SOM
