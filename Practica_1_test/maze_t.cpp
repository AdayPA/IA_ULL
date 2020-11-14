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
  int position = 0; 
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
  //cout << "Valores: " << endl;
  //cout << i_start_ << j_start_ << i_end_ << j_end_ << endl;
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


/*
void maze_t::camino(void){
 if(list_.empty()){
    cout<<"Enjoy the darkness because there is no escape";
    cout<<endl;
    cout<<counter<<endl;
 }else{
    cout<<"This is the PATH for your freedom ("<<steps<<" pasos) :" <<endl;

       sll_node_t<char>* aux=list_.head();
    while(aux){
       cout<<aux->get_data()<<"->";
       aux=aux->get_next();
       }
    if(!aux) cout<<" Freedom cannot be bestowed, it must be achieved. (FRANKLIN D. ROOSEVELT)"<<endl;
    cout<<endl;
    cout<<counter<<endl;
 }
}
*/
void maze_t::A_Start(void) {
  visited_(i_start_, j_start_) = true;
  std::vector<Node*> tree;
  std::vector<Node_f*> final;
  int minN = 10000;
  int minS = 10000;
  int minE = 10000;
  int minO = 10000;
  int stage = 0;
  if (isOK(i_start_-1,j_start_)) { // norte  1
    minN = Manhattan(i_start_-1,j_start_);
    cout << "Puedo ir norte, calculo: " << minN << endl;
    Node* a = new Node {minN,i_start_-1,j_start_,1,0};
    Node_f* b = new Node_f {i_start_-1,j_start_};
    final.push_back(b);
    tree.push_back(a);
  }
  if (isOK(i_start_+1,j_start_)) { // sur  2
    minS = Manhattan(i_start_+1,j_start_);
    cout << "Puedo ir sur, calculo: " << minS << endl;
    Node* a = new Node {minS,i_start_+1,j_start_,2,0};
    Node_f* b = new Node_f {i_start_+1,j_start_};
    final.push_back(b);
    tree.push_back(a);
  }
  if (isOK(i_start_,j_start_-1)) { // oeste  3
    minO = Manhattan(i_start_,j_start_-1);
    cout << "Puedo ir oeste, calculo: " << minO << endl;
    Node* a = new Node {minO,i_start_,j_start_-1,3,0};
     Node_f* b = new Node_f {i_start_,j_start_-1};
    final.push_back(b);
    tree.push_back(a);
  }
  if (isOK(i_start_,j_start_+1)) { // este  4
    minE = Manhattan(i_start_,j_start_+1);
    cout << "Puedo ir este, calculo: " << minE << endl;
    Node* a = new Node {minE,i_start_,j_start_+1,4,0};
    Node_f* b = new Node_f {i_start_,j_start_+1};
    final.push_back(b);
    tree.push_back(a);
  }
  int actual_i = i_start_;
  int actual_j = j_start_;
  bool fin = false;

  while (!tree.empty() && !fin) {
    cout << "tamaño tree: "<<tree.size();
    int temp = 50000;
    int min_pos = -1;
    for (int i = 0; i < tree.size(); ++i) {
      if ( tree.at(i)->value_ < temp ) {
        temp = tree.at(i)->value_;
        min_pos = i;
      }
    }
    Node* min_value = tree.at(min_pos);
    if ((min_value->i == i_end_) && (min_value->j == j_end_)) {
      cout << "encontre salida";
      fin = true;
    }
    cout << "saco";
    //matrix_(min_value->i,min_value->j) = PASS_ID;
    //matrix_(4,5) = PASS_ID;
    //matrix_(5,5) = PASS_ID;
    tree.erase(tree.begin()+min_pos);
    visited_(min_value->i,min_value->j) = true;
      actual_i = min_value->i;
      actual_j = min_value->j;
       if (isOK(min_value->i-1,min_value->j)) { // norte  1
        minN = Manhattan(min_value->i-1,min_value->j);
        cout << "Puedo ir norte, meto: " << minN << endl;
        Node* a = new Node {minN,min_value->i-1,min_value->j,1,0};
        Node_f* b = new Node_f {min_value->i-1, min_value->j}; 
        matrix_(min_value->i-1,min_value->j)=PATH_SOL_ID;
        tree.push_back(a);
        final.push_back(b);
      }
      if (isOK(min_value->i+1,min_value->j)) { // sur  2
        minS = Manhattan(min_value->i+1,min_value->j);
        cout << "Puedo ir sur, meto: " << minS << endl;
        Node* a = new Node {minS,min_value->i+1,min_value->j,2,0};
        Node_f* b = new Node_f {min_value->i+1, min_value->j};
        matrix_(min_value->i+1,min_value->j)=PATH_SOL_ID;
        tree.push_back(a);
        final.push_back(b);
      }
      if (isOK(min_value->i,min_value->j-1)) { // oeste  3
        minO = Manhattan(min_value->i,min_value->j-1);
        cout << "Puedo ir oeste, meto: " << minO << endl;
        Node* a = new Node {minO,min_value->i,min_value->j-1,3,0};
        Node_f* b = new Node_f {min_value->i, min_value->j-1};
        matrix_(min_value->i,min_value->j-1)=PATH_SOL_ID;
        tree.push_back(a);
        final.push_back(b);
      }
      if (isOK(min_value->i,min_value->j+1)) { // este  4
        minE = Manhattan(min_value->i,min_value->j+1);
        cout << "Puedo ir este, meto: " << minE << endl;
        Node* a = new Node {minE,min_value->i,min_value->j+1,4,0};
        Node_f* b = new Node_f {min_value->i,min_value->j+1};
        matrix_(min_value->i,min_value->j+1)=PATH_SOL_ID;
        tree.push_back(a);
        final.push_back(b);
      }
  }/*
    cout << endl;
   for (int i = 0; i < final.size(); ++i) {
     matrix_(final.at(i)->i,final.at(i)->j)=PATH_SOL_ID;
    cout << final.at(i)->i << " , "<< final.at(i)->j << endl;
  }*/
/*
  for (int i = 0; i < final.size(); ++i) {
    int temp_i = final.at(i)->i; 
    int temp_j = final.at(i)->j;
    if (i == final.size() -1 ) {
      break;
    }
    if ((temp_i == final.at(i+1)->i) || (temp_j == final.at(i+1)->j)) {
    } else {
      final.erase(final.begin() + (i + 1));
      i = i - 1 ;
    }
  }

  cout << endl;
  for (int i = 0; i < final.size(); ++i) {
    cout << final.at(i)->i << " , "<< final.at(i)->j << endl;
  }*/
}


