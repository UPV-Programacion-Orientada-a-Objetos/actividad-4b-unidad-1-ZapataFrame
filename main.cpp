#include <iostream>
#include "Simulador2D.h"
#include <limits>

// Imprimir mensaje
void print_message(const std::string msg);

// Obtener Inputs de un tipo de dato flexible:
template <typename T>
T getInput(const std::string prompt);

template <>
std::string getInput<std::string>(const std::string prompt);

int get_menu_option();
void print_menu();

int main()
{
    print_message("--- Simulador Generico de Fluidos (Difusion) ---");
    print_message(">> Inicializando Sistema (Tipo FLOAT) <<");
    print_message("Creando Grid (FLOAT) de 5x5...");
    Simulador2D<float> *Grid2d = new Simulador2D<float>(5, 5);

    std::cout << std::endl;
    print_message(">> Agregando Fuentes de Concentración <<");
    Grid2d->agregarFuente(100.0, 2, 2);
    Grid2d->agregarFuente(50.0, 4, 0);

    print_message("Grid[2,2] = 100.0 (Fuente 1)");
    print_message("Grid[4,0] = 50.0 (Fuente 2)");

    bool isProgramRunning = true;
    int steps_count = 0;
    while (isProgramRunning)
    {
        int opt = get_menu_option();
        switch (opt)
        {
        case 0:
        {
            isProgramRunning = false;
            break;
        }
        case 1:
        {
            print_message("Simulando Propagación...");
            Grid2d->simularPaso();
            print_message("-- Grid Después del Paso " + std::to_string(++steps_count) + " ---");
            Grid2d->mostrarGrid();
            break;
        }
        case 2:
        {
            int filas = getInput<int>("Filas:");
            int columnas = getInput<int>("Columnas:");
            print_message("-- Redimensionando Grid ---");
            try
            {
                Grid2d->redimensionarGrid(filas, columnas);
            }
            catch (std::bad_alloc)
            {
                print_message("Se te acabo la memoria pai");
            }
            Grid2d->mostrarGrid();
            break;
        }
        case 3:
        {
            float value = getInput<float>("Value:");
            int fila = getInput<int>("Fila:");
            int columna = getInput<int>("Columna:");
            try
            {

                Grid2d->agregarFuente(value, fila, columna);
            }
            catch (std::exception &e)
            {
                print_message("Error :" + std::string(e.what()));
            }
            break;
        }
        }
    }

    print_message("Liberando memoria del Grid y Fuentes... Sistema cerrado.");
    // liberando recursos
    delete Grid2d;

    return 0;
}

int get_menu_option()
{
    bool isValidOption = false;
    int opt;
    do
    {
        print_menu();
        opt = getInput<int>("Ingrese una opcion [0-3]:");
        if (opt < 0 || opt > 3)
        {
            std::cout << "[Error] Ingrese una opcion valida" << std::endl;
            continue;
        }
        isValidOption = true;
    } while (!isValidOption);
    return opt;
}

void print_menu()
{
    const std::string MENU_TOP_ROUND = "╭───────────────{ ✦ MENÚ PRINCIPAL ✦ }────────────────╮";
    const std::string MENU_BORDER = "│";
    const std::string MENU_BOT_ROUND = "╰─────────────────────────────────────────────────────╯";
    const int content_width = 53;
    int options_count = 3;
    std::string options[] = {
        "Simular Paso",
        "Redimensionar",
        "Agregar fuente"};

    std::cout << MENU_TOP_ROUND << std::endl;
    // for each option print in menu
    for (int i = 0; i < options_count; i++)
    {
        // create option line
        std::string option = " " + std::to_string(i + 1) + ". " + options[i];
        std::cout << MENU_BORDER << std::left << std::setw(content_width) << option << MENU_BORDER << std::endl;
    }
    // add leave at the end
    std::cout << MENU_BORDER << std::left << std::setw(content_width) << " 0. Salir" << MENU_BORDER << std::endl;

    std::cout << MENU_BOT_ROUND << std::endl;
}

template <typename T>
T getInput(const std::string prompt)
{
    T input;
    std::cout << prompt;
    while (!(std::cin >> input))
    {
        std::cout << "Ingrese un input valido!" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << prompt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

template <>
std::string getInput(const std::string prompt)
{
    std::cout << prompt;
    std::string temp;
    std::getline(std::cin, temp);
    return temp;
}

void print_message(const std::string msg)
{
    const std::string MENU_TOP_ROUND = "╭─────────────────────────────────────────────────────╮";
    const std::string MENU_BORDER = "│";
    const std::string MENU_BOT_ROUND = "╰─────────────────────────────────────────────────────╯";
    const int content_width = 50;
    std::cout << MENU_TOP_ROUND << std::endl;
    std::string formatted_msg = " " + msg + " ";
    std::cout << MENU_BORDER << std::left << std::setw(content_width) << formatted_msg << std::endl;
    std::cout << MENU_BOT_ROUND << std::endl;
}
