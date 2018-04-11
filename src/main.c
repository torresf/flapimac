#include "world.h"

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

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	World world;
	initWorld(&world);
	loadLevel(&world);
	
	int player_status = 0;
	int loop = 1;
	float x_move = 0;

	int shooting = 0;
	int loaded = world.player->shooting_rate;

	/* Boucle d'affichage */
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* Code de dessin */

		glClear(GL_COLOR_BUFFER_BIT); // Toujours commencer par clear le buffer

		// drawLandmark(); // Dessin du repère

		/* Déplacement du joueur */
		if (player_status == 1)
			moveUp(&world.player);
		else if (player_status == -1)
			moveDown(&world.player);
		else if (player_status == 0)
			slowDown(&world.player);

		movePlayer(&world.player); // Applique les transformations de positions au joueur
		checkPlayerPos(&world.player); // Recentre le joueur à l'intérieur si il sort de la fenêtre


		/* Gestion des collisions */
		// Suppression du joueur lorsqu'il touche un obstacle ou un ennemi, et sortie de la boucle
		if (checkIntersections(world.player, &(world.obstacleList)) || checkIntersections(world.player, &(world.enemyList))) { 
			printf("Partie perdu. Sortie du programme.\n");
			break;
		}

		// Suppression du joueur lorsqu'il touche un obstacle ou un ennemi, et sortie de la boucle
		if (checkIntersections(world.player, &(world.finishLineList))) { 
			printf("Niveau terminé !\n");
			break;
		}

		// Supprime les bonus lorsqu'ils sont récupérés par le joueur)
		if (checkIntersections(world.player, &(world.bonusList))) {
			printf("Bonus Récupéré !\n");
			world.player->nb_bonus++;
		}

		// Supprime un ennemi lorsqu'on lui tire dessus
		if (checkIntersections(world.player->missiles, &(world.enemyList))) { 
			printf("Ennemi tué !\n");
		}
		
		// Permet de détruire les obsacles
		checkIntersections(world.player->missiles, &(world.obstacleList));

		checkBonus(&world.player);

		/* Evenement de tir */
		if (shooting == 1 && loaded >= world.player->shooting_rate) {
			// Création d'un élement missile et ajout à la liste
			addElementToList(allocElement(4, world.player->x, world.player->y, PLAYER_SPEED_X * 3, 0, 0, 0), &(world.player->missiles));
			loaded = 0;
		}
		loaded++;
		
		/* DEPLACEMENT DES MISSILES */
		moveMissiles(&(world.player));

		/* Affichage du plateau */
		glPushMatrix();
			glTranslatef(x_move -= PLAYER_SPEED_X, 0, 0); // Translation du monde pour suivre le joueur
			drawWorld(world);
		glPopMatrix();

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

						case SDLK_q:
							loop = 0;
							break;

						case SDLK_UP:
							player_status = 1;
							break;

						case SDLK_DOWN:
							player_status = -1;
							break;

						case SDLK_SPACE:
							// Déclenchement du tir
							shooting = 1;
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