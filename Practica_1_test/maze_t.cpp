#include "maze_t.hpp"

std::vector<std::string> maze_t::Split (std::string str, std::string delim) {
  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do {
    pos = str.find(delim, prev);
    if (pos == std::string::npos) pos = str.length();
    std::string token = str.substr(prev, pos-prev);
    if (!token.empty()) tokens.push_back(token);
    prev = pos + delim.length();
  }
  while (pos < str.length() && prev < str.length());
  return tokens;
}

void maze_t::ReadFile(string filename) {
  ifstream file;
  string data;
  int n, m, valueformatrix;
  unsigned position = 0; 
  int numberoflines = 0; // Hace la función de iterador i, cada vez que ocurre un getline(), se aumenta
  bool readyformatrix = false;
  bool valuedeployed = false;
  file.open(filename);
  std::vector<std::string> datos;
  while (getline(file, data)) {
    if (readyformatrix == false) {
      datos = Split(data, " ");
      m = std::stoi(datos[0]); 
      n = std::stoi(datos[1]);
      matrix_.resize(m, n);
      visited_.resize(m,n);
    }
    if (readyformatrix ==  true) {
      numberoflines++;
      position = 0;
      for (int j = 1; j <= n; j++) {
        valuedeployed = false;
        for (unsigned k = 0; k < data.size(); k++) {
          if (valuedeployed == false) {
            valueformatrix = data[position] - '0';
            valuedeployed = true;
            if (position < data.size()) {
              // Los valores van a estar de 2 en 2, por eso este salto
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
  assert (i_start_ != -1 && j_start_ != -1 && i_end_ != -1 && j_end_ != -1);
  file.close();
}


istream& maze_t::read(istream& is)
{
  int m, n;
  is >> m >> n;
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
        case TRASH_ID: os << TRASH_CHR; break;
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


istream& operator>>(istream& is, maze_t& M)
{ return M.read(is);
}
  
ostream& operator<<(ostream& os, const maze_t& M)
{ return M.write(os);
}

int maze_t::Manhattan(int i, int j) {
  return abs(i - i_end_) + abs(j - j_end_);
}

int maze_t::Euclidean(int i, int j) {
  return sqrt(pow((i - i_end_), 2) + pow((j - j_end_), 2));
}


std::vector<int> maze_t::A_Start_Manhattan(void) {
  std::vector<Node*> tree;
  std::vector<std::vector<Node*>> master;
  int minN = 10000;
  int minS = 10000;
  int minE = 10000;
  int minO = 10000;
  int step = 0;
  int step_path = 0;
  visited_(i_start_, j_start_) = true;
  // Norte
  if (isOK(i_start_-1,j_start_)) { 
    visited_(i_start_-1, j_start_) = true;
    minN = Manhattan(i_start_-1,j_start_);
    Node* a = new Node {minN + step,i_start_-1,j_start_};
    tree.push_back(a);
    master.push_back(tree);
  }
  // Sur
  if (isOK(i_start_+1,j_start_)) { 
    visited_(i_start_+1,j_start_) = true;
    minS = Manhattan(i_start_+1,j_start_);
    Node* a = new Node {minS + step,i_start_+1,j_start_};
    tree.push_back(a);
    master.push_back(tree);
  }
  // Oeste
  if (isOK(i_start_,j_start_-1)) { 
    visited_(i_start_,j_start_-1) = true;
    minO = Manhattan(i_start_,j_start_-1);
    Node* a = new Node {minO + step,i_start_,j_start_-1};
    tree.push_back(a);
    master.push_back(tree);
  }
  // Este
  if (isOK(i_start_,j_start_+1)) { 
    visited_(i_start_,j_start_+1) = true;
    minE = Manhattan(i_start_,j_start_+1);
    Node* a = new Node {minE + step,i_start_,j_start_+1};
    tree.push_back(a);
    master.push_back(tree);
  }

  bool final = false;
  int salida_final = -1;
  while (!final) {
    // Contamos cada vez que nos movemos
    ++step;
    // COMPRUEBO SI HE LLEGADO AL FINAL PARA ACABAR EL WHILE
    for (unsigned i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          final = true;
          salida_final = i;
        }
    }
    int temp_min = 10000;
    int borrar_rama = -1;
    for (unsigned i = 0; i < master.size(); ++i) {
      for (unsigned j = 0; j < master.at(i).size(); ++j) {
        if (master.at(i).at(j)->value_ < temp_min ) {
          temp_min = master.at(i).at(j)->value_;
          borrar_rama = i;
        }
      }
    }
    // Norte
    if (isOK(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j)) { 
      visited_(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j) = true;
      minN = Manhattan(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j);
      Node* a = new Node {minN + step,master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i-1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
    }
    // Sur
    if (isOK(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j)) { 
      visited_(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j) = true;
      minS = Manhattan(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j);
      Node* a = new Node {minS + step,master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i+1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
    }
    // Oeste
    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1)) { // Oeste
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1) = true;
      minO = Manhattan(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1);
      Node* a = new Node {minO + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j-1)) {
        final = true;
        break;
      }
    }
    // Eeste
    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1)) { 
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1) = true;
      minE = Manhattan(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1);
      Node* a = new Node {minE + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j+1)) {
        final = true;
        break;
      }
    }
    master.erase(master.begin() + borrar_rama);
  }
  for (unsigned i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          salida_final = i;
        }
  }
  for (unsigned k = 0; k < master.at(salida_final).size(); ++k)  {
    matrix_(master.at(salida_final).at(k)->i,master.at(salida_final).at(k)->j) = PATH_ID;
    step_path++;
  }
  std::vector<int> salida;
  salida.push_back(step_path);
  salida.push_back(step);
  return salida;
}

