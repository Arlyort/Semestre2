#include <iostream>
using namespace std;

class Estructura {
public:
    virtual void insertar(int dato) = 0;
    virtual void eliminar() = 0;
    virtual void mostrar() = 0;
    virtual ~Estructura() {}
};

class Nodo {
public:
    int dato;
    Nodo* siguiente;

    Nodo(int d) {
        dato = d;
        siguiente = nullptr;
    }
};

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
    }

    void eliminar() override {
        if(tope != nullptr) {
            Nodo* aux = tope;
            tope = tope->siguiente;
            delete aux;
        }
    }

    void mostrar() override {
        Nodo* aux = tope;

        cout << "\nPila:\n";

        while(aux != nullptr) {
            cout << aux->dato << endl;
            aux = aux->siguiente;
        }
    }

    ~Pila() {
        while(tope != nullptr)
            eliminar();
    }
};

int main() {

    Pila p;

    p.insertar(10);
    p.insertar(20);
    p.insertar(30);

    p.mostrar();

    p.eliminar();

    p.mostrar();

    return 0;
}