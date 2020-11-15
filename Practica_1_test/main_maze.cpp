#include "maze_t.hpp"


using namespace std;


int main()
{ 
  string file;
  std::vector<int> resultado1;
  std::vector<int> resultado2;
  bool fin = true;
  bool trigger = false;
  int option = 0;
  maze_t M, C, R, H;
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
        float percent = 0;
        cout << "\033[2J\033[1;1H";
        cout << "Seleccione el porcentaje de obstáculos (0-1): " << endl;
        trigger = true;
        cin >> percent;
        R.ReadFile(file);
        H.ReadFile(file);
        R.AddAutoObstacle(percent, H);
        cout << R << endl;
        getchar();
        break; 
      }  
      case 3: {
        cout << "\033[2J\033[1;1H";
        cout << "Resolviendo con Manhattan " << endl;
        if (!trigger){
          M.ReadFile(file);
          start_m = std::chrono::system_clock::now(); 
          resultado1 = M.A_Start_Manhattan();
          end_m = std::chrono::system_clock::now();        
          cout << M << endl; 
          getchar();
          break;
        } else {
          start_m = std::chrono::system_clock::now(); 
          resultado1 = R.A_Start_Manhattan();
          end_m = std::chrono::system_clock::now();        
          cout << R << endl; 
          getchar();
          break;
        }
      }
      case 4: {
        cout << "\033[2J\033[1;1H";
        cout << "Resolviendo con Euclides " << endl;
        if (!trigger) {
          C.ReadFile(file);
          start_e = std::chrono::system_clock::now();
          resultado2 = C.A_Start_Euclides();
          end_e = std::chrono::system_clock::now(); 
          cout << C << endl;
          getchar();
          break;
        } else {
          start_e = std::chrono::system_clock::now();
          resultado2 = H.A_Start_Euclides();
          end_e = std::chrono::system_clock::now(); 
          cout << H << endl;
          getchar();
          break;
        }
      }
      case 5: {
        std::chrono::duration<double> elapsed_seconds_m = end_m - start_m;
        std::chrono::duration<double> elapsed_seconds_e = end_e - start_e;
        cout << "\033[2J\033[1;1H";
        cout << " ------------------------------------------------------" << endl;
        cout << "                                                      " << endl;
        cout << "            Tiempo (s)"<< "\tPasos"<<"\tNodos"<<"\t¿Encontró final?"<< endl;
        cout << " Manhatan   "<< elapsed_seconds_m.count()<<"\t"<<resultado1[0]<< "\t" <<resultado1[2];
        if (resultado1[0]== 0) {
          cout << "\tNo"<<endl;
        } else {
          cout << "\tSi"<<endl;
        }
        cout << " Euclides   "<< elapsed_seconds_e.count()<<"\t"<<resultado2[0]<< "\t" <<resultado2[2];
        if (resultado2[0]== 0) {
          cout << "\tNo"<<endl;
        } else {
          cout << "\tSi"<<endl;
        }
        cout << "                                             " << endl;
        cout << " ------------------------------------------------------" << endl;
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
