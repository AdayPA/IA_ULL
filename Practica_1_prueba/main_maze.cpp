#include <iostream>
#include "maze_t.hpp"

using namespace std;



int main(int argc, char** argv){
  
  maze_t M;
  cin >> M;
  //M.add_manualobstacle(1,1);
  M.add_autoobstacle(0.5);

  M.write();
  
    
  return 1;
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
