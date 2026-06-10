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



//----aporte wendy----------------------
RutaResultado calcularRutaConContingencias(const std::vector<Nodo>& nodos, int inicio, int fin);
 void cortarRuta(std::vector<Nodo>& nodos, int origen, int destino);

 void habilitarRuta (std::vector<Nodo>& nodos,
                   const std::vector<Nodo>& nodosOriginales);
//----------fin------------wendy--------------
void mostrarRutasCortadas(const std::vector<Nodo>& nodos, const std::vector<Nodo>& nodosOriginales);//aviso al compilador que la funcion existe mas abajo
// funcion agregarNodo-----kevin------inicio

void agregarNodo(std::vector<Nodo> &nodos)
{
    std::string nombreNuevaCiudad; //creo la variable
    
    std::cout << "\nIngrese el nombre de la nueva ciudad: "; //pregunto el nombre


    // std::cin >> nombreNuevaCiudad; si uso cin y pongo una ciudad con espacios el programa se vuelve loco
    //por que cin lee hasta el primer espacio y lo que queda despues es basura que una vez que el menu se reinicia lo lee infinitamente
   

    std::cin.ignore(1000, '\n'); // Ignora hasta 1000 caracteres de basura, o hasta encontrar el Enter  
    
    
    std::getline(std::cin, nombreNuevaCiudad); //Leemos la respuesta completa con espacios incluidos

    //  a cada ciudad que existe, le agregamos una nueva ruta hacia la ciudad nueva con distancia 0
    for (unsigned int fila = 0; fila < nodos.size(); fila++)
    {
     nodos[fila].distancias.push_back(0); 
    }

    // Creamos una nueva ciudad 
    Nodo nuevaCiudad;
      nuevaCiudad.nombre = nombreNuevaCiudad;

    //  Le asignamos sus propias distancias hacia las otras ciudades.
    
    for (unsigned int columna = 0; columna <= nodos.size(); columna++)
    {
     nuevaCiudad.distancias.push_back(0);
    }

    
    nodos.push_back(nuevaCiudad);//  insertamos la nueva ciudad al final de la lista principal

    std::cout << "Ciudad '" << nombreNuevaCiudad << "' agregada con exito al sistema.\n";
}
//---------funcion agregarNodo----FIN

//--------------------FUNCIONES QUE FALTA DESARROLLAR---
//LUEGO DE REALIZAR BORRAR ESTE COMENTARIO

/*

modificarNodo();
crearRuta();
modificarRuta();
guardarHistorial();
*/
//-----------------LUEGO DE REALIZAR ESTAS FUNCIONES BORRAR ESTE COMENTARIO




//--------------------------------------MAIN---------------------------------------------

