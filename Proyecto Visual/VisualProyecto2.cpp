//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "VisualProyecto2.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <climits>
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

class Nodo
{
public:
	std::string nombre;
	std::vector<unsigned int> distancias;
};

struct informaRuta
{
	int origen;
	int destino;
	std::string nombreRuta;
};

std::vector<Nodo> nodos;
std::vector<Nodo> nodosOriginales;
std::vector<informaRuta> rutasReales;

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

struct RutaResultado {
	int distanciaTotal;
	std::vector<int> camino;
};

RutaResultado calcularDijkstra(const std::vector<Nodo>& nodos, int inicio, int fin) {
	int n = nodos.size();

	std::vector<int> dist(n, INT_MAX);
	std::vector<int> padre(n, -1);
	std::vector<bool> visitado(n, false);

	dist[inicio] = 0;

	for (int i = 0; i < n - 1; i++) {
		int u = -1;

		for (int j = 0; j < n; j++) {
			if (!visitado[j] && (u == -1 || dist[j] < dist[u])) {
				u = j;
			}
		}

		if (u == -1 || dist[u] == INT_MAX) {
			break;
		}

		visitado[u] = true;

		for (int v = 0; v < n; v++) {
			unsigned int peso = nodos[u].distancias[v];

			if (peso > 0 && dist[u] + (int)peso < dist[v]) {
				dist[v] = dist[u] + peso;
				padre[v] = u;
			}
		}
	}

	RutaResultado res;
	res.distanciaTotal = dist[fin];

	if (dist[fin] != INT_MAX) {
		for (int v = fin; v != -1; v = padre[v]) {
			res.camino.push_back(v);
		}

		std::reverse(res.camino.begin(), res.camino.end());
	}

	return res;
}

void guardarHistorialRutaVisual(const std::vector<Nodo>& nodos,
                                int origen,
                                int destino,
                                const RutaResultado& resultado)
{
    std::ofstream archivo("historial_rutas.txt", std::ios::app);

    if (!archivo.is_open())
    {
        return;
    }

    archivo << "Origen: " << nodos[origen].nombre << "\n";
    archivo << "Destino: " << nodos[destino].nombre << "\n";
    archivo << "Ruta: ";

    for (unsigned int i = 0; i < resultado.camino.size(); i++)
    {
        archivo << nodos[resultado.camino[i]].nombre;

        if (i < resultado.camino.size() - 1)
        {
            archivo << " -> ";
        }
    }

    archivo << "\nDistancia total: " << resultado.distanciaTotal << " km\n";
    archivo << "-----------------------------\n";

    archivo.close();
}

void cortarRutaVisual(std::vector<Nodo>& nodos, int origen, int destino)
{
    if (origen < 0 || origen >= nodos.size() ||
        destino < 0 || destino >= nodos.size())
    {
        ShowMessage("Seleccione ciudades validas.");
        return;
    }

    if (origen == destino)
    {
        ShowMessage("No se puede cortar una ruta entre la misma ciudad.");
        return;
    }

    if (nodos[origen].distancias[destino] == 0)
    {
        ShowMessage("No existe una ruta directa entre esas ciudades o ya esta cortada.");
        return;
    }

    nodos[origen].distancias[destino] = 0;
    nodos[destino].distancias[origen] = 0;

    ShowMessage("Ruta cortada correctamente.");
}

void habilitarRutaVisual(std::vector<Nodo>& nodos,
                         const std::vector<Nodo>& nodosOriginales,
                         int origen,
                         int destino)
{
    if (origen < 0 || origen >= nodos.size() ||
        destino < 0 || destino >= nodos.size())
    {
        ShowMessage("Seleccione ciudades validas.");
        return;
    }

    if (origen == destino)
    {
        ShowMessage("No se puede habilitar una ruta entre la misma ciudad.");
        return;
    }

    if (nodosOriginales[origen].distancias[destino] == 0)
    {
        ShowMessage("No existe una ruta original entre esas ciudades.");
        return;
    }

    nodos[origen].distancias[destino] = nodosOriginales[origen].distancias[destino];
    nodos[destino].distancias[origen] = nodosOriginales[destino].distancias[origen];

    ShowMessage("Ruta habilitada correctamente.");
}

