#include "red.h"
#include "iostream"
#include "fstream"

class Red {
private:
    vector<Enrutador> enrutadores;

public:
    Red();

    // Archivos y Flujo Principal
    bool cargarEnrutadores();
    void ejecutar();

    // Gestión de Nodos (Enrutadores)
    void agregarEnrutador(Enrutador enrutador);
    void eliminarEnrutador(string nombre);

    // Gestión de Enlaces (Conexiones)
    void agregarEnlace(string origen, string destino, int costo);
    void eliminarEnlace(string origen, string destino);

    // Algoritmos y Cálculo
    void actualizarTablaCostos();
    void actualizarCaminos();
    void recalcularTodo();

    // Consultas
    void impresion(string origen, string destino);
};

Red::Red():enrutadores() {}
/*bool Red::cargarEnrutadores(){
    string archi= "C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\practica_4\\enutadores";
    ifstream archivo(archi);
    string linea;
    try{
        if(!archivo.is_open()){
            throw string("No se pudo abrir el archivo");
        }
        while(getline(archivo,linea)){
            stringstream flujo(linea);
            string enru,costo,princi;
            getline(flujo,princi,';');
            enrutadores.push_back(Enrutador(princi));
            while(getline(flujo,enru,",")){
                getline(flujo,costo,";");

            }

        }
    }
}
*/
void Red::agregarEnrutador(Enrutador enrutador){
    enrutadores.push_back(enrutador);
}
void Red::eliminarEnrutador(string nombre){
    for(auto i=enrutadores.begin();i!=enrutadores.end();++i){
        if (i->getNombre()==nombre){
            this->enrutadores.erase(i);
            return;
        }
    }
}

void Red::agregarEnlace(string origen, string destino, int costo){
    bool a=false,b=false;
    for(auto i=enrutadores.begin();i!=enrutadores.end();++i){
        if(i->getNombre()==origen){
            i->agregarVecino(destino,costo);
            a=true;
        }
        else if (i->getNombre()==destino){
            i->agregarVecino(origen,costo);
            b=true;
        }
        if(a==true&& b==true){
            return;

    }
}
