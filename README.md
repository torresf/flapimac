# Flapimac
Jeu en OpenGL développé par Tom Samaille et Florian Torres

## Procédure pour lancer le jeu (Linux)
Il faut en premier lieu s’assurer d’avoir les bibliothèques OpenGL et SDL installées sur sa machine (ainsi que les paquets libsdl-mixer1.2-dev - libsdl-image1.2-dev).
Une fois le dossier contenant les différents fichiers sources du jeu récupérés, s’y rendre avec l’invite de commande puis effectuer la compilation du programme avec la commande :
`make`
Ensuite, lancer le jeu avec la commande : 
`./flapimac`

## Fonctionnalités
- [x] Menu : Sélection de niveau
- [x] Mise en pause de la partie
- [x] Bonus : Amélioration de la portée, de la fréquence de tir, et tir double
- [x] Vie des éléments
- [x] Obstacles destructibles
- [x] Animation d’éléments dans la scène (Ex: Hélice)
- [x] Affichage du nombre de vies restantes et du nombre de bonus récupérer
- [x] Textures transparentes
- [x] Musique tout au long du jeu

## Prochaines fonctionnalités à développer
- [ ] Gestion et affichage du score du joueur
- [ ] Choix aléatoire textures des obstacles et obstacles cassables pour faire moins monotone
- [ ] Tir à fréquence aléatoire des ennemis pour augmenter la difficulté et rendre ces derniers imprévisibles
- [ ] Différents ennemis / projectiles
- [ ] Bande de couleur : Accélération du joueur jusqu'à la prochaine bande / pendant un temps imparti
- [ ] Ajout d’un niveau final avec boss à combattre (avec barre de vie lui-aussi)
- [ ] Sprites animés 
- [ ] Animation d'explosion sur les collisions
- [ ] Enregistrement de la partie dans un fichier externe
- [ ] Multijoueur
