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

	float translation = 0.;
	int player_status = 0;
	int loop = 1;
	int shooting = 0;
	int start = 0;
	int player_loaded = 0;
	int chosen_level = 1;
	int level_loaded = 0;
	float right_arrow_scale = 1.;
	float left_arrow_scale = 1.;

	/* Load malus texture */
	GLuint malus = createRGBATexture("./textures/main/malus.png");
	GLuint background1 = createRGBTexture("./textures/level_1/fond.jpg");
	GLuint background2 = createRGBTexture("./textures/level_2/fond.jpg");
	GLuint background3 = createRGBTexture("./textures/level_3/fond.jpg");
	GLuint background4 = createRGBTexture("./textures/level_4/fond.jpg");
	GLuint arrow = createRGBATexture("./textures/main/fleche.png");
	GLuint select_text = createRGBATexture("./textures/main/select.png");
	GLuint esc_text = createRGBATexture("./textures/main/esc.png");
	GLuint pause = createRGBATexture("./textures/main/pause.png");

	/* Boucle d'affichage */
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* Code de dessin */
		glClear(GL_COLOR_BUFFER_BIT); // Toujours commencer par clear le buffer

		// Affichage de la texture de fond en fonction du niveau choisi
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

		if (start == 0) {
			/* Menu */
			if (level_loaded == 1) {
				// Menu Pause
				/* Affichage du plateau gelé en fond */
				glPushMatrix();
					glTranslatef(2 - world.player->x, 0, 0);
					drawWorld(world);
				glPopMatrix();

				glPushMatrix();
					glTranslatef(NB_UNITS_X / 2, NB_UNITS_Y / 2, 0);
					displayTexture(pause, NB_UNITS_X, NB_UNITS_Y);
				glPopMatrix();

				glPushMatrix();
					glTranslatef(1, NB_UNITS_Y-1, 0);
					displayTexture(esc_text, 1.04, 0.55);
				glPopMatrix();
				
			} else {
				// Menu sélection du niveau
				if (chosen_level != 1) {
					glPushMatrix();
						glTranslatef(13, 4, 0);
						glRotatef(90, 0, 0, 1);
						glScalef(left_arrow_scale, left_arrow_scale, 1);
						displayTexture(arrow, 1.02, 1.32);
					glPopMatrix();
				}
				if (chosen_level != 4) {
					glPushMatrix();
						glTranslatef(17, 4, 0);
						glRotatef(-90, 0, 0, 1);
						glScalef(right_arrow_scale, right_arrow_scale, 1);
						displayTexture(arrow, 1.02, 1.32);
					glPopMatrix();
				}
				glPushMatrix();
					glTranslatef(15, 5, 0);
					displayTexture(select_text, 5.86, 0.55);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(1, NB_UNITS_Y-1, 0);
					displayTexture(esc_text, 1.04, 0.55);
				glPopMatrix();
			}
		} else {
			// Lancement du jeu
			if (level_loaded == 0) {
				loadLevel(&world, chosen_level);
			}
			level_loaded = 1;

			/* Translation de la texture de fond */
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
				printf("Partie perdue. Retour au menu.\n");
				start = 0;
				level_loaded = 0;
				translation = 0;
				initWorld(&world);
				continue;
			}

			if (world.enemyList) {
				if (checkMissilesIntersections(&world)) {
					printf("Touché par missile ennemi.\n");
					start = 0;
					level_loaded = 0;
					translation = 0;
					initWorld(&world);
					continue;
				}
			}
			
			// Fin du niveau lorsque le joueur passe la ligne d'arrivée
			if (checkIntersections(&(world.player), &(world.finishLineList), 0)) { 
				printf("Niveau terminé. Retour au menu.\n");
				start = 0;
				level_loaded = 0;
				translation = 0;
				initWorld(&world);
				continue;
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
				float missile_size = 0.4;
				// Création d'un élement missile et ajout à la liste
				addElementToList(allocElement(4, world.player->x+world.player->width, world.player->y + (world.player->height - missile_size)/2, missile_size, missile_size, world.player->speed_x + 0.2, 0, 0, 0, malus), &(world.player->missiles));	
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

						case SDLK_ESCAPE:
							if (start == 1) {
								/* Pause */
								start = 0;
							} else {
								if (level_loaded == 1) {
									/* Une partie a été lancée, on retourne au menu et on réinitialise. */
									level_loaded = 0;
									initWorld(&world);
									translation = 0;
								} else {
									/* On quitte */
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
							left_arrow_scale = .8;
							break;

						case SDLK_RIGHT:
							right_arrow_scale = .8;
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

						case SDLK_LEFT:
							left_arrow_scale = 1;
							if (start == 0 && chosen_level > 1 && level_loaded == 0)
								chosen_level--;
							break;

						case SDLK_RIGHT:
							right_arrow_scale = 1;
							if (start == 0 && chosen_level < 4 && level_loaded == 0)
								chosen_level++;
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