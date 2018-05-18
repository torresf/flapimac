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
		exit(0);
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

	char line[64];
	int r, g, b;
	int width, j, height, i, color_max;

	/* 
	 * Première ligne du fichier 
	 * <type>
	*/
	fgets(line, sizeof line, level1);
	if (strcmp(line, "P3\n") != 0) {
		printf("Erreur : Format du fichier non pris en charge.\n");
		exit(0);
	}

	/* 
	 * Deuxième ligne du fichier 
	 * <width> <height>
	*/
	fgets(line, sizeof line, level1);
	if (line[0] == '#')
	{
		fgets(line, sizeof line, level1);
	}
	sscanf(line, "%d %d", &width, &height);

	/* 
	 * Troisième ligne du fichier 
	 * <color_max>
	*/
	fgets(line, sizeof line, level1);
	sscanf(line, "%d", &color_max);

	for (i = 0; i < height; ++i) {	
		for (j = 0; j < width; ++j)	{
			fgets(line, 64, level1);
			sscanf(line, "%d", &r);
			fgets(line, 64, level1);
			sscanf(line, "%d", &g);
			fgets(line, 64, level1);
			sscanf(line, "%d", &b);
			
			if (r != 255 || g != 255 || b != 255) {
				if (r == 0 && g == 0 && b == 255) {
					/* Joueur */
					addElementToList(allocElement(0, j, (NB_UNITS_Y - 1) - i, 0.10, 0, 5, 30, player), &((*world).player));
				}
				if (r == 0 && g == 0 && b == 0) {
					/* Obstacle */
					addElementToList(allocElement(1, j, (NB_UNITS_Y - 1) - i, 0, 0, 0, 0, n_o_1), &((*world).obstacleList));
				}
				if (r == 255 && g == 0 && b == 0) {
					/* Ennemi */
					addElementToList(allocElement(2, j, (NB_UNITS_Y - 1) - i, 0, 0.04, -4, 30, ennemi), &((*world).enemyList));
				}
				if (r == 0 && g == 255 && b == 0) {
					/* Bonus */
					addElementToList(allocElement(3, j, (NB_UNITS_Y - 1) - i, 0, 0.1, 0, 0, bonus), &((*world).bonusList));
				}
				if (r == 255 && g == 255 && b == 0) {
					/* Ligne d'arrivée */
					addElementToList(allocElement(5, j, (NB_UNITS_Y - 1) - i, 0, 0, 0, 0, finish), &((*world).finishLineList));
				}
				if (r == 0 && g == 255 && b == 255) {
					/* Obstacle cassable */
					addElementToList(allocElement(1, j, (NB_UNITS_Y - 1) - i, 0, 0, 0, 0, b_o_1), &((*world).brokableObstacleList));
				}
			}
		}
	}
	fclose (level1);
}

int random_0_1(void){
    srand(time(NULL));
    return (rand()&1);
}

int random_0_4(void){
    srand(time(NULL));
    return (rand()%5);
}