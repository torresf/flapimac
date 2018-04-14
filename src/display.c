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
    }
}