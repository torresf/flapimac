#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pam.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 600;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombres de cases visibles */
static unsigned int NB_UNITS_X = 30;
static unsigned int NB_UNITS_Y = 20;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0., (float)NB_UNITS_X, 0., (float)NB_UNITS_Y);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

void drawLandmark(){
    int i;
    glPointSize(4.); 
    glBegin(GL_POINTS);
        glColor3ub(255, 0, 0);
        for (i = 0; i < NB_UNITS_X; i++){
            glVertex2f(NB_UNITS_X - i, 0);
        }
        glColor3ub(0, 255, 0);
        for (i = 0; i < NB_UNITS_Y; i++){
            glVertex2f(0, NB_UNITS_Y - i);
        }
    glEnd();
}

int main(int argc, char** argv) {

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

    int loop = 1;

    /* Boucle d'affichage */
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Code de dessin */

        glClear(GL_COLOR_BUFFER_BIT); // Toujours commencer par clear le buffer

        // Dessin

        drawLandmark();

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

                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);

                    switch(e.key.keysym.sym) {

                        case SDLK_q:
                            loop = 0;
                            break;

                        case SDLK_SPACE:
                            // Déclenchement du tir
                            printf("Tir\n");
                            break;

                        default:
                            break;
                    }

                    break;

                case SDL_KEYUP:
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

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}