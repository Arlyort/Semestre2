#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <climits>
#include <algorithm>
#include <sstream>

using namespace std;

struct Arco {
    int verticeDestino;
    int pesoCosto;
    int pesoTiempo;
};

class SistemaDigrafo {
private:
    int totalVertices;
    vector<vector<Arco>> listaConexiones;

public:
    // Constructor del sistema
    SistemaDigrafo(int verticesIniciales = 0) {
        inicializarEstructura(verticesIniciales);
    }

    void inicializarEstructura(int verticesIniciales) {
        totalVertices = verticesIniciales;
        listaConexiones.assign(verticesIniciales + 1, vector<Arco>());
    }

    int obtenerCantidadVertices() const { 
        return totalVertices; 
    }

    void expandirContenedor(int verticeId) {
        if (verticeId > totalVertices) {
            totalVertices = verticeId;
            listaConexiones.resize(totalVertices + 1);
        }
    }

    // Vinculacion unidireccional para el digrafo
    void registrarDireccion(int origen, int destino, int c, int t) {
        expandirContenedor(max(origen, destino));
        listaConexiones[origen].push_back({destino, c, t});
    }

    // METODO MOVIDO HACIA ARRIBA: Lectura y procesamiento del JSON
    void parsearJSON(string archivoRuta) {
        ifstream archivo(archivoRuta);
        if (!archivo.is_open()) {
            cout << "AVISO: No se encontro '" << archivoRuta << "'. Se iniciara con un digrafo vacio." << endl;
            return;
        }

        string bufferLinea;
        int maxIdVertice = 0;
        struct ElementoArco { int u, v, c, t; };
        vector<ElementoArco> arcosParseados;

        while (getline(archivo, bufferLinea)) {
            if (bufferLinea.find("\"origen\":") != string::npos) {
                size_t posO = bufferLinea.find("\"origen\":") + 9;
                size_t posD = bufferLinea.find("\"destino\":") + 10;
                size_t posC = bufferLinea.find("\"costo\":") + 8;
                size_t posT = bufferLinea.find("\"tiempo\":") + 9;

                int o = stoi(bufferLinea.substr(posO, bufferLinea.find(",", posO) - posO));
                int d = stoi(bufferLinea.substr(posD, bufferLinea.find(",", posD) - posD));
                int c = stoi(bufferLinea.substr(posC, bufferLinea.find(",", posC) - posC));
                int t = stoi(bufferLinea.substr(posT, bufferLinea.find("}", posT) - posT));

                arcosParseados.push_back({o, d, c, t});
                if (o > maxIdVertice) maxIdVertice = o;
                if (d > maxIdVertice) maxIdVertice = d;
            }
        }
        archivo.close();

        if (maxIdVertice > 0) {
            inicializarEstructura(maxIdVertice);
            for (auto& arco : arcosParseados) {
                registrarDireccion(arco.u, arco.v, arco.c, arco.t);
            }
            cout << "Digrafo cargado desde archivo con " << maxIdVertice << " vertices." << endl;
        }
    }

