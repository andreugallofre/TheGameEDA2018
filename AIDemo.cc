#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */

#define PLAYER_NAME Demo


// DISCLAIMER: The following Demo player is *not* meant to do anything
// sensible. It is provided just to illustrate how to use the API.
// Please use AINull.cc as a template for your player.


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  typedef vector<char> VC;
  typedef vector<VC> MC;

  struct point {
    int x, y, xa, ya;
  };

  MC matrix, m2;

  static constexpr int I[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
  static constexpr int J[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

  typedef vector<int> VE;


  void generate_matrix(){
      for (int i = 1; i < matrix[0].size()-1; ++i){
          for (int j = 1; j < matrix.size()-1; ++j) {
              if (post_value(i-1,j-1) != -1 and post_value(i-1,j-1) != -2) matrix[i][j] = 'B';
              else if (what(i-1,j-1) == 3) matrix[i][j] = 'X';
              else if (what(i-1,j-1) == 4) matrix[i][j] = 'X';
              else if (what(i-1,j-1) == 2) matrix[i][j] = '.';
              else if (what(i-1,j-1) == 1) matrix[i][j] = '.';
          } 
      }
  }

  void generate_matrix2(MC &matrix){
    for (int i = 1; i < matrix[0].size()-1; ++i){
        for (int j = 1; j < matrix.size()-1; ++j) {
            if (post_value(i-1,j-1) != -1 and post_value(i-1,j-1) != -2){
              matrix[i][j] = 'B';
              if (post_owner(i-1,j-1) == me())  matrix[i][j] = '.';
            } 
            else if (what(i-1,j-1) == 3) matrix[i][j] = 'X';
            else if (what(i-1,j-1) == 4) matrix[i][j] = 'X';
            else if (what(i-1,j-1) == 2) matrix[i][j] = '.';
            else if (what(i-1,j-1) == 1) matrix[i][j] = '.';
            else if (fire_time(i-1,j-1) > 0) matrix[i][j] = 'X';
        } 
    }
  }

  void getPath(point l, vector<point>& v, stack<point>& cami) {
    for (int i=v.size()-1; i>=0; --i) {
        if (v[i].x == l.xa and v[i].y == l.ya) {
            l = v[i];
            cami.push(l);
        }
    }
  }

  pair<int,int> generateMove(stack<point>& cami, point p) {
    pair<int, int> res;

    if (!cami.empty()){
        point p1 = cami.top();
        cami.pop();
        if (!cami.empty()){
            point p2 = cami.top();
            res.first = p2.x-1; res.second = p2.y-1;
            return res;
    }
  }
  res.first = 0; res.second = 0;
    return res;
}


  point create_point(int x, int y, int xa, int ya) {
      point p;
      p.x = x;
      p.y = y;
      p.xa = xa;
      p.ya = ya;
      return p;
  }

  void bfs_ite(MC& G, vector<point>& v, point& b) {

    int i = 0;
    while (true) {

        point p = v[i];

            ////cerr << p.x << " " << p.y << " " << i << endl;

        if (G[p.x][p.y] == 'B') {
            cerr << "PUNT! : " << p.x << " "<< p.y << endl;
            b = p;
            v.push_back(b);
            return;
        }else if (p.x == 0 or p.y == 0) {
          return;
      }else if (G[p.x][p.y] == '.') {
        ////cerr << p.x << " " << p.y << " " << i << endl;
        if (G[p.x-1][p.y] != 'X') v.push_back(create_point(p.x-1, p.y, p.x, p.y));
        if (G[p.x+1][p.y] != 'X') v.push_back(create_point(p.x+1, p.y, p.x, p.y));
        if (G[p.x][p.y-1] != 'X') v.push_back(create_point(p.x, p.y-1, p.x, p.y));
        if (G[p.x][p.y+1] != 'X') v.push_back(create_point(p.x, p.y+1, p.x, p.y));
        
        /*if (G[p.x-1][p.y-1] != 'X') v.push_back(create_point(p.x-1, p.y-1, p.x, p.y));
        if (G[p.x+1][p.y+1] != 'X') v.push_back(create_point(p.x+1, p.y+1, p.x, p.y));
        if (G[p.x+1][p.y-1] != 'X') v.push_back(create_point(p.x+1, p.y-1, p.x, p.y));
        if (G[p.x-1][p.y+1] != 'X') v.push_back(create_point(p.x-1, p.y+1, p.x, p.y));*/
        
        G[p.x][p.y] = 'X';
    }
    ++i;
  }
  }

  void play_soldier(int id) {
    Data in = data(id);
    int i = in.pos.i;
    int j = in.pos.j;
    int player = in.player;
    
    m2 = matrix;
    generate_matrix2(m2);
    stack<point> cami;
    pair<int,int> mov;
    vector<point> v;

    point p = create_point(i+1,j+1,-1,-1);
    v.push_back(p);

    point b;
    //cerr << "Start bfs" << endl;
    bfs_ite(m2,v,b);
    //cerr << "end bfs" << endl;
    cami.push(b);
    getPath(b,v,cami);
    //cerr << "end get path" << endl;
    mov = generateMove(cami, p);

    for (int k = 0; k < 8; ++k) {
      int ii = i + I[k];
      int jj = j + J[k];
      if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX) {
        int id2 = which_soldier(ii, jj);
        // If there is an enemy next to us, we attack.
        if (id2 and data(id2).player != player) {
          command_soldier(id, ii, jj);
          return;
        }
      }
    }
      //cerr << mov.first << " " << mov.second;
    if (mov.first != 0)
      command_soldier(id, mov.first, mov.second);

  }

  void play_helicopter(int id) {

    // If we can, we throw napalm.
    /*if (data(id).napalm == 0) {
      command_helicopter(id, NAPALM);
      return;
    }*/

    // With probability 20% we turn counter clockwise,
    // otherwise we try to move forward two steps.
    int c = random(1, 5);
    command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
  }

  void throw_parachuter(int helicopter_id) {
    // We get the data of the helicopter...
    Data in = data(helicopter_id);
    // ... and try to throw a parachuter, without even examining the land.
    int ii = in.pos.i + random(-2, 2);
    int jj = in.pos.j + random(-2, 2);
    if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX)
      command_parachuter(ii, jj);
  }
  
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {

    int player = me();
    VE H = helicopters(player); // helicopters of my player
    VE S = soldiers(player); //    soldiers of my player

    int columnes = 60+2;
    int files = 60+2;


    if(round() == 0) {
      matrix = MC(files, VC(columnes,'X'));
      generate_matrix();

      /*for (int i = 0; i < matrix.size(); ++i){
                for (int j = 0; j < matrix.size(); ++j) cerr << matrix[i][j];
                     cerr << endl;
             }*/

    }
    // If in a random helicopter I have parachuters, I throw one.
    int helicopter_id = H[random(0, H.size()-1)];
    if (not data(helicopter_id).parachuters.empty()) throw_parachuter(helicopter_id);

    for (int i = 0; i < (int)H.size(); ++i) play_helicopter(H[i]);
    for (int i = 0; i < (int)S.size(); ++i) play_soldier(S[i]);
  }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
