#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Estructura del nodo del arbol
struct Nodo {
    int dato;
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(int val) {
        dato = val;
        izquierdo = nullptr;
        derecho = nullptr;
    }
};

class ArbolBinario {
private:
    Nodo* raiz;

    // Auxiliar para insertar de manera recursiva (Mantiene propiedad de BST)
    Nodo* insertarRaiz(Nodo* nodo, int dato) {
        if (nodo == nullptr) {
            return new Nodo(dato);
        }
        if (dato < nodo->dato) {
            nodo->izquierdo = insertarRaiz(nodo->izquierdo, dato);
        } else if (dato > nodo->dato) {
            nodo->derecho = insertarRaiz(nodo->derecho, dato);
        }
        return nodo;
    }

    // Auxiliar para mostrar el arbol de forma visual en la terminal
    void mostrarArbolAsimetrico(Nodo* nodo, int espacio) {
        if (nodo == nullptr) return;

        espacio += 5;
        mostrarArbolAsimetrico(nodo->derecho, espacio);

        cout << "\n";
        for (int i = 5; i < espacio; i++) cout << " ";
        cout << nodo->dato << "\n";

        mostrarArbolAsimetrico(nodo->izquierdo, espacio);
    }

    // Auxiliar para liberar la memoria
    void destruirArbol(Nodo* nodo) {
        if (nodo != nullptr) {
            destruirArbol(nodo->izquierdo);
            destruirArbol(nodo->derecho);
            delete nodo;
        }
    }

    // --- AUXILIARES DE EXPORTACION ---

    // TXT: Recorrido en Preorden (Raiz - Izquierdo - Derecho) con marcas '#' para nulos
    void exportarTXT(Nodo* nodo, ofstream& f) {
        if (nodo == nullptr) {
            f << "# ";
            return;
        }
        f << nodo->dato << " ";
        exportarTXT(nodo->izquierdo, f);
        exportarTXT(nodo->derecho, f);
    }

    // JSON: Estructura recursiva de objetos anidados
    void exportarJSON(Nodo* nodo, ofstream& f, int sangria) {
        string indent(sangria, ' ');
        if (nodo == nullptr) {
            f << "null";
            return;
        }
        f << "{\n";
        f << indent << "  \"dato\": " << nodo->dato << ",\n";
        f << indent << "  \"izquierdo\": ";
        exportarJSON(nodo->izquierdo, f, sangria + 2);
        f << ",\n";
        f << indent << "  \"derecho\": ";
        exportarJSON(nodo->derecho, f, sangria + 2);
        f << "\n" << indent << "}";
    }

    // XML: Etiquetas anidadas estructuradas
    void exportarXML(Nodo* nodo, ofstream& f, int sangria) {
        string indent(sangria, ' ');
        if (nodo == nullptr) return;

        f << indent << "<nodo>\n";
        f << indent << "  <dato>" << nodo->dato << "</dato>\n";
        
        if (nodo->izquierdo != nullptr) {
            f << indent << "  <izquierdo>\n";
            exportarXML(nodo->izquierdo, f, sangria + 4);
            f << indent << "  </izquierdo>\n";
        }
        if (nodo->derecho != nullptr) {
            f << indent << "  <derecho>\n";
            exportarXML(nodo->derecho, f, sangria + 4);
            f << indent << "  </derecho>\n";
        }
        f << indent << "</nodo>\n";
    }

public:
    ArbolBinario() {
        raiz = nullptr;
    }

    ~ArbolBinario() {
        destruirArbol(raiz);
    }

    void vaciarArbol() {
        destruirArbol(raiz);
        raiz = nullptr;
    }

    void insertar(int dato) {
        raiz = insertarRaiz(raiz, dato);
    }

    void mostrar() {
        if (raiz == nullptr) {
            cout << "El arbol esta vacio." << endl;
            return;
        }
        cout << "\n=== ESTRUCTURA DEL ARBOL (ROTADO 90 GRADOS) ===" << endl;
        mostrarArbolAsimetrico(raiz, 0);
        cout << "===============================================" << endl;
    }