std::vector<int> maze_t::A_Start2(void) {
  std::vector<Node*> tree;
  std::vector<std::vector<Node*>> master;
  int minN = 10000;
  int minS = 10000;
  int minE = 10000;
  int minO = 10000;
  int step = 0;
  int step_path = 0;
  visited_(i_start_, j_start_) = true;
  if (isOK(i_start_-1,j_start_)) { // norte  1
    visited_(i_start_-1, j_start_) = true;
    minN = Manhattan(i_start_-1,j_start_);
    //cout << "Puedo ir norte, calculo: " << minN << endl;
    Node* a = new Node {minN + step,i_start_-1,j_start_,1,0};
    tree.push_back(a);
    master.push_back(tree);
  }
  if (isOK(i_start_+1,j_start_)) { // sur  2
    visited_(i_start_+1,j_start_) = true;
    minS = Manhattan(i_start_+1,j_start_);
    //cout << "Puedo ir sur, calculo: " << minS << endl;
    Node* a = new Node {minS + step,i_start_+1,j_start_,2,0};
    tree.push_back(a);
    master.push_back(tree);
  }
  if (isOK(i_start_,j_start_-1)) { // oeste  3
    visited_(i_start_,j_start_-1) = true;
    minO = Manhattan(i_start_,j_start_-1);
   // cout << "Puedo ir oeste, calculo: " << minO << endl;
    Node* a = new Node {minO + step,i_start_,j_start_-1,3,0};
    tree.push_back(a);
    master.push_back(tree);
  }
  if (isOK(i_start_,j_start_+1)) { // este  4
    visited_(i_start_,j_start_+1) = true;
    minE = Manhattan(i_start_,j_start_+1);
    //cout << "Puedo ir este, calculo: " << minE << endl;
    Node* a = new Node {minE + step,i_start_,j_start_+1,4,0};
    tree.push_back(a);
    master.push_back(tree);
  }

  bool final = false;
  int salida_final = -1;
/*
  tree:  2,2
         2,3

  master:       2,2
                2,3
*/
  //cout << "Master mide: " << endl;
 // cout << master.size() << endl;
  while (!final) {
    ++step;
    /*
    *  COMPRUEBO SI HE LLEGADO AL FINAL PARA ACABAR EL WHILE
    */
    /*for (int i = 0; i < master.size(); ++i) {
      for (int j = 0; j < master.at(i).size(); ++j) {
        if ((master.at(i).at(j)->i == i_end_) && (master.at(i).at(j)->j == j_end_)) {
          final = true;
          salida_final = i;
        }
      }
    }
*/
    for (int i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          final = true;
          salida_final = i;
        }
    }


    int temp_min = 10000;
    int borrar_rama = -1;
    for (int i = 0; i < master.size(); ++i) {
      for (int j = 0; j < master.at(i).size(); ++j) {
        if (master.at(i).at(j)->value_ < temp_min ) {
          temp_min = master.at(i).at(j)->value_;
          borrar_rama = i;
        }
      }
    }

    
    if (isOK(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j)) { // norte  1
      visited_(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j) = true;
      minN = Manhattan(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j);
      //cout << "Puedo ir norte, calculo: " << minN << endl;
      Node* a = new Node {minN + step,master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j,1,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,1,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i-1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
      
      //master.erase(master.begin() + borrar_rama);
    }

    if (isOK(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j)) { // sur 2
      visited_(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j) = true;
      minS = Manhattan(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j);
      //cout << "Puedo ir sur, calculo: " << minS << endl;
      Node* a = new Node {minS + step,master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j,2,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,2,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i+1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
     // master.erase(master.begin() + borrar_rama);
    }

    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1)) { // Oeste
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1) = true;
      minO = Manhattan(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1);
      //cout << "Puedo ir oeste, calculo: " << minO << endl;
      Node* a = new Node {minO + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1,3,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,3,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j-1)) {
        final = true;
        break;
      }
     // master.erase(master.begin() + borrar_rama);
    }

    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1)) { // este 2
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1) = true;
      minE = Manhattan(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1);
      //cout << "Puedo ir este, calculo: " << minE << endl;
      Node* a = new Node {minE + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1,4,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,4,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j+1)) {
        final = true;
        break;
      }
     // master.erase(master.begin() + borrar_rama);
    }
    
    master.erase(master.begin() + borrar_rama);
  }
  

  for (int i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          salida_final = i;
        }
  }

  
  //cout << "salida: " << salida_final <<endl;
  for (int k = 0; k < master.at(salida_final).size(); ++k)  {
   // cout << "voy a:" << master.at(salida_final).at(k)->i <<  master.at(salida_final).at(k)->j << endl;
    matrix_(master.at(salida_final).at(k)->i,master.at(salida_final).at(k)->j) = PATH_ID;
    step_path++;
  }

  
  //cout << "traza: " << endl;
  for (int i = 0; i < master.size(); ++i) {
  //  cout << "pos de rama " << i << ": ";
    for (int j = 0; j < master.at(i).size(); ++j) {
      //cout << master.at(i).at(j)->i << master.at(i).at(j)->j << "->" << master.at(i).at(j)->value_<<", ";
    }
    cout << endl;
  }
  std::vector<int> salida;
  salida.push_back(step_path);
  salida.push_back(step);
  return salida;


}

