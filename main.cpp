#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

//3D cartesian coordinate axes.
struct Vec3 { float x, y, z; };

//parametrically defined function, evaluated later. Will be replaced with particle track later.
Vec3 trajectory(float t) {
    return { std::cos(t), std::sin(t), std::sin(t)*std::sin(t)};
}

//Project the 3D function onto 2D based on where the camera is.
sf::Vector2f projectRotated(const Vec3 &p, float angleX, float angleY, float angleZ, float zoom) {
    float cosX = std::cos(angleX), sinX = std::sin(angleX);
    float y1 = cosX * p.y - sinX * p.z;
    float z1 = sinX * p.y + cosX * p.z;

    float cosY = std::cos(angleY), sinY = std::sin(angleY);
    float x2 = cosY * p.x + sinY * z1;
    float z2 = -sinY * p.x + cosY * z1;

    float cosZ = std::cos(angleZ), sinZ = std::sin(angleZ);
    float x3 = cosZ * x2 - sinZ * y1;
    float y3 = sinZ * x2 + cosZ * y1;

    return sf::Vector2f(x3 * 200 * zoom + 400, y3 * 200 * zoom + 300);
}

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Trajectory plot");
    window.setFramerateLimit(60);

    //evaluate previously defined function.
    std::vector<Vec3> points;
    for (float t = 0; t < 1000; t += 0.01f) points.push_back(trajectory(t));

    //initial camera conditions.
    float zoom = 1.0f;
    float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
    const float rotStep = 0.1f;

    //origins. May be useful to change origin later.
    Vec3 origin = {0.0f, 0.0f, 0.0f};
    Vec3 axisX = {1.0f, 0.0f, 0.0f};
    Vec3 axisY = {0.0f, 1.0f, 0.0f};
    Vec3 axisZ = {0.0f, 0.0f, 1.0f};

    //Watch for keystrokes and scrolls.
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseWheelScrolled) {
                zoom += event.mouseWheelScroll.delta * 0.1f;
                if (zoom < 0.1f) zoom = 0.1f;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  angleY -= rotStep;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) angleY += rotStep;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    angleX -= rotStep;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  angleX += rotStep;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))     angleZ -= rotStep;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))     angleZ += rotStep;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))     break;

        window.clear(sf::Color::Black);

        //Place lines.
        std::vector<sf::Vertex> line;
        for (auto &p : points) {
            line.emplace_back(projectRotated(p, angleX, angleY, angleZ, zoom), sf::Color::White);
        }
        if (!line.empty()) window.draw(&line[0], line.size(), sf::LineStrip);

        // Draw coordinate axes
        sf::Vertex xAxis[] = {
            sf::Vertex(projectRotated(origin, angleX, angleY, angleZ, zoom), sf::Color::Red),
            sf::Vertex(projectRotated(axisX, angleX, angleY, angleZ, zoom), sf::Color::Red)
        };
        sf::Vertex yAxis[] = {
            sf::Vertex(projectRotated(origin, angleX, angleY, angleZ, zoom), sf::Color::Green),
            sf::Vertex(projectRotated(axisY, angleX, angleY, angleZ, zoom), sf::Color::Green)
        };
        sf::Vertex zAxis[] = {
            sf::Vertex(projectRotated(origin, angleX, angleY, angleZ, zoom), sf::Color::Blue),
            sf::Vertex(projectRotated(axisZ, angleX, angleY, angleZ, zoom), sf::Color::Blue)
        };
        window.draw(xAxis, 2, sf::Lines);
        window.draw(yAxis, 2, sf::Lines);
        window.draw(zAxis, 2, sf::Lines);

        window.display();
    }

    return 0;
}
