#include <iomanip>

template <typename T>
class Simulador2D
{
private:
    // Matriz de la cuadrícula (Dinámica y Genérica)
    T **_grid;
    int _filas;
    int _columnas;

    // Vector de Fuentes (Dinámico y Genérico)
    T *_fuentes;
    int *_fuentesFilas;
    int *_fuentesColumnas;
    int _numFuentes;
    int _capacidadFuentes;

    // Vector de Constantes (Estático)
    float _constantes[3]; // [0] = Coeficiente de Difusión, [1] = DeltaX, [2] = DeltaT

public:
    // MÉTODOS PÚBLICOS OBLIGATORIOS:
    Simulador2D(int f, int c); // Constructor
    ~Simulador2D();            // Destructor
    void redimensionarGrid(int nuevaF, int nuevaC);
    void agregarFuente(T valor, int fila, int column); // Expansión manual del vector dinámico si es necesario
    void simularPaso();                                // Implementación del algoritmo

    void mostrarGrid();
    // ... otros métodos de acceso y visualización

private:
    void deleteMatriz(T **matriz, int filas, int columnas = 0);
    T **createMatiz(int filas, int columnas);
    T **copyMatriz(T **matriz, int filas, int columnas);
    void redimensionarFuentes();

    // Getters
public:
    T **getGrid() { return this->_grid; };
    int getFilas() { return this->_filas; };
    int getColumnas() { return this->_columnas; };
};

// Implementacion
template <typename T>
Simulador2D<T>::Simulador2D(int f, int c)
{
    // Crear matriz con filas y columnas
    this->_grid = createMatiz(f, c);

    // Asignar filas y columnas
    this->_filas = f;
    this->_columnas = c;

    // Crear vector de fuentes
    this->_capacidadFuentes = 10;
    this->_numFuentes = 0;
    this->_fuentes = new T[this->_capacidadFuentes];
    this->_fuentesFilas = new int[this->_capacidadFuentes];
    this->_fuentesColumnas = new int[this->_capacidadFuentes];

    // Definir valor de constantes
    this->_constantes[0] = 1;
    this->_constantes[1] = 1;
    this->_constantes[2] = 1;
};

template <typename T>
Simulador2D<T>::~Simulador2D()
{
    // Destruir la matriz grid
    this->deleteMatriz(this->_grid, this->_filas, this->_columnas);

    // Destruir fuentes
    delete[] this->_fuentes;
    delete[] this->_fuentesFilas;
    delete[] this->_fuentesColumnas;
};

template <typename T>
void Simulador2D<T>::redimensionarGrid(int nuevaF, int nuevaC)
{
    // validar tamaño positivo
    if (nuevaF < 0 || nuevaC < 0)
        throw std::invalid_argument("No es posible una medida negativo");

    // 1. Crear nueva matriz e inicializar con valores 0
    T **nuevaGrid = createMatiz(nuevaF, nuevaC);

    // 2. Copiar valores originales a la nueva matriz

    // 2.1 Definir cual es el limite a copiar
    int max_cpy_filas = (nuevaF > this->_filas) ? this->_filas : nuevaF;
    int max_cpy_columnas = (nuevaC > this->_columnas) ? this->_columnas : nuevaC;

    // 2.2 Copiar valores
    for (int i = 0; i < max_cpy_filas; i++)
    {
        for (int j = 0; j < max_cpy_columnas; j++)
            nuevaGrid[i][j] = this->_grid[i][j];
    }

    // 4. Eliminar anterior matriz
    this->deleteMatriz(this->_grid, this->_filas, this->_columnas);
    this->_grid = nullptr;

    // 5. Remplazamos el puntero
    this->_grid = nuevaGrid;

    // 6. Despunteamos el auxiliar
    nuevaGrid = nullptr;

    // 7. actualizamos medidas
    this->_filas = nuevaF;
    this->_columnas = nuevaC;
};

