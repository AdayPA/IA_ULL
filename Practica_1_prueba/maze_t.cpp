#include "maze_t.hpp"
#include "sll_node_t.hpp"
#include "sll_t.hpp"

#include <cassert>
#include <math.h>
#include <stdlib.h> 

bool maze_t::solve()
{
  return solve(i_start_, j_start_);
}

void maze_t::ReadFile(void) {
  ifstream file;
  string data;
  int n, m, valueformatrix;
  int position = 0; 
  int numberoflines = 0; // Hace la función de iterador i, cada vez que ocurre un getline(), se aumenta
  bool readyformatrix = false;
  bool valuedeployed = false;
  file.open("maze1.txt");
  while (getline(file, data)) {
    if (readyformatrix == false) {
      n = data[0] - '0';
      m = data[2] - '0';
      matrix_.resize(n, m);
    }
    if (readyformatrix ==  true) {
      numberoflines++;
      position = 0;
      for (int j = 1; j <= n; j++) {
        valuedeployed = false;
        for (int k = 0; k < data.size(); k++) {
          if (valuedeployed == false) {
            valueformatrix = data[position] - '0';
            valuedeployed = true;
            if (position < data.size()) {
              // Los valores van a estar de 2 en 2, por eso hago esta línea
              position += 2;
            }
          }
        }
        if (valueformatrix == START_ID)    { i_start_ = numberoflines, j_start_ = j; }
        else if (valueformatrix == END_ID) { i_end_   = numberoflines, j_end_   = j; }
        matrix_(numberoflines, j) = valueformatrix;
        }
    }
    readyformatrix =  true;
  }
  cout << "Valores: " << endl;
  //cout << i_start_ << j_start_ << i_end_ << j_end_ << endl;
  assert (i_start_ != -1 && j_start_ != -1 && i_end_ != -1 && j_end_ != -1);
  file.close();
}

istream& maze_t::read(istream& is)
{
  int m, n;
  is >> m >> n;
  //cout << "m: " << m << "n: " << n;
  assert(m != 0 && n != 0);
 
  matrix_.resize(m, n);
  visited_.resize(m, n);
  
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      short s;
      is >> s;
      if (s == START_ID)    { i_start_ = i, j_start_ = j; }
      else if (s == END_ID) { i_end_   = i, j_end_   = j; }
      matrix_(i, j) = s;
    }
  }
  assert (i_start_ != -1 && j_start_ != -1 && i_end_ != -1 && j_end_ != -1);
  return is;
}

ostream& maze_t::write(ostream& os) const
{
  os << matrix_.get_m() << "x" << matrix_.get_n() << endl;
  for (int i = 1; i <= matrix_.get_m(); i++) {
    for (int j = 1; j <= matrix_.get_n(); j++)
      switch (matrix_(i, j))
      { case START_ID: os << START_CHR; break;
        case END_ID:   os << END_CHR;   break;
        case WALL_ID:  os << WALL_CHR;  break;
        case PASS_ID:  os << PASS_CHR;  break;
        case PATH_ID:  os << PATH_CHR;  break;
        case PATH_SOL_ID:  os << PATH_SOL_CHR;  break;
      }
    os << endl;
  }
  return os; 
}

bool maze_t::isOK(int i, int j)
{ 
  return (i >= 1 && i <= matrix_.get_m()) && 
         (j >= 1 && j <= matrix_.get_n()) &&
         matrix_(i, j) != WALL_ID && 
         !visited_(i, j);
}

