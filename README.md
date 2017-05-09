# Projet Puissance 4
Développé par Jérémie Décome

Licence 2 Informatique - CERI Avignon

Fichier mis à jour le 28 mai 2014

Version actuelle du programme : 1.5

## Détails du projet :
Le programme est composée de 3 classes :
- Classe Jeu : gère le jeu (placement, suppression de pion, rotation, ...) et le MinMax
- Classe Joueur : gère le joueur et ses paramètres (nom, identifiant, type, niveau (si IA) et score)
- Classe Fichier : gère l'accès en lecture ou écriture du fichier (sert d'interface entre le programme et le fichier)

## Fichiers :
- main.cpp : point d'entrée du programme. Permet le déroulement du jeu
- commun.h : contient les librairies communes à toutes les classes
- makefile : permet la compilation du projet et l'édition de lien avec les 5 classes
- save.txt : fichier de sauvegarde des options et du jeu

## Situation au 28 mai 2014 - Rendu final
Les classes Pion, Options et IA ont été supprimées car inutiles.
Nettoyage du code source
Amélioration du confort de jeu
Réécriture de la méthode de rotation

- Principal (fichier main.cpp)
Déclencher l'aglo MinMax si c'est l'ordinateur qui joue
Les déterminations de victoire sont en cours de vérification

- Classe Jeu :
Algorithme MinMax en cours de réalisation
Alternance MinMax OK - Méthode d'évaluation en cours
	- Alternance MinMax fonctionne
	- Fonction d'évaluation fonctionne seulement pour lignes et colonnes. Fonctionne avec des poids selon les alignements

- Classe Fichier :
Lit et écrit dans le fichier de sauvegarde :
	- les options de jeu
	- la partie (nom des joueurs + la grille + le joueur qui doit jouer)


Bugs et erreurs connus :
- Si des paramètres sont manquants, aucun système ne permet de charger des valeurs par défaut
- Ne détecte pas la victoire en mode 1 joueur. Probablement une erreur dans l'algo dans le main
 -Détecte une victoire sur diagonale lorsque il y a 2 diagonales (une de 3 pions et une à côté de 2 pions)
 -En cas de modifications de la taille de la grille dans les options (choix 7), elle n'est pas pris en compte au redémarrage.

Améliorations prévues :
- Techniquement, il est possible de faire jouer l'ordinateur contre l'ordinateur. La méthode permettant à l'ordinateur de jouer prend en paramètre le joueur actuel. 
	=> implémenter un mode Ordi vs Ordi (juste pour voir l'ordinateur jouer tout seul)
- Suppression d'un pion : possibilité d'indiquer au programme les coordonées du pion à supprimer sous cette forme : X;Y
- Utilisation un format normé (type XML) pour le fichier de sauvegarde
- Implémentation de l'algorithme AlphaBeta une fois que le MinMax est terminé et fonctionnel. L'optimisation de l'évaluation est également prévue
- Création d'une interface graphique avec la SDL par exemple
- Jeu en réseau (sur un modèle client / serveur)
- Modification du score de chaque joueur en fonction des alignements
- Possibilité de recommencer une partie avec les mêmes paramètres des joueurs et évolution du score de chacun
- Mode aléatoire : secoue le plateau et redispose les pions aléatoirement

-----------------------------------------------

Situation au 19 mars 2014 - Rendu intermédiaire
- Principal (fichier main.cpp)
Alternance des deux joueurs : Fonctionnel
Affichage du menu et de la grille à chaque itération : Fonctionnel
Le jeu fonctionne dans l'ensemble mais ne détermine pas encore qui a gagné

- Classe Jeu :
Placement d'un pion : Fonctionnel
Suppression d'un pion : Fonctionnel
Rotation de la grille : Fonctionne à droite mais pas à gauche
Jeu en fonction des options : Fonctionnel
Détermination de la victoire : En cours (vérification sur ligne)

- Classe Fichier : 
Non implementé

- Classe Options : 
Retourne les valeurs des options de jeu : Fonctionnel
Retourne les valeurs de la taille de la grille : Fonctionnel
Modification des options : Fonctionnel mais pas inscrit dans le fichier

- Classe Pion :
En cours d'implémentation

- Classe IA :
Non implémenté

Travaux actuels :
Algorithme de rotation à gauche en cours
Définition de la victoire en cours de conception
Conception de la classe IA permettant le jeu contre l'ordinateur