void agregarCiudadVisual(std::vector<Nodo>& nodos, String nombreCiudad)
{
    if (nombreCiudad.Trim() == "")
    {
        ShowMessage("Ingrese el nombre de la ciudad.");
        return;
    }

    for (unsigned int i = 0; i < nodos.size(); i++)
    {
        if (String(nodos[i].nombre.c_str()).LowerCase() == nombreCiudad.LowerCase())
        {
            ShowMessage("Esa ciudad ya existe.");
            return;
        }
    }

    for (unsigned int i = 0; i < nodos.size(); i++)
    {
        nodos[i].distancias.push_back(0);
    }

    Nodo nuevaCiudad;
    nuevaCiudad.nombre = AnsiString(nombreCiudad).c_str();

    for (unsigned int i = 0; i <= nodos.size(); i++)
    {
        nuevaCiudad.distancias.push_back(0);
    }

    nodos.push_back(nuevaCiudad);

	Form1->ComboBoxOrigen->Items->Add(nombreCiudad);
	Form1->ComboBoxDestino->Items->Add(nombreCiudad);

    ShowMessage("Ciudad agregada correctamente.");
}


void crearRutaVisual(std::vector<Nodo>& nodos,
                     std::vector<informaRuta>& rutasReales,
					 int origen,
					 int destino,
					 unsigned int distancia,
					 String nombreRuta)
{
	if (origen < 0 || origen >= nodos.size() ||
		destino < 0 || destino >= nodos.size())
	{
		ShowMessage("Seleccione ciudades validas.");
		return;
	}

	if (origen == destino)
	{
		ShowMessage("No puede conectar una ciudad consigo misma.");
		return;
	}

	if (distancia == 0)
	{
		ShowMessage("Distancia invalida.");
		return;
	}

	if (nombreRuta.Trim() == "")
	{
		ShowMessage("Ingrese el nombre de la ruta.");
		return;
	}

	if (nodos[origen].distancias[destino] != 0)
	{
		ShowMessage("La ruta ya existe.");
		return;
	}

	nodos[origen].distancias[destino] = distancia;
	nodos[destino].distancias[origen] = distancia;

	informaRuta nuevaRuta;
	nuevaRuta.origen = origen;
	nuevaRuta.destino = destino;
	nuevaRuta.nombreRuta = AnsiString(nombreRuta).c_str();

	rutasReales.push_back(nuevaRuta);

	ShowMessage("Ruta creada correctamente.");
}

