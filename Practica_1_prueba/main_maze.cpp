#include <iostream>
#include "maze_t.hpp"

using namespace std;

int main()
{
  maze_t M;
  
  cin >> M;
  
  cout << M << endl;
  
  if (M.solve()) cout << "¡¡ Se ha encontrado una salida al laberinto !!" << endl;
  else           cout << "No se ha podido encontrar la salida del laberinto..." << endl;

cout << M << endl;
M.camino();
  return 1;
}