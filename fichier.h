/* Projet Puissance 4 évolué
Auteur : Jérémie Décome
L2 INFORMATIQUE TD1 - TP2
Mise à jour : 27 mai 2014
Fichier : fichier.h
Déclarations des méthodes de la classe Fichier */

#ifndef __FICHIER__
#define __FICHIER__

// Librairies
#include <fstream> // permet de gérer les fichiers
#include <vector>
#include <stdlib.h> // pour la fonction atoi

// Classes

// Autres
#include "commun.h"


class Fichier
{
	int lignes; // nbre de lignes de la grille
	int colonnes; // nbre de colonnes de la grille
	bool rotation, jeuRotation, suppressionPion; // options de jeu
	int nLignes; // nbre de lignes / colonnes / diagonales gagnantes
	int mPions; // nombre de pions pour gagner
	bool ouvert; // signale que le fichier a bien été ouvert (et donc la lecture des infos a pu etre faite)
	bool save; // indique si une partie a déja été sauvegardé
	int ** grille; // sauvegarde de la grille de jeu
	int id1, id2; // identifiant des joueurs
	std::string nom1, nom2;
	bool bot1, bot2;
	int lvl1, lvl2; // niveau de jeu des deux joueurs (pour l'IA)
	int joueurEnCours; // joueur en cours de la partie dans le fichier
private:
	std::string lireMot(std::string); // retourne la valeur après le signe = de la ligne lue dans le fichier
	bool stringToBool(std::string); // converti valeur string en bool
	int ** creerGrille(int, int); // crée une grille
	void detruireGrille(int **, int); // détruit la grille
public:
	Fichier(bool);
	bool enregistrement(bool, int); // fonction d'enregistrement des paramètres à l'arret du programme et la partie si non gagné (false)
	int lireTaille(std::string); // retourne le nbre de lignes ou le nbre de colonnes
	bool lireParametre(std::string); // lit le paramètre passé en paramètre
	int lireVictoire(std::string); // retourne la condition de victoire passée en paramètre (nbre de lignes ou nbre de pions)
	// Getteurs
	bool getOuverture(); // retourne l'état de l'ouverture du fichier
	bool getSauvegarde(); // indique si une partie a déja été sauvegardée
	int getJoueurDebut(); // retourne le joueur qui jouais lors de la sauvegarde
	std::string getNom(int); // retourne le nom du joueur en paramètre
	bool getBot(int); // indique si le joueur en parametre est l'ordi ou non
	int getLevel(int); // retourne le niveau de jeu du joueur en paramètre (pour l'IA)
	int **getGrille(); // retourne la grille du jeu sauvegardée
	// Setteurs
	void setTaille(int, int); // définit la nouvelle taille de la grille
	void setVictoire(int, int); // définit les nouvelles options de victoire
	void setParametre(std::string, bool); // définit la nouvelle valeur de l'option (bool) passée en parametre (string)
	void setJoueur(int, std::string, bool, int); // définit le nom du joueur passé en paramètre
	void setGrille(int **, int, int);
	~Fichier();
};

#endif
