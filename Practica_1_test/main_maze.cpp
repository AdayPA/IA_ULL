#include <iostream>
#include "maze_t.hpp"
#include <ctime>
#include <chrono>

using namespace std;


int main()
{ 
  string file;
  std::vector<int> resultado1;
  std::vector<int> resultado2;
  bool fin = true;
  int option = 0;
  maze_t M, C;
  std::chrono::time_point<std::chrono::system_clock> start_m, end_m, start_e, end_e;

 while (fin){
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
        cout << "\033[2J\033[1;1H";
        cout << "Ingrese el nombre del fichero: ";
        cin >> file;
        M.ReadFile(file);
        cout << M << endl;
        cout << "Cargado;" << endl;
        getchar();
        break;
      }  
      case 2: {
        cout << "\033[2J\033[1;1H";
        cout << "Se han añadido obstaculos aleatorios" << endl;
        cout << M << endl;
        getchar();
        break; 
      }  
      case 3: {
        cout << "\033[2J\033[1;1H";
        cout << "Resolviendo con Manhattan " << endl;
        M.ReadFile(file);
        start_m = std::chrono::system_clock::now(); 
        resultado1 = M.A_Start_Manhattan();
        end_m = std::chrono::system_clock::now();        
        cout << M << endl; 
        getchar();
        break;
      }
      case 4: {
        cout << "\033[2J\033[1;1H";
        cout << "Resolviendo con Euclides " << endl;
        C.ReadFile(file);
        start_e = std::chrono::system_clock::now();
        resultado2 = C.A_Start_Euclides();
        end_e = std::chrono::system_clock::now(); 
        cout << C << endl;
        getchar();
        break;
        }
      case 5: {
        cout << "\033[2J\033[1;1H";
        cout << "Tiempos: " << endl;
        cout << "Tiempo para Manhattan: " << endl;
        std::chrono::duration<double> elapsed_seconds_m = end_m - start_m;
        cout << elapsed_seconds_m.count()  << " segundos." << endl;
        cout << "Tiempo para Euclides:  " << endl;
        std::chrono::duration<double> elapsed_seconds_e = end_e - start_e;
        cout << elapsed_seconds_e.count() << " segundos" << endl;
        //cout << "Hay una diferencia de: " << abs (duration1 - duration) << endl;
        getchar();
        break;
      }
      case 6: {
        cout << "Saliendo" << endl;
        fin = false;
        break;
      }
    }
    

  }
  return 0;
}
