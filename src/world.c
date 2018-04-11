#include "world.h"

/* Fonction qui nous sert à redimensionner notre repère dans la fenêtre de travail */
void resizeViewport() {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., (float)NB_UNITS_X, 0., (float)NB_UNITS_Y);
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

/* Initialise les champs et listes chainées du monde */
void initWorld(World *world) {
	world->player = NULL;
	world->obstacleList = NULL;
	world->enemyList = NULL;
	world->bonusList = NULL;
	world->finishLineList = NULL;
}

/* Affiche les éléments du monde : Le joueur, les obstacles, les ennemis, les bonus */
void drawWorld(World world) {
	drawElements(world.player);
	drawElements(world.obstacleList);
	drawElements(world.enemyList);
	drawElements(world.bonusList);
	drawElements(world.finishLineList);
	if (world.player->missiles)
		drawElements(world.player->missiles);
}

/* Libère la mémoire associée au monde */
void deleteWorld(World *world) {
	if ((*world).player->missiles)
		deleteElements(&((*world).player)->missiles);
	deleteElements(&((*world).player));
	deleteElements(&((*world).obstacleList));
	deleteElements(&((*world).enemyList));
	deleteElements(&((*world).bonusList));
	deleteElements(&((*world).finishLineList));
	
}

/* Chargement du niveau */
void loadLevel(World *world) {
	FILE *level1;

	/* opening file for reading */
	level1 = fopen("levels/level1.ppm", "r");
	if (level1 == NULL) {
		perror("Error opening file");
		return;
	}

	char line[MAX_SIZE];
	int line_number = 0;
	int r, g, b;
	int l_index = 0, c_index = 0;
	int offset;
	while (fgets(line, sizeof line, level1) != NULL) {
		if (line[0] != '#') {
			if (line_number >= 4) {
				char *data = line;
				while (sscanf(data, " %d %d %d%n", &r, &g, &b, &offset) == 3) {
					data += offset;
					if (r != 255 || g != 255 || b != 255) {
						if (r == 0 && g == 0 && b == 255) {
							/* Joueur */
							addElementToList(allocElement(0, c_index, (NB_UNITS_Y - 1) - l_index, PLAYER_SPEED_X, 0), &((*world).player));
						}
						if (r == 0 && g == 0 && b == 0) {
							/* Obstacle */
							addElementToList(allocElement(1, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0), &((*world).obstacleList));
						}
						if (r == 255 && g == 0 && b == 0) {
							/* Ennemi */
							addElementToList(allocElement(2, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0), &((*world).enemyList));
						}
						if (r == 0 && g == 255 && b == 0) {
							/* Bonus */
							addElementToList(allocElement(3, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0), &((*world).bonusList));
						}
						if (r == 255 && g == 255 && b == 0) {
							/* Ligne d'arrivée */
							addElementToList(allocElement(5, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0), &((*world).finishLineList));
						}
					}
					c_index++;
				}
				c_index = 0;
				l_index++;
			}
			line_number++;
		}
	}
	fclose (level1);
}