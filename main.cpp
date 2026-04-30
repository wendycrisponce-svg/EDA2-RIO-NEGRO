#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

int main()
{
	std::vector<Nodo> nodos;

	if (!cargarDatos(nodos))
	{
		return 1;
	}

	borrarNodo(nodos, 7);

	imprimirNodos(nodos);

	guardarDatos(nodos);

	return 0;
}