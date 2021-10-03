#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

sf::ConvexShape CreateConvexShape(const std::vector<sf::Vector2f>& vertices) noexcept {
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


int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Cyrus Beck algorithm");
    sf::ConvexShape convexShape;

    std::vector<sf::Vector2f> vertices;
    size_t counterVertex = 0;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if ((event.type == sf::Event::Closed) ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                vertices.emplace_back(sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x),
                                                   static_cast<float>(sf::Mouse::getPosition(window).y)));
                counterVertex++;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && counterVertex > 2) {
                convexShape = CreateConvexShape(vertices);
                vertices.clear();
            }
        }

        window.clear();
        window.draw(convexShape);
        window.display();
    }

    return EXIT_SUCCESS;
}