void borrarCiudadVisual(std::vector<Nodo>& nodos, int index)
{
    if (index < 0 || index >= nodos.size())
    {
        ShowMessage("Seleccione una ciudad valida para borrar.");
        return;
    }

    String nombreCiudad = nodos[index].nombre.c_str();

    for (unsigned int fila = 0; fila < nodos.size(); fila++)
    {
        if (fila != index)
        {
            nodos[fila].distancias.erase(nodos[fila].distancias.begin() + index);
        }
    }

    nodos.erase(nodos.begin() + index);

    Form1->ComboBoxOrigen->Items->Delete(index);
    Form1->ComboBoxDestino->Items->Delete(index);

    Form1->ComboBoxOrigen->ItemIndex = -1;
    Form1->ComboBoxDestino->ItemIndex = -1;

    ShowMessage("Ciudad eliminada correctamente: " + nombreCiudad);
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BotonCalcularClick(TObject *Sender)
{
    int origen = ComboBoxOrigen->ItemIndex;
    int destino = ComboBoxDestino->ItemIndex;

    if (origen == -1 || destino == -1)
    {
        ShowMessage("Seleccione una ciudad de origen y una ciudad de destino.");
        return;
    }

    RutaResultado resultado = calcularDijkstra(nodos, origen, destino);

	MemoResultado->Visible = true;
    MemoResultado->Lines->Clear();
    MemoResultado->Lines->Add("HOJA DE RUTA");
    MemoResultado->Lines->Add("-----------------------------");
    MemoResultado->Lines->Add("Origen: " + ComboBoxOrigen->Text);
    MemoResultado->Lines->Add("Destino: " + ComboBoxDestino->Text);
    MemoResultado->Lines->Add("");

    if (resultado.distanciaTotal == INT_MAX)
    {
        MemoResultado->Lines->Add("No existe una ruta disponible.");
        return;
    }

    String ruta = "Itinerario: ";

    for (int i = 0; i < resultado.camino.size(); i++)
    {
        int id = resultado.camino[i];
        ruta += nodos[id].nombre.c_str();

        if (i < resultado.camino.size() - 1)
        {
            ruta += " -> ";
        }
    }

    MemoResultado->Lines->Add(ruta);
    MemoResultado->Lines->Add("");
    MemoResultado->Lines->Add("Distancia total optimizada: " + IntToStr(resultado.distanciaTotal) + " km");

    double tiempoHoras = (double)resultado.distanciaTotal / 80.0;
    MemoResultado->Lines->Add("Tiempo estimado de viaje: " + FloatToStr(tiempoHoras) + " horas");
    guardarHistorialRutaVisual(nodos, origen, destino, resultado);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    if (cargarDatos(nodos))
    {

		nodosOriginales = nodos;
		rutasReales.clear();

		rutasReales.push_back({0, 7, "RN 3"});
		rutasReales.push_back({1, 2, "RN 22"});
		rutasReales.push_back({1, 5, "RN 22"});
		rutasReales.push_back({2, 3, "RN 237"});
		rutasReales.push_back({3, 4, "RN 40"});
		rutasReales.push_back({5, 6, "RN 22"});
		rutasReales.push_back({5, 7, "RN 250"});

		ComboBoxOrigen->Items->Clear();
		ComboBoxDestino->Items->Clear();

		for (int i = 0; i < nodos.size(); i++)
		{
			ComboBoxOrigen->Items->Add(nodos[i].nombre.c_str());
			ComboBoxDestino->Items->Add(nodos[i].nombre.c_str());
        }

        MemoResultado->Lines->Clear();
        MemoResultado->Lines->Add("Ciudades cargadas correctamente.");
    }
    else
    {
        ShowMessage("No se pudo cargar el archivo rio_negro.csv");
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CortarRutaClick(TObject *Sender)
{
    int origen = ComboBoxOrigen->ItemIndex;
	int destino = ComboBoxDestino->ItemIndex;

	cortarRutaVisual(nodos, origen, destino);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VerHistorialClick(TObject *Sender)
{
	std::ifstream archivo("historial_rutas.txt");

    MemoResultado->Visible = true;
    MemoResultado->Lines->Clear();

    if (!archivo.is_open())
    {
        MemoResultado->Lines->Add("No hay historial guardado.");
        return;
    }

    std::string linea;

    while (getline(archivo, linea))
    {
        MemoResultado->Lines->Add(linea.c_str());
    }

    archivo.close();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::AgregarCiudadClick(TObject *Sender)
{
    agregarCiudadVisual(nodos, EditarCiudad->Text);

    nodosOriginales = nodos;

    EditarCiudad->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BorrarCiudadClick(TObject *Sender)
{
    int index = ComboBoxOrigen->ItemIndex;

	borrarCiudadVisual(nodos, index);

	nodosOriginales = nodos;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CrearRutaClick(TObject *Sender)
{
	int origen = ComboBoxOrigen->ItemIndex;
	int destino = ComboBoxDestino->ItemIndex;

	unsigned int distancia = StrToIntDef(EditarDistancia->Text, 0);

	crearRutaVisual(nodos,
					rutasReales,
					origen,
					destino,
					distancia,
					EditarNombreRuta->Text);

	nodosOriginales = nodos;

	EditarDistancia->Clear();
	EditarNombreRuta->Clear();

}






//---------------------------------------------------------------------------

void __fastcall TForm1::HabilitarRutaClick(TObject *Sender)
{
int origen = ComboBoxOrigen->ItemIndex;
    int destino = ComboBoxDestino->ItemIndex;

    habilitarRutaVisual(nodos,
                        nodosOriginales,
                        origen,
						destino);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VerRutasCortadasClick(TObject *Sender)
{
MemoResultado->Visible = true;
    MemoResultado->Lines->Clear();
    MemoResultado->Lines->Add("RUTAS CORTADAS");
    MemoResultado->Lines->Add("-----------------------------");

    bool hayCortadas = false;

    for (int i = 0; i < nodos.size(); i++)
    {
        for (int j = i + 1; j < nodos.size(); j++)
        {
            if (nodos[i].distancias[j] == 0 &&
                nodosOriginales[i].distancias[j] > 0)
            {
                MemoResultado->Lines->Add(
                    String(nodos[i].nombre.c_str()) +
                    " <-> " +
                    String(nodos[j].nombre.c_str())
                );

                hayCortadas = true;
            }
        }
    }

    if (!hayCortadas)
    {
        MemoResultado->Lines->Add("No hay rutas cortadas.");
	}
}
//---------------------------------------------------------------------------


