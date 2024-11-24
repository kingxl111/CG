#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// Глобальные переменные для управления точками
std::vector<sf::CircleShape> controlPoints; // Контрольные точки
std::vector<sf::Vertex> polyLine;          // Ломаная линия
float pointRadius = 5.f;                   // Радиус точек
bool dragging = false;                     // Состояние перетаскивания
int draggedIndex = -1;                     // Индекс перетаскиваемой точки

// Обновление ломаной линии
void updatePolyline() {
    polyLine.clear();
    for (const auto& point : controlPoints) {
        polyLine.emplace_back(point.getPosition() + sf::Vector2f(pointRadius, pointRadius), sf::Color::White);
    }
}

int main() {
    // Создание окна
    sf::RenderWindow window(sf::VideoMode(800, 600), "Poly-Line Editor");
    window.setFramerateLimit(60);

    // Часы для анимации
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Закрытие окна
            if (event.type == sf::Event::Closed)
                window.close();

            // Добавление точки при левом клике
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    bool pointSelected = false;
                    for (size_t i = 0; i < controlPoints.size(); ++i) {
                        if (controlPoints[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            dragging = true;
                            draggedIndex = i;
                            pointSelected = true;
                            break;
                        }
                    }
                    // Если не выбрана существующая точка, создаём новую
                    if (!pointSelected) {
                        sf::CircleShape point(pointRadius);
                        point.setPosition(sf::Vector2f(event.mouseButton.x - pointRadius, event.mouseButton.y - pointRadius));
                        point.setFillColor(sf::Color::Red);
                        controlPoints.push_back(point);
                        updatePolyline();
                    }
                }
                // Удаление точки при правом клике
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    for (auto it = controlPoints.begin(); it != controlPoints.end(); ++it) {
                        if (it->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            controlPoints.erase(it);
                            updatePolyline();
                            break;
                        }
                    }
                }
            }

            // Завершение перетаскивания
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    dragging = false;
                    draggedIndex = -1;
                }
            }

            // Перетаскивание точки
            if (event.type == sf::Event::MouseMoved && dragging) {
                if (draggedIndex != -1) {
                    controlPoints[draggedIndex].setPosition(
                        event.mouseMove.x - pointRadius,
                        event.mouseMove.y - pointRadius
                    );
                    updatePolyline();
                }
            }
        }

        // Анимация точек
        for (size_t i = 0; i < controlPoints.size(); ++i) {
            float offset = 50.f * sin(clock.getElapsedTime().asSeconds() + i);
            controlPoints[i].move(0, offset * 0.01f);
        }
        updatePolyline();

        // Отрисовка
        window.clear();
        // Рисуем ломаную линию
        if (!polyLine.empty()) {
            window.draw(&polyLine[0], polyLine.size(), sf::LineStrip);
        }
        // Рисуем точки
        for (const auto& point : controlPoints) {
            window.draw(point);
        }
        window.display();
    }

    return 0;
}
