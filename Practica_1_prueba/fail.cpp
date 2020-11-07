#include <iostream>
#include "maze_t.hpp"

using namespace std;



int main(void){
  bool boolean = true;
  int option = 0;
  maze_t M;
  while (boolean){
    cout << "Menú: " << endl;
    cout << "\t1. Introducir fichero" << endl;
    cout << "\t2. Salir del programa" << endl;
    cin >> option;
    switch(option){
      case 1: 
        cout << "Ingrese el nombre del fichero: ";
        M.ReadFile();
        cout << M << endl;
        cout << "cargado;";
        break;
      case 2: 
        cout << "Saliendo";
        boolean = false;
        break;
    }


  }
  


/*

  1. introducir fichero
  2. imprimir matrix
  3. intrudocir obstaculo manual
  4. intrudocir obstaculo aleatorio
  5. quitar obstaculo
  6. resolver 
  7. salir

*/
  
 // cout << M << endl;
  
  return 1;
}