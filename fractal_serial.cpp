#include "fractal_serial.h"

#include <complex>

// variables externas definidas en main.cpp
extern int max_iterations; // número máximo de iteraciones antes de declarar "no divergente"
extern std::complex<double> c; // constante 'c' del conjunto de Julia

// Esta función calcula si el punto complejo z0 diverge bajo la iteración z_{n+1} = z_n^2 + c.
// Devuelve un color (uint32 empaquetado en int) según si diverge o no.
int divergente_1(std::complex<double> z0)
{
    /**
     * z_{n+1} = z_n^2 + c
     * Si |z| supera 2.0, se considera que diverge.
     * 'c' y 'max_iterations' vienen del archivo main.cpp (variables externas).
     */
    int iter = 1;
    std::complex<double> z = z0;
    // Iteramos hasta que |z| >= 2.0 (divergencia) o hasta agotar iteraciones
    while (std::abs(z) < 2.0 && iter < max_iterations)
    {
        z = z * z + c; // la actualización z_{n+1} = z_n^2 + c
        iter++;
    }

    // Si se escapó antes de alcanzar max_iterations => diverge y asignamos color
    if (iter < max_iterations)
    {
        return 0xFF0000FF; // color RGBA compacto (ejemplo: rojo con alpha 0xFF)
    }

    // Si no diverge en el límite de iteraciones => color de "dentro" (negro)
    return 0xFF000000; // negro
}

// Recorre la ventana en coordenadas X/Y y evalúa la función divergente_1 para cada píxel.
// Es la función que escribe directamente en el pixel_buffer.
void julia_serial_1(double x_min, double y_min, double x_max,
                    double y_max, uint32_t width, uint32_t height, uint32_t *pixel_buffer)
{
    // Paso entre píxeles en coordenadas complejas
    double dx = (x_max - x_min) / (width);
    double dy = (y_max - y_min) / (height);

    // Recorremos cada columna (i) y cada fila (j)
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // Mapeo del índice de píxel a coordenadas complejas (x, y)
            double x = x_min + i * dx;
            double y = y_min + j * dy;

            std::complex<double> z(x, y); // punto complejo inicial z0

            // Llamada a la función que decide el color según la divergencia.
            // Esto es un ejemplo de "llamar una cosa dentro de otra":
            // la función julia_serial_1 llama a divergente_1 para cada píxel.
            auto color = divergente_1(z);  // inferencia de tipo con auto

            // Guardamos el color en el buffer: indexado como [fila * ancho + columna]
            pixel_buffer[j * width + i] = color;
        }
    }
}
