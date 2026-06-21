#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// Archivos globales para el historial de ordenamiento indirecto
const string ARCHIVO_TXT = "orden_indirecto_historial.txt";
const string ARCHIVO_XML = "orden_indirecto_historial.xml";
const string ARCHIVO_JSON = "orden_indirecto_historial.json";

// Funciones para verificar y preparar la estructura de almacenamiento si no existen
void inicializarArchivos() {
    // Verificar o crear TXT
    ifstream comprobar_txt(ARCHIVO_TXT);
    if (!comprobar_txt.is_open()) {
        ofstream nuevo_txt(ARCHIVO_TXT);
        nuevo_txt << "=== REGISTRO DE EVENTOS DE ORDENAMIENTO INDIRECTO ===\n";
        nuevo_txt.close();
    } else {
        comprobar_txt.close();
    }

    // Verificar o crear XML
    ifstream comprobar_xml(ARCHIVO_XML);
    if (!comprobar_xml.is_open()) {
        ofstream nuevo_xml(ARCHIVO_XML);
        nuevo_xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<historial_indirecto>\n</historial_indirecto>\n";
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

// Convierte un arreglo entero en una cadena separada por espacios
string transformarArregloAString(int arr[], int tam) {
    string resultado = "";
    for (int i = 0; i < tam; i++) {
        resultado += to_string(arr[i]) + (i == tam - 1 ? "" : " ");
    }
    return resultado;
}

// Genera la cadena de la secuencia de datos usando el mapeo de los indices dados
string mapearResultadoString(int datos[], int indices[], int tam) {
    string resultado = "";
    for (int i = 0; i < tam; i++) {
        resultado += to_string(datos[indices[i]]) + (i == tam - 1 ? "" : " ");
    }
    return resultado;
}

// Funciones de persistencia que guardan el estado de los indices y el resultado final
void registrarEnTXT(string algoritmo, int datos[], int indAntes[], int indDespues[], int tam) {
    ofstream archivo(ARCHIVO_TXT, ios::app);
    if (archivo.is_open()) {
        archivo << "Algoritmo: " << algoritmo << " | Elementos: " << tam << "\n";
        archivo << "  Datos Fijos:       " << transformarArregloAString(datos, tam) << "\n";
        archivo << "  Indices Iniciales: " << transformarArregloAString(indAntes, tam) << "\n";
        archivo << "  Indices Finales:   " << transformarArregloAString(indDespues, tam) << "\n";
        archivo << "  Resultado Visual:  " << mapearResultadoString(datos, indDespues, tam) << "\n";
        archivo << "--------------------------------------------------------\n";
        archivo.close();
    }
}

void registrarEnXML(string algoritmo, int datos[], int indAntes[], int indDespues[], int tam) {
    ifstream lectura(ARCHIVO_XML);
    string contenido = "", linea;
    
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("</historial_indirecto>") != string::npos) {
                contenido += "  <evento>\n";
                contenido += "    <algoritmo>" + algoritmo + "</algoritmo>\n";
                contenido += "    <datos_fijos>" + transformarArregloAString(datos, tam) + "</datos_fijos>\n";
                contenido += "    <indices_iniciales>" + transformarArregloAString(indAntes, tam) + "</indices_iniciales>\n";
                contenido += "    <indices_finales>" + transformarArregloAString(indDespues, tam) + "</indices_finales>\n";
                contenido += "    <resultado_visual>" + mapearResultadoString(datos, indDespues, tam) + "</resultado_visual>\n";
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

void registrarEnJSON(string algoritmo, int datos[], int indAntes[], int indDespues[], int tam) {
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
                contenido += "      \"datos_fijos\": \"" + transformarArregloAString(datos, tam) + "\",\n";
                contenido += "      \"indices_iniciales\": \"" + transformarArregloAString(indAntes, tam) + "\",\n";
                contenido += "      \"indices_finales\": \"" + transformarArregloAString(indDespues, tam) + "\",\n";
                contenido += "      \"resultado_visual\": \"" + mapearResultadoString(datos, indDespues, tam) + "\"\n";
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

// Tu clase base original para Ordenamiento Indirecto
class OrdenamientoIndirecto {
protected:
    int* datos;
    int* indice;
    int n;

public:
    OrdenamientoIndirecto(int arr[], int tam) {
        n = tam;
        datos = new int[n];
        indice = new int[n];

        for (int i = 0; i < n; i++) {
            datos[i] = arr[i];
            indice[i] = i;
        }
    }

    virtual void ordenar() = 0;

    void mostrar() {
        cout << "\nOrden indirecto:\n";
        for (int i = 0; i < n; i++)
            cout << datos[indice[i]] << " ";
        cout << endl;
    }

    virtual ~OrdenamientoIndirecto() {
        delete[] datos;
        delete[] indice;
    }
};

// Tu clase QuickIndirecto modificada con llamadas de volcado sincronizado
class QuickIndirecto : public OrdenamientoIndirecto {
private:
    int particion(int bajo, int alto) {
        int pivote = datos[indice[alto]];
        int i = bajo - 1;

        for (int j = bajo; j < alto; j++) {
            if (datos[indice[j]] < pivote) {
                i++;
                swap(indice[i], indice[j]);
            }
        }
        swap(indice[i + 1], indice[alto]);
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
    QuickIndirecto(int arr[], int tam)
        : OrdenamientoIndirecto(arr, tam) {}

    void ordenar() override {
        // Hacemos una copia del arreglo de indices original (0, 1, 2, ...) antes de alterarlo
        int* indicesOriginales = new int[n];
        for (int i = 0; i < n; i++) {
            indicesOriginales[i] = indice[i];
        }

        // Llamada al metodo de ordenamiento nativo
        quick(0, n - 1);

        // Registro exhaustivo multi-formato tras concluir el algoritmo
        registrarEnTXT("QuickIndirecto", datos, indicesOriginales, indice, n);
        registrarEnXML("QuickIndirecto", datos, indicesOriginales, indice, n);
        registrarEnJSON("QuickIndirecto", datos, indicesOriginales, indice, n);

        delete[] indicesOriginales;
    }
};

int main() {
    // Inicializar los archivos planos y de marcado al arrancar
    inicializarArchivos();

    int opcion;
    do {
        cout << "================ ORDENAMIENTO INDIRECTO CON LOGS ================" << endl;
        cout << "1. Ingresar datos y ejecutar QuickSort Indirecto" << endl;
        cout << "2. Salir" << endl;
        cout << "=================================================================" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            int tam;
            cout << "¿Cuantos elementos deseas procesar de forma indirecta?: ";
            cin >> tam;

            if (tam <= 0) {
                cout << "Cantidad fuera de rango.\n" << endl;
                continue;
            }

            int* v = new int[tam];
            cout << "Ingresa los " << tam << " valores del arreglo:" << endl;
            for (int i = 0; i < tam; i++) {
                cout << "Dato [" << i + 1 << "]: ";
                cin >> v[i];
            }

            QuickIndirecto obj(v, tam);

            cout << "\nProcesando ordenamiento indirecto y guardando archivos..." << endl;
            obj.ordenar();

            obj.mostrar();
            cout << "Los mapeos de memoria se han exportado exitosamente a TXT, XML y JSON.\n" << endl;

            delete[] v;
        } else if (opcion != 2) {
            cout << "Opcion no reconocida. Reintenta.\n" << endl;
        }

    } while (opcion != 2);

    cout << "Finalizando operaciones y asegurando escrituras." << endl;
    return 0;
}