template <typename T>
void Simulador2D<T>::agregarFuente(T valor, int fila, int columna)
{
    if (fila < 0 || fila >= this->_filas || columna < 0 || columna >= this->_columnas)
    {
        throw std::out_of_range("Posición de fuente fuera de los límites del grid");
    }
    if (this->_numFuentes >= this->_capacidadFuentes)
    {
        redimensionarFuentes();
    }
    this->_fuentes[this->_numFuentes] = valor;
    this->_fuentesFilas[this->_numFuentes] = fila;
    this->_fuentesColumnas[this->_numFuentes] = columna;
    this->_numFuentes++;

    // Actualizar grid para agregar fuente
    this->_grid[fila][columna] = valor;
};

template <typename T>
void Simulador2D<T>::simularPaso()
{
    // Copiar la matriz a una auxiliar
    T **aux = copyMatriz(this->_grid, this->_filas, this->_columnas);

    for (int i = 1; i < this->_filas - 1; i++)
    {
        for (int j = 1; j < this->_columnas - 1; j++)
        {
            T sum = this->_grid[i - 1][j];
            sum += this->_grid[i + 1][j];
            sum += this->_grid[i][j - 1];
            sum += this->_grid[i][j + 1];

            aux[i][j] = (sum) / 4;
        }
    }

    // Asignar nuevamente los valores de las fuentes

    for (int i = 0; i < this->_numFuentes; i++)
    {
        if (this->_fuentesFilas[i] >= this->_filas || this->_fuentesColumnas[i] >= this->_columnas)
        {
            continue;
        }
        int f = this->_fuentesFilas[i];
        int c = this->_fuentesColumnas[i];
        aux[f][c] = this->_fuentes[i];
    }

    // Eliminar matriz anterior al paso
    deleteMatriz(this->_grid, this->_filas, this->_columnas);

    // asignar nueva matriz
    this->_grid = aux;
};
template <typename T>
void Simulador2D<T>::mostrarGrid()
{
    for (int i = 0; i < this->_filas; i++)
    {
        std::cout << "|";
        for (int j = 0; j < this->_columnas; j++)
        {
            std::cout << std::setw(6) << std::fixed << std::setprecision(2) << this->_grid[i][j] << "|";
        }
        std::cout << std::endl;
    }
}

template <typename T>
void Simulador2D<T>::deleteMatriz(T **matriz, int filas, int columnas)
{
    for (int i = 0; i < filas; i++)
        delete[] matriz[i];
    delete[] matriz;
}

template <typename T>
T **Simulador2D<T>::createMatiz(int filas, int columnas)
{
    // intentar crear una matriz
    try
    {
        // 1. Crear nueva matriz e inicializar con valores 0
        T **matriz = new T *[filas];
        for (int i = 0; i < filas; i++)
        {
            matriz[i] = new T[columnas];
            for (int j = 0; j < columnas; j++)
                matriz[i][j] = 0;
        };
        return matriz;
    }
    catch (std::bad_alloc)
    {
        throw;
    }
};
template <typename T>
T **Simulador2D<T>::copyMatriz(T **matriz, int filas, int columnas)
{
    T **matriz_cpy = createMatiz(filas, columnas);
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            matriz_cpy[i][j] = matriz[i][j];
        }
    }
    return matriz_cpy;
}

template <typename T>
void Simulador2D<T>::redimensionarFuentes()
{
    const int newSize = this->_capacidadFuentes * 2;

    // Crear nuevo arreglo
    T *nuevoFuentes = new T[newSize];
    int *nuevoFuentesFilas = new int[newSize];
    int *nuevoFuentesColumnas = new int[newSize];

    // copiar arreglo viejo al nuevo

    for (int i = 0; i < this->_numFuentes; i++)
    {
        nuevoFuentes[i] = this->_fuentes[i];
        nuevoFuentesFilas[i] = this->_fuentesFilas[i];
        nuevoFuentesColumnas[i] = this->_fuentesColumnas[i];
    }

    // Eliminar el arreglo veijo
    delete[] this->_fuentes;
    delete[] this->_fuentesFilas;
    delete[] this->_fuentesColumnas;

    // pasar la referencia
    this->_fuentes = nuevoFuentes;
    this->_fuentesFilas = nuevoFuentesFilas;
    this->_fuentesColumnas = nuevoFuentesColumnas;

    // Actualziar el tamaño
    this->_capacidadFuentes = newSize;
}