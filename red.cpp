#include "red.h"
#include "iostream"
#include "fstream"
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>


Red::Red():enrutadores() {}

// Lee el archivo de topología. Formato de cada línea: A;B,4;C,10;D,5
// (enrutador; vecino,costo; vecino,costo; ...)
bool Red::cargarEnrutadores(){
    string archi= "C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\practica_4\\enrutadores";
    ifstream archivo(archi);

    // Si la ruta fija no abre, se intenta con el archivo en la carpeta del programa
    if(!archivo.is_open()){
        archivo.clear();
        archivo.open("enrutadores");
    }

    string linea;
    try{
        if(!archivo.is_open()){
            throw string("No se pudo abrir el archivo");
        }
        while(getline(archivo,linea)){
            if(!linea.empty() && linea.back()=='\r') linea.pop_back();

            // Se ignoran las líneas en blanco
            if(linea.empty()) continue;

            stringstream flujo(linea);
            string enru,costo,princi;

            getline(flujo,princi,';');

            if(princi.empty()) continue;

            enrutadores.push_back(Enrutador(princi));

            // Se agregan los vecinos de la línea al último enrutador creado
            while(getline(flujo,enru,',')){
                getline(flujo,costo,';');
                enrutadores.back().agregarVecino(enru,stoi(costo));
            }
        }
        archivo.close();
    }
    catch(string &error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    // Captura el error de stoi cuando el costo está vacío o no es un número
    catch(...){
        cout<<"Error: el archivo tiene un formato invalido"<<endl;
        return false;
    }
    return true;
}

// Agrega un enrutador nuevo, sin permitir nombres repetidos
void Red::agregarEnrutador(Enrutador enrutador){
    for(auto &e:enrutadores){
        if(e.getNombre()==enrutador.getNombre()){
            cout<<"Ya existe un enrutador con ese nombre"<<endl;
            return;
        }
    }
    enrutadores.push_back(enrutador);
}

// Elimina un enrutador y lo quita de los vecinos de los demás
void Red::eliminarEnrutador(string nombre){
    bool existe=false;
    for(auto i=enrutadores.begin();i!=enrutadores.end();++i){
        if (i->getNombre()==nombre){
            this->enrutadores.erase(i);
            existe=true;
            break;
        }
    }
    if(!existe){
        cout<<"El enrutador no existe"<<endl;
        return;
    }
    for(auto &e:enrutadores){
        e.eliminarVecino(nombre);
    }
}

// Crea un enlace en ambos sentidos entre dos enrutadores existentes
void Red::agregarEnlace(string origen, string destino, int costo){
    // Validaciones
    if(origen==destino){
        cout<<"No se puede enlazar un enrutador consigo mismo"<<endl;
        return;
    }
    if(costo<=0){
        cout<<"El costo debe ser mayor que cero"<<endl;
        return;
    }
    bool existeO=false,existeD=false;
    for(auto &e:enrutadores){
        if(e.getNombre()==origen) existeO=true;
        if(e.getNombre()==destino) existeD=true;
    }
    if(!existeO || !existeD){
        cout<<"Alguno de los enrutadores no existe"<<endl;
        return;
    }

    // Se agrega el vecino en cada uno de los dos enrutadores
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
}

// Elimina el enlace en ambos sentidos entre dos enrutadores
void Red::eliminarEnlace(string origen, string destino){
    bool a=false,b=false;
    for(auto i=enrutadores.begin();i!=enrutadores.end();++i){
        if(i->getNombre()==origen){
            i->eliminarVecino(destino);
            a=true;
        }
        else if (i->getNombre()==destino){
            i->eliminarVecino(origen);
            b=true;
        }
        if(a==true&& b==true){
            return;
        }
    }
}

// Algoritmo de Dijkstra: calcula el costo mínimo desde nodoOrigen hacia todos los nodos
void dijkstra(string nodoOrigen,const map<string, map<string, int>>& grafo,map<string, int>& tablaCostos) {

    // Todos los costos empiezan en "infinito" y el del origen en 0
    for (const auto& par : grafo) {
        tablaCostos[par.first] = 999999;
    }
    tablaCostos[nodoOrigen] = 0;

    // Cola ordenada de {costoAcumulado, nodo}: siempre se procesa el de menor costo
    set<pair<int, string>> conjuntoNodos;
    conjuntoNodos.insert({0, nodoOrigen});

    while (!conjuntoNodos.empty()) {

        auto actual = *conjuntoNodos.begin();
        conjuntoNodos.erase(conjuntoNodos.begin());

        int costoActual = actual.first;
        string u = actual.second;

        // Se descartan registros viejos si ya hay una mejor ruta
        if (costoActual > tablaCostos[u]) continue;

        if (grafo.count(u)) {

            // Se revisan los vecinos de u
            for (const auto& vecino : grafo.at(u)) {
                string v = vecino.first;
                int peso = vecino.second;

                // Si pasar por u es más barato, se actualiza el costo de v
                if (tablaCostos[u] + peso < tablaCostos[v]) {
                    conjuntoNodos.erase({tablaCostos[v], v});
                    tablaCostos[v] = tablaCostos[u] + peso;
                    conjuntoNodos.insert({tablaCostos[v], v});
                }
            }
        }
    }
}

// Arma el grafo de toda la red a partir de los vecinos de cada enrutador.
// Solo se copian los vecinos que existen como enrutador.
void Red::construirGrafos(){

    grafoGlobal.clear();

    set<string> nombres;
    for(auto& i:enrutadores){
        nombres.insert(i.getNombre());
    }

    for(auto& i:enrutadores){

        map<string,int> vecinosValidos;
        for(auto& v:i.getVecinos()){
            if(nombres.count(v.first)){
                vecinosValidos[v.first]=v.second;
            }
        }
        grafoGlobal[i.getNombre()]=vecinosValidos;
    }
}

// Calcula con Dijkstra la tabla de costos de cada enrutador
void Red::actualizarTablaCostos() {
    for (auto i = enrutadores.begin(); i != enrutadores.end(); ++i) {
        map<string, int> nuevosCostos;

        dijkstra(i->getNombre(),this-> grafoGlobal, nuevosCostos);

        i->setTablaCostos(nuevosCostos);
    }
}

// Igual que dijkstra, pero además guarda en 'previo' el nodo anterior de cada nodo,
// lo que permite reconstruir los caminos completos
void dijkstraRutas(string nodoOrigen,const map<string, map<string, int>>& grafo,map<string, int>& tablaCostos,map<string, string>& previo) {

    for (const auto& par : grafo) {
        tablaCostos[par.first] = 999999;
    }
    tablaCostos[nodoOrigen] = 0;

    set<pair<int, string>> conjuntoNodos;
    conjuntoNodos.insert({0, nodoOrigen});

    while (!conjuntoNodos.empty()) {
        auto actual = *conjuntoNodos.begin();
        conjuntoNodos.erase(conjuntoNodos.begin());

        int costoActual = actual.first;
        string u = actual.second;

        if (costoActual > tablaCostos[u]) continue;

        if (grafo.count(u)) {
            for (const auto& vecino : grafo.at(u)) {
                string v = vecino.first;
                int peso = vecino.second;

                if (tablaCostos[u] + peso < tablaCostos[v]) {
                    conjuntoNodos.erase({tablaCostos[v], v});

                    tablaCostos[v] = tablaCostos[u] + peso;
                    // Para llegar a 'v' el nodo anterior es 'u'
                    previo[v] = u;

                    conjuntoNodos.insert({tablaCostos[v], v});
                }
            }
        }
    }
}

// Calcula, para cada enrutador, el camino completo hacia cada destino
void Red::actualizarCaminos(){
    for(auto i=enrutadores.begin();i!=enrutadores.end();++i){
        map<string, string> previo;
        map<string, int> nuevosCostos;
        map<string, vector<string>> nuevosCaminos;

        string origen = i->getNombre();

        dijkstraRutas(origen, grafoGlobal, nuevosCostos, previo);

        // Se arma un camino para cada destino
        for(auto &d : nuevosCostos){
            string destino = d.first;

            // Se saltan el propio origen y los destinos inalcanzables
            if(destino == origen || d.second >= 999999) continue;

            if(previo.find(destino)==previo.end()) continue;

            // Se retrocede desde el destino hasta el origen usando 'previo'
            vector<string> camino;
            string actual = destino;

            while(actual != origen){
                camino.push_back(actual);
                actual = previo[actual];
            }
            camino.push_back(origen);

            // El camino quedó de destino a origen, se invierte
            reverse(camino.begin(), camino.end());

            nuevosCaminos[destino] = camino;
        }

        i->setTablaCaminos(nuevosCaminos);
    }
}

// Reconstruye el grafo y actualiza costos y caminos de todos los enrutadores.
// Se llama después de cualquier cambio en la topología.
void Red:: recalcularTodo(){
    construirGrafos();
    actualizarCaminos();
    actualizarTablaCostos();
}

// Muestra el camino y el costo entre dos enrutadores
void Red::impresion(string origen, string destino){
    for(auto i=enrutadores.begin();i!=enrutadores.end();++i){
        if(i->getNombre()==origen){

            if(origen==destino){
                cout<<"El origen y el destino son el mismo enrutador"<<endl;
                return;
            }

            map<string,vector<string>> cami=i->getTablaCaminos();

            // Si el destino no está en la tabla, no existe o no hay conexión
            auto it=cami.find(destino);
            if(it==cami.end()){
                cout<<"No hay conexion entre los enrutadores"<<endl;
                return;
            }

            cout<<"---------------------------------------------"<<endl;
            for (auto &e:it->second){
                if(e==destino){
                    cout<<e;
                    continue;
                }
                cout<<e<<"->";
            }
            cout<<endl;
            map<string,int> costo= i->getTablaCostos();
            cout<<"Costo asociado: "<<costo[destino]<<endl;

            return;
        }
    }
    cout<<"El enrutador origen no existe"<<endl;
}

// Flujo principal: carga el archivo y muestra el menú hasta que el usuario salga
void Red::ejecutar(){
    if(!cargarEnrutadores()){
        cout<<"No se pudo abrir el archivo"<<endl;
        return;
    }
    recalcularTodo();

    int x,costo;
    string origen,destino;
    while(true){
        cout << "==================================================" << endl;
        cout << "              MENU DE OPCIONES                    " << endl;
        cout << "==================================================" << endl;
        cout << " 1. Agregar enrutador" << endl;
        cout << " 2. Eliminar enrutador" << endl;
        cout << " 3. Consultar camino entre dos enrutadores" << endl;
        cout << " 4. Agregar enlace" << endl;
        cout << " 5. Eliminar enlace" << endl;
        cout << " 0. Salir" << endl;
        cout << "==================================================" << endl;

        // Si el usuario escribe algo que no es número, se limpia la entrada
        if(!(cin>>x)){
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"ingreso no valido"<<endl;
            continue;
        }

        switch (x) {
        case 0:
            return;
        case 1:
            cout<<"Ingrese nombre del enrutador"<<endl;
            cin>>origen;
            agregarEnrutador(Enrutador(origen));
            break;
        case 2:
            cout<<"Ingrese nombre del enrutador"<<endl;
            cin>>origen;
            eliminarEnrutador(origen);
            break;
        case 3:
            cout<<"Ingrese origen"<<endl;
            cin>>origen;
            cout<<"Ingrese destino"<<endl;
            cin>>destino;
            impresion(origen,destino);
            break;
        case 4:
            cout<<"Ingrese nombre del enrutador origen"<<endl;
            cin>>origen;
            cout<<"Ingrese nombre del enrutador destino"<<endl;
            cin>>destino;
            cout<<"ingrese costo"<<endl;

            if(!(cin>>costo)){
                cin.clear();
                cin.ignore(1000,'\n');
                cout<<"costo no valido"<<endl;
                break;
            }
            agregarEnlace(origen,destino,costo);
            break;
        case 5:
            cout<<"Ingrese nombre del enrutador origen"<<endl;
            cin>>origen;
            cout<<"Ingrese nombre del enrutador destino"<<endl;
            cin>>destino;
            eliminarEnlace(origen,destino);
            break;
        default:
            cout<<"ingreso no valido"<<endl;
            break;
        }
        recalcularTodo();
    }

}
