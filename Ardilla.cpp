// Incluimos los archivos de cabecera necesarios
#include <stdarg.h.h> // Archivo de cabecera est�ndar de Windows
#include <iostream> // Biblioteca est�ndar de entrada/salida
#include <GL\glut.h> // Biblioteca GLUT de OpenGL
#include <array> // Biblioteca de matrices est�ndar de C++

using namespace std; // Utilizamos el espacio de nombres est�ndar para evitar escribir 'std::'

// Constantes
const float PI = 3.1415; // Definimos la constante PI
const float BACKGROUND_COLOR[4] = { 1.0, 1.0, 1.0, 1.0 }; // Definimos el color de fondo como blanco

// Matrices para almacenar los �ngulos de las partes del cuerpo de la ardilla
array<float, 10> current_angles = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // �ngulos actuales
array<float, 10> target_angles; // �ngulos objetivo
array<float, 10> prev_angles; // �ngulos previos
float animation_speed = 1.0; // Velocidad de la animaci�n
int fps = 200; // Cuadros por segundo (frames per second)

int mode = 0; // Modo actual seleccionado
bool is_front = false; // Indica si la vista es frontal o lateral

// Declaraci�n de funciones
void updateMode(int new_mode);
void timer(int);
void updateAngles(void);
void drawCylinder(GLfloat radius, GLfloat height);
void initEnvironment();
void displaySquirrel(void);
void KeyboardKeyPressed(unsigned char key, int x, int y);
void menu(int id);

// Funci�n para actualizar el modo de animaci�n
void updateMode(int new_mode)
{
    mode = new_mode; // Asignamos el nuevo modo

    switch (mode)
    {
    case 1: // Posici�n inicial
    {
        array<float, 10> new_target = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Definimos los �ngulos objetivo
        target_angles = new_target; // Asignamos los �ngulos objetivo
        animation_speed = 1.5; // Establecemos la velocidad de la animaci�n
        break;
    }
    case 2: // Saltar
    {
        array<float, 10> new_target = { 30, 0, 0, -30, 30, -30, 30, -30, 30, -30 }; // Definimos los �ngulos objetivo
        target_angles = new_target; // Asignamos los �ngulos objetivo
        animation_speed = 1.5; // Establecemos la velocidad de la animaci�n
        break;
    }
    case 3: // Cola Arriba
    {
        array<float, 10> new_target = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 30 }; // Definimos los �ngulos objetivo
        target_angles = new_target; // Asignamos los �ngulos objetivo
        animation_speed = 1.5; // Establecemos la velocidad de la animaci�n
        break;
    }
    }

    prev_angles = current_angles; // Guardamos los �ngulos previos

    glutPostRedisplay(); // Indicamos que se debe volver a mostrar la ventana
}

// Funci�n del temporizador para actualizar los �ngulos de las partes del cuerpo
void timer(int)
{
    updateAngles(); // Llamamos a la funci�n para actualizar los �ngulos
    glutTimerFunc(1000.0 / fps, timer, 0); // Establecemos el temporizador para llamar a la funci�n peri�dicamente
}

// Funci�n para actualizar los �ngulos de las partes del cuerpo
void updateAngles(void)
{
    bool animation_finished = true; // Indicador para verificar si la animaci�n ha terminado

    // Iteramos sobre todas las partes del cuerpo
    for (int i = 0; i < 10; i++)
    {
        // Verificamos si se ha alcanzado el �ngulo objetivo para esta parte del cuerpo
        if (((target_angles[i] - prev_angles[i] >= 0) && (current_angles[i] >= target_angles[i])) ||
            ((target_angles[i] - prev_angles[i] <= 0) && (current_angles[i] <= target_angles[i])))
        {
            current_angles[i] = target_angles[i]; // Establecemos el �ngulo actual como el �ngulo objetivo
        }
        else
        {
            animation_finished = false; // La animaci�n a�n no ha terminado
            current_angles[i] += (target_angles[i] - prev_angles[i]) / (fps * animation_speed); // Calculamos el nuevo �ngulo
        }
    }

    glutPostRedisplay(); // Indicamos que se debe volver a mostrar la ventana
}

// Funci�n para dibujar un cilindro
void drawCylinder(GLfloat radius, GLfloat height)
{
    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat angle = 0.0;
    GLfloat angle_stepsize = 0.1;

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while (angle < 2 * PI)
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glVertex3f(x, y, 0.0);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);
    glVertex3f(radius, 0.0, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
    angle = 0.0;
    while (angle < 2 * PI)
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);
    glEnd();
}

// Funci�n para inicializar el entorno
void initEnvironment()
{
    glEnable(GL_BLEND); // Habilitamos la mezcla de colores
    glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], BACKGROUND_COLOR[2]); // Establecemos el color de fondo
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);

    updateMode(1); // Establecemos el modo inicial
    timer(0); // Iniciamos el temporizador
}

