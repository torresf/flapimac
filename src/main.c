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

	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	World world;
	initWorld(&world);
	loadLevel(&world);	
	
	int player_status = 0;
	int loop = 1;
	float x_move = 0;

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
		// Supprime le joueur lorsqu'il touche un obstacle ou un ennemi et sortie de la boucle
		if (checkIntersections(world.obstacleList, &(world.player)) || checkIntersections(world.player, &(world.enemyList))) { 
			printf("Fin de la partie\n");
			break;
		}
		if (checkIntersections(world.player, &(world.bonusList))) { // Supprime les bonus lorsqu'ils sont récupérés par le joueur)
			printf("Bonus Récupéré !\n");
		}

		/* Affichage du plateau */
		world.player->x += PLAYER_SPEED_X; // Le joueur avance
		glPushMatrix();
			glTranslatef(x_move -= PLAYER_SPEED_X, 0, 0); // Translation pour suivre le joueur
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
			
			/* Quelques exemples de traitement d'evenements : */
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
							printf("Début Tir\n");
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
							printf("Fin tir\n");
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