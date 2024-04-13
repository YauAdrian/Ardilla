// Incluimos los archivos de cabecera necesarios
#include <stdarg.h.h> // Archivo de cabecera estándar de Windows
#include <iostream> // Biblioteca estándar de entrada/salida
#include <GL\glut.h> // Biblioteca GLUT de OpenGL
#include <array> // Biblioteca de matrices estándar de C++

using namespace std; // Utilizamos el espacio de nombres estándar para evitar escribir 'std::'

// Constantes
const float PI = 3.1415; // Definimos la constante PI
const float BACKGROUND_COLOR[4] = { 1.0, 1.0, 1.0, 1.0 }; // Definimos el color de fondo como blanco

// Matrices para almacenar los ángulos de las partes del cuerpo de la ardilla
array<float, 10> current_angles = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Ángulos actuales
array<float, 10> target_angles; // Ángulos objetivo
array<float, 10> prev_angles; // Ángulos previos
float animation_speed = 1.0; // Velocidad de la animación
int fps = 200; // Cuadros por segundo (frames per second)

int mode = 0; // Modo actual seleccionado
bool is_front = false; // Indica si la vista es frontal o lateral

// Declaración de funciones
void updateMode(int new_mode);
void timer(int);
void updateAngles(void);
void drawCylinder(GLfloat radius, GLfloat height);
void initEnvironment();
void displaySquirrel(void);
void KeyboardKeyPressed(unsigned char key, int x, int y);
void menu(int id);

// Función para actualizar el modo de animación
void updateMode(int new_mode)
{
    mode = new_mode; // Asignamos el nuevo modo

    switch (mode)
    {
    case 1: // Posición inicial
    {
        array<float, 10> new_target = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Definimos los ángulos objetivo
        target_angles = new_target; // Asignamos los ángulos objetivo
        animation_speed = 1.5; // Establecemos la velocidad de la animación
        break;
    }
    case 2: // Saltar
    {
        array<float, 10> new_target = { 30, 0, 0, -30, 30, -30, 30, -30, 30, -30 }; // Definimos los ángulos objetivo
        target_angles = new_target; // Asignamos los ángulos objetivo
        animation_speed = 1.5; // Establecemos la velocidad de la animación
        break;
    }
    case 3: // Cola Arriba
    {
        array<float, 10> new_target = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 30 }; // Definimos los ángulos objetivo
        target_angles = new_target; // Asignamos los ángulos objetivo
        animation_speed = 1.5; // Establecemos la velocidad de la animación
        break;
    }
    }

    prev_angles = current_angles; // Guardamos los ángulos previos

    glutPostRedisplay(); // Indicamos que se debe volver a mostrar la ventana
}

// Función del temporizador para actualizar los ángulos de las partes del cuerpo
void timer(int)
{
    updateAngles(); // Llamamos a la función para actualizar los ángulos
    glutTimerFunc(1000.0 / fps, timer, 0); // Establecemos el temporizador para llamar a la función periódicamente
}

// Función para actualizar los ángulos de las partes del cuerpo
void updateAngles(void)
{
    bool animation_finished = true; // Indicador para verificar si la animación ha terminado

    // Iteramos sobre todas las partes del cuerpo
    for (int i = 0; i < 10; i++)
    {
        // Verificamos si se ha alcanzado el ángulo objetivo para esta parte del cuerpo
        if (((target_angles[i] - prev_angles[i] >= 0) && (current_angles[i] >= target_angles[i])) ||
            ((target_angles[i] - prev_angles[i] <= 0) && (current_angles[i] <= target_angles[i])))
        {
            current_angles[i] = target_angles[i]; // Establecemos el ángulo actual como el ángulo objetivo
        }
        else
        {
            animation_finished = false; // La animación aún no ha terminado
            current_angles[i] += (target_angles[i] - prev_angles[i]) / (fps * animation_speed); // Calculamos el nuevo ángulo
        }
    }

    glutPostRedisplay(); // Indicamos que se debe volver a mostrar la ventana
}

// Función para dibujar un cilindro
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

// Función para inicializar el entorno
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

