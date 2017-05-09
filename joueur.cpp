/* Projet Puissance 4 évolué
Auteur : Jérémie Décome
L2 INFORMATIQUE TD1 - TP2
Mise à jour : 14 mai 2014
Fichier : joueur.cpp
Implémentation de la classe Joueur */

#include "joueur.h"

using namespace std;

int Joueur::nbre = 0; // initialisation du compteur de joueur

// Constructeurs
Joueur::Joueur() // joueur neutre
{
	this->score = 0;
	this->nom = "";
	this->bot = false;
	this->id = 0; // pas d'id pour le joueur neutre
	this->niveau = -1;
}
Joueur::Joueur(string nom, bool ia, int niveau) // nouveau joueur
{
	nbre++; // ajout d'un joueur
	this->score = 0;
	this->nom = nom;
	this->bot = ia;
	this->id = nbre;
	if(niveau > LEVEL_MAX) // si il dépasse le niveau max
		niveau = LEVEL_MAX;
	this->niveau = niveau; // niveau de l'ordinateur
	cout<<"Joueur "<<this->id<<" :"<<endl;
	cout<<"    Nom : "<<this->nom<<" - Type : ";
	if(this->bot)
		cout<<"Ordinateur ("<<this->niveau<<")"<<endl;
	else
		cout<<"Joueur"<<endl;
	
}
void Joueur::echange(Joueur &j)
{
	if(this->id != j.id) // joueur différent de l'actuel ?
	{
		this->nom = j.nom;
		this->score = j.score;
		this->bot = j.bot;
		this->id = j.id;
		this->niveau = j.niveau;
	}
}
bool Joueur::estIA()
{
	return this->bot;
}
// Setteurs
void Joueur::setScore(int score)
{
	this->score = score;
}
// Getteurs
int Joueur::getScore()
{
	return this->score;
}
string Joueur::getNom()
{
	return this->nom;
}
int Joueur::getId()
{
	return this->id;
}
int Joueur::getLevel()
{
	return this->niveau;
}
// Destructeur
Joueur::~Joueur()
{
}
