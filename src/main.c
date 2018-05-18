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

	GLuint background = createRGBTexture("./textures/fond.jpg");

	World world;
	initWorld(&world);
	loadLevel(&world);
	
	float translation = 0.;
	int player_status = 0;
	int loop = 1;
	int shooting = 0;
	int loaded = world.player->shooting_rate;
	int enemy_loaded = 0;

	/* Load malus texture */
	GLuint malus = createRGBATexture("./textures/malus.png");

	/* Boucle d'affichage */
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* Code de dessin */

		glClear(GL_COLOR_BUFFER_BIT); // Toujours commencer par clear le buffer

		/* Affichage et parallax de la texture de fond */
		glPushMatrix();
			glTranslatef(translation,0,0);
			displayBackground(background, NB_UNITS_X, NB_UNITS_Y); // Affiche le fond du niveau
		glPopMatrix();
		translation -= 0.01;

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
		if (checkIntersections(world.player, &(world.obstacleList)) || checkIntersections(world.player, &(world.enemyList)) || checkIntersections(world.player, &(world.brokableObstacleList))) { 
			printf("Partie perdu. Sortie du programme.\n");
			break;
		}
		if (world.enemyList) {
			if (checkIntersections(world.player, &(world.enemyList->missiles))) { 
				printf("Touché par ennemi.\n");
				break;
			}
		}
		
		// Fin du niveau lorsque le joueur passe la ligne d'arrivée
		if (checkIntersections(world.player, &(world.finishLineList))) { 
			printf("Niveau terminé\n");
			break;
		}
		// Supprime les bonus lorsqu'ils sont récupérés par le joueur)
		if (checkIntersections(world.player, &(world.bonusList))) {
			printf("Bonus Récupéré\n");
			world.player->nb_bonus++;
		}
		// Supprime un ennemi lorsqu'on lui tire dessus
		if (checkIntersections(world.player->missiles, &(world.enemyList)) && checkIntersections(world.enemyList, &(world.player->missiles))) { 
			printf("Ennemi tué\n");
		}
		// Supprime un obstacle cassable lorsqu'on lui tire dessus
		if (checkIntersections(world.player->missiles, &(world.brokableObstacleList)) && checkIntersections(world.brokableObstacleList, &(world.player->missiles))) { 
			printf("Osbstacle detruit\n");
		}
		// Détruit un missile lorsqu'il touche un obstacle
		checkIntersections(world.obstacleList, &(world.player->missiles));
		
		// Calcul les caractéristiques du joueur en fonction du nombre de bonus
		checkBonus(&world.player);

		/* Evenement de tir */
		if (shooting == 1 && loaded >= world.player->shooting_rate) {
			// Création d'un élement missile et ajout à la liste
			addElementToList(allocElement(4, world.player->x+1, world.player->y, world.player->speed_x + 0.2, 0, 0, 0, malus), &(world.player->missiles));	
			loaded = 0;
		}
		loaded++;
		
		if (world.enemyList) {
			if (enemy_loaded >= world.enemyList->shooting_rate) {
					// Création d'un élement missile et ajout à la liste
					addElementToList(allocElement(4, world.enemyList->x-1, world.enemyList->y, -0.2, 0, 0, 0, malus), &(world.enemyList->missiles));
				enemy_loaded = 0;
			}
			enemy_loaded++;
		}

		/* Affichage du plateau */
		glPushMatrix();
			glTranslatef(2 - world.player->x, 0, 0); // Translation du monde pour suivre le joueur
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