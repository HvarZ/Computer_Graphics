#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <cmath>

auto CreateConvexShape(const std::vector<sf::Vector2f>& vertices) noexcept -> sf::ConvexShape {
    sf::ConvexShape convexShape;
    convexShape.setPointCount(vertices.size());

    for (size_t i = 0; i < convexShape.getPointCount(); ++i) {
        convexShape.setPoint(i, vertices[i]);
    }

    convexShape.setFillColor(sf::Color::Black);
    convexShape.setOutlineThickness(10.f);
    convexShape.setOutlineColor(sf::Color::White);

    return convexShape;
}

auto CreateLine(const std::array<sf::Vector2f, 2>& vertices) noexcept -> sf::RectangleShape {
    sf::RectangleShape line;
    line.setFillColor(sf::Color::White);
    line.setSize(sf::Vector2f(std::sqrt(std::pow(vertices[1].y - vertices[0].y, 2) +
                                       std::pow(vertices[1].x - vertices[0].x, 2)),
                                    10));
    line.setPosition(vertices[0].x, vertices[0].y);
    if (vertices[0].x < vertices[1].x) {
        line.setRotation((180 / static_cast<float>(M_PI)) *
                         std::atan((vertices[1].y - vertices[0].y) / (vertices[1].x - vertices[0].x)));
    } else {
        line.setRotation(180 + (180 / static_cast<float>(M_PI)) *
                         std::atan((vertices[1].y - vertices[0].y) / (vertices[1].x - vertices[0].x)));
    }
    return line;
}

auto Dot(const sf::Vector2i& point0, const sf::Vector2i& point1) noexcept -> int {
    return point0.x * point1.x + point0.y * point1.y;
}



int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Cyrus Beck algorithm");
    window.setFramerateLimit(60);
    sf::ConvexShape convexShape;
    sf::RectangleShape line;

    std::vector<sf::Vector2f> vertices;
    std::array<sf::Vector2f, 2> lineVertices;

    size_t counterVertex = 0;
    bool isConvexCreating = true;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if ((event.type == sf::Event::Closed) ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && isConvexCreating) {
                vertices.emplace_back(sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x),
                                                   static_cast<float>(sf::Mouse::getPosition(window).y)));
                counterVertex++;
            }

            else if (event.type == sf::Event::MouseButtonPressed && !isConvexCreating && counterVertex < 2) {
                lineVertices[counterVertex] = sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x),
                                                       static_cast<float>(sf::Mouse::getPosition(window).y));
                counterVertex++;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && counterVertex > 2 && isConvexCreating) {
                convexShape = CreateConvexShape(vertices);
                vertices.clear();
                isConvexCreating = false;
                counterVertex = 0;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && !isConvexCreating) {
                line = CreateLine(lineVertices);
            }
        }

        window.clear();
        window.draw(convexShape);
        window.draw(line);
        window.display();
    }

    return EXIT_SUCCESS;
}