std::vector<int> maze_t::A_Start3(void) {
  std::vector<Node*> tree;
  std::vector<std::vector<Node*>> master;
  int minN = 10000;
  int minS = 10000;
  int minE = 10000;
  int minO = 10000;
  int step = 0;
  int step_path = 0;
  visited_(i_start_, j_start_) = true;
  if (isOK(i_start_-1,j_start_)) { // norte  1
    visited_(i_start_-1, j_start_) = true;
    minN = Euclidean(i_start_-1,j_start_);
    //cout << "Puedo ir norte, calculo: " << minN << endl;
    Node* a = new Node {minN + step,i_start_-1,j_start_,1,0};
    tree.push_back(a);
    master.push_back(tree);
  }
  if (isOK(i_start_+1,j_start_)) { // sur  2
    visited_(i_start_+1,j_start_) = true;
    minS = Euclidean(i_start_+1,j_start_);
    //cout << "Puedo ir sur, calculo: " << minS << endl;
    Node* a = new Node {minS + step,i_start_+1,j_start_,2,0};
    tree.push_back(a);
    master.push_back(tree);
  }
  if (isOK(i_start_,j_start_-1)) { // oeste  3
    visited_(i_start_,j_start_-1) = true;
    minO = Euclidean(i_start_,j_start_-1);
    //cout << "Puedo ir oeste, calculo: " << minO << endl;
    Node* a = new Node {minO + step,i_start_,j_start_-1,3,0};
    tree.push_back(a);
    master.push_back(tree);
  }
  if (isOK(i_start_,j_start_+1)) { // este  4
    visited_(i_start_,j_start_+1) = true;
    minE = Euclidean(i_start_,j_start_+1);
    //cout << "Puedo ir este, calculo: " << minE << endl;
    Node* a = new Node {minE + step,i_start_,j_start_+1,4,0};
    tree.push_back(a);
    master.push_back(tree);
  }
  
  bool final = false;
  int salida_final = -1;
/*
  tree:  2,2
         2,3

  master:       2,2
                2,3
*/

  while (!final) {
    ++step;
    /*
    *  COMPRUEBO SI HE LLEGADO AL FINAL PARA ACABAR EL WHILE
    */
    /*for (int i = 0; i < master.size(); ++i) {
      for (int j = 0; j < master.at(i).size(); ++j) {
        if ((master.at(i).at(j)->i == i_end_) && (master.at(i).at(j)->j == j_end_)) {
          final = true;
          salida_final = i;
        }
      }
    }
*/
    for (int i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          final = true;
          salida_final = i;
        }
    }
    

    int temp_min = 10000;
    int borrar_rama = -1;
    for (int i = 0; i < master.size(); ++i) {
      //cout << "Hasta aquí funciono" << endl;
      for (int j = 0; j < master.at(i).size(); ++j) {
        //cout << "Hasta aquí funciono 2" << endl;
        if (master.at(i).at(j)->value_ < temp_min ) {
          temp_min = master.at(i).at(j)->value_;
          borrar_rama = i;
        }
      }
    }
  
    
    if (isOK(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j)) { // norte  1
      visited_(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j) = true;
      minN = Euclidean(master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j);
      //cout << "Puedo ir norte, calculo: " << minN << endl;
      Node* a = new Node {minN + step,master.at(borrar_rama).back()->i-1,master.at(borrar_rama).back()->j,1,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,1,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i-1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
      
      //master.erase(master.begin() + borrar_rama);
    }

    if (isOK(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j)) { // sur 2
      visited_(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j) = true;
      minS = Euclidean(master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j);
      //cout << "Puedo ir sur, calculo: " << minS << endl;
      Node* a = new Node {minS + step,master.at(borrar_rama).back()->i+1,master.at(borrar_rama).back()->j,2,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,2,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i+1 )&&(j_end_ == master.at(borrar_rama).back()->j)) {
        final = true;
        break;
      }
     // master.erase(master.begin() + borrar_rama);
    }

    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1)) { // Oeste
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1) = true;
      minO = Euclidean(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1);
      //cout << "Puedo ir oeste, calculo: " << minO << endl;
      Node* a = new Node {minO + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j-1,3,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,3,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j-1)) {
        final = true;
        break;
      }
     // master.erase(master.begin() + borrar_rama);
    }

    if (isOK(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1)) { // este 2
      visited_(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1) = true;
      minE = Euclidean(master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1);
      //cout << "Puedo ir este, calculo: " << minE << endl;
      Node* a = new Node {minE + step,master.at(borrar_rama).back()->i,master.at(borrar_rama).back()->j+1,4,0}; 
      std::vector<Node*> tree_temp;
      for (int i = 0; i < master.at(borrar_rama).size(); ++i) {
        Node* b = new Node {master.at(borrar_rama).at(i)->value_,
                            master.at(borrar_rama).at(i)->i,       
                            master.at(borrar_rama).at(i)->j,4,0};
        tree_temp.push_back(b);
      }
      tree_temp.push_back(a);
      master.push_back(tree_temp);
      if ((i_end_ == master.at(borrar_rama).back()->i )&&(j_end_ == master.at(borrar_rama).back()->j+1)) {
        final = true;
        break;
      }
     // master.erase(master.begin() + borrar_rama);
    }
    
    master.erase(master.begin() + borrar_rama);
  }
  

  for (int i = 0; i < master.size(); ++i) {
        if ((master.at(i).back()->i == i_end_) && (master.at(i).back()->j == j_end_)) {
          salida_final = i;
        }
  }

  
  //cout << "salida: " << salida_final <<endl;
  for (int k = 0; k < master.at(salida_final).size(); ++k)  {
    //cout << "voy a:" << master.at(salida_final).at(k)->i <<  master.at(salida_final).at(k)->j << endl;
    matrix_(master.at(salida_final).at(k)->i,master.at(salida_final).at(k)->j) = PATH_ID;
    step_path++;
  }

  
  //cout << "traza: " << endl;
  for (int i = 0; i < master.size(); ++i) {
   // cout << "pos de rama " << i << ": ";
    for (int j = 0; j < master.at(i).size(); ++j) {
      //cout << master.at(i).at(j)->i << master.at(i).at(j)->j << "->" << master.at(i).at(j)->value_<<", ";
    }
    cout << endl;
  }
  std::vector<int> salida;
  salida.push_back(step_path);
  salida.push_back(step);
  return salida;

}