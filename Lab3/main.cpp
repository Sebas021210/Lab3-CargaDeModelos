#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include <SDL.h>
#include "glm/gtc/matrix_transform.hpp"



SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
float rotationAngle = 0.0f;

struct Vertex {
    glm::vec3 position;
};

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;
};

std::vector<Vertex> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
std::vector<Face> faces;

bool loadOBJ(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error al abrir el archivo OBJ: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            // Leer vértices (líneas que comienzan con "v")
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({glm::vec3(x, y, z)});
        } else if (type == "vt") {
            // Leer coordenadas de textura (líneas que comienzan con "vt")
            float u, v;
            iss >> u >> v;
            uvs.push_back(glm::vec2(u, v));
        } else if (type == "vn") {
            // Leer normales (líneas que comienzan con "vn")
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            normals.push_back(glm::vec3(nx, ny, nz));
        } else if (type == "f") {
            // Leer caras (líneas que comienzan con "f")
            Face face;
            char slash;
            unsigned int vertexIndex, uvIndex, normalIndex;
            while (iss >> vertexIndex >> slash >> uvIndex >> slash >> normalIndex) {
                face.vertexIndices.push_back(vertexIndex - 1); // Los índices en OBJ comienzan desde 1
                face.uvIndices.push_back(uvIndex - 1);
                face.normalIndices.push_back(normalIndex - 1);
            }
            faces.push_back(face);
        }
    }

    file.close();
    return true;
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void render() {
    // Calcular la matriz de transformación de escala y rotación
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(10.0f), glm::vec3(0.01f)); // Escala de 0.01
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelMatrix = rotationMatrix * scaleMatrix;

    // Dibujar los triángulos del modelo OBJ
    for (const Face& face : faces) {
        for (size_t i = 0; i < face.vertexIndices.size(); i += 3) {
            unsigned int vertexIndex1 = face.vertexIndices[i];
            unsigned int vertexIndex2 = face.vertexIndices[i + 1];
            unsigned int vertexIndex3 = face.vertexIndices[i + 2];

            glm::vec3 vertex1 = vertices[vertexIndex1].position;
            glm::vec3 vertex2 = vertices[vertexIndex2].position;
            glm::vec3 vertex3 = vertices[vertexIndex3].position;

            // Aplicar la transformación de escala y rotación
            glm::vec4 transformedVertex1 = modelMatrix * glm::vec4(vertex1, 1.0f);
            glm::vec4 transformedVertex2 = modelMatrix * glm::vec4(vertex2, 1.0f);
            glm::vec4 transformedVertex3 = modelMatrix * glm::vec4(vertex3, 1.0f);

            // Reflejar en el eje Y (porque OpenGL y SDL usan coordenadas de ventana invertidas en el eje Y)
            transformedVertex1.y *= -1.0f;
            transformedVertex2.y *= -1.0f;
            transformedVertex3.y *= -1.0f;

            // Convertir las coordenadas de los vértices a espacio de pantalla
            int x1 = static_cast<int>(transformedVertex1.x * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2);
            int y1 = static_cast<int>(transformedVertex1.y * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2);

            int x2 = static_cast<int>(transformedVertex2.x * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2);
            int y2 = static_cast<int>(transformedVertex2.y * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2);

            int x3 = static_cast<int>(transformedVertex3.x * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2);
            int y3 = static_cast<int>(transformedVertex3.y * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2);

            // Dibujar el triángulo
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
            SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
        }
    }
}

int main(int argc, char** argv) {
    init();
    bool running = true;

    if (!loadOBJ("../OBJ/nave.obj")) {
        std::cerr << "Error al cargar el modelo OBJ." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear();
        const float rotationSpeed = 0.001f; // Puedes ajustar la velocidad de rotación aquí
        rotationAngle += rotationSpeed;
        render();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
