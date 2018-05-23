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
void loadLevel(World *world, int chosen_level) {
	FILE *level;
		
	char level_name[64];
	sprintf(level_name, "levels/level%d.ppm", chosen_level);
	
	/* Ouverture du fichier de niveau pour la lecture */
	level = fopen(level_name, "r");
	
	if (level == NULL) {
		perror("Error opening file");
		exit(0);
	}

	/* Load textures */

	/* Textures éléments */
    GLuint ennemi = createRGBATexture("textures/main/ennemi.png");
    GLuint player = createRGBATexture("textures/main/avion_fat.png");
    GLuint bonus = createRGBATexture("textures/main/bonus.png");

    /* Textures obstacles */
	char obstacle1_path[64];
	char obstacle2_path[64];
	sprintf(obstacle1_path, "textures/level_%d/obstacle_cassable_1.png", chosen_level);
	sprintf(obstacle2_path, "textures/level_%d/obstacle_normal_1.png", chosen_level);

    GLuint finish = createRGBATexture("textures/main/finish.png");
    GLuint b_o_1 = createRGBATexture(obstacle1_path);
    GLuint n_o_1 = createRGBATexture(obstacle2_path);

    /* Traitement du fichier ppm */
	char line[64];
	int r, g, b;
	int width, j, height, i, color_max;

	/* 
	 * Première ligne du fichier 
	 * <type>
	*/
	if (fgets(line, sizeof line, level) == NULL)
		exit(0);

	if (strcmp(line, "P3\n") != 0) {
		printf("Erreur : Format du fichier non pris en charge.\n");
		exit(0);
	}

	/* 
	 * Deuxième ligne du fichier 
	 * <width> <height>
	*/
	if (fgets(line, sizeof line, level) == NULL)
		exit(0);
	if (line[0] == '#')
	{
		if (fgets(line, sizeof line, level) == NULL)
			exit(0);
	}
	sscanf(line, "%d %d", &width, &height);

	/* 
	 * Troisième ligne du fichier 
	 * <color_max>
	*/
	if (fgets(line, sizeof line, level) == NULL)
		exit(0);
	sscanf(line, "%d", &color_max);

	for (i = 0; i < height; ++i) {	
		for (j = 0; j < width; ++j)	{
			if (fgets(line, sizeof line, level) == NULL)
				exit(0);
			sscanf(line, "%d", &r);
			if (fgets(line, sizeof line, level) == NULL)
				exit(0);
			sscanf(line, "%d", &g);
			if (fgets(line, sizeof line, level) == NULL)
					exit(0);
			sscanf(line, "%d", &b);
			
			if (r != 255 || g != 255 || b != 255) {
				if (r == 0 && g == 0 && b == 255) {
					/* Joueur */
					float player_width = 1.31;
					float player_height = 0.89;

					addElementToList(allocElement(0, j, (NB_UNITS_Y - (1-(1-player_height)/2)) - i, player_width, player_height, 0.10, 0, 5, 30, player), &((*world).player));
				}
				if (r == 0 && g == 0 && b == 0) {
					/* Obstacle */
					addElementToList(allocElement(1, j, (NB_UNITS_Y - 1) - i, 1, 1, 0, 0, 0, 0, n_o_1), &((*world).obstacleList));
				}
				if (r == 255 && g == 0 && b == 0) {
					/* Ennemi */
					addElementToList(allocElement(2, j, (NB_UNITS_Y - 1) - i, 1, 1, 0, 0.04, -4, 30, ennemi), &((*world).enemyList));
				}
				if (r == 0 && g == 255 && b == 0) {
					/* Bonus */
					addElementToList(allocElement(3, j, (NB_UNITS_Y - 1) - i, 1, 1, 0, 0.1, 0, 0, bonus), &((*world).bonusList));
				}
				if (r == 255 && g == 255 && b == 0) {
					/* Ligne d'arrivée */
					addElementToList(allocElement(5, j, (NB_UNITS_Y - 1) - i, 1, 1, 0, 0, 0, 0, finish), &((*world).finishLineList));
				}
				if (r == 0 && g == 255 && b == 255) {
					/* Obstacle cassable */
					addElementToList(allocElement(1, j, (NB_UNITS_Y - 1) - i, 1, 1, 0, 0, 0, 0, b_o_1), &((*world).brokableObstacleList));
				}
			}
		}
	}
	fclose (level);
}

int random_0_1(void){
    srand(time(NULL));
    return (rand()&1);
}

int random_0_4(void){
    srand(time(NULL));
    return (rand()%5);
}