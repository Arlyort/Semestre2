#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <climits>
#include <algorithm>
#include <sstream>

using namespace std;

struct Enlace {
    int nodoDestino;
    int valorCosto;
    int valorTiempo;
};

class RedGrafo {
private:
    int totalNodos;
    vector<vector<Enlace>> listaAdyacencia;

public:
    // Constructor con inicializacion por defecto
    RedGrafo(int numNodos = 0) {
        configurarEstructura(numNodos);
    }

    // Cambiado de orden: El inicializador ahora esta arriba
    void configurarEstructura(int numNodos) {
        totalNodos = numNodos;
        listaAdyacencia.assign(numNodos + 1, vector<Enlace>());
    }

    int contarNodos() const { 
        return totalNodos; 
    }

    // Verifica y expande la dimension del vector si ingresan IDs mas grandes
    void ajustarDimension(int nodoId) {
        if (nodoId > totalNodos) {
            totalNodos = nodoId;
            listaAdyacencia.resize(totalNodos + 1);
        }
    }

    // Agrega aristas de forma simetrica (No dirigido)
    void registrarConexion(int idOrigen, int idDestino, int costo, int tiempo) {
        ajustarDimension(max(idOrigen, idDestino));
        listaAdyacencia[idOrigen].push_back({idDestino, costo, tiempo});
        listaAdyacencia[idDestino].push_back({idOrigen, costo, tiempo});
    }

    // Cambiado de orden: La lectura del archivo ahora se encuentra antes de mostrar y del Dijkstra
    void leerDesdeJSON(string pathArchivo) {
        ifstream archivo(pathArchivo);
        if (!archivo.is_open()) {
            cout << "AVISO: No se encontro '" << pathArchivo << "'. Se iniciara con un grafo vacio." << endl;
            return;
        }

        string cadenaLinea;
        int maxNodoEncontrado = 0;
        struct RegistroTemporal { int ori, des, cos, tie; };
        vector<RegistroTemporal> bufferDatos;

        while (getline(archivo, cadenaLinea)) {
            if (cadenaLinea.find("\"origen\":") != string::npos) {
                size_t pOri = cadenaLinea.find("\"origen\":") + 9;
                size_t pDes = cadenaLinea.find("\"destino\":") + 10;
                size_t pCos = cadenaLinea.find("\"costo\":") + 8;
                size_t pTie = cadenaLinea.find("\"tiempo\":") + 9;

                int o = stoi(cadenaLinea.substr(pOri, cadenaLinea.find(",", pOri) - pOri));
                int d = stoi(cadenaLinea.substr(pDes, cadenaLinea.find(",", pDes) - pDes));
                int c = stoi(cadenaLinea.substr(pCos, cadenaLinea.find(",", pCos) - pCos));
                int t = stoi(cadenaLinea.substr(pTie, cadenaLinea.find("}", pTie) - pTie));

                bufferDatos.push_back({o, d, c, t});
                if (o > maxNodoEncontrado) maxNodoEncontrado = o;
                if (d > maxNodoEncontrado) maxNodoEncontrado = d;
            }
        }
        archivo.close();

        if (maxNodoEncontrado > 0) {
            configurarEstructura(maxNodoEncontrado);
            for (auto& reg : bufferDatos) {
                registrarConexion(reg.ori, reg.des, reg.cos, reg.tie);
            }
            cout << "Grafo cargado desde archivo con " << maxNodoEncontrado << " vertices." << endl;
        }
    }

