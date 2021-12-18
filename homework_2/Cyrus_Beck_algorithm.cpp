#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>

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

auto CreateLine(const std::array<sf::Vector2f, 2> &vertices,
                sf::Color color) noexcept -> sf::RectangleShape {
  sf::RectangleShape line;
  line.setFillColor(color);
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

auto Dot(const sf::Vector2f& point0, const sf::Vector2f& point1) noexcept ->
    int {
    return point0.x * point1.x + point0.y * point1.y;
}

auto CyrusBeck(const sf::ConvexShape& convex, std::array<sf::Vector2f, 2>& line)
        noexcept -> sf::RectangleShape {
  const auto numberPoints = convex.getPointCount();
  std::vector<sf::Vector2f> normals(numberPoints);
  for (int i = 0; i < numberPoints; i++) {
    normals[i].y =
        convex.getPoint((i + 1) % numberPoints).x - convex.getPoint(i).x;
    normals[i].x =
        convex.getPoint(i).y - convex.getPoint((i + 1) % numberPoints).y;
  }


  sf::Vector2f P1_P0{line[1].x - line[0].x,
                     line[1].y - line[0].y};

  std::vector<sf::Vector2f> P0_PEi(numberPoints);

  for (int i = 0; i < numberPoints; i++) {
    P0_PEi[i].x
        = convex.getPoint(i).x - line[0].x;
    P0_PEi[i].y
        = convex.getPoint(i).y - line[0].y;
  }

  std::vector<int> numerator(numberPoints), denominator(numberPoints);

  for (int i = 0; i < numberPoints; i++) {
    numerator[i] = Dot(normals[i], P0_PEi[i]);
    denominator[i] = Dot(normals[i], P1_P0);
  }

  std::vector<float> t(numberPoints);

  std::vector<float> tE, tL;

  for (int i = 0; i < numberPoints; i++) {

    t[i] = (float)(numerator[i]) / (float)(denominator[i]);

    if (denominator[i] > 0)
      tE.push_back(t[i]);
    else
      tL.push_back(t[i]);
  }

  float temp[2];

  tE.push_back(0.f);
  temp[0] = *std::max_element(tE.begin(), tE.end());

  tL.push_back(1.f);
  temp[1] = *std::min_element(tL.begin(), tL.end());

  std::vector<sf::Vector2f> newLine(2);
  newLine[0].x
      = (float)line[0].x
        + (float)P1_P0.x * (float)temp[0];
  newLine[0].y
      = (float)line[0].y
        + (float)P1_P0.y * (float)temp[0];
  newLine[1].x
      = (float)line[0].x
        + (float)P1_P0.x * (float)temp[1];
  newLine[1].y
      = (float)line[0].y
        + (float)P1_P0.y * (float)temp[1];

  return CreateLine(std::array<sf::Vector2f, 2>{newLine[0], newLine[1]},
                    sf::Color::Green);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Cyrus Beck algorithm");
    window.setFramerateLimit(60);
    sf::ConvexShape convexShape;
    sf::RectangleShape line;
    sf::RectangleShape clipLine;
    clipLine.setFillColor(sf::Color::Green);

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

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter
                && counterVertex > 2 && isConvexCreating) {
                convexShape = CreateConvexShape(vertices);
                vertices.clear();
                isConvexCreating = false;
                counterVertex = 0;
            }
            else if (event.type == sf::Event::KeyPressed &&
                     event.key.code == sf::Keyboard::Enter && !isConvexCreating) {
                line = CreateLine(lineVertices, sf::Color::White);
            } else if (event.type == sf::Event::KeyPressed &&
                     event.key.code == sf::Keyboard::C) {
                clipLine = CyrusBeck(convexShape, lineVertices);
            }
        }

        window.clear();
        window.draw(convexShape);
        window.draw(line);
        window.draw(clipLine);
        window.display();
    }

    return EXIT_SUCCESS;
}
