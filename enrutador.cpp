#include "enrutador.h"
Enrutador::Enrutador(): nombre(""),vecinos(),tablaCostos(),tablaCaminos() {}
Enrutador::Enrutador(string nombre):nombre(nombre){}

string Enrutador::getNombre(){
    return this->nombre;
}
map<string,int> Enrutador::getVecinos(){
    return this-> vecinos;
}
map<string,int> Enrutador::getTablaCostos(){
    return this->tablaCostos;
}
map<string,vector<string>> Enrutador::getTablaCaminos(){
    return this-> tablaCaminos;
}
void Enrutador::setTablaCostos(map<string, int> nuevosCostos){
    this->tablaCostos=nuevosCostos;
}
void Enrutador::setTablaCaminos(map<string, vector<string>> nuevosCaminos){
    this->tablaCaminos=nuevosCaminos;
}
void Enrutador::agregarVecino(string vecino, int costo){
    this->vecinos[vecino]=costo;
}
void Enrutador::eliminarVecino(string vecino){
    this->vecinos.erase(vecino);
}
void Enrutador::limpiarTablas(){
    this->tablaCostos.clear();
    this->tablaCaminos.clear();
}


