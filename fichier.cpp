/* Projet Puissance 4 évolué
Auteur : Jérémie Décome
L2 INFORMATIQUE TD1 - TP2
Mise à jour : 28 mai 2014
Fichier : fichier.cpp
Implémentation de la classe Fichier */

#include "fichier.h"

using namespace std;

/* Constructeur :
Lit le fichier de sauvegarde contenant les options de jeu et la partie en cours. 
Extrait les valeurs des options (les définit par défaut si le fichier est corrompu ou absent)
*/
Fichier::Fichier(bool main) // si le constructeur est appelé dans le main (true) ou non
{
	ifstream fichier("save.txt");  // Ouverture d'un fichier en lecture seule
	this->save = false;
	bool sauvegarde = false;
	bool presenceGrille = false; // indique si une grille est présente dans le fichier
	int tailleFichier = 0;
	int l = 0;  // indicateur du nombre de lignes de la grille sauvegardée
	vector<string> grilleTmp(1); // stockage temporaire de la grille (1 ligne minimum)
	if((fichier != NULL) && (fichier)) // le fichier existe et a pu etre ouvert
	{
		string ligne;
		int i = 0; // indicateur de la ligne lue
		while(getline(fichier, ligne)) // lit le fichier ligne par ligne
		{
			string val = "";
			if(i <= 6) // lecture des options
			{
				val = this->lireMot(ligne); // récupère la valeur situé après le égal sur la ligne dans le fichier
				switch(i)
				{
					case 0: // nbre de lignes
					{
						this->lignes = atoi(val.c_str()); // conversion string en const char * pour la fonction int atoi()
						if(this->lignes <= 0)
							this->lignes = 3; // valeur par défaut
					}
						break;
					case 1: // nbre de colonnes
					{
						this->colonnes = atoi(val.c_str()); // idem
						if(this->colonnes <= 0)
							this->colonnes = 4; // valeur par défaut
					}
						break;
					case 2: // rotation
						this->rotation = this->stringToBool(val);
						break;
					case 3: // jeu après une rotation
						this->jeuRotation = this->stringToBool(val);
						break;
					case 4: // suppression d'un pion
						this->suppressionPion = this->stringToBool(val);
						break;
					case 5: // nbre de rangées (ligne / colonne / diagonale) pour gagner
					{
						this->nLignes = atoi(val.c_str());
						if(this->nLignes <= 0)
							this->nLignes = 1; // valeur par défaut
					}	
						break;
					case 6: // nbre de pions pour gagner
					{
						this->mPions = atoi(val.c_str());
						if(this->mPions <= 0)
							this->mPions = 4; // valeur par défaut
					}
						break;
					default: // valeurs par défaut
						break;
				}
			}
			else // lecture de la sauvegarde de jeu
			{
				if(i == 7)
					this->nom1 = this->lireMot(ligne); // nom du premier joueur
				else if(i == 8)
					this->bot1 = this->stringToBool(this->lireMot(ligne)); // type du premier joueur
				else if(i == 9) // niveau du premier joueur
				{
					string valeur = this->lireMot(ligne);
					this->lvl1 = atoi(valeur.c_str());
				}
				else if(i == 10) // nom du second joueur
					this->nom2 = this->lireMot(ligne);
				else if(i == 11) // type du second joueur
					this->bot2 = this->stringToBool(this->lireMot(ligne));
				else if(i == 12) // niveau du second joueur
				{
					string valeur = this->lireMot(ligne);
					this->lvl2 = atoi(valeur.c_str());
				}
				else if(i == 13) // identifiant du joueur ayant "quitté" la précédente partie
				{
					string valeur = this->lireMot(ligne);
					this->joueurEnCours = atoi(valeur.c_str());
				}
				else // la grille sera lu plus loin
				{
					presenceGrille = true;
					grilleTmp.push_back(ligne); // stockage des valeurs (chaque ligne) temporairement dans un vecteur
					l++; // compteur de lignes de la grille
				}
				this->save = true;
				
			}
			this->ouvert = true; // le fichier a bien été lu
			i++; // incrémente la ligne actuelle
			tailleFichier = i; // sauvegarde de la taille
		}
		fichier.close(); // toutes les infos ont été lu, le fichier est fermé
	}
	else // le fichier n'a pas pu etre ouvert
	{
		cout<<"lecture impossible"<<endl;
		this->ouvert = false;
		this->save = false;
		// valeurs par défaut :
		this->lignes = 6;
		this->colonnes = 7;
		this->nLignes = 1;
		this->mPions = 4;
		this->rotation = false;
		this->jeuRotation = false;
		this->suppressionPion = true;
	}
	this->grille = this->creerGrille(this->colonnes, this->lignes); // création de la grille
	if((presenceGrille) && (tailleFichier > 14)) // chargement de la grille sauvegardée
	{
		if(!main) // l'objet Fichier dans le main ne doit pas créer la grille
		{
			string ligne, V; // stockage temporaire
			int valeur, colonne;
			for(int i = 1; i < (l + 1); i++)
			{
				ligne = grilleTmp[i]; // lecture de la ligne courante
				for(int j = 0; j < ligne.size(); j += 2) // lecture de la ligne i par une boucle
				{
					V = ligne[j]; // valeur stockée temporairement dans un string (erreur sinon)
					valeur = atoi(V.c_str());
					colonne = j / 2; // calcul de l'index pour la grille
					this->grille[i - 1][colonne] = valeur; // i décalé pour lire les valeurs dans le tableau
				}
			}
		}
	}	
}
// Méthodes privées
string Fichier::lireMot(string ligne) // lit le paramètre de la ligne
{
    string valeur = "";
	int posEgal;
	for(int i = 0; i < ligne.size(); i++)
    {
		if(ligne[i] == '=')
		{
			posEgal = i; // sauvegarde de la position du signe =
			break;
		}
	}
	int j = posEgal + 1;
	while(ligne[j] != '\0')
	{
		valeur += ligne[j]; // concatene les valeurs
		j++;
	}
	return valeur;
}
bool Fichier::stringToBool(string valeur) // converti la valeur dans le fichier en booléen
{
	bool val;
	if(valeur.size() >= 2)
		valeur = valeur[0]; // on ne garde que le premier caractère
	if((valeur == "V") || (valeur == "O") || (valeur == "T")) // reconnais V, O ou T pour la valeur true
		val = true;
	else
		val = false;
	return val;
}
int** Fichier::creerGrille(const int col, const int ligne) // construit un tableau dynamiquement avec les paramètres
{
	int **tableau;
	try // Méthode dynamique
	{
		int** tableau = new int* [ligne];
		for(int i = 0; i < ligne; i++)
			tableau[i] = new int [col];
		for(int i = 0; i < ligne; i++) // initialisation à 0 du tableau
		{
			for(int j = 0; j < col; j++)
				tableau[i][j] = 0;
		}
		return tableau;

	}
	catch (const std::bad_alloc &) // erreur d'allocation
	{
		// désallouer tout ce qui a été alloué avec succès
		cout<<"Erreur d'allocation"<<endl;
		for(int i = 0; i < col; i++ )
			delete []tableau[i];
		delete [] tableau;
		exit(4); // arret du programme
	}
}
void Fichier::detruireGrille(int ** grille, int col)
{
	for(int i = 0; i < col; i++) 
		delete [] grille[i];
	delete [] grille;
}
// Méthodes publiques
bool Fichier::enregistrement(bool gagner, int joueurActuel)
{
	bool rec = false; // état de l'enregistrement
	ofstream fichier("save.txt", ios::out | ios::trunc); // on efface le fichier et on le recrée
	if((fichier != NULL) && (fichier)) // le fichier existe et a bien été ouvert
	{
		fichier<<"lignes="<<this->lignes<<endl; // enregistrement du nombre de lignes
		fichier<<"colonnes="<<this->colonnes<<endl; // enregistrement du nombre de colonnes
		if(this->rotation)// enregistrement de l'option rotation
			fichier<<"rotation=O"<<endl;
		else
			fichier<<"rotation=N"<<endl;
		if(this->jeuRotation) // enregistrement de l'option jeu après rotation
			fichier<<"jeuRotation=O"<<endl;
		else
			fichier<<"jeuRotation=N"<<endl;
		if(this->suppressionPion) // enregistrement de l'option suppression pion
			fichier<<"suppressionPion=O"<<endl;
		else
			fichier<<"suppressionPion=N"<<endl;
		fichier<<"nLignes="<<this->nLignes<<endl;// enregistrement nombre de lignes victoire
		fichier<<"mPions="<<this->mPions<<endl;  // enregistrement nombre de pions victoire
		rec = true; // l'enregistrement a réussi
		if(!gagner) // enregistrement de la partie si non gagné
		{
			string choix = "";
			cout<<"Voulez vous sauvegarder la partie (0) oui - (N) non ? ";
			cin>>choix;
			if((choix == "O") || (choix == "o") || (choix == "0"))
			{
				fichier<<"joueur1="<<this->nom1<<endl;
				if(this->bot1) // si le joueur 1 est une IA
					fichier<<"IA1=O"<<endl;
				else
					fichier<<"IA1=N"<<endl;
				fichier<<"lvl1="<<this->lvl1<<endl;
				fichier<<"joueur2="<<this->nom2<<endl;
				if(this->bot2) // si le joueur 2 est une IA
					fichier<<"IA2=O"<<endl;
				else
					fichier<<"IA2=N"<<endl;
				fichier<<"lvl2="<<this->lvl2<<endl;
				fichier<<"actuel="<<joueurActuel<<endl;
				for(int i = 0; i < this->lignes; i++)
				{
					for(int j = 0; j < this->colonnes; j++)
						fichier<<this->grille[i][j]<<" ";
					fichier<<endl;
				}
				cout<<"La sauvegarde a réussi !"<<endl;
			}
			else
				cout<<"La partie n'a pas été sauvegardée, seules les options l'ont été"<<endl;
		}
		fichier.close(); // fermeture du fichier
	}
	else
		cout<<"Erreur dans l'ouverture du fichier, votre partie et options de jeu n'ont pas été sauvegardés"<<endl;
	return rec;
}
// Getteurs
int **Fichier::getGrille()
{
	return this->grille;
}
bool Fichier::lireParametre(string parametre)
{
	if(parametre == "rotation")
		return this->rotation;
	else if(parametre == "jeuRotation")
		return this->jeuRotation;
	else if(parametre == "suppressionPion")
		return this->suppressionPion;
	else
		return NULL;
}
int Fichier::lireTaille(string parametre)
{
	if(parametre == "lignes")
		return this->lignes;
	else if(parametre == "colonnes")
		return this->colonnes;
	else
		cout<<"erreur"<<endl;
}
int Fichier::lireVictoire(string parametre)
{
	if(parametre == "lignes")
		return this->nLignes;
	else if(parametre == "pions")
		return this->mPions;
	else
		cout<<"Paramètre invalide"<<endl;
}
bool Fichier::getOuverture()
{
	return this->ouvert;
}
int Fichier::getJoueurDebut()
{
	return this->joueurEnCours;
}
string Fichier::getNom(int id)
{
	if(id == 1)
		return this->nom1;
	else if(id == 2)
		return this->nom2;
	else
		return " ";
}
bool Fichier::getBot(int id)
{
	if(id == 1)
		return this->bot1;
	else if(id == 2)
		return this->bot2;
	else
		return NULL;
}
int Fichier::getLevel(int id)
{
	if(id == 1)
		return this->lvl1;
	else if(id == 2)
		return this->lvl2;
	else
		return -4;
}
bool Fichier::getSauvegarde()
{
	return this->save;
}
// Setteurs
void Fichier::setTaille(int colonnes, int lignes)
{
	this->lignes = lignes;
	this->colonnes = colonnes;
	this->enregistrement(true, 0); // enregistre le changement
}
void Fichier::setParametre(string parametre, bool valeur)
{
	if(parametre == "rotation")
		this->rotation = valeur;
	else if(parametre == "jeuRotation")
		this->jeuRotation = valeur;
	else if(parametre == "suppressionPion")
		this->suppressionPion = valeur;
	else
		cout<<"Parametre inconnu"<<endl;
	this->enregistrement(true, 0); // enregistre le changement
}
void Fichier::setVictoire(int nbLignes, int nbPions)
{
	this->nLignes = nbLignes;
	this->mPions = nbPions;
	this->enregistrement(true, 0); // enregistre le changement
}
void Fichier::setJoueur(int id, string nom, bool bot, int lvl)
{
	if(id == 1)
	{
		this->id1 = id;
		this->nom1 = nom;
		this->bot1 = bot;
		this->lvl1 = lvl;
	}
	else if(id == 2)
	{
		this->id2 = id;
		this->nom2 = nom;
		this->bot2 = bot;
		this->lvl2 = lvl;
	}
	else
		cout<<"erreur"<<endl;
}
void Fichier::setGrille(int **grilleJeu, int colonne, int ligne)
{
	this->setTaille(colonne, ligne);
	this->grille = this->creerGrille(this->colonnes, this->lignes); // réajuste la grille à la bonne taille
	for(int i = 0; i < this->lignes; i++)
	{
		for(int j = 0; j < this->colonnes; j++)
			this->grille[i][j] = grilleJeu[i][j];
	}
}
// Destructeur
Fichier::~Fichier()
{
}
