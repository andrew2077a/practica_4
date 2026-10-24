#ifndef RED_H
#define RED_H

#include <string>
#include <vector>
#include <map>
#include "Enrutador.h"

using namespace std;

class Red {
private:
    vector<Enrutador> enrutadores;
    map<string,map<string,int>>grafoGlobal;

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
    void construirGrafos();
    void actualizarTablaCostos();
    void actualizarCaminos();
    void recalcularTodo();
    // Consultas
    void impresion(string origen, string destino);
};

#endif // RED_H
