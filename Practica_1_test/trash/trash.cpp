// viene de maze_t.cpp

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