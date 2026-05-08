#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
//----------aporte de SEBASTIAN------------
class Nodo
{
	public:
		std::string nombre;
		std::vector<unsigned int> distancias;
};

bool cargarDatos(std::vector<Nodo> &nodos)
{
	unsigned int filaActual = 0;

	std::string linea;
	std::string subLinea;

	std::vector<unsigned int> distancias;

	std::ifstream archivo;

	archivo.open("rio_negro.csv");

	if (!archivo.is_open())
	{
		std::perror("Error");

		return false;
	}

	while (getline(archivo, linea))
	{
		std::stringstream ss(linea);

		if (filaActual == 0)
		{
			while (getline(ss, subLinea, ','))
			{
				Nodo nuevoNodo;
				nuevoNodo.nombre = subLinea;
				nodos.push_back(nuevoNodo);
			}

			filaActual++;

			continue;
		}

		distancias.clear();

		while (getline(ss, subLinea, ','))
		{
			distancias.push_back(stoi(subLinea));
		}

		nodos[filaActual - 1].distancias = distancias;

		filaActual++;
	}

	archivo.close();

	return true;
}

bool guardarDatos(std::vector<Nodo> &nodos)
{
	unsigned int numNodos = nodos.size();

	std::ofstream archivo;

	archivo.open("rio_negro.txt");

	if (!archivo.is_open())
	{
		std::perror("Error");

		return false;
	}

	// NOMBRES
	for (unsigned int index = 0; index < numNodos; index++)
	{
		if (index < (numNodos - 1))
		{
			archivo << nodos[index].nombre << ','; // TERMINA CON ','
		}
		else
		{
			archivo << nodos[index].nombre << std::endl; // TERMINA CON '\n'
		}
	}

	// DISTANCIAS
	for (unsigned int fila = 0; fila < numNodos; fila++)
	{
		for (unsigned int columna = 0; columna < numNodos; columna++)
		{
			if (columna < (numNodos - 1))
			{
				archivo << nodos[fila].distancias[columna] << ','; // TERMINA CON ','
			}
			else if (columna == (numNodos - 1) && fila != (numNodos - 1))
			{
				archivo << nodos[fila].distancias[columna] << std::endl; // TERMINA CON '\n'
			}
			else
			{
				archivo << nodos[fila].distancias[columna]; // TERMINA CON ''
			}
		}
	}

	archivo.close();

	return true;
}

void borrarNodo(std::vector<Nodo> &nodos, unsigned int index)
{
	unsigned int numNodos = nodos.size();

	for (unsigned int fila = 0; fila < numNodos; fila++)
	{
		if (fila != index)
		{
			nodos[fila].distancias.erase(nodos[fila].distancias.begin() + index);
		}
	}

	nodos.erase(nodos.begin() + index);
}

void imprimirNodos(std::vector<Nodo> &nodos)
{
	for (unsigned int fila = 0; fila < nodos.size(); fila++)
	{
		std::cout << "Nombre: " << nodos[fila].nombre << std::endl;
		std::cout << "Distancias: ";

		for (unsigned int columna = 0; columna < nodos[fila].distancias.size(); columna++)
		{
			std::cout << nodos[fila].distancias[columna] << " ";
		}

		std::cout << std::endl << std::endl;
	}
}

//----------aporte de WENDY------------
// declaracion de la funcion
void preguntarRuta(const std::vector<Nodo>& nodos);

// -------Aporte de KEVIN ------

// Estructura que almacena el resultado final
struct RutaResultado {
    int distanciaTotal;
    std::vector<int> camino;
};

// Algoritmo de Dijkstra que encuentra el camino más corto
RutaResultado calcularDijkstra(const std::vector<Nodo>& nodos, int inicio, int fin) {
    int n = nodos.size();
    std::vector<int> dist(n, INT_MAX);    // Distancias mínimas desde el inicio
    std::vector<int> padre(n, -1);       // Para reconstruir la ruta al final
    std::vector<bool> visitado(n, false); // Nodos ya procesados

    dist[inicio] = 0;

    for (int i = 0; i < n - 1; i++) {
        // 1. Buscar el nodo no visitado con la distancia mínima
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u]))
                u = j;
        }

        if (dist[u] == INT_MAX) break; // No hay más nodos alcanzables
        visitado[u] = true;

        // 2. Actualizar distancias de los vecinos
        for (int v = 0; v < n; v++) {
            unsigned int peso = nodos[u].distancias[v];
            // Si hay conexión (peso > 0) y el nuevo camino es más corto
            if (peso > 0 && dist[u] + (int)peso < dist[v]) {
                dist[v] = dist[u] + peso;
                padre[v] = u;
            }
        }
    }

    // 3. Reconstruir el camino desde el destino al origen
    RutaResultado res;
    res.distanciaTotal = dist[fin];
    if (dist[fin] != INT_MAX) {
        for (int v = fin; v != -1; v = padre[v]) {
            res.camino.push_back(v);
        }
        // Invertimos el camino para que vaya de origen a fin
        std::reverse(res.camino.begin(), res.camino.end());
    }
    return res;
}

