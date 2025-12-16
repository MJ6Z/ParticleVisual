#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "mylib/projections.h"

/*
    ORBIT STRUCT
    a  = semi-major axis
    e  = eccentricity (0 = circle)
    mu = GM
*/
struct Orbit {
    float a;
    float e;
    float mu;
    float phase;
    Vec3 tilt;
    sf::Color color;
};

/*
    Solve Kepler's Equation about the origin.
        M = E - e sin(E)

    using Newton-Raphson method.
    This enforces Kepler's 2nd Law!.
*/
float solveKepler(float M, float e) {
    float E = M; // initial guess
    for (int i = 0; i < 5; ++i)
        E -= (E - e * std::sin(E) - M) / (1 - e * std::cos(E));
    return E;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Keplerian Orbits");
    window.setFramerateLimit(60);

    sf::Clock clock;

    // Camera
    float zoom = 1.0f;
    Vec3 angle = {0.0f, 0.0f, 0.0f};
    const float stepAngle = 0.1f;

    //Init some example Keplerian orbits.
    std::vector<Orbit> orbits = {
        //a, e, mu, phase, {x,y,z}, colour
        {1.0f, 0.9f, 1.0f, 0.0f, {0.0f, 0.0f, 0.0f}, sf::Color::White},
        {1.8f, 0.4f, 1.0f, 1.0f, {0.4f, 0.0f, 0.0f}, sf::Color::Cyan},
        {2.8f, 0.6f, 1.0f, 2.0f, {0.0f, 0.5f, 0.0f}, sf::Color::Yellow}
    };

    Vec3 origin = {0.0f, 0.0f, 0.0f};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelScrolled) {
                zoom += event.mouseWheelScroll.delta * 0.1f;
                if (zoom < 0.1f) zoom = 0.1f;
            }
        }

        // Camera rotation
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  angle.y -= stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) angle.y += stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    angle.x -= stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  angle.x += stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))     angle.z -= stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))     angle.z += stepAngle;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;

        float t = clock.getElapsedTime().asSeconds();

        window.clear(sf::Color::Black);

        // Draw axes
        sf::Vertex axes[] = {
            { projections::eulerAngle(origin, angle, zoom), sf::Color::Red },
            { projections::eulerAngle({1,0,0}, angle, zoom), sf::Color::Red },
            { projections::eulerAngle(origin, angle, zoom), sf::Color::Green },
            { projections::eulerAngle({0,1,0}, angle, zoom), sf::Color::Green },
            { projections::eulerAngle(origin, angle, zoom), sf::Color::Blue },
            { projections::eulerAngle({0,0,1}, angle, zoom), sf::Color::Blue }
        };
        window.draw(axes, 6, sf::Lines);

        for (const auto& orbit : orbits) {

            //Average angular velocity (mean motion) n = sqrt(mu / a^3)
            float n = std::sqrt(orbit.mu / (orbit.a * orbit.a * orbit.a));

            //Mean anomaly M
            float M = n * t + orbit.phase;

            //Solve Kepler's equation to get eccentric anomaly E from the (circular) mean anomaly.
            float E = solveKepler(M, orbit.e);

            /*
                Kepler's 1st Law:
                Parametric ellipse equations:
                    x = a (cos E - e)           /The focus is at x=0
                    y = a sqrt(1 - e^2)sin(E)
            */

            float x = orbit.a * (std::cos(E) - orbit.e);
            float y = orbit.a * std::sqrt(1 - orbit.e * orbit.e) * std::sin(E);

            Vec3 planetPos = {x, y, 0.0f};

            // Draw orbit path (static ellipse)
            std::vector<sf::Vertex> path;
            for (int i = 0; i <= 300; ++i) {
                float Ei = 2.0f * M_PI * i / 300.0f;
                float px = orbit.a * (std::cos(Ei) - orbit.e);
                float py = orbit.a * std::sqrt(1 - orbit.e * orbit.e) * std::sin(Ei);
                Vec3 p = {px, py, 0.0f};

                path.emplace_back(
                    projections::eulerAngle(p, angle + orbit.tilt, zoom),
                    sf::Color(100, 100, 100)
                );
            }
            window.draw(path.data(), path.size(), sf::LineStrip);

            // Draw planet
            sf::Vector2f screen =
                projections::eulerAngle(planetPos, angle + orbit.tilt, zoom);

            sf::CircleShape planet(5.0f);
            planet.setOrigin(5.0f, 5.0f);
            planet.setFillColor(orbit.color);
            planet.setPosition(screen);
            window.draw(planet);
        }

        window.display();
    }

    return 0;
}
