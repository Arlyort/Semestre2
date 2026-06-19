#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Red {

private:

    int cantidadNodos;

    vector<vector<int>> conexiones;

    void explorar(
        int origen,
        vector<bool>& marca,
        vector<int>& ruta)
    {

        marca[origen] = true;

        ruta.push_back(origen);

        cout << "Ruta: ";

        for(size_t i=0;
            i<ruta.size();
            i++)
        {

            cout << ruta[i];

            if(i<ruta.size()-1)
                cout << " -> ";
        }

        cout << endl;

        for(int destino:
            conexiones[origen])
        {

            if(!marca[destino])
            {
                explorar(
                    destino,
                    marca,
                    ruta
                );
            }
        }

        ruta.pop_back();

        marca[origen] = false;
    }

public:

    Red(int n)
    {
        cantidadNodos = n;

        conexiones.resize(
            cantidadNodos+1
        );
    }

    void crearRed(
        int limite)
    {

        for(int i=1;
            i<=cantidadNodos;
            i++)
        {

            for(int j=1;
                j<=limite;
                j++)
            {

                int vecino = i+j;

                if(vecino<=cantidadNodos)
                {

                    conexiones[i]
                    .push_back(vecino);

                    conexiones[vecino]
                    .push_back(i);
                }
            }
        }
    }

    void imprimir()
    {

        cout
        << "\nRED GENERADA\n\n";

        for(int i=1;
            i<=cantidadNodos;
            i++)
        {

            cout
            << "Nodo "
            << i
            << ": ";

            for(int v:
                conexiones[i])
            {
                cout
                << v
                << " ";
            }

            cout
            << endl;
        }
    }

    void mostrarRutas()
    {

        vector<bool> marca(
            cantidadNodos+1,
            false
        );

        vector<int> ruta;

        explorar(
            1,
            marca,
            ruta
        );
    }

    void guardarTXT()
    {

        ofstream archivo(
            "grafo.txt"
        );

        for(int i=1;
            i<=cantidadNodos;
            i++)
        {

            archivo
            << i
            << " -> ";

            for(int v:
                conexiones[i])
                archivo
                << v
                << " ";

            archivo
            << endl;
        }
    }

    void guardarJSON()
    {

        ofstream archivo(
            "grafo.json"
        );

        archivo
        << "{\n";

        for(int i=1;
            i<=cantidadNodos;
            i++)
        {

            archivo
            << "  \""
            << i
            << "\":[";

            for(size_t j=0;
                j<conexiones[i].size();
                j++)
            {

                archivo
                << conexiones[i][j];

                if(j<
                   conexiones[i].size()-1)
                    archivo
                    << ",";
            }

            archivo
            << "]";

            if(i<
               cantidadNodos)
                archivo
                << ",";

            archivo
            << "\n";
        }

        archivo
        << "}";
    }

    void guardarXML()
    {

        ofstream archivo(
            "grafo.xml"
        );

        archivo
        << "<?xml version=\"1.0\"?>\n";

        archivo
        << "<red>\n";

        for(int i=1;
            i<=cantidadNodos;
            i++)
        {

            archivo
            << "  <nodo id=\""
            << i
            << "\">\n";

            for(int v:
                conexiones[i])
            {

                archivo
                << "    <vecino>"
                << v
                << "</vecino>\n";
            }

            archivo
            << "  </nodo>\n";
        }

        archivo
        << "</red>";
    }
};

int main()
{
    int nodos;
    int conexionesMax;

    cout<<"Cantidad de nodos: ";
    cin>>nodos;

    cout<<"Conexiones maximas: ";
    cin>>conexionesMax;

    Red G(nodos);

    G.crearRed(
        conexionesMax
    );

    G.imprimir();

    G.mostrarRutas();

    G.guardarTXT();
    G.guardarJSON();
    G.guardarXML();

    return 0;
}