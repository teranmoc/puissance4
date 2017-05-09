/* Projet Puissance 4 évolué
Auteur : Jérémie Décome
L2 INFORMATIQUE TD1 - TP2
Mise à jour : 14 mai 2014
Fichier : joueur.h
Déclarations des méthodes de la classe Joueur */

#ifndef __JOUEUR__
#define __JOUEUR__

#include "commun.h"

class Joueur
{
	static int nbre; // nombre de joueurs
	int score; // score de la partie en cours
	int id; // identifiant du joueur
	int niveau; // niveau de l'ordinateur
	std::string nom; // nom du joueur
	bool bot; // si c'est un bot (true) ou non (false)
public:
	Joueur(); // construit un joueur neutre (constructeur par défaut)
	Joueur(std::string, bool, int); // construit un vrai joueur (surcharge du constructeur par défaut)
	void echange(Joueur &); // switche du joueur actuel
	bool estIA(); // indique si c'est l'ordinateur (true) ou un joueur (false)
	// Setteurs
	void setScore(int);
	void setNom(std::string);
	// Getteurs
	int getScore();
	std::string getNom();
	int getId();
	int getLevel();
	~Joueur();
};

#endif