bool maze_t::solve(int i, int j)
{
  counter++;
  // CASO BASE:
  // retornar 'true' si 'i' y 'j' han llegado a la salida
  if(matrix_(i,j)==END_ID) return true;

  // marcamos la celda como visitada
  visited_(i, j) = true;
  
  // CASO GENERAL:
  // para cada una de las 4 posibles direcciones (N, E, S, W) ver si es posible
  // el desplazamiento (isOK), y en ese caso, intentar resolver el laberinto
  // llamando recursivamente a 'solve'. Si la llamada devuelve 'true', 
  // propagarla retornando también 'true'
  cout << "estoy en :" << i << j <<endl;

  int minN = 100000; 
  if(isOK(i-1,j)){
    minN = Manhattan(i-1, j);
  }
  int minS = 100000;
  if(isOK(i+1,j)){
    minS = Manhattan(i+1, j);
  }
  int minE = 100000;
  if(isOK(i,j+1)) {
    minE = Manhattan(i,j+1);
  }
  int minO = 100000;
  if(isOK(i,j-1)) {
    minO = Manhattan(i,j-1);
  }
  
cout << "minN: " << minN<< endl;
cout << "minS: " << minS<< endl;
cout << "minE: " << minE<< endl;
cout << "minO: " << minO<< endl;
  /*
  if ((minN < minS) && (minN < minE) && (minN < minO) ) {
    cout << "Me muevo al norte";
    matrix_(i-1,j)=PATH_SOL_ID;
    if(solve(i-1,j)) {
      return true;
    }    
  } else if ((minS < minE) && (minS < minO) && (minS < minN)) {
    cout << "Me muevo al sur";
    matrix_(i+1,j)=PATH_SOL_ID;
    if(solve(i+1,j)){
      return true; 
    } 
  } else if ((minE < minO) && (minE < minN) && (minE < minS)) {
    cout << "Me muevo al este";
    matrix_(i,j+1)=PATH_SOL_ID;
    if(solve(i,j+1)){
      return true; 
    }
  } else {
    cout << "Me muevo al oeste";
      matrix_(i,j-1)=PATH_SOL_ID;
     if(solve(i,j-1)){
      return true;  
    }
  }
  */
  if ((minS < minN) && (minS < minO) && (minS < minE)) {
    cout << "Me muevo al sur";
    matrix_(i+1,j) = PATH_SOL_ID;
    if(solve(i+1,j)) {
      return true; 
    }
  }
  if ((minE < minN) && (minE < minO) && (minE < minS)) {
    cout << "Me muevo al este";
    matrix_(i,j+1) = PATH_SOL_ID;
    if(solve(i,j+1)) {
      return true; 
    }
  }
/*


  if(isOK(i-1,j)){ //norte
    // calculo distancias --> distancia menor --> asigno PATH_SOL_ID
    if(solve(i-1,j)) {
      matrix_(i-1,j)=PATH_SOL_ID;
      steps++; 
      return true;
    } 
  }

  if(isOK(i+1,j)){ //sur

      if(solve(i+1,j)){
      list_.insert_head(new sll_node_t<char>('S')); 
      matrix_(i+1,j)=PATH_SOL_ID;
      steps++; 
        return true; 
      } 
   }

  if(isOK(i,j-1)){ //oeste
      if(solve(i,j-1)){
      list_.insert_head(new sll_node_t<char>('W')); 
      matrix_(i,j-1)=PATH_SOL_ID;
      steps++; 
        return true;  
      } 
   }

  if(isOK(i,j+1)){ //este
      if(solve(i,j+1)){
      list_.insert_head(new sll_node_t<char>('E')); 
      matrix_(i,j+1)=PATH_SOL_ID;
      steps++; 
        return true; 
      }
     }

*/
  // desmarcamos la celda como visitada (denominado "backtracking") y
  // retornamos 'false'
  visited_(i, j) = false;
  return false;
}

istream& operator>>(istream& is, maze_t& M)
{ return M.read(is);
}
  
ostream& operator<<(ostream& os, const maze_t& M)
{ return M.write(os);
}


void maze_t::camino(void){}

void maze_t::add_manualobstacle(int i, int j){
  matrix_(i, j) = WALL_ID;
}

void maze_t::add_autoobstacle(float percen){
  int space = (matrix_.get_m() * matrix_.get_n()) - 2;
  int temp = floor(space * percen);
  for (int i = 0; i < temp; ++i){
    auto v1 = rand() % matrix_.get_m() + 1;
    auto v2 = rand() % matrix_.get_n() + 1;
    if ((matrix_(v1,v2) != START_ID) || (matrix_(v1,v2) != END_ID)){
      matrix_(v1, v2) = WALL_ID;
    }
  }
<<<<<<< HEAD
}
=======

}

int maze_t::Manhattan(int i, int j) {
  return abs(i - i_end_) + abs(j - j_end_);
}

>>>>>>> 21c90217a7ec080906387814ab9597ab7721ccf5
