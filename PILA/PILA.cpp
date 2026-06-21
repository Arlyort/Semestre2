#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Archivos globales para el historial de la pila
const string ARCHIVO_TXT = "pila_historial.txt";
const string ARCHIVO_XML = "pila_historial.xml";
const string ARCHIVO_JSON = "pila_historial.json";

// Funciones para verificar y preparar la estructura de almacenamiento si no existen
void inicializarArchivos() {
    // Verificar o crear TXT
    ifstream comprobar_txt(ARCHIVO_TXT);
    if (!comprobar_txt.is_open()) {
        ofstream nuevo_txt(ARCHIVO_TXT);
        nuevo_txt << "=== REGISTRO DE EVENTOS DE LA PILA ===\n";
        nuevo_txt.close();
    } else {
        comprobar_txt.close();
    }

    // Verificar o crear XML
    ifstream comprobar_xml(ARCHIVO_XML);
    if (!comprobar_xml.is_open()) {
        ofstream nuevo_xml(ARCHIVO_XML);
        nuevo_xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<historial_pila>\n</historial_pila>\n";
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

// Funciones de persistencia adaptadas a los eventos de la pila (Push / Pop)
void registrarEnTXT(string accion, int valor) {
    ofstream archivo(ARCHIVO_TXT, ios::app);
    if (archivo.is_open()) {
        if (accion == "PUSH") {
            archivo << "Accion: Push (Apilar)   | Nodo: " << valor << "\n";
        } else {
            archivo << "Accion: Pop (Desapilar) | Nodo eliminado: " << valor << "\n";
        }
        archivo.close();
    }
}

void registrarEnXML(string accion, int valor) {
    ifstream lectura(ARCHIVO_XML);
    string contenido = "", linea;
    
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("</historial_pila>") != string::npos) {
                contenido += "  <evento>\n";
                contenido += "    <accion>" + accion + "</accion>\n";
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

void registrarEnJSON(string accion, int valor) {
    ifstream lectura(ARCHIVO_JSON);
    string contenido = "", linea;
    bool tieneElementosPrevios = false;

    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("\"accion\"") != string::npos) {
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
                contenido += "      \"accion\": \"" + accion + "\",\n";
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

// Tu clase abstracta base original
class Estructura {
public:
    virtual void insertar(int dato) = 0;
    virtual void eliminar() = 0;
    virtual void mostrar() = 0;
    virtual ~Estructura() {}
};

// Tu nodo original
class Nodo {
public:
    int dato;
    Nodo* siguiente;

    Nodo(int d) {
        dato = d;
        siguiente = nullptr;
    }
};

// Clase Pila implementando la persistencia integrada
class Pila : public Estructura {
private:
    Nodo* tope;

public:
    Pila() {
        tope = nullptr;
    }

    void insertar(int dato) override {
        Nodo* nuevo = new Nodo(dato);
        nuevo->siguiente = tope;
        tope = nuevo;

        // Registro de la operacion Push
        registrarEnTXT("PUSH", dato);
        registrarEnXML("PUSH", dato);
        registrarEnJSON("PUSH", dato);
    }

    void eliminar() override {
        if (tope == nullptr) {
            cout << "La pila esta vacia (Stack Underflow). Nada que eliminar." << endl;
            return;
        }

        Nodo* aux = tope;
        int valorEliminado = aux->dato; // Guardar antes de liberar memoria
        tope = tope->siguiente;
        delete aux;

        // Registro de la operacion Pop
        registrarEnTXT("POP", valorEliminado);
        registrarEnXML("POP", valorEliminado);
        registrarEnJSON("POP", valorEliminado);
        cout << "Elemento " << valorEliminado << " desapilado y registrado correctamente." << endl;
    }

    void mostrar() override {
        if (tope == nullptr) {
            cout << "Pila vacia." << endl;
            return;
        }

        Nodo* aux = tope;
        cout << "\n--- ESTADO ACTUAL DE LA PILA ---" << endl;
        while (aux != nullptr) {
            cout << " | " << aux->dato << " | " << (aux == tope ? "<- TOPE" : "") << endl;
            aux = aux->siguiente;
        }
        cout << " -----\n" << endl;
    }

    ~Pila() {
        while (tope != nullptr) {
            Nodo* aux = tope;
            tope = tope->siguiente;
            delete aux;
        }
    }
};

int main() {
    // Inicializa el sistema de control de archivos en disco
    inicializarArchivos();

    Pila p;
    int opcion, valor;

    do {
        cout << "================ CONTROL DE PILA CON LOGS ================" << endl;
        cout << "1. Apilar elemento (Push / Insertar)" << endl;
        cout << "2. Desapilar elemento (Pop / Eliminar)" << endl;
        cout << "3. Mostrar estado de la Pila" << endl;
        cout << "4. Salir" << endl;
        cout << "==========================================================" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Digita el valor entero a empujar (Push): ";
                cin >> valor;
                p.insertar(valor);
                cout << "Valor " << valor << " añadido e indexado en TXT, XML y JSON." << endl;
                break;

            case 2:
                p.eliminar();
                break;

            case 3:
                p.mostrar();
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