// Función para dibujar la ardilla
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
        glRotatef(20, 0.0, 1.0, 0.0); // Rotamos la vista para mostrar la ardilla de frente con un ligero ángulo
    }

    // Dibujamos el cuerpo de la ardilla
    glColor3ub(101, 67, 33); // Marrón
    glRotatef(current_angles[0], 1.0, 0.0, 0.0); // Rotamos el cuerpo de acuerdo al ángulo actual
    glPushMatrix(); // Guardamos la matriz de transformación actual
    glTranslatef(0, 0, -30); // Movemos al centro del cuerpo
    drawCylinder(10, 60); // Dibujamos el cilindro que representa el cuerpo

    // Dibujamos la cola de la ardilla
    glColor3ub(101, 67, 33); // Marrón
    glTranslatef(0, 0, 60); // Movemos al final del cuerpo
    glRotatef(current_angles[9], 1.0, 0.0, 0.0); // Rotamos la cola de acuerdo al ángulo actual
    glRotatef(-30, 1.0, 0.0, 0.0); // Ajustamos el ángulo inicial de la cola
    drawCylinder(3, 20); // Dibujamos el cilindro que representa la cola

    glPopMatrix(); // Restauramos la matriz de transformación

    // Dibujamos la cabeza de la ardilla
    glColor3ub(101, 67, 33); // Marrón
    glPushMatrix(); // Guardamos la matriz de transformación
    glTranslatef(0, 0, 30); // Movemos al centro de la cabeza
    glRotatef(current_angles[1], 1.0, 0.0, 0.0); // Rotamos la cabeza de acuerdo al ángulo actual
    glutSolidSphere(10, 16, 16); // Dibujamos una esfera que representa la cabeza

    // Dibujamos las orejas de la ardilla
    glColor3ub(101, 67, 33); // Marrón
    glPushMatrix(); // Guardamos la matriz de transformación
    glTranslatef(-8, 0, 5); // Movemos a la posición de la oreja izquierda
    glRotatef(current_angles[2], 0.0, 1.0, 0.0); // Rotamos la oreja izquierda de acuerdo al ángulo actual
    glutSolidCone(5, 15, 16, 16); // Dibujamos un cono que representa la oreja izquierda
    glPopMatrix(); // Restauramos la matriz de transformación

    glPushMatrix(); // Guardamos la matriz de transformación
    glTranslatef(8, 0, 5); // Movemos a la posición de la oreja derecha
    glRotatef(current_angles[3], 0.0, 1.0, 0.0); // Rotamos la oreja derecha de acuerdo al ángulo actual
    glutSolidCone(5, 15, 16, 16); // Dibujamos un cono que representa la oreja derecha
    glPopMatrix(); // Restauramos la matriz de transformación

    // Dibujamos los ojos de la ardilla
    glColor3ub(0, 0, 0); // Negro
    glPushMatrix(); // Guardamos la matriz de transformación
    glTranslatef(-4, 0, 5); // Movemos a la posición del ojo izquierdo
    glutSolidSphere(1, 8, 8); // Dibujamos una esfera que representa el ojo izquierdo
    glPopMatrix(); // Restauramos la matriz de transformación

    glPushMatrix(); // Guardamos la matriz de transformación
    glTranslatef(4, 0, 5); // Movemos a la posición del ojo derecho
    glutSolidSphere(1, 8, 8); // Dibujamos una esfera que representa el ojo derecho
    glPopMatrix(); // Restauramos la matriz de transformación

    glPopMatrix(); // Restauramos la matriz de transformación

    glutSwapBuffers(); // Intercambiamos los buffers para mostrar la imagen
}

// Función para manejar el teclado
void KeyboardKeyPressed(unsigned char key, int x, int y)
{
    if (key == 32) // Si se presiona la barra espaciadora
    {
        is_front = !is_front; // Cambiamos entre vista frontal y vista lateral
        glutPostRedisplay(); // Indicamos que se debe volver a mostrar la ventana
    }
}

// Función para manejar el menú
void menu(int id)
{
    if (id != 4) // Si no se selecciona 'Salir'
    {
        updateMode(id); // Actualizamos el modo de animación
    }
    else
    {
        exit(0); // Salimos del programa
    }
}

// Función principal
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Inicializamos GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Establecemos el modo de visualización
    glutInitWindowSize(500, 500); // Establecemos el tamaño de la ventana

    glutInitWindowPosition(500, 200); // Establecemos la posición de la ventana
    glutCreateWindow("Ardilla en 3D"); // Creamos la ventana con el título especificado
    glEnable(GL_DEPTH_TEST); // Habilitamos la prueba de profundidad para evitar problemas de renderizado

    glutCreateMenu(menu); // Creamos el menú
    glutAddMenuEntry("Posición Inicial", 1); // Añadimos una entrada para la posición inicial
    glutAddMenuEntry("Saltar", 2); // Añadimos una entrada para la animación de saltar
    glutAddMenuEntry("Cola Arriba", 3); // Añadimos una entrada para la animación de levantar la cola
    glutAddMenuEntry("Salir", 4); // Añadimos una entrada para salir del programa
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Adjuntamos el menú al botón derecho del ratón

    glutKeyboardFunc(KeyboardKeyPressed); // Asignamos la función para manejar el teclado
    glutDisplayFunc(displaySquirrel); // Asignamos la función para dibujar la ardilla

    initEnvironment(); // Inicializamos el entorno
    glutMainLoop(); // Entramos en el bucle principal de GLUT

    return 0; // Salimos del programa
}
