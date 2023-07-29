#pragma once
#include "Color.h"

class Point {
public:
    static Color currentColor; // Variable global que representa el color actual

    struct Vertex2 {
        int x;
        int y;
    };

    static void punto(const Vertex2& vertex) {
        // Colocar un punto del color actual en la pantalla en la coordenada (vertex.x, vertex.y)
        // Implementación específica según la biblioteca gráfica o entorno de desarrollo utilizado
        // Por ejemplo:
        // - Si estás utilizando SFML, puedes utilizar sf::RenderWindow y sf::CircleShape para dibujar el punto
        // - Si estás utilizando SDL, puedes utilizar SDL_Renderer y SDL_RenderDrawPoint para dibujar el punto
        // - Si estás utilizando una biblioteca o entorno diferente, deberás consultar su documentación para realizar el dibujo adecuado
    }
};

Color Point::currentColor; // Inicialización de la variable global currentColor fuera de la clase
