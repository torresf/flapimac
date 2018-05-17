#include "display.h"

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

void drawSquare(int full) {
    GLenum primitiveType = full ? GL_QUADS : GL_LINE_LOOP;

    glBegin(primitiveType);
        glVertex2f(-0.5, -0.5);
        glVertex2f(-0.5,  0.5);
        glVertex2f( 0.5,  0.5);
        glVertex2f( 0.5, -0.5);
    glEnd();
}

/* Fonction qui dessine les Elements de la liste passée en paramètre sans valeur de retour */
void drawElements(ElementList list) {
    /* VERSION SANS TEXTURES
    int r, g, b;
    r = 0;
    g = 0;
    b = 0;
    while (list) {
        switch (list->type){
            case 0:
                b = 255;
                r = 255;
                g = 255;
                break;
            case 1:
                break;
            case 2:
                r = 255;
                break;
            case 3:
                g = 255;
                break;
            case 4:
                b = 255;
                break;
            case 5:
                r = 255;
                g = 255;
                break;
            default:
                break;
        }
        glColor3ub(r, g, b);
        glPushMatrix();
            glTranslatef(0.5 + list->x, 0.5 + list->y, 0);
            if (list->type == 0)
                glRotatef(list->speed_y * 60, 0, 0, 1);
            drawSquare(1);
        glPopMatrix();
        list = list->next;
    } */

    while (list) {
        glPushMatrix();
            glTranslatef(0.5 + list->x, 0.5 + list->y, 0);
            if (list->type == 0)
                glRotatef(list->speed_y * 60, 0, 0, 1);
            displayTexture(list->texture);
        glPopMatrix();
        list = list->next;
    }
}

/* Créer et initialise la texture avec l'image dont l'URL est passé en paramètre */
GLuint createTexture(const char* theFileName){
    SDL_Surface* surface;
    surface = IMG_Load(theFileName);
    if (surface == NULL){
        printf("Erreur lors du chargement de l'image\n");
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        surface->w,
        surface->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        surface->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(surface);
    return textureID;
}

/* Affiche la texture dont l'id est passé en paramètre */
void displayTexture(GLuint idTexture){
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glTexCoord2f(0 , 0);
        glVertex2f(-0.5 , 0.5);
        glTexCoord2f(1 , 0);
        glVertex2f(0.5, 0.5);
        glTexCoord2f(1 , 1);
        glVertex2f(0.5, -0.5);
        glTexCoord2f(0 , 1);
        glVertex2f(-0.5, -0.5);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/* Supprime la texture */
void clearTexture(GLuint* idTexture){
    glDeleteTextures(1,idTexture);
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
    if (world.enemyList && world.enemyList->missiles)
        drawElements(world.enemyList->missiles);
}