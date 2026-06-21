#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// Archivos globales para el historial de ordenamiento
const string ARCHIVO_TXT = "ordenamiento_historial.txt";
const string ARCHIVO_XML = "ordenamiento_historial.xml";
const string ARCHIVO_JSON = "ordenamiento_historial.json";

// Funciones para verificar y preparar la estructura de almacenamiento si no existen
void inicializarArchivos() {
    // Verificar o crear TXT
    ifstream comprobar_txt(ARCHIVO_TXT);
    if (!comprobar_txt.is_open()) {
        ofstream nuevo_txt(ARCHIVO_TXT);
        nuevo_txt << "=== REGISTRO DE EVENTOS DE ORDENAMIENTO ===\n";
        nuevo_txt.close();
    } else {
        comprobar_txt.close();
    }

    // Verificar o crear XML
    ifstream comprobar_xml(ARCHIVO_XML);
    if (!comprobar_xml.is_open()) {
        ofstream nuevo_xml(ARCHIVO_XML);
        nuevo_xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<historial_ordenamiento>\n</historial_ordenamiento>\n";
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

// Convierte un arreglo entero en una cadena separada por espacios o comas
string transformarArregloAString(int arr[], int tam) {
    string resultado = "";
    for (int i = 0; i < tam; i++) {
        resultado += to_string(arr[i]) + (i == tam - 1 ? "" : " ");
    }
    return resultado;
}

// Funciones de persistencia adaptadas para guardar los arreglos antes y despues
void registrarEnTXT(string algoritmo, int antes[], int despues[], int tam) {
    ofstream archivo(ARCHIVO_TXT, ios::app);
    if (archivo.is_open()) {
        archivo << "Algoritmo: " << algoritmo << " | Tam: " << tam << "\n";
        archivo << "  Original: " << transformarArregloAString(antes, tam) << "\n";
        archivo << "  Ordenado: " << transformarArregloAString(despues, tam) << "\n";
        archivo << "-------------------------------------------\n";
        archivo.close();
    }
}

void registrarEnXML(string algoritmo, int antes[], int despues[], int tam) {
    ifstream lectura(ARCHIVO_XML);
    string contenido = "", linea;
    
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("</historial_ordenamiento>") != string::npos) {
                contenido += "  <evento>\n";
                contenido += "    <algoritmo>" + algoritmo + "</algoritmo>\n";
                contenido += "    <elementos>" + to_string(tam) + "</elementos>\n";
                contenido += "    <original>" + transformarArregloAString(antes, tam) + "</original>\n";
                contenido += "    <ordenado>" + transformarArregloAString(despues, tam) + "</ordenado>\n";
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

void registrarEnJSON(string algoritmo, int antes[], int despues[], int tam) {
    ifstream lectura(ARCHIVO_JSON);
    string contenido = "", linea;
    bool tieneElementosPrevios = false;

    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("\"algoritmo\"") != string::npos) {
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
                contenido += "      \"algoritmo\": \"" + algoritmo + "\",\n";
                contenido += "      \"elementos\": " + to_string(tam) + ",\n";
                contenido += "      \"original\": \"" + transformarArregloAString(antes, tam) + "\",\n";
                contenido += "      \"ordenado\": \"" + transformarArregloAString(despues, tam) + "\"\n";
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

// Tu clase base original con un metodo para capturar el estado previo antes del ordenamiento
class Ordenamiento {
protected:
    int* datos;
    int n;

public:
    Ordenamiento(int arr[], int tam) {
        n = tam;
        datos = new int[n];
        for (int i = 0; i < n; i++)
            datos[i] = arr[i];
    }

    virtual void ordenar() = 0;

    void mostrar() {
        for (int i = 0; i < n; i++)
            cout << datos[i] << " ";
        cout << endl;
    }

    virtual ~Ordenamiento() {
        delete[] datos;
    }
};

// Tu clase QuickSort modificada para invocar los guardados en disco tras procesar
class QuickSort : public Ordenamiento {
private:
    int particion(int bajo, int alto) {
        int pivote = datos[alto];
        int i = bajo - 1;

        for (int j = bajo; j < alto; j++) {
            if (datos[j] < pivote) {
                i++;
                swap(datos[i], datos[j]);
            }
        }
        swap(datos[i + 1], datos[alto]);
        return i + 1;
    }

    void quick(int bajo, int alto) {
        if (bajo < alto) {
            int pi = particion(bajo, alto);
            quick(bajo, pi - 1);
            quick(pi + 1, alto);
        }
    }

public:
    QuickSort(int arr[], int tam)
        : Ordenamiento(arr, tam) {}

    void ordenar() override {
        // Guardamos una copia del estado inicial (desordenado) para el registro
        int* estadoOriginal = new int[n];
        for (int i = 0; i < n; i++) {
            estadoOriginal[i] = datos[i];
        }

        // Ejecutamos tu algoritmo QuickSort nativo
        quick(0, n - 1);

        // Al finalizar, registramos sincronizadamente el antes y el despues
        registrarEnTXT("QuickSort", estadoOriginal, datos, n);
        registrarEnXML("QuickSort", estadoOriginal, datos, n);
        registrarEnJSON("QuickSort", estadoOriginal, datos, n);

        delete[] estadoOriginal;
    }
};

int main() {
    // Inicializa la estructura base de datos en disco si no existe
    inicializarArchivos();

    int opcion;
    do {
        cout << "================ GESTION DE ORDENAMIENTOS ================" << endl;
        cout << "1. Ingresar datos y ejecutar QuickSort" << endl;
        cout << "2. Salir" << endl;
        cout << "==========================================================" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            int tam;
            cout << "¿Cuantos elementos deseas ordenar?: ";
            cin >> tam;

            if (tam <= 0) {
                cout << "Cantidad invalida." << endl;
                continue;
            }

            int* v = new int[tam];
            cout << "Ingresa los " << tam << " numeros enteros uno por uno:" << endl;
            for (int i = 0; i < tam; i++) {
                cout << "Elemento [" << i + 1 << "]: ";
                cin >> v[i];
            }

            QuickSort q(v, tam);
            
            cout << "\nArreglo original ingresado: ";
            q.mostrar();

            cout << "Ordenando y guardando tracking en logs..." << endl;
            q.ordenar();

            cout << "Arreglo ordenado final: ";
            q.mostrar();
            cout << "Historial actualizado con exito en TXT, XML y JSON.\n" << endl;

            delete[] v;
        } else if (opcion != 2) {
            cout << "Opcion invalida. Intenta de nuevo.\n" << endl;
        }

    } while (opcion != 2);

    cout << "Saliendo y cerrando hilos de escritura..." << endl;
    return 0;
}