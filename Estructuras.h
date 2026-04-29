#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <iostream>
#include <stack>
#include <queue>
#include <list>
using namespace std;

// ===================== 16. BURBUJA =====================
class Burbuja {
public:
    void ordenar(int arr[], int n) {
        for(int i=0;i<n;i++)
            for(int j=0;j<n-i-1;j++)
                if(arr[j] > arr[j+1])
                    swap(arr[j], arr[j+1]);
    }
};

// ===================== 17. MERGE SORT =====================
class MergeSort {
private:
    void merge(int arr[], int l, int m, int r) {
        int temp[100];
        int i=l, j=m+1, k=0;

        while(i<=m && j<=r)
            temp[k++] = (arr[i]<arr[j]) ? arr[i++] : arr[j++];

        while(i<=m) temp[k++] = arr[i++];
        while(j<=r) temp[k++] = arr[j++];

        for(int i=0;i<k;i++)
            arr[l+i]=temp[i];
    }

    void dividir(int arr[], int l, int r) {
        if(l<r) {
            int m=(l+r)/2;
            dividir(arr,l,m);
            dividir(arr,m+1,r);
            merge(arr,l,m,r);
        }
    }

public:
    void ordenar(int arr[], int n) {
        dividir(arr,0,n-1);
    }
};

// ===================== 18. QUICK SORT =====================
class QuickSort {
private:
    int particion(int arr[], int low, int high) {
        int pivote = arr[high];
        int i = low - 1;

        for(int j=low;j<high;j++) {
            if(arr[j] < pivote) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i+1], arr[high]);
        return i+1;
    }

    void quick(int arr[], int low, int high) {
        if(low<high) {
            int pi = particion(arr,low,high);
            quick(arr,low,pi-1);
            quick(arr,pi+1,high);
        }
    }

public:
    void ordenar(int arr[], int n) {
        quick(arr,0,n-1);
    }
};

// ===================== 19. BURBUJA INDIRECTO =====================
class BurbujaIndirecto {
public:
    void ordenar(int arr[], int idx[], int n) {
        for(int i=0;i<n;i++)
            for(int j=0;j<n-i-1;j++)
                if(arr[idx[j]] > arr[idx[j+1]])
                    swap(idx[j], idx[j+1]);
    }
};

// ===================== 20. MERGE INDIRECTO =====================
class MergeIndirecto {
private:
    void merge(int arr[], int idx[], int l, int m, int r) {
        int temp[100];
        int i=l,j=m+1,k=0;

        while(i<=m && j<=r) {
            if(arr[idx[i]] < arr[idx[j]])
                temp[k++] = idx[i++];
            else
                temp[k++] = idx[j++];
        }

        while(i<=m) temp[k++] = idx[i++];
        while(j<=r) temp[k++] = idx[j++];

        for(int i=0;i<k;i++)
            idx[l+i]=temp[i];
    }

    void dividir(int arr[], int idx[], int l, int r) {
        if(l<r) {
            int m=(l+r)/2;
            dividir(arr,idx,l,m);
            dividir(arr,idx,m+1,r);
            merge(arr,idx,l,m,r);
        }
    }

public:
    void ordenar(int arr[], int idx[], int n) {
        dividir(arr,idx,0,n-1);
    }
};

// ===================== 21. QUICK INDIRECTO =====================
class QuickIndirecto {
private:
    int particion(int arr[], int idx[], int low, int high) {
        int pivote = arr[idx[high]];
        int i = low - 1;

        for(int j=low;j<high;j++) {
            if(arr[idx[j]] < pivote) {
                i++;
                swap(idx[i], idx[j]);
            }
        }
        swap(idx[i+1], idx[high]);
        return i+1;
    }

    void quick(int arr[], int idx[], int low, int high) {
        if(low<high) {
            int pi = particion(arr,idx,low,high);
            quick(arr,idx,low,pi-1);
            quick(arr,idx,pi+1,high);
        }
    }

public:
    void ordenar(int arr[], int idx[], int n) {
        quick(arr,idx,0,n-1);
    }
};

// ===================== 2. ADT =====================
class ADT {
private:
    int valor;
public:
    ADT(int v) { valor = v; }
    void setValor(int v) { valor = v; }
    int getValor() { return valor; }
};

// ===================== 3. PILA ESTATICA =====================
class PilaEstatica {
private:
    int arr[100];
    int tope;
public:
    PilaEstatica() { tope = -1; }

    void push(int x) {
        if(tope < 99) arr[++tope] = x;
    }

    void pop() {
        if(tope >= 0) tope--;
    }
};

// ===================== 4. COLA ESTATICA =====================
class ColaEstatica {
private:
    int arr[100];
    int frente, final;
public:
    ColaEstatica() { frente=0; final=-1; }

    void enqueue(int x) {
        if(final < 99) arr[++final] = x;
    }

    void dequeue() {
        if(frente <= final) frente++;
    }
};

// ===================== 5. LISTA ESTATICA =====================
class ListaEstatica {
private:
    int arr[100];
    int size;
public:
    ListaEstatica() { size = 0; }

    void insertar(int x) {
        if(size < 100) arr[size++] = x;
    }
};

// ===================== 6,7. ESTRUCTURAS DINAMICAS =====================
class Nodo {
public:
    int dato;
    Nodo* sig;
    Nodo(int d) { dato=d; sig=NULL; }
};

class PilaDinamica {
private:
    Nodo* tope;
public:
    PilaDinamica() { tope=NULL; }

    void push(int x) {
        Nodo* nuevo = new Nodo(x);
        nuevo->sig = tope;
        tope = nuevo;
    }

    void pop() {
        if(tope) {
            Nodo* temp = tope;
            tope = tope->sig;
            delete temp;
        }
    }
};

class ColaDinamica {
private:
    Nodo *frente, *final;
public:
    ColaDinamica() { frente=final=NULL; }

    void enqueue(int x) {
        Nodo* nuevo = new Nodo(x);
        if(!final) frente=final=nuevo;
        else {
            final->sig = nuevo;
            final = nuevo;
        }
    }

    void dequeue() {
        if(frente) {
            Nodo* temp = frente;
            frente = frente->sig;
            delete temp;
        }
    }
};

class ListaDinamica {
private:
    Nodo* cabeza;
public:
    ListaDinamica() { cabeza=NULL; }

    void insertar(int x) {
        Nodo* nuevo = new Nodo(x);
        nuevo->sig = cabeza;
        cabeza = nuevo;
    }
};

// ===================== 8. PILA STL =====================
class PilaSTL {
private:
    stack<int> pila;
public:
    void push(int x) { pila.push(x); }
    void pop() { if(!pila.empty()) pila.pop(); }
    int top() { return pila.top(); }
};

// ===================== 9. COLA STL =====================
class ColaSTL {
private:
    queue<int> cola;
public:
    void enqueue(int x) { cola.push(x); }
    void dequeue() { if(!cola.empty()) cola.pop(); }
    int front() { return cola.front(); }
};

// ===================== 10. LISTA STL =====================
class ListaSTL {
private:
    list<int> lista;
public:
    void insertarFinal(int x) { lista.push_back(x); }
    void insertarInicio(int x) { lista.push_front(x); }

    void mostrar() {
        for(int x : lista) cout << x << " ";
    }
};

#endif