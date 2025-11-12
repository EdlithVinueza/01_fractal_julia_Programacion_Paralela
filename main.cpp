#include <iostream>
#include <complex> // permite trabajar con números complejos

#include <fmt/core.h>
#include <SFML/Graphics.hpp>

// Proviene de nuestro proyecto: fuente embebida y funciones del fractal
#include "arial.ttf.h"
#include "fractal_serial.h"


#ifdef _WIN32
#include <windows.h>
#endif

// Parámetros: límites del rectángulo complejo que vamos a muestrear
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;

// Control de iteraciones y constante c para el conjunto de Julia
int max_iterations = 100;
std::complex<double> c(-0.7, 0.27015);

// Tamaño de la imagen (ancho x alto)
#define WIDTH 1600
#define HEIGHT 900

// Buffer de píxeles (RGBA empaquetado en uint32_t). Se inicializa en main.
uint32_t *pixel_buffer = nullptr;

int main()
{
    // - inicializar el buffer de píxeles
    pixel_buffer = new uint32_t[WIDTH * HEIGHT];

    // Llamada que pinta el fractal en pixel_buffer.
    // Aquí se ve el flujo: main -> julia_serial_1 -> divergente_1
    julia_serial_1(x_min, y_min, x_max, y_max, WIDTH, HEIGHT, pixel_buffer);

    // Inicializar SFML para mostrar el resultado
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Julia Set - SFML");

#ifdef _WIN32
    // Maximizamos la ventana en Windows (opcional)
    HWND hwnd = window.getNativeHandle();
    ShowWindow(hwnd, SW_MAXIMIZE);
#endif

    // Creamos una textura SFML y le pasamos el buffer de píxeles.
    // Nota: sf::Texture::update espera un puntero a bytes; aquí se castea.
    sf::Texture texture({WIDTH, HEIGHT});
    texture.update((const uint8_t *)pixel_buffer);

    sf::Sprite sprite(texture);

    // Texto en pantalla (usa la fuente embebida en arial_ttf)
    const sf::Font font(arial_ttf, sizeof(arial_ttf));
    sf::Text text(font, "Julia Set", 24);
    text.setFillColor(sf::Color::White);
    text.setPosition({10, 10});
    text.setStyle(sf::Text::Bold);

    std::string options = "OPTIONS: [1] Serial 1";
    sf::Text textOptions(font, options, 24);
    textOptions.setFillColor(sf::Color::White);
    textOptions.setStyle(sf::Text::Bold);
    textOptions.setPosition({10, window.getView().getSize().y - 40});

    while (window.isOpen())
    {
        // Nota: la forma correcta de usar pollEvent en SFML suele ser:
        // sf::Event event; while (window.pollEvent(event)) { ... }
        // en el código actual se usa otra forma (std::optional) que puede necesitar ajuste.
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        {
            window.draw(sprite);
            window.draw(text);
            window.draw(textOptions);
        }
        window.display();
    }

    // Liberar memoria al final (buena práctica)
    delete[] pixel_buffer;
    return 0;
}
