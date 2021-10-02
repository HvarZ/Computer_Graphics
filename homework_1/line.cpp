#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>


void PrintBresenhamLine(sf::Vector2<int>& firstPoint,
                   sf::Vector2<int>& secondPoint,
                   const float sizePoint = 10.f,
                   sf::RenderWindow* window = nullptr) noexcept {
    bool steep = std::abs(secondPoint.y - firstPoint.y) > std::abs(secondPoint.x - firstPoint.x);
    window->clear();
    if (steep) {
        std::swap(firstPoint.x, firstPoint.y);
        std::swap(secondPoint.x, secondPoint.y);
    }

    if (firstPoint.x > secondPoint.x) {
        std::swap(firstPoint.x, secondPoint.x);
        std::swap(firstPoint.y, secondPoint.y);
    }

    int dx = secondPoint.x - firstPoint.x;
    int dy = std::abs(secondPoint.y - firstPoint.y);
    int error = dx / 2;
    float yStep = (firstPoint.y < secondPoint.y) ? sizePoint : -sizePoint;
    float y = firstPoint.y;

    sf::RectangleShape point;
    point.setFillColor(sf::Color::White);
    point.setSize(sf::Vector2f(sizePoint, sizePoint));
    int i = 0;

    for (auto x = firstPoint.x; x <= secondPoint.x; x += sizePoint, i++) {
        point.setPosition(steep ? y : x, steep ? x : y);
        window->draw(point);
        error -= dy;
        if (error < 0) {
            y += yStep;
            error += dx;
        }
    }
    window->display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Bresenham Line");
    window.setFramerateLimit(60);
    window.display();
    bool isClickMouse = false;

    sf::Vector2<int> point1;
    sf::Vector2<int> point2;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.waitEvent(event)) {
            if ((event.type == sf::Event::Closed) ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (!isClickMouse) {
                    point1 = sf::Mouse::getPosition(window);
                    isClickMouse = true;
                } else {
                    point2 = sf::Mouse::getPosition(window);
                    PrintBresenhamLine(point1, point2, 10, &window);
                    isClickMouse = false;
                }
            }
        }

    }
    return 0;
}