    // Exporta el contenido al JSON sin comas sueltas al final de la lista
    void escribirHaciaJSON(string pathArchivo) {
        ofstream archivo(pathArchivo);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo para guardar." << endl;
            return;
        }
        archivo << "{\n  \"grafo\": [\n";
        bool banderaPrimerNodo = true;
        for (int i = 1; i <= totalNodos; i++) {
            for (auto& item : listaAdyacencia[i]) {
                if (i < item.nodoDestino) { 
                    if (!banderaPrimerNodo) archivo << ",\n";
                    archivo << "    {\"origen\": " << i << ", \"destino\": " << item.nodoDestino 
                            << ", \"costo\": " << item.valorCosto << ", \"tiempo\": " << item.valorTiempo << "}";
                    banderaPrimerNodo = false;
                }
            }
        }
        archivo << "\n  ]\n}";
        archivo.close();
        cout << "Grafo guardado exitosamente en '" << pathArchivo << "'" << endl;
    }

    void desplegarGrafo() {
        cout << "\n=== ESTRUCTURA DEL GRAFO ACTUAL ===" << endl;
        cout << "Total de vertices registrados: " << totalNodos << endl;
        bool vacio = true;
        for (int i = 1; i <= totalNodos; i++) {
            for (auto& item : listaAdyacencia[i]) {
                if (i < item.nodoDestino) {
                    cout << "  Origen: " << i << " -> Destino: " << item.nodoDestino 
                         << " [Costo: " << item.valorCosto << ", Tiempo: " << item.valorTiempo << "]" << endl;
                    vacio = false;
                }
            }
        }
        if (vacio) cout << "  (El grafo no tiene aristas aun)" << endl;
        cout << "===================================\n" << endl;
    }

    // Implementacion de Dijkstra con nombres de variables cambiados
    void calcularRutaMasCorta(int nodoInicio, int nodoFin, bool utilizarTiempo) {
        if (nodoInicio > totalNodos || nodoFin > totalNodos || nodoInicio <= 0 || nodoFin <= 0) {
            cout << "Nodos de inicio o fin invalidos." << endl;
            return;
        }

        vector<int> acumuladoPesos(totalNodos + 1, INT_MAX);
        vector<int> nodoPredecesor(totalNodos + 1, -1);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> colaPrioridad;

        acumuladoPesos[nodoInicio] = 0;
        colaPrioridad.push({0, nodoInicio});

        while (!colaPrioridad.empty()) {
            int pesoActual = colaPrioridad.top().first;
            int nodoActual = colaPrioridad.top().second;
            colaPrioridad.pop();

            if (nodoActual == nodoFin) break;
            if (pesoActual > acumuladoPesos[nodoActual]) continue;

            for (auto& vinculo : listaAdyacencia[nodoActual]) {
                int factorEvaluado = utilizarTiempo ? vinculo.valorTiempo : vinculo.valorCosto;
                if (acumuladoPesos[nodoActual] + factorEvaluado < acumuladoPesos[vinculo.nodoDestino]) {
                    acumuladoPesos[vinculo.nodoDestino] = acumuladoPesos[nodoActual] + factorEvaluado;
                    nodoPredecesor[vinculo.nodoDestino] = nodoActual;
                    colaPrioridad.push({acumuladoPesos[vinculo.nodoDestino], vinculo.nodoDestino});
                }
            }
        }

        cout << (utilizarTiempo ? "\nMejor ruta por TIEMPO: " : "\nMejor ruta por COSTO: ");
        if (acumuladoPesos[nodoFin] == INT_MAX) { cout << "No hay ruta de " << nodoInicio << " a " << nodoFin << "." << endl; return; }
        
        vector<int> secuenciaCamino;
        for (int paso = nodoFin; paso != -1; paso = nodoPredecesor[paso]) secuenciaCamino.push_back(paso);
        reverse(secuenciaCamino.begin(), secuenciaCamino.end());
        for (size_t idx = 0; idx < secuenciaCamino.size(); idx++) cout << secuenciaCamino[idx] << (idx == secuenciaCamino.size() - 1 ? "" : " -> ");
        cout << " (Total: " << acumuladoPesos[nodoFin] << ")" << endl;
    }
};

void ingresarNuevasAristas(RedGrafo &red) {
    int o, d, c, t;
    cout << "\n--- Ingresa las aristas (Origen, Destino, Costo, Tiempo) ---" << endl;
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
            red.registrarConexion(o, d, c, t);
            cout << "Arista agregada temporalmente." << endl;
        }
    }
}

int main() {
    string fJson = "grafo.json";
    RedGrafo g;
    g.leerDesdeJSON(fJson);

    int opcMenu;
    do {
        cout << "\n================ MENU DE GRAFOS ================" << endl;
        cout << "1. Mostrar Grafo Actual" << endl;
        cout << "2. Buscar Mejor Ruta (Dijkstra)" << endl;
        cout << "3. Modificar Grafo (Agregar nuevas conexiones)" << endl;
        cout << "4. Crear un Grafo Nuevo desde cero" << endl;
        cout << "5. Guardar Cambios y Salir" << endl;
        cout << "================================================" << endl;
        cout << "Selecciona una opcion: "; cin >> opcMenu;

        switch (opcMenu) {
            case 1: g.desplegarGrafo(); break;
            case 2: {
                if (g.contarNodos() == 0) {
                    cout << "Aviso: El grafo esta vacio. Agrega datos primero." << endl;
                    break;
                }
                int inicio, fin;
                cout << "Nodo de partida: "; cin >> inicio;
                cout << "Nodo de destino: "; cin >> fin;
                g.calcularRutaMasCorta(inicio, fin, false);
                g.calcularRutaMasCorta(inicio, fin, true);
                break;
            }
            case 3:
                cout << "\n[Modificando Grafo] Agrega nuevas aristas. Si usas IDs mas altos, el grafo crecera automaticamente." << endl;
                ingresarNuevasAristas(g);
                break;
            case 4: {
                int n;
                cout << "\n[Creando Nuevo Grafo] ¿Cual es el numero estimado de vertices iniciales?: "; cin >> n;
                g.configurarEstructura(n);
                ingresarNuevasAristas(g);
                break;
            }
            case 5: g.escribirHaciaJSON(fJson); cout << "Saliendo del programa..." << endl; break;
            default: cout << "Opcion invalida. Intenta de nuevo." << endl;
        }
    } while (opcMenu != 5);

    return 0;
}