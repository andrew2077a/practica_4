#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class Enrutador {
private:
    string nombre;
    map<string, int> vecinos;
    map<string, int> tablaCostos;
    map<string, vector<string>> tablaCaminos;

public:
    Enrutador();
    Enrutador(string nombre);
    // Getters
    string getNombre();
    map<string, int> getVecinos();
    map<string, int> getTablaCostos();
    map<string, vector<string>> getTablaCaminos();

    // Setters y Modificadores
    void setTablaCostos(map<string, int> nuevosCostos);
    void setTablaCaminos(map<string, vector<string>> nuevosCaminos);
    void agregarVecino(string vecino, int costo);
    void eliminarVecino(string vecino);
    void limpiarTablas();
};

#endif // ENRUTADOR_H
