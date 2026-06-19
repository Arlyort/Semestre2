#include <iostream>
using namespace std;

class Nodo {
public:
    int dato;
    Nodo* sig;

    Nodo(int d) {
        dato = d;
        sig = nullptr;
    }
};

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
    }

    void insertarFinal(int dato) {

        Nodo* nuevo = new Nodo(dato);

        if(cabeza == nullptr) {
            cabeza = nuevo;
            return;
        }

        Nodo* aux = cabeza;

        while(aux->sig != nullptr)
            aux = aux->sig;

        aux->sig = nuevo;
    }

    void mostrar() {

        Nodo* aux = cabeza;

        while(aux != nullptr) {
            cout << aux->dato << " ";
            aux = aux->sig;
        }

        cout << endl;
    }

    ~Lista() {

        while(cabeza != nullptr) {

            Nodo* aux = cabeza;

            cabeza = cabeza->sig;

            delete aux;
        }
    }
};

int main() {

    Lista l;

    l.insertarInicio(10);
    l.insertarInicio(20);
    l.insertarFinal(50);

    l.mostrar();

    return 0;
}