    // Escritura de datos hacia el archivo JSON
    void volcarAJSON(string archivoRuta) {
        ofstream archivo(archivoRuta);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo para guardar." << endl;
            return;
        }
        archivo << "{\n  \"grafo\": [\n";
        bool esPrimerElemento = true;
        for (int i = 1; i <= totalVertices; i++) {
            for (auto& arco : listaConexiones[i]) {
                if (!esPrimerElemento) archivo << ",\n";
                archivo << "    {\"origen\": " << i << ", \"destino\": " << arco.verticeDestino 
                        << ", \"costo\": " << arco.pesoCosto << ", \"tiempo\": " << arco.pesoTiempo << "}";
                esPrimerElemento = false;
            }
        }
        archivo << "\n  ]\n}";
        archivo.close();
        cout << "Digrafo guardado exitosamente en '" << archivoRuta << "'" << endl;
    }

    void imprimirDigrafo() {
        cout << "\n=== ESTRUCTURA DEL DIGRAFO ACTUAL ===" << endl;
        cout << "Total de vertices registrados: " << totalVertices << endl;
        bool vacio = true;
        for (int i = 1; i <= totalVertices; i++) {
            for (auto& arco : listaConexiones[i]) {
                cout << "  Origen: " << i << " --> Destino: " << arco.verticeDestino 
                     << " [Costo: " << arco.pesoCosto << ", Tiempo: " << arco.pesoTiempo << "]" << endl;
                vacio = false;
            }
        }
        if (vacio) cout << "  (El digrafo no tiene aristas aun)" << endl;
        cout << "======================================\n" << endl;
    }

    // Algoritmo de Dijkstra corregido y funcional
    void ejecutarDijkstraDirigido(int inicio, int fin, bool evaluarTiempo) {
        if (inicio > totalVertices || fin > totalVertices || inicio <= 0 || fin <= 0) {
            cout << "Nodos de inicio o fin invalidos." << endl;
            return;
        }

        vector<int> tablaCostos(totalVertices + 1, INT_MAX);
        vector<int> antecesor(totalVertices + 1, -1);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> colaDijkstra;

        tablaCostos[inicio] = 0;
        colaDijkstra.push({0, inicio});

        while (!colaDijkstra.empty()) {
            int evaluacionCosto = colaDijkstra.top().first;
            int nodoFocal = colaDijkstra.top().second;
            colaDijkstra.pop();

            if (nodoFocal == fin) break;
            if (evaluacionCosto > tablaCostos[nodoFocal]) continue;

            for (auto& trayecto : listaConexiones[nodoFocal]) {
                int metricaUso = evaluarTiempo ? trayecto.pesoTiempo : trayecto.pesoCosto;
                if (tablaCostos[nodoFocal] + metricaUso < tablaCostos[trayecto.verticeDestino]) {
                    tablaCostos[trayecto.verticeDestino] = tablaCostos[nodoFocal] + metricaUso;
                    antecesor[trayecto.verticeDestino] = nodoFocal;
                    colaDijkstra.push({tablaCostos[trayecto.verticeDestino], trayecto.verticeDestino});
                }
            }
        }

        cout << (evaluarTiempo ? "\nMejor ruta por TIEMPO: " : "\nMejor ruta por COSTO: ");
        if (tablaCostos[fin] == INT_MAX) { cout << "No hay ruta de " << inicio << " a " << fin << "." << endl; return; }
        
        vector<int> reconstruccionRuta;
        for (int paso = fin; paso != -1; paso = antecesor[paso]) reconstruccionRuta.push_back(paso);
        reverse(reconstruccionRuta.begin(), reconstruccionRuta.end());
        for (size_t i = 0; i < reconstruccionRuta.size(); i++) cout << reconstruccionRuta[i] << (i == reconstruccionRuta.size() - 1 ? "" : " -> ");
        cout << " (Total: " << tablaCostos[fin] << ")" << endl;
    }
};

void recopilarArcosManual(SistemaDigrafo &digrafo) {
    int o, d, c, t;
    cout << "\n--- Ingresa las aristas dirigidas (Origen, Destino, Costo, Tiempo) ---" << endl;
    cout << "(Para terminar, ingresa -1 en el Nodo Origen)" << endl;
    
    while (true) {
        cout << "\nNodo Origen: "; cin >> o;
        if (o == -1) break;
        cout << "Nodo Destino: "; cin >> d;
        cout << "Costo de la ruta: "; cin >> c;
        cout << "Tiempo de la ruta: "; cin >> t;

        if (o <= 0 || d <= 0) {
            cout << "Error: Los IDs de los nodos deben ser mayores a 0. Intenta de nuevo." << endl;
        } else {
            digrafo.registrarDireccion(o, d, c, t);
            cout << "Arista dirigida [" << o << " -> " << d << "] agregada temporalmente." << endl;
            cout << "Nota: Si deseas un camino de regreso, tendras que agregarlo de forma inversa como otra arista." << endl;
        }
    }
}

int main() {
    string path = "grafo.json";
    SistemaDigrafo dg;
    dg.parsearJSON(path);

    int menuSeleccion;
    do {
        cout << "\n================ GESTION DE DIGRAFOS ================" << endl;
        cout << "1. Imprimir Estructura del Digrafo" << endl;
        cout << "2. Calcular Trayectoria Minima" << endl;
        cout << "3. Insertar Nuevos Arcos Dirigidos" << endl;
        cout << "4. Inicializar Digrafo Completo Desde Cero" << endl;
        cout << "5. Confirmar Cambios y Salir" << endl;
        cout << "====================================================" << endl;
        cout << "Selecciona una opcion: "; cin >> menuSeleccion;

        switch (menuSeleccion) {
            case 1: dg.imprimirDigrafo(); break;
            case 2: {
                if (dg.obtenerCantidadVertices() == 0) {
                    cout << "Aviso: El digrafo esta vacio. Agrega datos primero." << endl;
                    break;
                }
                int inicio, fin;
                cout << "Nodo de partida: "; cin >> inicio;
                cout << "Nodo de destino: "; cin >> fin;
                dg.ejecutarDijkstraDirigido(inicio, fin, false);
                dg.ejecutarDijkstraDirigido(inicio, fin, true);
                break;
            }
            case 3:
                cout << "\n[Modificando] Recuerda que las aristas tienen direccion unica." << endl;
                recopilarArcosManual(dg);
                break;
            case 4: {
                int v;
                cout << "\n[Nuevo Digrafo] ¿Cual es el numero estimado de vertices iniciales?: "; cin >> v;
                dg.inicializarEstructura(v);
                recopilarArcosManual(dg);
                break;
            }
            case 5: dg.volcarAJSON(path); cout << "Saliendo del programa..." << endl; break;
            default: cout << "Opcion invalida. Intenta de nuevo." << endl;
        }
    } while (menuSeleccion != 5);

    return 0;
}