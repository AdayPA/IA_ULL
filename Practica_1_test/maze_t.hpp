#pragma once 

#include <iostream>
#include "matrix_t.hpp"
#include <forward_list>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>


#define PASS_ID  0
#define WALL_ID  1
#define PATH_ID  2

#define START_ID 8
#define END_ID   9

#define TRASH_ID 3
#define TRASH_CHR "X"

#define PATH_SOL_ID -1
#define PATH_SOL_CHR "▓"

#define WALL_CHR  "█"
#define PASS_CHR  " "
#define PATH_CHR  "·"
#define START_CHR "A"
#define END_CHR   "B"

typedef matrix_t<short> matrix_t_int;
typedef matrix_t<bool> matrix_t_bool;

// enumera las direcciones Norte, Este, Sur y Oeste
enum direction_t {N, E, S, W};

// define vectores de desplazamiento en las 4 direcciones
//                    N   E  S   W
const short i_d[] = { -1, 0, 1,  0};
const short j_d[] = {  0, 1, 0, -1};

using namespace std;

class maze_t 
{
private:
  matrix_t_int matrix_;
  matrix_t_bool visited_;
  int i_start_, j_start_, i_end_, j_end_;
  int steps;
  int counter;
  struct Node {
    int value_;
    int i;
    int j;
  };
  struct Node_f{
    int i;
    int j;
  };
public:
  maze_t(): matrix_(), visited_(),
            i_start_(-1), j_start_(-1), 
            i_end_(-1), j_end_(-1),
            steps(0), counter(0)
  { }
    
  ~maze_t() {}

  void ReadFile(string);
  std::vector<std::string> Split (std::string , std::string);
  int Manhattan(int i, int j);
  int Euclidean(int i, int j);
  std::vector<int> A_Start_Manhattan(void);
  std::vector<int> A_Start_Euclides(void);
  
  istream& read(istream& = cin);
  ostream& write(ostream& = cout) const;

private:
  bool isOK(int, int);
};

istream& operator>>(istream&, maze_t&);
ostream& operator<<(ostream&, const maze_t&);
