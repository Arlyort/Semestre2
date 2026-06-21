#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Archivos globales para el historial de la lista
const string ARCHIVO_TXT = "lista_historial.txt";
const string ARCHIVO_XML = "lista_historial.xml";
const string ARCHIVO_JSON = "lista_historial.json";

// Funciones para verificar y preparar la estructura de almacenamiento si no existen
void inicializarArchivos() {
    // Verificar o crear TXT
    ifstream comprobar_txt(ARCHIVO_TXT);
    if (!comprobar_txt.is_open()) {
        ofstream nuevo_txt(ARCHIVO_TXT);
        nuevo_txt << "=== REGISTRO DE EVENTOS DE LA LISTA ENLAZADA ===\n";
        nuevo_txt.close();
    } else {
        comprobar_txt.close();
    }

    // Verificar o crear XML
    ifstream comprobar_xml(ARCHIVO_XML);
    if (!comprobar_xml.is_open()) {
        ofstream nuevo_xml(ARCHIVO_XML);
        nuevo_xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<historial_lista>\n</historial_lista>\n";
        nuevo_xml.close();
    } else {
        comprobar_xml.close();
    }

    // Verificar o crear JSON
    ifstream comprobar_json(ARCHIVO_JSON);
    if (!comprobar_json.is_open()) {
        ofstream nuevo_json(ARCHIVO_JSON);
        nuevo_json << "{\n  \"eventos\": [\n  ]\n}";
        nuevo_json.close();
    } else {
        comprobar_json.close();
    }
}

// Funciones de persistencia adaptadas a los eventos de la lista
void registrarEnTXT(string metodo, int valor) {
    ofstream archivo(ARCHIVO_TXT, ios::app);
    if (archivo.is_open()) {
        archivo << "Accion: Insertar (" << metodo << ") | Nodo: " << valor << "\n";
        archivo.close();
    }
}

void registrarEnXML(string metodo, int valor) {
    ifstream lectura(ARCHIVO_XML);
    string contenido = "", linea;
    
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("</historial_lista>") != string::npos) {
                contenido += "  <evento>\n";
                contenido += "    <metodo>" + metodo + "</metodo>\n";
                contenido += "    <valor>" + to_string(valor) + "</valor>\n";
                contenido += "  </evento>\n";
            }
            contenido += linea + "\n";
        }
        lectura.close();

        ofstream escritura(ARCHIVO_XML);
        if (escritura.is_open()) {
            escritura << contenido;
            escritura.close();
        }
    }
}

void registrarEnJSON(string metodo, int valor) {
    ifstream lectura(ARCHIVO_JSON);
    string contenido = "", linea;
    bool tieneElementosPrevios = false;

    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("\"metodo\"") != string::npos) {
                tieneElementosPrevios = true;
                break;
            }
        }
        lectura.close();
    }

    lectura.open(ARCHIVO_JSON);
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("]") != string::npos) {
                if (tieneElementosPrevios) {
                    contenido += ",\n";
                }
                contenido += "    {\n";
                contenido += "      \"metodo\": \"" + metodo + "\",\n";
                contenido += "      \"valor\": " + to_string(valor) + "\n";
                contenido += "    }\n";
            }
            contenido += linea + "\n";
        }
        lectura.close();

        ofstream escritura(ARCHIVO_JSON);
        if (escritura.is_open()) {
            escritura << contenido;
            escritura.close();
        }
    }
}

// Tu nodo original
class Nodo {
public:
    int dato;
    Nodo* sig;

    Nodo(int d) {
        dato = d;
        sig = nullptr;
    }
};

// Clase Lista con persistencia integrada en sus metodos de insercion
class Lista {
private:
    Nodo* cabeza;

public:
    Lista() {
        cabeza = nullptr;
    }

    void insertarInicio(int dato) {
        Nodo* nuevo = new Nodo(dato);
        nuevo->sig = cabeza;
        cabeza = nuevo;

        // Registro de la insercion al inicio
        registrarEnTXT("INICIO", dato);
        registrarEnXML("INICIO", dato);
        registrarEnJSON("INICIO", dato);
    }

    void insertarFinal(int dato) {
        Nodo* nuevo = new Nodo(dato);

        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo* aux = cabeza;
            while (aux->sig != nullptr)
                aux = aux->sig;
            aux->sig = nuevo;
        }

        // Registro de la insercion al final
        registrarEnTXT("FINAL", dato);
        registrarEnXML("FINAL", dato);
        registrarEnJSON("FINAL", dato);
    }

    void mostrar() {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia." << endl;
            return;
        }

        Nodo* aux = cabeza;
        cout << "Cabeza -> ";
        while (aux != nullptr) {
            cout << "[" << aux->dato << "] -> ";
            aux = aux->sig;
        }
        cout << "NULL" << endl;
    }

    ~Lista() {
        while (cabeza != nullptr) {
            Nodo* aux = cabeza;
            cabeza = cabeza->sig;
            delete aux;
        }
    }
};

int main() {
    // Inicializa el sistema de archivos en disco
    inicializarArchivos();

    Lista l;
    int opcion, valor;

    do {
        cout << "================ GESTION DE LISTA CON LOGS ================" << endl;
        cout << "1. Insertar elemento al Inicio" << endl;
        cout << "2. Insertar elemento al Final" << endl;
        cout << "3. Mostrar Lista enlazada" << endl;
        cout << "4. Salir" << endl;
        cout << "===========================================================" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Digita el valor entero a insertar al inicio: ";
                cin >> valor;
                l.insertarInicio(valor);
                cout << "Valor " << valor << " insertado y registrado en TXT, XML y JSON." << endl;
                break;

            case 2:
                cout << "Digita el valor entero a insertar al final: ";
                cin >> valor;
                l.insertarFinal(valor);
                cout << "Valor " << valor << " insertado y registrado en TXT, XML y JSON." << endl;
                break;

            case 3:
                cout << "\nEstado actual de la lista:" << endl;
                l.mostrar();
                break;

            case 4:
                cout << "Saliendo y cerrando hilos de escritura..." << endl;
                break;

            default:
                cout << "Opcion invalida. Intenta de nuevo." << endl;
        }
        cout << endl;
    } while (opcion != 4);

    return 0;
}