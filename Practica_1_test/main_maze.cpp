#include <iostream>
#include "maze_t.hpp"
#include <ctime>
#include <chrono>

using namespace std;


int main()
{ /*
  maze_t M;
  
  cin >> M;
  
  cout << M << endl;
  
 /* if (M.solve()) cout << "¡¡ Se ha encontrado una salida al laberinto !!" << endl;
  else           cout << "No se ha podido encontrar la salida del laberinto..." << endl;

//M.A_Start();
M.A_Start2();
cout << M << endl;
//M.camino();
  return 1;*/

//////////////////////////////////////////////////////////////////////////////////////////////
  string nombre;
  std::vector<int> resultado1;
  std::vector<int> resultado2;
  bool boolean = true;
  int option = 0;
  maze_t M, C;
  std::chrono::time_point<std::chrono::system_clock> start, end;
  
  //cin >> M;
  //M.ReadFile();
 while (boolean){
    cout << "Menú: " << endl;
    cout << "\t1. Introducir fichero" << endl;
    cout << "\t2. Añadir obstáculos aleatorios " << endl;
    cout << "\t3. Resolver A* con Manhattan" << endl;
    cout << "\t4. Resolver A* con Euclides" << endl;
    cout << "\t5. Tabla de resultados " << endl;
    cout << "\t6. Salir del programa" << endl;
    std::cin >> option;
    switch(option) {
      case 1: { 
        cout << "Ingrese el nombre del fichero: ";
        cin >> nombre;
        M.ReadFile(nombre);
        cout << M << endl;
        cout << "Cargado;" << endl;
        break;
      }  
      case 2: {
        cout << "Se han añadido obstaculos aleatorios" << endl;
        cout << M << endl;
        break; 
      }  
      case 3: {
        cout << "Resolviendo con Manhattan " << endl;
        M.ReadFile(nombre);
        start = std::chrono::system_clock::now(); 
        auto start1 = std::chrono::steady_clock::now();
        resultado1 = M.A_Start2();
        end = std::chrono::system_clock::now();
        auto end2 = std::chrono::steady_clock::now(); 
        std::chrono::duration<double> elapsed_seconds = end - start;         
        cout << M << endl; 
        break;
      }
      case 4: {
        cout << "Resolviendo con Euclides " << endl;
        C.ReadFile(nombre);
        //auto t2 = std::chrono::high_resolution_clock::now();
        resultado2 = C.A_Start3();
        //auto t3 = std::chrono::high_resolution_clock::now();
        //auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
        cout << C << endl;
        break;
        }
      case 5: {
        cout << "Tiempos: " << endl;
        //cout << "Tiempo para Manhattan: " << elapsed_seconds << endl;
        cout << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count())  << " milisegundos" << endl;
        //cout << "Tiempo para Euclides:  " << duration1 << endl;
        //cout << "Hay una diferencia de: " << abs (duration1 - duration) << endl;
        break;
      }
      case 6: {
        cout << "Saliendo" << endl;
        boolean = false;
        break;
      }
    }
    

  }
  return 0;
}
