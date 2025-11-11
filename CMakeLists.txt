# CMakeLists.txt
# --------------------------------------------
# Este archivo le dice a CMake cómo construir el proyecto.
# --------------------------------------------

# Versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.16)

# Nombre del proyecto
# Este nombre también puede usarse como base para el ejecutable o variables.
project(FractalJulia)

# Busca e importa la librería 'fmt'.
# 'find_package' localiza el paquete ya instalado en el sistema
# o en vcpkg. 'CONFIG REQUIRED' indica que debe existir.
find_package(fmt CONFIG REQUIRED)


# Crea un ejecutable llamado "fractal_julia"
# A partir del archivo fuente main.cpp.
add_executable(fractal_julia main.cpp)

# Enlaza las librerías externas al ejecutable.
# 'target_link_libraries' conecta el ejecutable con las
# dependencias requeridas para compilar y ejecutar correctamente.
target_link_libraries(fractal_julia PRIVATE fmt::fmt)
