/* Projet Puissance 4 évolué
Auteur : Jérémie Décome
L2 INFORMATIQUE TD1 - TP2
Mise à jour : 28 mai 2014
Fichier : jeu.cpp
Implémentation de la classe Jeu */

#include "jeu.h"

using namespace std;

// Constructeur
Jeu::Jeu(Joueur j1, Joueur j2, bool sauvegarde) : file(false) // passage de l'argument à l'objet composé Fichier
{
	this->joueur1 = j1;
	this->joueur2 = j2;
	// initialisation des variables
	this->partieGagner = false;
	// initialisation des options
	this->rotation = file.lireParametre("rotation"); // rotation de la grille autorisé ?
	this->jeuRotation = file.lireParametre("jeuRotation"); // jouer un pion après la rotation autorisé ?
	this->suppressionPion = file.lireParametre("suppressionPion"); //suppression d'un pion autorisé ?
	// initialisation de la grille
	this->colonnes = file.lireTaille("colonnes"); // colonnes de la grille
	this->lignes = file.lireTaille("lignes"); // lignes de la grille
	this->nLignes = file.lireVictoire("lignes"); // nbre rangées pour gagner
	this->mPions = file.lireVictoire("pions"); // nbre de pions par rangées
	file.setJoueur(1, this->joueur1.getNom(), this->joueur1.estIA(), this->joueur1.getLevel()); // définition joueur 1
	file.setJoueur(2, this->joueur2.getNom(), this->joueur2.estIA(), this->joueur2.getLevel()); // définition joueur 2
	if(sauvegarde) // la partie créée est une partie sauvegardée
	{
		cout<<"Chargement de la partie sauvegardée"<<endl;
		this->grille = file.getGrille();
	}
	else
		this->grille = this->creerGrille(this->colonnes, this->lignes); // création de la grille du jeu
}
// Méthodes privées
int** Jeu::creerGrille(const int col, const int ligne)
{
	int **tableau;
	try // Méthode dynamique
	{
		tableau = new int* [ligne];
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
void Jeu::detruireGrille(int ** grille, int col)
{
	for(int i = 0; i < col; i++) 
		delete [] grille[i];
	delete [] grille;
}
void Jeu::afficherScore()
{
	cout<<"Voici les scores actuels :"<<endl;
	cout<<"1. "<<this->joueur1.getNom()<<" a "<<joueur1.getScore()<<" point(s)"<<endl;
	cout<<"2. "<<this->joueur2.getNom()<<" a "<<joueur2.getScore()<<" point(s)"<<endl;
}
void Jeu::afficherOptions()
{
	if(!file.getOuverture()) // défaut d'ouverture du fichier de sauvegarde
	{
		cout<<"---------"<<endl<<"Attention : le fichier n'a pas été trouvé ou n'a pas pu etre ouvert."<<endl;
		cout<<"Valeurs par défaut chargées"<<endl;
	}
	cout<<"Options actuelles :"<<endl;
	cout<<"Taille de la grille :"<<endl;
	cout<<" - colonnes : "<<this->colonnes<<endl;
	cout<<" - lignes : "<<this->lignes<<endl;
	cout<<"Rotation de la grille : ";
	if(this->rotation)
		cout<<"Autorisée"<<endl;
	else
		cout<<"Interdite"<<endl;
	cout<<"Jeu après une rotation : ";
	if(this->jeuRotation)
		cout<<"Autorisé"<<endl;
	else
		cout<<"Interdit"<<endl;
	cout<<"Suppression d'un pion : ";
	if(this->suppressionPion)
		cout<<"Autorisé"<<endl;
	else
		cout<<"Interdite"<<endl;
	cout<<"Pour gagner, il faut "<<this->nLignes<<" ligne(s) de "<<this->mPions<<" chacune."<<endl<<endl;
}
void Jeu::quitter(bool confirmation, bool gagnant, int joueurActuel)
{
	if(confirmation) // confirmation pour le choix 7
	{
		string choix = "";
		cout<<"Voulez vous vraiment quitter le jeu (O) oui - (N) non ? ";
		cin>>choix;
		if((choix == "O") || (choix == "o") || choix == "0")
		{
			file.setGrille(this->grille, this->colonnes, this->lignes); // Sauvegarde de la grille
			if(file.enregistrement(gagnant, joueurActuel)) // enregistre les paramètres et de la partie si non gagnée
			{
				cout<<"Enregistrement réussi"<<endl;
				cout<<"Merci et a bientot !"<<endl;
				exit(0);
			}
		}
		else
			cout<<"Mauvaise manip ?"<<endl;
	}
	else // ferme le programme sans confirmation ni sauvegarde
		exit(0);
}
int Jeu::placeLibre(int colonne)
{
	int position = -1;
	int i = this->lignes; // on commence en bas de la colonne ...
	while(i != 0)
	{
		i--; // ... et on la remonte
		if(grille[i][colonne] == VIDE) // si la case est vide, on retourne sa position et on arrete la boucle
		{
			position = i;
			break;
		}
		else // sinon on poursuit la recherche
			continue;
	}
	return position;
}
bool Jeu::supprimerPion(int colonne, int ligne, int pion)
{
	bool ok = false;
	if(this->suppressionPion) // si la suppression d'un pion est autorisé
	{
		if((colonne > 0) || (ligne > 0)) // si le joueur n'a pas entré 0 (annulation)
		{
			colonne--; // le joueur donne les coordonnées indiqués sur la console, donc il faut les décaler pour la grille
			ligne--; // idem pour la ligne
			if(this->grille[ligne][colonne] == pion)
			{
				this->grille[ligne][colonne] = VIDE;
				ok = true;
			}
			this->gravite();
		}
	}
	return ok;
}
int Jeu::insererPion(int colonne, int id)
{
	int ligne = -2;
	colonne -= 1; // la colonne en paramètre est décalée pour l'affichage pour lire la grille, il faut la remettre au bon endroit
	if((colonne >= 0) && (colonne <= (this->colonnes - 1))) // si la colonne n'est pas hors grille
	{
		ligne = this->placeLibre(colonne); // 1ère ligne libre en partant du bas => axe i
		if(ligne != -1)
		{
			if(id == 1) // joueur 1
				grille[ligne][colonne] = ROND;
			else if(id == 2) // joueur 2
				grille[ligne][colonne] = CROIX;
			else
				grille[ligne][colonne] = VIDE;
		}
	}
	else
		cout<<"Hors cadre pour "<<(colonne + 1)<<", veuillez rejouer"<<endl;
	return ligne;
}
void Jeu::gravite() // permet d'appliquer la gravitée terrestre (g = 9,80 m/s²)
{
	for(int i = 0; i < (this->lignes - 1); i++) // lignes
  	{
		for(int j = 0; j < this->colonnes; j++) // colonnes
		{
			if((grille[i][j] != VIDE) && (grille[i + 1][j] == VIDE)) // si la case actuelle n'est pas vide ET que celle du dessous est vide
			{
				grille[i + 1][j] = grille[i][j]; // permutation
				grille[i][j] = VIDE; // suppression du pion déplacé
				i = 0; // retour au début de la colonne et on recommence !
			}
		}
	}
}
bool Jeu::rotationGrille(int sens)
{
	bool ok = false; // indique si le joueur a bien réalisé une rotation
	if(this->rotation)
	{ // pas de mise en commum d'une partie des algos car il va être possible de faire des rotations à 180 ou 270 °
		if(sens == 90) // rotation à droite
		{
			int colonne = this->lignes;
			int ligne = this->colonnes;
			int **newGrille = this->creerGrille(colonne, ligne);
			for(int i = 0; i < this->lignes; i++)
			{
				int c = (this->lignes - 1) - i; // calcul de l'inverse de i
				for(int j = 0; j < this->colonnes; j++)
					newGrille[j][c] = grille[i][j];
			}
			this->colonnes = colonne;
			this->lignes = ligne;
			this->grille = this->creerGrille(this->colonnes, this->lignes); // Réallocatution de la grille d'origine
			for(int i = 0; i < this->lignes; i++) // Recopie
			{
				for(int j = 0; j < this->colonnes; j++)
					grille[i][j] = newGrille[i][j];
			}
			this->detruireGrille(newGrille, ligne); // destruction de la grille temporaire
			cout<<"La grille a été tournée à droite (+90 °)"<<endl;
			ok = true;
		}
		else if(sens == -90) // rotation à gauche
		{
			int colonne = this->lignes;
			int ligne = this->colonnes;
			int **newGrille = this->creerGrille(colonne, ligne);
			for(int i = 0; i < this->lignes; i++)
			{
				for(int j = 0; j < this->colonnes; j++)
				{
					int l = (this->colonnes -1) - j; // calcul de l'inverse de j
					newGrille[l][i] = grille[i][j];
				}
			}
			this->lignes = ligne;
			this->colonnes = colonne;
			this->grille = this->creerGrille(this->colonnes, this->lignes); // Réallocatution de la grille d'origine
			for(int i = 0; i < this->lignes; i++) // Recopie
			{
				for(int j = 0; j < this->colonnes; j++)
					grille[i][j] = newGrille[i][j];
			}
			this->detruireGrille(newGrille, ligne); // destruction de la grille temporaire
			cout<<"La grille a été tournée à gauche (-90 °)"<<endl;
			ok = true;
		}
		else
			cout<<"Sens non pris en charge"<<endl;
		this->gravite(); // application de la gravité ...
		this->gravite(); // ... encore une fois (elle ne semble pas marcher la première fois qu'elle est appliquée)	
		file.setGrille(this->grille, this->colonnes, this->lignes); // mise à jour du fichier
	}
	else
		cout<<"La rotation n'est pas disponible"<<endl;
	return ok;
}
void Jeu::modifierOptions(int choix)
{
	int col, line;
	if(choix == 1) // modification de la taille de la grille
	{
		cout<<"Modification de la taille de la grille (ne sera pris en compte qu'au prochain lancement)"<<endl;
		cout<<"Nbre de colonnes ("<<this->colonnes<<") ? ";
		cin>>col;
		cout<<"Nbre de lignes ("<<this->lignes<<") ? ";
		cin>>line;
		file.setTaille(col, line);
	}
	else if(choix == 2) // modification des options du jeu
	{
		int choix;
		cout<<"Modification des options de jeu (ne sera pris en compte qu'au prochain lancement)"<<endl;
		cout<<"Autorisez vous la rotation du plateau ? (1) Oui - (0) Non ? ";
		cin>>choix;
		if(choix == 1)
			file.setParametre("rotation", true);
		else
			file.setParametre("rotation", false);
		cout<<"Autorisez vous le jeu après une rotation (1) Oui - (0) Non ? ";
		cin>>choix;
		if(choix == 1)
			file.setParametre("jeuRotation", true);
		else
			file.setParametre("jeuRotation", false);
		cout<<"Autorisez vous la suppression d'un pion (1) Oui - (0) Non ? ";
		cin>>choix;
		if(choix == 1)
			file.setParametre("suppressionPion", true);
		else
			file.setParametre("suppressionPion", false);
	}
	else if(choix == 3) // modification des options de victoire
	{
		int lignes, pions;
		cout<<"Modification des options de victoire (ne sera pris en compte qu'au prochain lancement)"<<endl;
		cout<<"Nombre de rangées pour gagner ("<<this->nLignes<<") : ";
		cin>>lignes;
		cout<<"Nombre de pions par rangée pour gagner ("<<this->mPions<<") : ";
		cin>>pions;
		file.setVictoire(lignes, pions);
	}
	else
		cout<<"Choix inconnu"<<endl;
}
bool Jeu::verifLignes(int ligne, int nbPions, int pion) // Méthode privé
{
	int cpt = 0, cac = 0;
	bool check = false; // valeur retour, faux = pas le nbre de pions nécessaire par ligne ; true = ok
	for(int i = 0; i < this->colonnes; i++)
	{
		if(this->grille[ligne][i] == pion)
			cpt++; // compte le nombre de pion (1 ou 2) présent sur la ligne
	}
	if(cpt >= nbPions) // on sait que la ligne contient au moins n pions mais sont - il à la suite ?
	{
		bool precedent = false; // indique si la case précédente contenait déjà le meme pion
		for(int i = 1; i < (this->colonnes - 1); i++) // vérification de la ligne
		{	
			if(this->grille[ligne][i] == pion)
			{
				if(precedent || i == 0) // si la case a un précédent OU que c'est la première
					cac++; // incrémentation du compteur de case côte à côte
				precedent = true; // la case i + 1 a un précédent*/
			}
			else
			{
				precedent = false;
				cac = 0; // réinitialisation du compteur de pion cote à cote
			}
			if(cac == (nbPions - 1)) // le nombre de pions cote a cote correspond au nombre de pion demandé
				check = true;
		}
	}
	return check;
}
bool Jeu::verifColonnes(int colonne, int nbPions, int pion)
{
	int cpt = 0, cac = 1;
	bool check = false; // valeur retour, faux = pas le nbre de pions nécessaire par ligne ; true = ok
	for(int i = 0; i < this->lignes; i++)
	{
		if(this->grille[i][colonne] == pion)
			cpt++; // compte le nombre de pion (1 ou 2) présent sur la ligne
	}
	if(cpt >= nbPions) // on sait que la ligne contient au moins n pions mais sont - il à la suite ?
	{
		bool precedent = false; // indique si la case précédente contenait déjà le meme pion
		for(int i = 0; i < this->lignes; i++)
		{
			if(grille[i][colonne] == pion)
			{
				if(precedent || i == 0) // si la case a un précédent OU que c'est la première
					cac++;
				precedent = true; // la case i + 1 a un précédent
			}
			else
			{
				precedent = false;
				cac = 1; // réinitialisation du compteur de pion cote à cote
			}
			if(cac == nbPions)
				check = true;
		}
	}
	return check;
}
bool Jeu::possible(int colonne)
{
	int place = this->placeLibre(colonne);
	if(place != -1)
		return true;
	else
		return false;
}
bool Jeu::grillePleine()
{
	bool pleine = false;
	int cptPions = 0; // compteur de pions
	int max = this->lignes * this->colonnes;
	for(int i = 0; i < this->lignes; i++)
	{
		for(int j = 0; j < this->colonnes; j++)
		{
			if((this->grille[i][j] == ROND) || (this->grille[i][j] == CROIX))
				cptPions++;
		}
	}
	if(cptPions == max)
		pleine = true;
	return pleine;
}
// Setteurs privés
// Getteurs privés
Joueur Jeu::getInverse(Joueur j)
{
	int id = j.getId();
	int adversaire;
	if(id == 1)
		adversaire = 2;
	else if(id == 2)
		adversaire = 1;
	else
		adversaire = 0;
	Joueur inverse = this->getJoueur(adversaire);
	return inverse;
}
int Jeu::getInverse(int id)
{
	int adversaire;
	if(id == 1)
		adversaire = 2;
	else if(id == 2)
		adversaire = 1;
	else
		adversaire = 0;
	return adversaire;
}
// Méthodes publiques
void Jeu::affichage()
{
	string caractere = "";
	cout<<"  ";
	for(int i = 0; i < this->colonnes; i++) // affichage de l'index des colonnes
	{
		if(i < 9)
			cout<<"| "<<(i + 1)<<" ";
		else
			cout<<"| "<<(i + 1);
	}
	cout<<"|"<<" "<<endl;
	for(int i = 0; i < (this->colonnes - 1); i++)
		cout<<"-----";
	cout<<endl;
	for(int i = 0; i < this->lignes; i++)
	{
		cout<<(i + 1);
		if(i < 9)
			cout<<" | ";
		else
			cout<<"| ";
		for(int j = 0; j < this->colonnes; j++)
		{
			if(this->grille[i][j] == VIDE)
				caractere = ".";
			else if(this->grille[i][j] == ROND)
				caractere = "O";
			else if(this->grille[i][j] == CROIX)
				caractere = "X";
			else
				caractere = "E";
			cout<<caractere<<" | ";
		}
		cout<<endl;
	}
}
void Jeu::afficheMenu()
{
	cout<<"Que souhaitez vous faire ?"<<endl;
	cout<<"1. Jouer un pion"<<endl;
	if(this->rotation)
		cout<<"2. Effectuer une rotation"<<endl;
	else
		cout<<"2. Effectuer une rotation [Non dispo]"<<endl;
	if(this->suppressionPion)
		cout<<"3. Supprimer un pion"<<endl;
	else
		cout<<"3. Supprimer un pion [Non dispo]"<<endl;
	cout<<"4. Passer son tour"<<endl;
	cout<<"5. Voir les scores"<<endl;
	cout<<"6. Voir les options du jeu"<<endl;
	cout<<"7. Modifier les options de jeu"<<endl;
	cout<<"8. Réafficher la grille"<<endl;
	cout<<"9. Quitter le jeu"<<endl;
}
bool Jeu::choixMenu(int choix, Joueur j)
{
	bool valide = false; // action valide ? Si oui, passage au joueur suivant
	switch(choix)
	{
		case 0: // 0 => quitter (mais pas affiché dans le menu)
			this->quitter(false, false, 0); // quitte le jeu sans sauvegarde
			break;
		case 1: // jouer un pion
		{
			int colonne;
			cout<<"Sur quelle colonne placer le pion (0 pour annuler) ? ";
			cin>>colonne;
			if(this->insererPion(colonne, j.getId()) > 0) // si l'insertion d'un pion a réussi
				valide = true;
		}
			break;
		case 2: // effectuer une rotation
		{
			if(this->rotation)
			{
				int sens;
				cout<<"Sens de la rotation (-90 ou +90 ° - 0 pour annuler) ? ";
				cin>>sens;
				if(this->rotationGrille(sens)) // si la rotation a réussi
					valide = true; // le passage au joueur suivant est autorisé
				if(valide && this->jeuRotation)
				{
					cout<<"Vous pouvez rejouer un coup (0 pour ne pas entrer un pion)"<<endl;
					this->affichage(); // réaffichage de la grille
					this->choixMenu(1, j); // le joueur peut rejouer un pion
				}
			}
			else
				cout<<"Ce choix n'est pas possible"<<endl;
		}
			break;
		case 3: // suppression d'un pion
		{
			if(this->suppressionPion)
			{ // tenter de faire entrer par l'utilisateur un truc dans le genre : x;y (en string)
				int colonne, ligne;
				int pion = j.getId();
				cout<<"Sur quelle colonne supprimer le pion ? (0 pour annuler) ";
				cin>>colonne;
				if(colonne != 0)
				{
					cout<<"Le pion à supprimer dans la colonne "<<colonne<<" (0 pour annuler) ? ";
					cin>>ligne;
					if(this->supprimerPion(colonne, ligne, pion))
						valide = true;
					else
						cout<<"Vous ne pouvez pas supprimer ce pion ! Il ne vous appartient peut être pas !"<<endl;
				}
			}
			else
				cout<<"Ce choix n'est pas possible"<<endl;
		}
			break;
		case 4:
			cout<<"Vous venez de passer votre tour"<<endl;
			valide = true; // autorise le passage au joueur suivant
			break;
		case 5:
			this->afficherScore();
			break;
		case 6:
			this->afficherOptions();
			break;
		case 7: // modifie les options du jeu
		{
			int choix;
			cout<<"Quelles options modifier ?"<<endl;
			cout<<"Note : les options modifiées ne seront prises en compte qu'au redémarrage du jeu (0 pour annuler)"<<endl;
			cout<<"Les valeurs actuelles sont indiquées entre parenthèse"<<endl;
			cout<<"1. Taille de la grille"<<endl;
			cout<<"2. Options du jeu (rotation, jeu après une rotation, suppression d'un pion)"<<endl;
			cout<<"3. Conditions de victoire"<<endl;
			cout<<"Votre choix ? ";
			cin>>choix;
			if(choix != 0)
				this->modifierOptions(choix);
			break;
		}
		case 8: // réaffiche la grille
		{
			system("clear");
			this->affichage();
		}
			break;
		case 9:
			this->quitter(true, this->partieGagner, j.getId());
			break;
		case 11: // Débugging : permet de voir le contenu d'une seule case
		{
			system("clear");
			this->affichage();
			int c, l;
			cout<<"Colonne a voir ? ";
			cin>>c;
			cout<<"Ligne ? ";
			cin>>l;
			c--;
			l--;
			cout<<"Voici le contenu case ["<<(l + 1)<<"]["<<(c + 1)<<"] : "<<this->grille[l][c];
			break;
		}
		case 12: // Débugging : permet de voir une seule colonne
		{
			system("clear");
			this->affichage();
			int c;
			cout<<"Colonne ? ";
			cin>>c;
			c--;
			cout<<"Voici la colonne "<<(c + 1)<<" :"<<endl;
			for(int i = 0; i < this->colonnes; i++)
				cout<<"  ["<<(i + 1)<<"]["<<(c + 1)<<"] : "<<this->grille[i][c]<<endl;
			break;
		}
		case 13: // Débugging : permet de voir une seule ligne
		{
			system("clear");
			this->affichage(); 
			int l;
			cout<<"Ligne ? ";
			cin>>l;
			cout<<"Voici la ligne "<<(l + 1)<<" :"<<endl;
			l--;
			for(int i = 0; i < this->lignes; i++)
				cout<<"  ["<<(l + 1)<<"]["<<(i + 1)<<"] : "<<this->grille[l][i]<<endl;
			break;
		}
		case 14: // permet de voir les modifications qu'apporte la version
		{
			cout<<"Version 1.5 du 28 mai 2014"<<endl;
			cout<<"- Implémentation de l'algo MinMax en cours"<<endl;
			cout<<"- Implémentation de la sauvegarde et restauration de la partie"<<endl;
			cout<<"- Amélioration du confort de jeu"<<endl;
			cout<<"- Débugage erreur d'écriture de la condition de victoire"<<endl;
			cout<<"- Amélioration de l'algorithme de rotation +90 et -90"<<endl;
			cout<<"- Nettoyage du code source"<<endl;
		}
			break;
		case 15: // permet de supprimer la partie sauvegardée
		{
			cout<<"Attention : les options modifiées pendant le jeu ne sont pas enregistrées !"<<endl;
			file.enregistrement(true, 0); // simule un jeu gagné pour enregistrer uniquement les paramètres
			cout<<"Sauvegarde supprimée."<<endl;
		}
			break;
		default:
			cout<<"Choix invalide"<<endl;
			break;
	}
	return valide;
}
bool Jeu::definirGagnant(Joueur j)
{
	int pion = j.getId(); // valeur du pion du joueur
	int ligneVictoire = -1, colonneVictoire = -1;
	int gauche = 0, droite = 0; // compteur pour les diagonales
	bool gagnant = false;
	for(int i = 0; i < this->lignes; i++) // vérification lignes par ligne
	{
		if(this->verifLignes(i, this->mPions, pion))
			ligneVictoire = i;
	}
	for(int j = 0; j < this->colonnes; j++)// vérfication colonne par colonne
	{
		if(this->verifColonnes(j, this->mPions, pion))
			colonneVictoire = j;
	}
	for(int i = 0; i < (this->lignes - 1); i++)
	{
		for(int j = 0 ; j < (this->colonnes - 1); j++)
		{
			if((this->grille[i][j] == pion) && (this->grille[i + 1][j + 1] == pion))
				gauche++;
		}
	}
	for(int i = 1; i < (this->lignes - 0); i++)
	{
		 for(int j = 1; j < (this->colonnes - 1); j++)
		 {
			 if((this->grille[i][j] == pion) && (this->grille[i - 1][j + 1] == pion))
			 {
				 droite++;
			 }
		}
	}
	if(droite == (this->mPions - 1))
	{
		gagnant = true;
		cout<<"Victoire diagonale droite"<<endl;
	}
	if(ligneVictoire > -1)
	{
		gagnant = true;
		cout<<"Victoire sur la ligne "<<ligneVictoire<<endl;
	}
	if(colonneVictoire > -1)
	{
		gagnant = true;
		cout<<"Victoire sur la colonne "<<colonneVictoire<<endl;
	}
	if(gauche == this->mPions)
	{
		gagnant = true;
		cout<<"Victoire diagonale gauche"<<endl;
	}
	return gagnant;
}
bool Jeu::jeuOrdi(Joueur j)
{
	bool valide = false;
	int val = -1;
	int valeurMax = MIN;
	int valeurJeu = -1; // colonne à jouer
	srand(time(NULL));
	bool etatSuppression = this->suppressionPion; // sauvegarde de l'état de l'option Suppression pion
	if(j.estIA())
	{
		this->suppressionPion = true; // activation de l'option suppression pion pour que l'IA puisse jouer
		int niveau = j.getLevel();
		int pion = j.getId();
		int ** copie = this->creerGrille(this->colonnes, this->lignes);	// sauvegarde de la grille
		for(int i = 0; i < this->lignes; i++)
		{
			for(int j = 0; j < this->colonnes; j++)
			{
				copie[i][j] = this->grille[i][j];
			}
		}
		cout<<"Calcul en cours ..."<<endl;
		for(int i = 0; i < this->colonnes; i++) // pour chaque colonne
		{
			if(this->possible(i)) // si la colonne n'est pas pleine, on peux jouer
			{
				int ligneJouer = this->insererPion((i + 1), pion); // décalage de la colonne (insererPion est prévue pour le jeu via un joueur réel)
				if((ligneJouer >= 0) && (ligneJouer <= (this->colonnes -1)))
				{
					val = this->parcoursMin(0, niveau, false, j, valeurMax); // départ avec le joueur actuel
					if(val > valeurMax)
					{
						valeurMax = val;
						valeurJeu = i;
					}
					
				}
				this->supprimerPion(i + 1, ligneJouer + 1, pion); // supprimerPion effectue un décalage de la ligne et colonne
			}
		}
		// restauration de la grille
		for(int i = 0; i < this->lignes; i++)
		{
			for(int j = 0; j < this->colonnes; j++)
				this->grille[i][j] = copie[i][j];
		}
		if(valeurJeu >= 0) // si l'IA a renvoyé une valeur correcte, on peux jouer
		{
			// placement du pion dans la meilleure solution
			this->insererPion(valeurJeu + 1, j.getId()); // + 1 car la méthode est prévue pour un joueur humain
			cout<<"L'ordinateur a joué en "<<valeurJeu + 1<<endl;
		}
		if(valeurJeu == -1) // choix impossible, l'IA joue dans une colonne aléatoire
		{
			int hasard = rand() % this->colonnes;
			this->insererPion(hasard + 1, j.getId());
			cout<<"L'ordinateur a joué aléatoirement en "<<hasard + 1<<endl;
		}
		this->detruireGrille(copie, this->colonnes); // destruction de la grille temporaire
		valide = true;
	}
	if(!etatSuppression) // si l'option Suppression pion était active, on la rétablie
		this->suppressionPion = false;
	return valide;
}
int Jeu::parcoursMin(int pCourant, int profondeur, bool min, Joueur j, int mini)
{
	int pion = j.getId(); // pion du joueur
	Joueur adverse = this->getInverse(j);
	int n = -1;
	mini = MAX;
	if((this->grillePleine()) || (pCourant == profondeur)) // fin de parcours
		return this->evaluation(pion, pCourant);
	else
	{
		pCourant++; // descente d'un niveau
		for(int i = 0; i < this->colonnes; i++) // parcours de chaque colonne
		{
			if(this->possible(i)) // si elle n'est pas pleine, jeu
			{
				int ligneJouer = this->insererPion((i + 1), pion); // décalage de la colonne (insererPion est prévue pour le jeu via un joueur réel)
				if((ligneJouer >= 0) && (ligneJouer <= (this->colonnes -1)))
				{
					// nouvelle évaluation à partir de cet état
					n = this->parcoursMax(pCourant, profondeur, !min, adverse, mini);
					if(n < mini)
						mini = n;
					
				}
				this->supprimerPion(i + 1, ligneJouer + 1, pion); // supprimerPion effectue un décalage de la ligne et colonne
			}//*/
		}
	}
	return mini;
}
int Jeu::parcoursMax(int pCourant, int profondeur, bool max, Joueur j, int maxi) // privé, ici pour développement
{
	int pion = j.getId(); // pion du joueur
	Joueur adverse = this->getInverse(j);
	int n = -1;
	maxi = MIN;
	if((this->grillePleine()) || (pCourant == profondeur)) // fin de parcours
		return this->evaluation(pion, pCourant);
	else
	{
		pCourant++; // descente d'un niveau
		for(int i = 0; i < this->colonnes; i++) // parcours de chaque colonne
		{
			if(this->possible(i)) // si elle n'est pas pleine, jeu
			{
				int ligneJouer = this->insererPion((i + 1), pion); // décalage de la colonne (insererPion est prévue pour le jeu via un joueur réel)
				if((ligneJouer >= 0) && (ligneJouer <= (this->colonnes -1)))
				{
					// nouvelle évaluation à partir de cet état
					n = this->parcoursMin(pCourant, profondeur, !max, adverse, maxi);
					if(n > maxi)
						maxi = n;
					this->supprimerPion(i + 1, ligneJouer + 1, pion); // supprimerPion effectue un décalage de la ligne et colonne
				}
				this->supprimerPion(i + 1, ligneJouer + 1, pion); // supprimerPion effectue un décalage de la ligne et colonne
			}
		}
	}
	return maxi;
}
int Jeu::evaluation(int pion, int pCourant)
{
	int valeur = 18; // valeur de la dernière case
	int adversaire = this->getInverse(pion);
	int poids1 = this->calculPoids(pion); // calcul du poids des alignements du joueur actuel
	int poids2 = this->calculPoids(adversaire); // calcul du poids des alignements de l'adversaire
	if(poids1 > poids2) // avantage au joueur actuel
		valeur = MAX + pCourant;
	else if(poids1 < poids2) // avantage au joueur adverse
		valeur = MIN + pCourant;
	else
		valeur = 0;
	return valeur;
}
int Jeu::calculPoids(int pion)
{
	int poids = 0;
    // vérification lignes
    for(int i = 0; i < this->lignes; i++)
    {
        for(int k = 2; k < this->mPions; k++)
        {
            if(this->verifLignes(i, k, pion))
            {
                poids += (10 * k);
            }
        }
    }
    // vérification colonnes
    for(int i = 0; i < this->colonnes; i++)
    {
        for(int k = 2; k < this->mPions; k++)
        {
            if(this->verifColonnes(i, k, pion))
            {
                poids += (10 * k);
            }
        }
    }
    // vérification diagonale gauche
    // vérification diagonale droite
	return poids;
}
// Setteurs publiques
void Jeu::setGagner(bool gagner)
{
	this->partieGagner = gagner;
}
// Getteurs publiques
Joueur Jeu::getJoueur(int id)
{
	Joueur j;
	if(id == 1) // joueur 1
		j = this->joueur1;
	else if(id == 2) // joueur 2
		j = this->joueur2;
	else
		cout<<"Erreur"<<endl;
	return j;
}
string Jeu::getPion(Joueur j)
{
	string pion = "";
	if(j.getId() == 1)
		pion = "0";
	else if(j.getId() == 2)
		pion = "X";
	else
		pion = "E";
	return pion;
}
string Jeu::getPion(int pion)
{
	string caractere = "";
	if(pion == 1)
		caractere = "0";
	else if(pion == 2)
		caractere = "X";
	else
		caractere = "E";
	return caractere;
}
bool Jeu::getGagner()
{
	return this->partieGagner;
}
// Destructeur
Jeu::~Jeu()
{
	
}
