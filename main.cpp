/* Projet Puissance 4 évolué
Auteur : Jérémie Décome
L2 INFORMATIQUE TD1 - TP2
Mise à jour : 27 mai 2014
Fichier : main.cpp
Fichier principal */

#ifndef __MAIN__
#define __MAIN__

// Librairies
#include <stdlib.h>
#include <time.h>

// Classes
#include "jeu.h"
#include "joueur.h"
#include "fichier.h"

using namespace std;

/*Arguments (non obligatoire, auquel cas, le programme demande au joueur d'entrer le nombre de joueur et leurs noms)
Arg 1 : nombre de joueur 1 ou 2
Arg 2 : nom du joueur 1
Arg 3 : nom du joueur 2*/
int main(int argc, char* argv[])
{
	bool gagnant = false; // booléen pour déterminer si le jeu a été gagné (true) ou non (false)
	system("clear");
	cout<<"------ Puissance 4 évolué ------"<<endl;
	cout<<"Version 1.5"<<endl;
	cout<<"Projet L2 Informatique - Semestre 4"<<endl;
	cout<<"Jérémie Décome - TD1/TP2"<<endl<<endl;
	srand(time(NULL));
	int nbre = 0; // choix du nombre de joueurs
	int lvl1 = 0, lvl2 = -1;
	string nom1, nom2; // nom des joueurs
	Fichier sauvegarde(true); // objet Fichier appelé dans le main
	bool bot1 = false, bot2 = false; // type de joueur 1 et 2
	bool save = false;
	string choix = "N";
	if(sauvegarde.getSauvegarde())
	{
		cout<<"Une sauvegarde a été trouvée. "<<endl;
		cout<<"Voulez vous continuer cette partie ? (O) oui (N) non ? ";
		cin>>choix;
		system("clear");
	}
	if((choix == "O") || (choix == "o") || choix == "0") // on veut restaurer la sauvegarde
	{
		nom1 = sauvegarde.getNom(1); // lecture des paramètres de chacun joueur
		bot1 = sauvegarde.getBot(1);
		lvl1 = sauvegarde.getLevel(1);
		nom2 = sauvegarde.getNom(2);
		bot2 = sauvegarde.getBot(2);
		lvl2 = sauvegarde.getLevel(2);
		save = true; // indique au constructeur de Jeu que la partie en cours est une partie sauvegardée
		cout<<"La partie a bien été restaurée"<<endl;
	}
	else // on ne veux pas restaurer la sauvegarde
	{	
		if(argc == 1) // aucun argument passé
		{
			cout<<"Note : vous pouvez démarrer le jeu avec les arguments suivants :"<<endl;
			cout<<"1. Nombre de joueurs (1 ou 2)"<<endl;
			cout<<"2. Nom du premier joueur"<<endl;
			cout<<"3. Nom du second joueur (si jeu à 2)"<<endl<<"------------------------"<<endl;
			cout<<"Entrez le nombre de joueur (1 ou 2) : ";
			cin>>nbre;
			if(nbre == 1) // 1 joueur, donc il joue contre l'ordinateur
			{
				cout<<"Entrez le nom du premier joueur : ";
				cin>>nom1;
				nom2 = "Ordinateur";
				bot2 = true; // le second perso est l'ordinateur
				cout<<"Niveau de difficulté de l'ordinateur ? (1 à "<<LEVEL_MAX<<") ";
				cin>>lvl2;
			}
			else if(nbre == 2) // 2 joueurs
			{
				cout<<"Entrez le nom du premier joueur : ";
				cin>>nom1;
				cout<<"Entrez le nom du second joueur : ";
				cin>>nom2;
				lvl1 = 0;
			}
			else
			{
				cout<<"Ce jeu ne peux se jouer qu'a 1 ou 2 joueur. Merci de relancer l'application."<<endl;
				exit(1); // arret du jeu
			}
		}
		else if(argc >= 3) // argument passé
		{
			int nb = atoi(argv[1]); // conversion char => int
			nom1 = argv[2];
			if(nb == 1) // jeu solo
			{
				cout<<"Vous avez choisi de jouer contre l'ordinateur. "<<endl;
				cout<<"Entrez son niveau de jeu (1 à "<<LEVEL_MAX<<") : ";
				cin>>lvl2;
				bot2 = true;
				nom2 = "Ordinateur";
			}
			else if(nb == 2)
			{
				nom2 = argv[3]; // lecture du nom du second joueur (paramètre 4)
			}
		}
		else
		{
			cout<<"Erreur arguments"<<endl;
			exit(5);
		}
	}
	cout<<"Résumé des joueurs :"<<endl;
	Joueur j1(nom1, bot1, lvl1); // instanciation du joueur 1
	Joueur j2(nom2, bot2, lvl2); // instanciation du joueur 2
	Jeu game(j1, j2, save); // création du jeu avec les 2 joueurs => objet game
	int nb = 1; // par défaut, le premier joueur commence
	if(save)
		nb = sauvegarde.getJoueurDebut();
	else
		nb = rand() % (2 - 1) + 1;// tirage au sort du premier qui démarre
	Joueur actuel = game.getJoueur(nb); // retourne le joueur identité par l'id
	cout<<"-----"<<endl;
	game.afficherOptions();
	cout<<"Le jeu oppose "<<j1.getNom()<<" contre "<<j2.getNom()<<". Que le jeu commence !"<<endl;
	cout<<actuel.getNom()<<" démarre le premier"<<endl;
	cout<<"Appuyez sur [Entrée] pour démarrer le jeu";
	cin.ignore().get();
	// Début du jeu
	while(!gagnant) // tant que le jeu n'est pas gagné, on continu la boucle
	{
		system("clear"); // efface l'écran
		int choix = -1;
		bool valide = false;
		cout<<endl;
		game.affichage(); // affiche la grille
		cout<<endl<<"C'est au tour de "<<actuel.getNom()<<". Vous jouez les "<<game.getPion(actuel)<<endl;
		if(actuel.estIA()) // si le joueur est l'ordinateur
		{
			valide = game.jeuOrdi(actuel); // permet à l'ordinateur de jouer et indique si l'ordi a joué
			if(valide)
			{
				cout<<"Appuyez sur [Entrée]";
				cin.ignore().get();
			}
		}
		else // sinon, c'est le joueur réel
		{
			game.afficheMenu(); // affiche le menu
			cout<<"Votre choix : ";
			cin>>choix;
			valide = game.choixMenu(choix, actuel); // "point d'entrée" du jeu. La méthode appele les méthodes privées correspondantes
			if(!valide)
			{
				cout<<"Appuyez sur [Entrée]";
				cin.ignore().get();
			}
		}
		if(game.definirGagnant(actuel)) // vérifie si le gagnant actuel n'a pas gagné le jeu
		{
			cout<<"Victoire ! C'est "<<actuel.getNom()<<" ("<<game.getPion(actuel)<<") qui remporte le jeu."<<endl;
			gagnant = true;
		}
		else // on continue le jeu
		{
			gagnant = false;
			if(valide) // si l'action a été effectuée et qu'elle nécessite le changement de joueur
			{
				if(actuel.getId() == 1)
					actuel.echange(j2); // passage au joueur 2
				else
					actuel.echange(j1); // passage au joueur 1
			}
		}
		game.setGagner(gagnant);
	}
	cout<<"Fin du jeu :"<<endl;
	game.affichage();
	return 0;
}

#endif
