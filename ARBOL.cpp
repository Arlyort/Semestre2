#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Elemento {

public:

    int clave;
    vector<Elemento*> descendientes;

    Elemento(int valor) {
        clave = valor;
    }
};

class EstructuraArborea {

private:

    Elemento* inicio;
    vector<Elemento*> coleccion;

    void dibujar(
        Elemento* actual,
        string sangria,
        bool ultimo)
    {

        cout << sangria;

        if(actual != inicio)
            cout << (ultimo ? "└─ " : "├─ ");

        cout << actual->clave << endl;

        for(size_t i=0;
            i<actual->descendientes.size();
            i++)
        {

            dibujar(
                actual->descendientes[i],
                sangria +
                (ultimo ? "   " : "│  "),
                i == actual->descendientes.size()-1
            );
        }
    }

public:

    EstructuraArborea() {
        inicio = nullptr;
    }

    void construir(
        int cantidad,
        int ramas)
    {

        for(auto e : coleccion)
            delete e;

        coleccion.clear();

        for(int i=1;i<=cantidad;i++)
            coleccion.push_back(
                new Elemento(i)
            );

        inicio = coleccion[0];

        int posicion = 1;

        for(size_t i=0;
            i<coleccion.size() &&
            posicion<cantidad;
            i++)
        {

            for(int j=0;
                j<ramas &&
                posicion<cantidad;
                j++)
            {

                coleccion[i]
                ->descendientes
                .push_back(
                    coleccion[posicion]
                );

                posicion++;
            }
        }
    }

    void mostrar() {

        cout << "\nESTRUCTURA ARBOREA\n\n";

        dibujar(
            inicio,
            "",
            true
        );
    }

    void exportarTXT() {

        ofstream salida(
            "arbol.txt"
        );

        for(auto nodo:coleccion)
        {

            salida
            << nodo->clave
            << " -> ";

            for(auto hijo:
                nodo->descendientes)
            {
                salida
                << hijo->clave
                << " ";
            }

            salida << endl;
        }
    }

    void exportarJSON() {

        ofstream salida(
            "arbol.json"
        );

        salida << "{\n";

        for(size_t i=0;
            i<coleccion.size();
            i++)
        {

            salida
            << "  \""
            << coleccion[i]->clave
            << "\":[";

            for(size_t j=0;
                j<coleccion[i]
                ->descendientes.size();
                j++)
            {

                salida
                << coleccion[i]
                ->descendientes[j]
                ->clave;

                if(j <
                coleccion[i]
                ->descendientes.size()-1)
                    salida << ",";
            }

            salida << "]";

            if(i<
               coleccion.size()-1)
                salida << ",";

            salida << "\n";
        }

        salida << "}";
    }

    void exportarXML() {

        ofstream salida(
            "arbol.xml"
        );

        salida
        << "<?xml version=\"1.0\"?>\n";

        salida
        << "<estructura>\n";

        for(auto nodo:coleccion)
        {

            salida
            << "  <elemento id=\""
            << nodo->clave
            << "\">\n";

            for(auto hijo:
                nodo->descendientes)
            {

                salida
                << "    <rama>"
                << hijo->clave
                << "</rama>\n";
            }

            salida
            << "  </elemento>\n";
        }

        salida
        << "</estructura>";
    }

    ~EstructuraArborea() {

        for(auto e:coleccion)
            delete e;
    }
};

int main()
{
    int nodos;
    int ramas;

    cout<<"Cantidad de nodos: ";
    cin>>nodos;

    cout<<"Ramas por nodo: ";
    cin>>ramas;

    EstructuraArborea A;

    A.construir(
        nodos,
        ramas
    );

    A.mostrar();

    A.exportarTXT();
    A.exportarJSON();
    A.exportarXML();

    return 0;
}