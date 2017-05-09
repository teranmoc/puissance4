/* Projet Puissance 4 évolué
Auteur : Jérémie Décome
L2 INFORMATIQUE TD1 - TP2
Mise à jour : 27 mai 2014
Fichier : jeu.h
Déclarations des méthodes de la classe Jeu */

#ifndef __JEU__
#define __JEU__

//Librairies
#include <algorithm>
#include <string>

// Classes
#include "joueur.h"
#include "fichier.h"

// Autres
#include "commun.h"

/* Caractères d'affichage des pions dans la grille
Correspondance :
Caractère | Valeur associée dans la grille | Joueur
   VIDE   |               0 	           |   X
   ROND   |               1			       |   J1
   CROIX  |           	  2			       |   J2
*/
enum Caractere{ 
	VIDE, ROND, CROIX
};

class Jeu
{
	int colonnes, lignes; // taille de la grille
	int nLignes, mPions; // conditions de victoire
	int** grille; // grille alloué dynamiquement via un tableau à 2 dimensions
	Joueur joueur1, joueur2; // les deux joueurs  de type Joueur
	bool rotation, jeuRotation, suppressionPion; // options de jeu
	bool partieGagner; // partie gagnée (true) ou non (false)
	Fichier file; // objet fichier pour gérer la partie et les options
private:
	int** creerGrille(const int, const int); // initialise une grille (utiliser lors de la création du jeu et de la rotation)
	void detruireGrille(int**, int); // détruit une grille
	void quitter(bool, bool, int); // enregistre la partie et quitte le jeu
	void afficherScore();
	int placeLibre(int); // retourne la place de la première place libre
	int insererPion(int, int);
	bool supprimerPion(int, int, int); // supprimer le pion indiqué via sa position en paramètre et selon le joueur
	bool rotationGrille(int); // tourne la grille dans un sens ou dans l'autre (90° ou -90° )
	void gravite(); // fait descendre les pions de la grille par gravité
	void modifierOptions(int);
	bool verifLignes(int, int, int); // vérification par ligne
	bool verifColonnes(int, int, int); // vérification par colonne
	int parcoursMin(int, int, bool, Joueur, int);
	int parcoursMax(int, int, bool, Joueur, int);
	int evaluation(int, int); // évalue la grille
	bool possible(int); // évalue si le jeu est possible dans la colonne i
	bool grillePleine(); // indique si la grille est pleine ou non
	int calculPoids(int); // calcule le poids des alignements pour le joueur en paramètre (id)
	// Getteurs privés
	Joueur getInverse(Joueur); // retourne le joueur adverse du paramètre
	int getInverse(int); // retourne l'id de l'adversaire (surcharge)
public:
	Jeu(Joueur, Joueur, bool); // bool : partie sauvegardée, chargement de la grille
	void affichage(); // affiche la grille
	void afficheMenu(); // affiche le menu en fonction des options définies
	void afficherOptions(); // affiche les options du jeu
	bool choixMenu(int, Joueur); // appele les méthodes privées en fonction du choix du joueur (accessible par le(s) joueur(s) réels)
	bool definirGagnant(Joueur); // défini le gagnant selon les options et le jeu
	bool jeuOrdi(Joueur); // fait jouer l'ordinateur
	// Setteurs publiques
	void setGagner(bool); // indique l'état de la partie
	// Getteurs publiques
	Joueur getJoueur(int);
	std::string getPion(Joueur); // retourne le pion associé au joueur (pour affichage uniquement)
	std::string getPion(int);    // retourne le pion associé à l'identifiant du joueur (surcharge)
	bool getGagner();  // retourne la valeur de partieGagner
	~Jeu();
};

#endif
