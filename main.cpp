#include<cmath>
#include<vector>
#include<iostream>
#include<SFML/Graphics.hpp>

#include "mylib/projections.h"


//parametrically defined function, evaluated later. Will be replaced with particle track later.
Vec3 trajectory(float t) {
    return { 0.1f*t*std::cos(6*t), 0.1f*t*std::sin(6*t), 0.05f*t};
}

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Trajectory plot");
    window.setFramerateLimit(60);

    //evaluate previously defined function.
    std::vector<Vec3> points;
    for (float t = 0; t < 10; t += 0.01f) points.push_back(trajectory(t));

    //initial camera conditions.
    float zoom = 1.0f;
    Vec3 angle = {0.0f,0.0f,0.0f}; //theta_x,y,z.
    const float stepAngle = 0.1f; //step in rads.

    //origins. May be useful to change origin later.
    Vec3 origin = {0.0f, 0.0f, 0.0f};
    //axes unit vectors.
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  angle.y -= stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) angle.y += stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    angle.x -= stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  angle.x += stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))     angle.z -= stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))     angle.z += stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))     break;

        window.clear(sf::Color::Black);

        //Place lines.
        std::vector<sf::Vertex> line;
        for (auto &p : points) {
            line.emplace_back(projections::eulerAngle(p, angle, zoom), sf::Color::White);
        }
        if (!line.empty()) window.draw(&line[0], line.size(), sf::LineStrip);




        // Draw coordinate axes
        sf::Vertex xAxis[] = {
            sf::Vertex(projections::eulerAngle(origin, angle, zoom), sf::Color::Red),
            sf::Vertex(projections::eulerAngle(axisX, angle, zoom), sf::Color::Red)
        };
        sf::Vertex yAxis[] = {
            sf::Vertex(projections::eulerAngle(origin, angle, zoom), sf::Color::Green),
            sf::Vertex(projections::eulerAngle(axisY, angle, zoom), sf::Color::Green)
        };
        sf::Vertex zAxis[] = {
            sf::Vertex(projections::eulerAngle(origin, angle, zoom), sf::Color::Blue),
            sf::Vertex(projections::eulerAngle(axisZ, angle, zoom), sf::Color::Blue)
        };
        window.draw(xAxis, 2, sf::Lines);
        window.draw(yAxis, 2, sf::Lines);
        window.draw(zAxis, 2, sf::Lines);

        window.display();
    }

    return 0;
}