// Funci�n para dibujar la ardilla
void displaySquirrel(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (!is_front)
    {
        glRotatef(90, 0.0, 1.0, 0.0); // Rotamos la vista para mostrar el perfil de la ardilla
    }
    else
    {
        glRotatef(20, 0.0, 1.0, 0.0); // Rotamos la vista para mostrar la ardilla de frente con un ligero �ngulo
    }

    // Dibujamos el cuerpo de la ardilla
    glColor3ub(101, 67, 33); // Marr�n
    glRotatef(current_angles[0], 1.0, 0.0, 0.0); // Rotamos el cuerpo de acuerdo al �ngulo actual
    glPushMatrix(); // Guardamos la matriz de transformaci�n actual
    glTranslatef(0, 0, -30); // Movemos al centro del cuerpo
    drawCylinder(10, 60); // Dibujamos el cilindro que representa el cuerpo

    // Dibujamos la cola de la ardilla
    glColor3ub(101, 67, 33); // Marr�n
    glTranslatef(0, 0, 60); // Movemos al final del cuerpo
    glRotatef(current_angles[9], 1.0, 0.0, 0.0); // Rotamos la cola de acuerdo al �ngulo actual
    glRotatef(-30, 1.0, 0.0, 0.0); // Ajustamos el �ngulo inicial de la cola
    drawCylinder(3, 20); // Dibujamos el cilindro que representa la cola

    glPopMatrix(); // Restauramos la matriz de transformaci�n

    // Dibujamos la cabeza de la ardilla
    glColor3ub(101, 67, 33); // Marr�n
    glPushMatrix(); // Guardamos la matriz de transformaci�n
    glTranslatef(0, 0, 30); // Movemos al centro de la cabeza
    glRotatef(current_angles[1], 1.0, 0.0, 0.0); // Rotamos la cabeza de acuerdo al �ngulo actual
    glutSolidSphere(10, 16, 16); // Dibujamos una esfera que representa la cabeza

    // Dibujamos las orejas de la ardilla
    glColor3ub(101, 67, 33); // Marr�n
    glPushMatrix(); // Guardamos la matriz de transformaci�n
    glTranslatef(-8, 0, 5); // Movemos a la posici�n de la oreja izquierda
    glRotatef(current_angles[2], 0.0, 1.0, 0.0); // Rotamos la oreja izquierda de acuerdo al �ngulo actual
    glutSolidCone(5, 15, 16, 16); // Dibujamos un cono que representa la oreja izquierda
    glPopMatrix(); // Restauramos la matriz de transformaci�n

    glPushMatrix(); // Guardamos la matriz de transformaci�n
    glTranslatef(8, 0, 5); // Movemos a la posici�n de la oreja derecha
    glRotatef(current_angles[3], 0.0, 1.0, 0.0); // Rotamos la oreja derecha de acuerdo al �ngulo actual
    glutSolidCone(5, 15, 16, 16); // Dibujamos un cono que representa la oreja derecha
    glPopMatrix(); // Restauramos la matriz de transformaci�n

    // Dibujamos los ojos de la ardilla
    glColor3ub(0, 0, 0); // Negro
    glPushMatrix(); // Guardamos la matriz de transformaci�n
    glTranslatef(-4, 0, 5); // Movemos a la posici�n del ojo izquierdo
    glutSolidSphere(1, 8, 8); // Dibujamos una esfera que representa el ojo izquierdo
    glPopMatrix(); // Restauramos la matriz de transformaci�n

    glPushMatrix(); // Guardamos la matriz de transformaci�n
    glTranslatef(4, 0, 5); // Movemos a la posici�n del ojo derecho
    glutSolidSphere(1, 8, 8); // Dibujamos una esfera que representa el ojo derecho
    glPopMatrix(); // Restauramos la matriz de transformaci�n

    glPopMatrix(); // Restauramos la matriz de transformaci�n

    glutSwapBuffers(); // Intercambiamos los buffers para mostrar la imagen
}

// Funci�n para manejar el teclado
void KeyboardKeyPressed(unsigned char key, int x, int y)
{
    if (key == 32) // Si se presiona la barra espaciadora
    {
        is_front = !is_front; // Cambiamos entre vista frontal y vista lateral
        glutPostRedisplay(); // Indicamos que se debe volver a mostrar la ventana
    }
}

// Funci�n para manejar el men�
void menu(int id)
{
    if (id != 4) // Si no se selecciona 'Salir'
    {
        updateMode(id); // Actualizamos el modo de animaci�n
    }
    else
    {
        exit(0); // Salimos del programa
    }
}

// Funci�n principal
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Inicializamos GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Establecemos el modo de visualizaci�n
    glutInitWindowSize(500, 500); // Establecemos el tama�o de la ventana

    glutInitWindowPosition(500, 200); // Establecemos la posici�n de la ventana
    glutCreateWindow("Ardilla en 3D"); // Creamos la ventana con el t�tulo especificado
    glEnable(GL_DEPTH_TEST); // Habilitamos la prueba de profundidad para evitar problemas de renderizado

    glutCreateMenu(menu); // Creamos el men�
    glutAddMenuEntry("Posici�n Inicial", 1); // A�adimos una entrada para la posici�n inicial
    glutAddMenuEntry("Saltar", 2); // A�adimos una entrada para la animaci�n de saltar
    glutAddMenuEntry("Cola Arriba", 3); // A�adimos una entrada para la animaci�n de levantar la cola
    glutAddMenuEntry("Salir", 4); // A�adimos una entrada para salir del programa
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Adjuntamos el men� al bot�n derecho del rat�n

    glutKeyboardFunc(KeyboardKeyPressed); // Asignamos la funci�n para manejar el teclado
    glutDisplayFunc(displaySquirrel); // Asignamos la funci�n para dibujar la ardilla

    initEnvironment(); // Inicializamos el entorno
    glutMainLoop(); // Entramos en el bucle principal de GLUT

    return 0; // Salimos del programa
}
