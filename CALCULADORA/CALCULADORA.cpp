#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Funciones auxiliares para verificar y preparar la estructura de los archivos
void inicializarArchivos(string f_txt, string f_xml, string f_json) {
    // Verificar o crear TXT
    ifstream comprobar_txt(f_txt);
    if (!comprobar_txt.is_open()) {
        ofstream nuevo_txt(f_txt);
        nuevo_txt << "=== HISTORIAL DE OPERACIONES CALCULADORA ===\n";
        nuevo_txt.close();
    } else {
        comprobar_txt.close();
    }

    // Verificar o crear XML
    ifstream comprobar_xml(f_xml);
    if (!comprobar_xml.is_open()) {
        ofstream nuevo_xml(f_xml);
        nuevo_xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<historial>\n</historial>\n";
        nuevo_xml.close();
    } else {
        comprobar_xml.close();
    }

    // Verificar o crear JSON
    ifstream comprobar_json(f_json);
    if (!comprobar_json.is_open()) {
        ofstream nuevo_json(f_json);
        nuevo_json << "{\n  \"operaciones\": [\n  ]\n}";
        nuevo_json.close();
    } else {
        comprobar_json.close();
    }
}

// Guarda la operacion en formato TXT (Anexa al final del archivo)
void guardarEnTXT(string nombreArchivo, string operacion, double resultado) {
    ofstream archivo(nombreArchivo, ios::app); // ios::app sirve para escribir al final sin borrar lo anterior
    if (archivo.is_open()) {
        archivo << operacion << " = " << resultado << "\n";
        archivo.close();
    }
}

// Guarda la operacion en formato XML inyectando el nodo antes del cierre de la etiqueta raiz
void guardarEnXML(string nombreArchivo, double n1, string op, double n2, double res) {
    ifstream lectura(nombreArchivo);
    string contenido = "", linea;
    
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            // Buscamos la etiqueta de cierre del historial para meter los datos antes
            if (linea.find("</historial>") != string::npos) {
                contenido += "  <operacion>\n";
                contenido += "    <num1>" + to_string(n1) + "</num1>\n";
                contenido += "    <operador>" + op + "</operador>\n";
                contenido += "    <num2>" + to_string(n2) + "</num2>\n";
                contenido += "    <resultado>" + to_string(res) + "</resultado>\n";
                contenido += "  </operacion>\n";
            }
            contenido += linea + "\n";
        }
        lectura.close();

        ofstream escritura(nombreArchivo);
        if (escritura.is_open()) {
            escritura << contenido;
            escritura.close();
        }
    }
}

// Guarda la operacion en formato JSON manejando dinamicamente la colocacion de comas estructurales
void guardarEnJSON(string nombreArchivo, double n1, string op, double n2, double res) {
    ifstream lectura(nombreArchivo);
    string contenido = "", linea;
    bool tieneElementosPrevios = false; // LINEA 82: Corregida sin espacios

    // Primero analizamos si el JSON ya tiene elementos dentro del arreglo para saber si poner coma
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("\"num1\"") != string::npos) {
                tieneElementosPrevios = true;
                break;
            }
        }
        lectura.close();
    }

    lectura.open(nombreArchivo);
    if (lectura.is_open()) {
        while (getline(lectura, linea)) {
            if (linea.find("]") != string::npos) {
                if (tieneElementosPrevios) {
                    contenido += ",\n"; // Si ya habia una operacion, cerramos la anterior con coma
                }
                contenido += "    {\n";
                contenido += "      \"num1\": " + to_string(n1) + ",\n";
                contenido += "      \"operador\": \"" + op + "\",\n";
                contenido += "      \"num2\": " + to_string(n2) + ",\n";
                contenido += "      \"resultado\": " + to_string(res) + "\n";
                contenido += "    }\n";
            }
            contenido += linea + "\n";
        }
        lectura.close();

        ofstream escritura(nombreArchivo);
        if (escritura.is_open()) {
            escritura << contenido;
            escritura.close();
        }
    }
}

int main() {
    string archivo_txt = "resultados.txt";
    string archivo_xml = "resultados.xml";
    string archivo_json = "resultados.json";

    // Llamada obligatoria al iniciar el programa: Crea los archivos base si no existen
    inicializarArchivos(archivo_txt, archivo_xml, archivo_json);

    int opcion;
    double num1, num2, resultado;

    do {
        cout << "\n================ CALCULADORA CON HISTORIAL ================" << endl;
        cout << "1. Sumar (+)" << endl;
        cout << "2. Restar (-)" << endl;
        cout << "3. Multiplicar (*)" << endl;
        cout << "4. Dividir (/)" << endl;
        cout << "5. Salir" << endl;
        cout << "===========================================================" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        if (opcion >= 1 && opcion <= 4) {
            cout << "Ingresa el primer numero: "; cin >> num1;
            cout << "Ingresa el segundo numero: "; cin >> num2;
        } else if (opcion == 5) {
            cout << "Cerrando la calculadora..." << endl;
            break;
        } else {
            cout << "Opcion invalida. Intenta de nuevo." << endl;
            continue;
        }

        string operacionStr = "";
        string operadorSigno = "";
        bool operacionValida = true;

        switch (opcion) {
            case 1:
                resultado = num1 + num2;
                operadorSigno = "+";
                operacionStr = to_string(num1) + " + " + to_string(num2);
                break;
            case 2:
                resultado = num1 - num2;
                operadorSigno = "-";
                operacionStr = to_string(num1) + " - " + to_string(num2);
                break;
            case 3:
                resultado = num1 * num2;
                operadorSigno = "*";
                operacionStr = to_string(num1) + " * " + to_string(num2);
                break;
            case 4:
                if (num2 == 0) {
                    cout << "Error: No se permite la division entre cero." << endl;
                    operacionValida = false;
                } else {
                    resultado = num1 / num2;
                    operadorSigno = "/";
                    operacionStr = to_string(num1) + " / " + to_string(num2);
                }
                break;
        }

        if (operacionValida) {
            cout << "Resultado: " << resultado << endl;
            cout << "Guardando registro en TXT, XML y JSON..." << endl;
            
            // Envio sincronizado de los datos calculados a los tres formatos
            guardarEnTXT(archivo_txt, operacionStr, resultado);
            guardarEnXML(archivo_xml, num1, operadorSigno, num2, resultado);
            guardarEnJSON(archivo_json, num1, operadorSigno, num2, resultado);
        }

    } while (opcion != 5);

    return 0;
}