    // Guarda de manera consecutiva en los 3 formatos solicitados
    void guardarFormatos(string n_json, string n_txt, string n_xml) {
        // 1. JSON
        ofstream f_json(n_json);
        if (f_json.is_open()) {
            exportarJSON(raiz, f_json, 0);
            f_json.close();
            cout << "Arbol guardado exitosamente en '" << n_json << "'" << endl;
        }

        // 2. TXT
        ofstream f_txt(n_txt);
        if (f_txt.is_open()) {
            exportarTXT(raiz, f_txt);
            f_txt.close();
            cout << "Arbol guardado exitosamente en '" << n_txt << "'" << endl;
        }

        // 3. XML
        ofstream f_xml(n_xml);
        if (f_xml.is_open()) {
            f_xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            f_xml << "<arbol>\n";
            exportarXML(raiz, f_xml, 2);
            f_xml << "</arbol>\n";
            f_xml.close();
            cout << "Arbol guardado exitosamente en '" << n_xml << "'" << endl;
        }
    }

    // Lee el archivo JSON e inserta los numeros extrayendo los valores de las etiquetas `"dato":`
    void cargarJSON(string nombreArchivo) {
        ifstream f(nombreArchivo);
        if (!f.is_open()) {
            cout << "AVISO: No se encontro '" << nombreArchivo << "'. Se iniciara con un arbol vacio." << endl;
            return;
        }

        vaciarArbol(); // Limpiar datos previos si los hay
        string linea;
        int nodosCargados = 0;

        while (getline(f, linea)) {
            size_t posDato = linea.find("\"dato\":");
            if (posDato != string::npos) {
                size_t inicioNum = posDato + 7;
                size_t finNum = linea.find(",", inicioNum);
                
                string numeroStr = linea.substr(inicioNum, finNum - inicioNum);
                
                // Limpieza de espacios en blanco
                stringstream ss(numeroStr);
                int valor;
                if (ss >> valor) {
                    insertar(valor);
                    nodosCargados++;
                }
            }
        }
        f.close();

        if (nodosCargados > 0) {
            cout << "Arbol cargado desde archivo con " << nodosCargados << " nodos." << endl;
        }
    }
};

// Funcion para leer y mostrar el texto bruto de un archivo JSON en consola
void imprimirArchivoPlano(string nombreArchivo) {
    ifstream f(nombreArchivo);
    if (!f.is_open()) {
        cout << "Error: No se puede leer el archivo '" << nombreArchivo << "' o no existe." << endl;
        return;
    }
    cout << "\n--- CONTENIDO DIRECTO DEL ARCHIVO JSON ---" << endl;
    string linea;
    while (getline(f, linea)) {
        cout << linea << "\n";
    }
    cout << "------------------------------------------" << endl;
    f.close();
}

int main() {
    ArbolBinario arbol;
    string arch_json = "arbol.json";
    string arch_txt = "arbol.txt";
    string arch_xml = "arbol.xml";

    // Reconstruye el arbol leyendo el archivo JSON actual al arrancar
    arbol.cargarJSON(arch_json);

    int opcion;
    do {
        cout << "\n================ MENU DE ARBOLES ================" << endl;
        cout << "1. Insertar un elemento (Modificar arbol)" << endl;
        cout << "2. Mostrar estructura grafica del arbol" << endl;
        cout << "3. Ver el archivo JSON crudo (Texto plano)" << endl;
        cout << "4. Crear un arbol nuevo desde cero" << endl;
        cout << "5. Guardar cambios (JSON, TXT, XML) y salir" << endl;
        cout << "=================================================" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int valor;
                cout << "Digita el valor entero a insertar: ";
                cin >> valor;
                arbol.insertar(valor);
                cout << "Valor " << valor << " agregado temporalmente al arbol." << endl;
                break;
            }
            case 2:
                arbol.mostrar();
                break;

            case 3:
                imprimirArchivoPlano(arch_json);
                break;

            case 4:
                arbol.vaciarArbol();
                cout << "Se ha limpiado el arbol actual. Puedes empezar a insertar desde cero." << endl;
                break;

            case 5:
                arbol.guardarFormatos(arch_json, arch_txt, arch_xml);
                cout << "Archivos actualizados. Saliendo del programa..." << endl;
                break;

            default:
                cout << "Opcion invalida. Intenta de nuevo." << endl;
        }
    } while (opcion != 5);

    return 0;
}