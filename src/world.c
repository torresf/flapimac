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
	world->brokableObstacleList = NULL;
}

/* Libère la mémoire associée au monde */
void deleteWorld(World *world) {
	deleteElements(&((*world).player));
	deleteElements(&((*world).obstacleList));
	deleteElements(&((*world).enemyList));
	deleteElements(&((*world).bonusList));
	deleteElements(&((*world).finishLineList));
	deleteElements(&((*world).brokableObstacleList));
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

	/* Load textures */

	/* Textures éléments */
    GLuint ennemi = createRGBATexture("./textures/ennemi.png");
    GLuint player = createRGBATexture("./textures/avion.png");
    GLuint bonus = createRGBATexture("./textures/bonus.png");

    /* Textures obstacles */
    GLuint finish = createRGBATexture("./textures/finish.png");
    GLuint b_o_1 = createRGBATexture("./textures/obstacle_cassable_1.png");
    GLuint n_o_1 = createRGBATexture("./textures/obstacle_normal_1.png");
    /* GLuint b_o_2 = createRGBATexture("../textures/obstacle_cassable_2.png");
    GLuint n_o_1 = createRGBATexture("./textures/obstacle_normal_1.png");
    GLuint n_o_2 = createRGBATexture("./textures/obstacle_normal_2.png");
    GLuint n_o_3 = createRGBATexture("./textures/obstacle_normal_3.png");
    GLuint n_o_4 = createRGBATexture("./textures/obstacle_normal_4.png");
    GLuint n_o_5 = createRGBATexture("./textures/obstacle_normal_5.png"); */

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
							addElementToList(allocElement(0, c_index, (NB_UNITS_Y - 1) - l_index, 0.05, 0, 5, 30, player), &((*world).player));
						}
						if (r == 0 && g == 0 && b == 0) {
							/* Obstacle */
							addElementToList(allocElement(1, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0, 0, 0, n_o_1), &((*world).obstacleList));
						}
						if (r == 255 && g == 0 && b == 0) {
							/* Ennemi */
							addElementToList(allocElement(2, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0.02, -4, 30, ennemi), &((*world).enemyList));
						}
						if (r == 0 && g == 255 && b == 0) {
							/* Bonus */
							addElementToList(allocElement(3, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0.1, 0, 0, bonus), &((*world).bonusList));
						}
						if (r == 255 && g == 255 && b == 0) {
							/* Ligne d'arrivée */
							addElementToList(allocElement(5, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0, 0, 0, finish), &((*world).finishLineList));
						}
						if (r == 0 && g == 255 && b == 255) {
							/* Obstacle cassable */
							addElementToList(allocElement(1, c_index, (NB_UNITS_Y - 1) - l_index, 0, 0, 0, 0, b_o_1), &((*world).brokableObstacleList));
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