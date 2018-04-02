#include "display.h"
#include "main.h"
#include "struct.h"

void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0., (float)NB_UNITS_X, 0., (float)NB_UNITS_Y);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

int main(int argc, char** argv){

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    
    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("Flapimac", NULL);
    resizeViewport();

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ElementList playerList;
    ElementList obstacleList;
    ElementList enemyList;
    ElementList bonusList;

    playerList = NULL;
    obstacleList = NULL;
    enemyList = NULL;
    bonusList = NULL;

    /* map loading */
    FILE *level1;
    /* opening file for reading */
    level1 = fopen("levels/level1.ppm" , "r");
    if (level1 == NULL) {
        perror("Error opening file");
        return(-1);
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
                            addElementToList(allocElement(0, c_index, (NB_UNITS_Y - 1) - l_index), &playerList);
                        }
                        if (r == 0 && g == 0 && b == 0) {
                            /* Obstacle */
                            addElementToList(allocElement(1, c_index, (NB_UNITS_Y - 1) - l_index), &obstacleList);
                        }
                        if (r == 255 && g == 0 && b == 0) {
                            /* Ennemi */
                            addElementToList(allocElement(2, c_index, (NB_UNITS_Y - 1) - l_index), &enemyList);
                        }
                        if (r == 0 && g == 255 && b == 0) {
                            /* Ennemi */
                            addElementToList(allocElement(3, c_index, (NB_UNITS_Y - 1) - l_index), &bonusList);
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

        // Déplacement du joueur
        if (player_status == 1 && playerList->y < NB_UNITS_Y - 1)
            playerList->y +=.1;
        else if (player_status == -1 && playerList->y > 0)
            playerList->y -=.1;

        drawElements(playerList); // Dessin des joueurs
        glPushMatrix();
            glTranslatef(x_move-=.1, 0, 0);
            drawElements(obstacleList); // Dessin des obstacles
            drawElements(enemyList); // Dessin des ennemis
            drawElements(bonusList); // Dessin des bonus
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

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    // coordonnée x : e.button.x 
                    // coordonnée y : e.button.y 
                    break;

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
                            if (player_status == 1)
                                player_status = 0;
                            break;

                        case SDLK_DOWN:
                            if (player_status == -1)
                                player_status = 0;
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

    deleteElements(&playerList);
    deleteElements(&obstacleList);
    deleteElements(&enemyList);
    deleteElements(&bonusList);

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}