int main()
{
	std::vector<Nodo> nodos;

	if (!cargarDatos(nodos))
	{
		return 1;
	}
//----------aporte de WENDY------------

/*mini "menu" para preguntar y calcular ruta desde el usuario
y no del programador*/

 preguntarRuta(nodos);

 /*al imprimir la distancia entre las cuidades
 se imprime el cuadro completo de todas las ciudades.
*/

 //----------aporte de SEBASTIAN------------
	 // borrarNodo(nodos, 7); aca hay un error No existe la ciudad 7 Y el programa se rompe.
	 if (!nodos.empty()) {
    borrarNodo(nodos, 0); // Borra la primera ciudad (Viedma) de forma segura
}

	imprimirNodos(nodos);

	guardarDatos(nodos);

	return 0;
}


//----------aporte de WENDY------------
/*
lo separe en 2 funciones para que en caso de editarse
sea mas facil de hacerlo.
 */


/*
es la que calcula la distancia entre dos nodos (ciudades)
   luego de que el USUARIO indica de donde sale y a donde va
   usando los datos ya guardados
*/
int obtenerDistancia(const std::vector<Nodo>& nodos, int salida, int destino)
 {
    return nodos[salida].distancias[destino];
 }
/*
  es la funcion que pide al USUARIO que indique su lugar de salido(donde esta)
   y a donde quiera llegar
   hize que usando los datos guardados para que se cree una lista de las ciudades
   e indicando con un numero sea mas facil
   para el usuario indicar donde esta y a donde quiere ir
   
void preguntarRuta(const std::vector<Nodo>& nodos) {
    int salida, destino;

    std::cout << "Ciudades disponibles:\n";
    for (unsigned int i = 0; i < nodos.size(); i++) {
        std::cout << i << " - " << nodos[i].nombre << "\n";
    }
//pide que ingrese una opcion del 0 a 7
    std::cout << "\nIngrese el numero de la ciudad de salida: ";
    std::cin >> salida;

    std::cout << "Ingrese el numero de la ciudad de destino: ";
    std::cin >> destino;

    // obtiene de la otra funcion "obtener distancia" el valor (km) de distancia
    int distancia = obtenerDistancia(nodos, salida, destino);

    //imprime los 3 datos relevantes para el usuario. aunque se imprime el cuadro de ciudades, el cual no toque
    std::cout << "\nUsted esta en: " << nodos[salida].nombre << "\n";
    std::cout << "Quiere viajar a: " << nodos[destino].nombre << "\n";
    std::cout << "Distancia a recorrer: " << distancia << " km\n";
      std::cout << "\n";
}*/
// aporte de kevin:modifique la funcion de salida para que sea mas grafico y el dijkstra pueda apreciearse 
void preguntarRuta(const std::vector<Nodo>& nodos) {
    int salida, destino;

    std::cout << "Ciudades disponibles:\n";
    for (unsigned int i = 0; i < nodos.size(); i++) {
        std::cout << i << " - " << nodos[i].nombre << "\n";
    }

    std::cout << "\nIngrese el numero de la ciudad de salida: ";
    std::cin >> salida;

    std::cout << "Ingrese el numero de la ciudad de destino: ";
    std::cin >> destino;

    
    // Llamamos al algoritmo 
    RutaResultado resultado = calcularDijkstra(nodos, salida, destino);

    std::cout << "\n==========================================" << std::endl;
    std::cout << "      HOJA DE RUTA" <<std:: endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Origen:  " << nodos[salida].nombre <<std:: endl;
    std::cout << "Destino: " << nodos[destino].nombre <<std:: endl;

    // Mostramos el camino paso a paso que calculó el algoritmo
    std::cout << "\nITINERARIO SUGERIDO: ";
    for (int i = 0; i < resultado.camino.size(); i++) {
        int id = resultado.camino[i];
        std::cout << nodos[id].nombre;
        
        if (i < resultado.camino.size() - 1) {
            std::cout << " -> "; // Flecha entre ciudades
        }
    }

    std::cout << "\n\nDISTANCIA TOTAL OPTIMIZADA: " << resultado.distanciaTotal << " km" << std:: endl;

    // Cálculo extra basado en los datos de camiones (80 km/h) [cite: 102]
    double tiempoHoras = (double)resultado.distanciaTotal / 80.0;
    std::cout << "TIEMPO ESTIMADO DE VIAJE: " << tiempoHoras << " horas" << std::endl;
    std::cout << "==========================================\n" << std::endl;
}
