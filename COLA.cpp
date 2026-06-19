#include <iostream>
using namespace std;

class Estructura {
public:
    virtual void insertar(int) = 0;
    virtual void eliminar() = 0;
    virtual void mostrar() = 0;
};

class Nodo {
public:
    int dato;
    Nodo* sig;

    Nodo(int d) {
        dato = d;
        sig = nullptr;
    }
};

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

        if(fin == nullptr) {
            frente = fin = nuevo;
        }
        else {
            fin->sig = nuevo;
            fin = nuevo;
        }
    }

    void eliminar() override {

        if(frente == nullptr)
            return;

        Nodo* aux = frente;

        frente = frente->sig;

        delete aux;
    }

    void mostrar() override {

        Nodo* aux = frente;

        while(aux != nullptr) {
            cout << aux->dato << " ";
            aux = aux->sig;
        }

        cout << endl;
    }

    ~Cola() {
        while(frente != nullptr)
            eliminar();
    }
};

int main() {

    Cola c;

    c.insertar(5);
    c.insertar(10);
    c.insertar(15);

    c.mostrar();

    return 0;
}