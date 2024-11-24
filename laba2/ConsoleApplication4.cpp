#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

// Углы вращения
float angleX = 0.0f;
float angleY = 0.0f;

// Текущая проекция: 0 - фронтальная, 1 - вид сверху, 2 - сбоку
int currentProjection = 0;

void setOrthographicProjection(int projection) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projection == 0) {
        // Вид спереди
        glOrtho(-1.5f, 1.5f, -1.5f, 1.5f, -5.0f, 5.0f);
    }
    else if (projection == 1) {
        // Вид сверху
        glOrtho(-1.5f, 1.5f, -1.5f, 1.5f, -5.0f, 5.0f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
    }
    else if (projection == 2) {
        // Вид сбоку
        glOrtho(-1.5f, 1.5f, -1.5f, 1.5f, -5.0f, 5.0f);
        glRotatef(90, 0.0f, 1.0f, 0.0f);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawCube() {
    glBegin(GL_QUADS);

    // Передняя грань
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Задняя грань
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Левая грань
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Правая грань
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Верхняя грань
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Нижняя грань
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
}

int main() {
    sf::Window window(sf::VideoMode(800, 600), "Cube with Orthographic Projection", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);

    glEnable(GL_DEPTH_TEST);

    bool running = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                running = false;

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up)
                    angleX -= 5.0f;
                if (event.key.code == sf::Keyboard::Down)
                    angleX += 5.0f;
                if (event.key.code == sf::Keyboard::Left)
                    angleY -= 5.0f;
                if (event.key.code == sf::Keyboard::Right)
                    angleY += 5.0f;
                if (event.key.code == sf::Keyboard::Num1)
                    currentProjection = 0; // Вид спереди
                if (event.key.code == sf::Keyboard::Num2)
                    currentProjection = 1; // Вид сверху
                if (event.key.code == sf::Keyboard::Num3)
                    currentProjection = 2; // Вид сбоку
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setOrthographicProjection(currentProjection);

        glPushMatrix();
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        drawCube();
        glPopMatrix();

        window.display();
    }

    return 0;
}