std::vector<int> maze_t::A_Start_Euclides(void) {
  std::vector<Node*> tree;
  std::vector<std::vector<Node*>> master;
  int minN = 10000;
  int minS = 10000;
  int minE = 10000;
  int minO = 10000;
  int step = 0;
  int step_path = 0;
  visited_(i_start_, j_start_) = true;
  // Norte
  if (isOK(i_start_-1,j_start_)) { 
    visited_(i_start_-1, j_start_) = true;
    minN = Euclidean(i_start_-1,j_start_);
    Node* a = new Node {minN + step,i_start_-1,j_start_};
    tree.push_back(a);
    master.push_back(tree);
  }
  // Sur
  if (isOK(i_start_+1,j_start_)) { 
    visited_(i_start_+1,j_start_) = true;
    minS = Euclidean(i_start_+1,j_start_);
    Node* a = new Node {minS + step,i_start_+1,j_start_};
    tree.push_back(a);
    master.push_back(tree);
  }
  // Oeste
  if (isOK(i_start_,j_start_-1)) { 
    visited_(i_start_,j_start_-1) = true;
    minO = Euclidean(i_start_,j_start_-1);
    Node* a = new Node {minO + step,i_start_,j_start_-1};
    tree.push_back(a);
    master.push_back(tree);
  }
  // Este
  if (isOK(i_start_,j_start_+1)) { 
    visited_(i_start_,j_start_+1) = true;
    minE = Euclidean(i_start_,j_start_+1);
    Node* a = new Node {minE + step,i_start_,j_start_+1};
    tree.push_back(a);
    master.push_back(tree);
  }
  
  bool final = false;
  int salida_final = -1;

  while (!final) {
    ++step;
    // COMPRUEBO SI HE LLEGADO AL FINAL PARA ACABAR EL WHILE
    for (unsigned i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          final = true;
          salida_final = i;
        }
    }
    int temp_min = 10000;
    int borrar_rama = -1;
    for (unsigned i = 0; i < master.size(); ++i) {
      for (unsigned j = 0; j < master.at(i).size(); ++j) {
        if (master.at(i).at(j)->value_ < temp_min ) {
          temp_min = master.at(i).at(j)->value_;
          borrar_rama = i;
        }
      }
    }
    // Norte
    if (isOK(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j)) { 
      visited_(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j) = true;
      minN = Euclidean(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j);
      Node* a = new Node {minN + step,master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i-1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
    }
    // Sur
    if (isOK(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j)) { 
      visited_(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j) = true;
      minS = Euclidean(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j);
      Node* a = new Node {minS + step,master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i+1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
    }
    // Oeste
    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1)) { 
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1) = true;
      minO = Euclidean(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1);
      Node* a = new Node {minO + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j-1)) {
        final = true;
        break;
      }
    }
    // Este
    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1)) { 
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1) = true;
      minE = Euclidean(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1);
      Node* a = new Node {minE + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1}; 
      std::vector<Node*> tree_temp;
      for (unsigned i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j+1)) {
        final = true;
        break;
      }
    }
    master.erase(master.begin() + borrar_rama);
  }
  for (unsigned i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          salida_final = i;
        }
  }
  for (unsigned k = 0; k < master.at(salida_final).size(); ++k)  {
    matrix_(master.at(salida_final).at(k)->i,master.at(salida_final).at(k)->j) = PATH_ID;
    step_path++;
  }
  std::vector<int> salida;
  salida.push_back(step_path);
  salida.push_back(step);
  return salida;
}