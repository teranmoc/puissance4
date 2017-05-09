Puissance4: main.o jeu.o joueur.o fichier.o
	g++ main.o jeu.o joueur.o fichier.o -o Puissance4 

all: main.o jeu.o joueur.o options.o fichier.o
	@echo "Génère seulement les .o"

main.o: main.cpp jeu.h
	g++ -c main.cpp

jeu.o: jeu.cpp jeu.h
	g++ -c jeu.cpp

joueur.o: joueur.cpp joueur.h
	g++ -c joueur.cpp
	

fichier.o: fichier.cpp fichier.h
	g++ -c fichier.cpp

clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.o
	rm -f *.txt~
