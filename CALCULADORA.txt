#include <iostream>
using namespace std;

class Calculadora {

private:
    int op;
    float resultado;

public:

    Calculadora() {
        op = 0;
        resultado = 0;
    }

    void menu() {

        cout<<"\n=== CALCULADORA ===\n";
        cout<<"1. Suma\n";
        cout<<"2. Resta\n";
        cout<<"3. Multiplicacion\n";
        cout<<"4. Division\n";

        cin>>op;
    }

    int getOperacion() {
        return op;
    }

    void datos(float a[], int n) {

        for(int i=0;i<n;i++) {

            cout<<"Valor "<<i+1<<": ";
            cin>>a[i];
        }
    }

    void suma(float a[], int n) {

        resultado = 0;

        for(int i=0;i<n;i++)
            resultado += a[i];
    }

    void resta(float a[], int n) {

        resultado = a[0];

        for(int i=1;i<n;i++)
            resultado -= a[i];
    }

    void multiplicacion(float a[], int n) {

        resultado = 1;

        for(int i=0;i<n;i++)
            resultado *= a[i];
    }

    void division(float a[], int n) {

        resultado = a[0];

        for(int i=1;i<n;i++) {

            if(a[i] == 0) {
                cout<<"Error: division entre cero.\n";
                return;
            }

            resultado /= a[i];
        }
    }

    void mostrarResultado() {
        cout<<"\nResultado: "<<resultado<<endl;
    }
};

int main() {

    char continuar;

    do {

        int n;

        do{
            cout<<"\nCantidad de valores: ";
            cin>>n;
        }while(n <= 0);

        float* datos = new float[n];

        Calculadora calc;

        calc.menu();

        calc.datos(datos,n);

        switch(calc.getOperacion()) {

            case 1:
                calc.suma(datos,n);
                break;

            case 2:
                calc.resta(datos,n);
                break;

            case 3:
                calc.multiplicacion(datos,n);
                break;

            case 4:
                calc.division(datos,n);
                break;

            default:
                cout<<"Opcion invalida.\n";
        }

        calc.mostrarResultado();

        delete[] datos;

        cout<<"\nDesea continuar (S/N): ";
        cin>>continuar;

    }while(continuar=='s' || continuar=='S');

    return 0;
}