int main()
{
	std::vector<Nodo> nodos;
    std::vector<Nodo> nodosOriginales;
	if (!cargarDatos(nodos))
	{
		return 1;
	}
    nodosOriginales=nodos;
//----------aporte de WENDY------------



//opcion para guarda la opcion
int opcion=0;

while(opcion!=9){
    std::cout << "\n==========================================\n" ;
    std::cout << "    MENU \n" ;
    std::cout << "==========================================\n" ;
    std::cout <<"1) calcular ruta\n";
    std::cout <<"2) calcular ruta con contigencias\n";//corte de rutas
    std::cout <<"3) cortar ruta\n";//usuario indica que se corto la ruta.
    std::cout <<"4) habilitar ruta\n";//se habilita la ruta cortada si el usu ya sabe cual es...
    std::cout <<"5) Rutas Cortadas Actuales\n";
    std::cout <<"6) borrar ciudad\n";
    std::cout <<"7) ver nodos\n";
    std::cout <<"8) agregar ciudad\n"; // funcion agregarNodo()
    std::cout <<"9) guardar y salir\n"; 
    std::cout <<"SELECCIONE UNA OPCION:\t";
    std::cin >>opcion;
    //case para que ejecute la opcion indicada
    switch(opcion)
    {
    case 1: preguntarRuta(nodos);break;
    case 2:
        {
     std::cout << "\nCiudades disponibles:\n";

    for (int i = 0; i < nodos.size(); i++)
    {
        std::cout << i << " - " << nodos[i].nombre << std::endl;
    }

         int salida, destino;
        std:: cout <<"ingrese la ciudad de salida:\t";
        std::cin >>salida;
        std:: cout <<"ingrese la ciudad de destino:\t";
        std::cin >>destino;
  RutaResultado res = calcularRutaConContingencias(nodos, salida, destino);

           if (res.distanciaTotal == INT_MAX) {
        std::cout << "No existe ruta disponible (posible corte).\n";
    } else {
        std::cout << "Ruta optimizada con contingencias: " << res.distanciaTotal << " km\n";
    }


    break;
    }
    case 3:
        {
     std::cout << "\nCiudades disponibles:\n";

    for (int i = 0; i < nodos.size(); i++)
    {
        std::cout << i << " - " << nodos[i].nombre << std::endl;
    }
          int origen,destino;
          std::cout <<"ingrese ciudad de origen:";
          std::cin >>origen;
          std::cout <<"ingrese ciudad destino";
          std::cin >>destino;
          cortarRuta(nodos,origen,destino);
          //  la ruta fue cortada
    std::cout << "Ruta cortada. Si intenta calcular ahora, se ignorará esa conexión.\n";

        break;
        }
         case 4:
    habilitarRuta(nodos, nodosOriginales);
    break;
    
    case 5:
        mostrarRutasCortadas(nodos, nodosOriginales);
        break;

    case 6:
        {
          int indiceBorrar;

    std::cout << "\nCiudades disponibles:\n";

    for (int i = 0; i < nodos.size(); i++)
    {
        std::cout << i << " - " << nodos[i].nombre << std::endl;
    }

    std::cout << "\nIngrese el indice de la ciudad a borrar: ";
    std::cin >> indiceBorrar;

    borrarNodo(nodos, indiceBorrar);
    borrarNodo(nodosOriginales, indiceBorrar);
       break;
    }
    case 7: imprimirNodos(nodos);break;
	case 8:
        agregarNodo(nodos);
        // Sincronizamos las copias de seguridad  para que no se rompan las contingencias
        nodosOriginales = nodos; 
        break;
    case 9:guardarDatos(nodos);
    std::cout <<"proximante\n";//a comentar
    std::cout <<"guardando datos...saliendo\n";
    break;

    default: std::cout <<"opcion invalida\n";

    }

}
//-----------fin menu

/*mini "menu" para preguntar y calcular ruta desde el usuario
y no del programador*/

// preguntarRuta(nodos);

 /*al imprimir la distancia entre las cuidades
 se imprime el cuadro completo de todas las ciudades.
*/

 //----------aporte de SEBASTIAN------------
	 // borrarNodo(nodos, 7); aca hay un error No existe la ciudad 7 Y el programa se rompe.
/*	 if (!nodos.empty()) {
    borrarNodo(nodos, 0); // Borra la primera ciudad (Viedma) de forma segura
}

	imprimirNodos(nodos);

	guardarDatos(nodos);*/

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

    // Cálculo extra basado en los datos de camiones (80 km/h) 
	 double tiempoHoras = (double)resultado.distanciaTotal / 80.0;
    std::cout << "TIEMPO ESTIMADO DE VIAJE: " << tiempoHoras << " horas" << std::endl;
    std::cout << "==========================================\n" << std::endl;
}
//---------aporte wendy----------
//funcion para calcular ruta cuando el usuario dice que esta cortada.
RutaResultado calcularRutaConContingencias(const std::vector<Nodo>& nodos, int inicio, int fin) {
    // Llama a la funcion original que creo kevin y realizar el corte para recalculo.
    return calcularDijkstra(nodos, inicio, fin);
}

//funcion corte de ruta  esto lo indica el usuario y alli marca que se corto la ruta.
void cortarRuta(std::vector<Nodo>& nodos, int origen, int destino) {
    if (origen >= 0 && origen < nodos.size() &&
        destino >= 0 && destino < nodos.size()) {

        // Validar que realmente exista una ruta directa antes de cortarla
        if (nodos[origen].distancias[destino] == 0) {
            std::cout << "No existe una ruta directa entre " << nodos[origen].nombre 
                      << " y " << nodos[destino].nombre << " para cortar.\n";
            return;
        }

        
        nodos[origen].distancias[destino] = 0;
        nodos[destino].distancias[origen] = 0; 

        std::cout << "Ruta entre " << nodos[origen].nombre
                  << " y " << nodos[destino].nombre
                  << " marcada como CORTADA.\n";
    } else {
        std::cout << "Indices invalidos.\n";
    }
}


//muestra que rutas estan cortadas y son posibles habiitar nuevamente.
void mostrarRutasCortadas(const std::vector<Nodo>& nodos,
                          const std::vector<Nodo>& nodosOriginales)
{
    std::cout << "\n===== RUTAS CORTADAS =====\n";

    bool hayCortadas = false;

    for (int i = 0; i < nodos.size(); i++)
    {
        for (int j = i + 1; j < nodos.size(); j++)
        {
            if (nodos[i].distancias[j] == 0 &&
                nodosOriginales[i].distancias[j] > 0)
            {
                std::cout
                    << i << " - "
                    << nodos[i].nombre
                    << " <-> "
                    << j << " - "
                    << nodos[j].nombre
                    << std::endl;

                hayCortadas = true;
            }
        }
    }

    if (!hayCortadas)
    {
        std::cout << "No hay rutas cortadas.\n";
    }
}

//nueba funcion que habilita las rutas cortadas usando salida, destino y la km
void habilitarRuta (std::vector<Nodo>& nodos,
                   const std::vector<Nodo>& nodosOriginales){

    mostrarRutasCortadas(nodos, nodosOriginales);
    int origen, destino;

    std::cout << "Ingrese origen: ";
    std::cin >> origen;

    std::cout << "Ingrese destino: ";
    std::cin >> destino;

  if (origen >= 0 && origen < nodos.size() &&
        destino >= 0 && destino < nodos.size())
    {
         nodos[origen].distancias[destino] = nodosOriginales[origen].distancias[destino];
        nodos[destino].distancias[origen] = nodosOriginales[destino].distancias[origen];

        std::cout << "Ruta habilitada.\n";
    }
    else
    {
        std::cout << "Indices invalidos.\n";
    }
}
