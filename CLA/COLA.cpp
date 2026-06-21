#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Archivos globales para el historial de la cola
const string ARCHIVO_TXT = "cola_historial.txt";
const string ARCHIVO_XML = "cola_historial.xml";
const string ARCHIVO_JSON = "cola_historial.json";

// Funciones para verificar y preparar la estructura de almacenamiento
void inicializarArchivos() {
    // Verificar o crear TXT
    ifstream comprobar_txt(ARCHIVO_TXT);
    if (!comprobar_txt.is_open()) {
        ofstream nuevo_txt(ARCHIVO_TXT);
        nuevo_txt << "=== REGISTRO DE EVENTOS DE LA COLA ===\n";
        nuevo_txt.close();
    } else {
        comprobar_txt.close();
    }

    // Verificar o crear XML
    ifstream comprobar_xml(ARCHIVO_XML);
    if (!comprobar_xml.is_open()) {
        ofstream nuevo_xml(ARCHIVO_XML);
        nuevo_xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<historial_cola>\n</historial_cola>\n";
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

// Funciones de persistencia adaptadas a los eventos de la cola
void registrarEnTXT(string accion, int valor) {
    ofstream archivo(ARCHIVO_TXT, ios::app);
    if (archivo.is_open()) {
        if (accion == "INSERTAR") {
            archivo << "Accion: Encolar | Nodo: " << valor << "\n";
        } else {
            archivo << "Accion: Desencolar | Nodo eliminado: " << valor << "\n";
        }
        archivo.close();
    }
}

void registrarEnXML(string accion, int valor) {
    ifstream lectura(ARCHIVO_XML);
    string contenido = "", linea;
    
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("</historial_cola>") != string::npos) {
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
    virtual void insertar(int) = 0;
    virtual void eliminar() = 0;
    virtual void mostrar() = 0;
};

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

// Clase Cola implementando persistencia integrada en sus metodos
class Cola : public Estructura {
private:
    Nodo* frente;
    Nodo* fin;

public:
    Cola() {
        frente = fin = nullptr;
    }

    void insertar(int dato) override {
        Nodo* nuevo = new Nodo(dato);

        if (fin == nullptr) {
            frente = fin = nuevo;
        } else {
            fin->sig = nuevo;
            fin = nuevo;
        }

        // Caracteristica extra: Registro en cadena
        registrarEnTXT("INSERTAR", dato);
        registrarEnXML("INSERTAR", dato);
        registrarEnJSON("INSERTAR", dato);
    }

    void eliminar() override {
        if (frente == nullptr) {
            cout << "La cola esta vacia. Nada que eliminar." << endl;
            return;
        }

        Nodo* aux = frente;
        int valorEliminado = aux->dato; // Guardamos el dato antes de borrarlo de memoria
        frente = frente->sig;

        if (frente == nullptr) {
            fin = nullptr;
        }

        delete aux;

        // Caracteristica extra: Registro en cadena
        registrarEnTXT("ELIMINAR", valorEliminado);
        registrarEnXML("ELIMINAR", valorEliminado);
        registrarEnJSON("ELIMINAR", valorEliminado);
        cout << "Elemento " << valorEliminado << " eliminado y registrado correctamente." << endl;
    }

    void mostrar() override {
        if (frente == nullptr) {
            cout << "Cola vacia." << endl;
            return;
        }

        Nodo* aux = frente;
        cout << "Frente -> ";
        while (aux != nullptr) {
            cout << "[" << aux->dato << "] ";
            aux = aux->sig;
        }
        cout << "<- Fin" << endl;
    }

    ~Cola() {
        while (frente != nullptr) {
            Nodo* aux = frente;
            frente = frente->sig;
            delete aux;
        }
    }
};

int main() {
    // Crea las estructuras base si no existen en la carpeta del programa
    inicializarArchivos();

    Cola c;
    int opcion, valor;

    do {
        cout << "\n================ CONTROL DE COLA CON LOGS ================" << endl;
        cout << "1. Encolar elemento (Insertar)" << endl;
        cout << "2. Desencolar elemento (Eliminar)" << endl;
        cout << "3. Mostrar estado de la Cola" << endl;
        cout << "4. Salir" << endl;
        cout << "==========================================================" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Digita el valor entero a insertar: ";
                cin >> valor;
                c.insertar(valor);
                cout << "Valor " << valor << " registrado en los logs de TXT, XML y JSON." << endl;
                break;

            case 2:
                c.eliminar();
                break;

            case 3:
                cout << "\nEstado actual en memoria:" << endl;
                c.mostrar();
                break;

            case 4:
                cout << "Saliendo y cerrando flujos de datos..." << endl;
                break;

            default:
                cout << "Opcion invalida. Intenta de nuevo." << endl;
        }
    } while (opcion != 4);

    return 0;
}