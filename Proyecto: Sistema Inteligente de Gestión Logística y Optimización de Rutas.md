1. Descripción General
Desarrollar una aplicación de escritorio, que permita gestionar una red de distribución logística. El sistema deberá ser capaz de administrar nodos (ciudades/depósitos) y las conexiones entre ellos (rutas), permitiendo al usuario visualizar el mapa y calcular la ruta más eficiente entre dos puntos mediante algoritmos de teoría de grafos.
2. Requisitos Funcionales 
•	Gestión de Nodos: Alta, baja y modificación de ciudades. Cada ciudad debe tener un nombre, un ID único y coordenadas (X, Y) para su representación gráfica.
•	Gestión de Conexiones: Definir rutas entre ciudades especificando la distancia en kilómetros o tiempo estimado.
•	Visualización Gráfica:  Carga de una imagen de fondo (Mapa).
o	Dibujo dinámico de los nodos y las aristas sobre el mapa (imagen).
o	Uso de componentes como TImage, TComboBox, TStringGrid y TButton, o sus equivalentes según el entorno elegido.
•	Cálculo de Ruta Óptima: Implementación del Algoritmo para hallar el camino más corto entre un origen y un destino seleccionados por el usuario.
•	Gestión de Contingencias (Rutas Cortadas): El sistema debe permitir al usuario marcar una ruta como "Inactiva" o "Cortada" de forma temporal. Ante esta situación, el algoritmo de búsqueda deberá recalcular automáticamente la mejor ruta alternativa ignorando las conexiones deshabilitadas.
•	Persistencia de Datos: Los datos de las ciudades y sus conexiones deben almacenarse en archivos binarios (.dat) o de texto estructurado para que la información no se pierda al cerrar la aplicación.
•	El sistema deberá contar con un registro histórico de las operaciones de optimización realizadas, permitiendo la trazabilidad de las rutas calculadas por el usuario.
3. Requisitos Técnicos Programación Orientada a Objetos (POO): Uso de clases bien definidas.
•	Estructuras de Datos: Implementación de una Matriz de Adyacencia o Lista de Adyacencia para representar en el mapa.
•	Interfaz Gráfica: Uso de componentes como TImage, TComboBox, TStringGrid y TButton.
•	Algoritmia: Manejo de lógica de búsqueda y optimización.
4. Entregables 
1.	Código Fuente: Organizado en archivos de cabecera (.h) y de implementación (.cpp).
2.	Documentación Técnica: Explicación del funcionamiento del algoritmo.
3.	Manual de Usuario: Breve guía de cómo cargar ciudades y calcular rutas.
