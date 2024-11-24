#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

// Параметры камеры (орбита)
float cameraDistance = 5.0f;    // Расстояние от камеры до пирамиды
float cameraAngleX = 0.0f;      // Угол вращения по горизонтали (азимут)
float cameraAngleY = 30.0f;     // Угол вращения по вертикали
const float rotationSpeed = 2.0f; // Скорость вращения камеры

// Параметры масштабирования
float scale = 1.0f;
const float minScale = 0.5f, maxScale = 2.0f;

// Функция отрисовки пирамиды
void drawPyramid() {
    glBegin(GL_TRIANGLES);

    // Передняя грань
    glColor3f(1.0, 0.0, 0.0); // Красный
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);

    // Правая грань
    glColor3f(0.0, 1.0, 0.0); // Зеленый
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);

    // Задняя грань
    glColor3f(0.0, 0.0, 1.0); // Синий
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);

    // Левая грань
    glColor3f(1.0, 1.0, 0.0); // Желтый
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);

    glEnd();

    glBegin(GL_QUADS);
    // Основание
    glColor3f(0.5, 0.5, 0.5); // Серый
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glEnd();
}

// Обработка ввода
void handleInput() {
    // Вращение камеры
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) cameraAngleX -= rotationSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) cameraAngleX += rotationSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) cameraAngleY += rotationSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) cameraAngleY -= rotationSpeed;

    // Ограничение вертикального угла
    if (cameraAngleY > 89.0f) cameraAngleY = 89.0f;
    if (cameraAngleY < -89.0f) cameraAngleY = -89.0f;
}

// Обработка масштабирования
void handleScaling() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) scale += 0.01f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) scale -= 0.01f;

    // Ограничения масштаба
    if (scale < minScale) scale = minScale;
    if (scale > maxScale) scale = maxScale;
}

int main() {
    // Создание окна
    sf::Window window(sf::VideoMode(800, 600), "3D Pyramid", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);

    // Настройка OpenGL
    glEnable(GL_DEPTH_TEST); // Включение глубинного теста
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Основной цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Обработка ввода
        handleInput();
        handleScaling();

        // Расчёт позиции камеры
        float radX = cameraAngleX * 3.141592653589793f / 180.0f; // Угол в радианах
        float radY = cameraAngleY * 3.141592653589793f / 180.0f;
        float camX = cameraDistance * cos(radY) * sin(radX);
        float camY = cameraDistance * sin(radY);
        float camZ = cameraDistance * cos(radY) * cos(radX);

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Установка камеры
        glLoadIdentity();
        gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Применение масштабирования
        glPushMatrix();
        glScalef(scale, scale, scale);

        // Отрисовка пирамиды
        drawPyramid();

        glPopMatrix();

        // Обновление окна
        window.display();
    }

    return 0;
}
