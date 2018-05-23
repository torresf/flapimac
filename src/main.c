#include "world.h"
#include "display.h"

int main(int argc, char** argv){

	/* Initialisation de la SDL */
	if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if (NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		return EXIT_FAILURE;
	}
	SDL_WM_SetCaption("Flapimac", NULL);
	resizeViewport();


	World world;
	initWorld(&world);
	// loadLevel(&world);

	float translation = 0.;
	int player_status = 0;
	int loop = 1;
	int shooting = 0;
	int start = 0;
	int player_loaded = 0;
	int chosen_level = 1;
	int level_loaded = 0;

	/* Load malus texture */
	GLuint malus = createRGBATexture("./textures/main/malus.png");
	GLuint background1 = createRGBTexture("./textures/level_1/fond.jpg");
	GLuint background2 = createRGBTexture("./textures/level_2/fond.jpg");
	GLuint background3 = createRGBTexture("./textures/level_3/fond.jpg");
	GLuint background4 = createRGBTexture("./textures/level_4/fond.jpg");

	/* Boucle d'affichage */
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* Code de dessin */
		glClear(GL_COLOR_BUFFER_BIT); // Toujours commencer par clear le buffer

		if (start == 0) {
			/* Menu */
			// Affiche le fond du niveau
			switch (chosen_level) {
				case 1:
					displayBackground(background1, NB_UNITS_X, NB_UNITS_Y);
					break;
				case 2:
					displayBackground(background2, NB_UNITS_X, NB_UNITS_Y);
					break;
				case 3:
					displayBackground(background3, NB_UNITS_X, NB_UNITS_Y);
					break;
				case 4:
					displayBackground(background4, NB_UNITS_X, NB_UNITS_Y);
					break;
				default:
					break;
			}
		} else {
			// Lancement du jeu
			if (level_loaded == 0) {
				loadLevel(&world, chosen_level);
			}
			level_loaded = 1;

			/* Affichage et parallax de la texture de fond */
			glPushMatrix();
				glTranslatef(translation, 0, 0);
				switch (chosen_level) {
					case 1:
						displayBackground(background1, NB_UNITS_X, NB_UNITS_Y);
						break;
					case 2:
						displayBackground(background2, NB_UNITS_X, NB_UNITS_Y);
						break;
					case 3:
						displayBackground(background3, NB_UNITS_X, NB_UNITS_Y);
						break;
					case 4:
						displayBackground(background4, NB_UNITS_X, NB_UNITS_Y);
						break;
					default:
						break;
				}
			glPopMatrix();
			translation -= 0.02;

			/* Déplacement du joueur */
			if (player_status == 1)
				moveUp(&world.player);
			else if (player_status == -1)
				moveDown(&world.player);
			else if (player_status == 0)
				slowDown(&world.player);

			movePlayer(&world.player); // Applique les transformations de positions au joueur
			checkPlayerPos(&world.player); // Recentre le joueur à l'intérieur si il sort de la fenêtre

			/* Déplacement des missiles */
			moveMissiles(&(world.player));

			if (world.enemyList) {
				moveVertical(&(world.enemyList));
				moveMissiles(&(world.enemyList));
			}

			if (world.bonusList)
				moveVertical(&(world.bonusList));

			/* Gestion des collisions */
			// Suppression du joueur lorsqu'il touche un obstacle ou un ennemi, et sortie de la boucle
			if (checkIntersections(&(world.player), &(world.obstacleList), 0) || checkIntersections(&(world.player), &(world.enemyList),0) || checkIntersections(&(world.player), &(world.brokableObstacleList),0)) { 
				printf("Partie perdue. Sortie du programme.\n");
				break;
			}

			if (world.enemyList) {
				if (checkMissilesIntersections(&world)) {
					printf("Touché par missile ennemi.\n");
					break;
				}
			}
			
			// Fin du niveau lorsque le joueur passe la ligne d'arrivée
			if (checkIntersections(&(world.player), &(world.finishLineList), 0)) { 
				printf("Niveau terminé\n");
				break;
			}

			// Supprime les bonus lorsqu'ils sont récupérés par le joueur)
			if (checkIntersections(&(world.player), &(world.bonusList), 0)) {
				printf("Bonus Récupéré\n");
				world.player->nb_bonus++;
			}

			// Supprime un ennemi lorsqu'on lui tire dessus
			if (checkIntersections(&(world.player->missiles), &(world.enemyList), 1)) { 
				printf("Ennemi tué\n");
			}

			// Supprime un obstacle cassable lorsqu'on lui tire dessus
			if (checkIntersections(&(world.player->missiles), &(world.brokableObstacleList), 1)) { 
				printf("Obstacle détruit\n");
			}

			// Détruit un missile lorsqu'il touche un obstacle
			checkIntersections(&(world.obstacleList), &(world.player->missiles), 0);
			
			// Calcul les caractéristiques du joueur en fonction du nombre de bonus
			checkBonus(&world.player);

			/* Evenement de tir */
			if (shooting == 1 && player_loaded >= world.player->shooting_rate) {
				// Création d'un élement missile et ajout à la liste
				addElementToList(allocElement(4, world.player->x+1, world.player->y + 0.4, 0.2, 0.2, world.player->speed_x + 0.2, 0, 0, 0, malus), &(world.player->missiles));	
				player_loaded = 0;
			}
			player_loaded++;
			
			/* Evenement de tir ennemi */
			enemyShooting(&world.enemyList, malus);

			/* Affichage du plateau */
			glPushMatrix();
				glTranslatef(2 - world.player->x, 0, 0); // Translation du monde pour suivre le joueur
				drawWorld(world);
			glPopMatrix();

		}

		/* Boucle traitant les evenements */
		SDL_Event e;
		while(SDL_PollEvent(&e)) {

			/* L'utilisateur ferme la fenêtre : */
			if(e.type == SDL_QUIT) {
				loop = 0;
				break;
			}
			
			/* Traitement d'evenements : */
			switch(e.type) {

				/* Touche clavier */
				case SDL_KEYDOWN:
					// printf("touche pressée (code = %d)\n", e.key.keysym.sym);
					switch(e.key.keysym.sym) {

						case 256: // 0
							chosen_level = 0;
							break;

						case 257: // 1
							chosen_level = 1;
							break;

						case SDLK_ESCAPE:
							if (start == 1) {
								printf("Pause\n");
								start = 0;
							} else {
								printf("Le jeu est déjà en pause.\n");
								if (level_loaded == 1) {
									printf("Une partie a été lancée, on retourne au menu et on réinitialise.\n");
									level_loaded = 0;
									initWorld(&world);
									translation = 0;
								} else {
									printf("Aucune partie n'a été lancée, on quitte.\n");
									loop = 0;
								}
							}
							break;

						case SDLK_UP:
							player_status = 1;
							break;

						case SDLK_DOWN:
							player_status = -1;
							break;

						case SDLK_LEFT:
							if (start == 0 && chosen_level > 1 && level_loaded == 0)
								chosen_level--;
							break;

						case SDLK_RIGHT:
							if (start == 0 && chosen_level < 4 && level_loaded == 0)
								chosen_level++;
							break;

						case SDLK_SPACE:
							// Déclenchement du tir
							if (start == 1) {
								shooting = 1;
							} else {
								start = 1;
							}
							break;

						default:
							break;
					}

					break;

				case SDL_KEYUP:
					switch(e.key.keysym.sym) {

						case SDLK_UP:
							if (player_status == 1) {
								player_status = 0;
							}
							break;

						case SDLK_DOWN:
							if (player_status == -1) {
								player_status = 0;
							}
							break;

						case SDLK_SPACE:
							shooting = 0;
							break;

						default:
							break;
					}
					break;

				case SDL_VIDEORESIZE:
					WINDOW_WIDTH = e.resize.w;
					WINDOW_HEIGHT = e.resize.h;
					resizeViewport();

				default:
					break;
			}
		}

		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();

		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;

		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
	}

	deleteWorld(&world);

	/* Liberation des ressources associées à la SDL */ 
	SDL_Quit();

	return EXIT_SUCCESS;
}