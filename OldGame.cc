#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AtsukyD3_v6 


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


   struct structure {
        int   posi; // The id of the unit on the cell if any, -1 otherwise.
        int   posj; // If type == CITY, the id of the city, -1 otherwise.
        int   id; // If type == PATH, the id of the path, -1 otherwis
    };

    typedef vector<int> VI;
    typedef vector<VI>  VVI;

    typedef vector<char> VC;
    typedef vector<VC> VVC;

    typedef vector<structure> VS;

    struct point {
        int x, y, xa, ya;
    };

    // Stores the location of orks.
    VVI ork_at;
    vector<int> test;
    map<int, pair<int,int>> position_id_orks;

    typedef vector< vector<string> > graph;

    VVC matrix;
    VVC m2;

    VS cities;
    VS paths; 

    stack<string> moviments;

    // Moves ork with identifier id.
    void move(int id) {
        Unit u = unit(id);
        Pos pos = u.pos;
        // Try to move to a position within the board.
        for (int d = 0; d != DIR_SIZE; ++d) {
            Dir dir = Dir(d);
            Pos npos = pos + dir;
            if (pos_ok(npos)) {
                execute(Command(id, dir));
                return;
            }
        }
    }

    void generate_matrix(){

        for (int i = 1; i < matrix[0].size()-1; ++i){
            for (int j = 1; j < matrix.size()-1; ++j) {
                Cell c = cell(i-1, j-1);
                if (c.type == CITY) matrix[i][j] = 'B';
                else if (c.type == PATH) matrix[i][j] = 'B';
                else if (c.type == WATER) matrix[i][j] = 'X';
                else if (c.type == GRASS) matrix[i][j] = '.';
                else if (c.type == FOREST) matrix[i][j] = '.';
                else if (c.type == SAND) matrix[i][j] = '.';
            } 
        }
    }

    void generate_matrix_2(VVC &matrix){

        for (int i = 1; i < matrix[0].size()-1; ++i){
            for (int j = 1; j < matrix.size()-1; ++j) {
                Cell c = cell(i-1, j-1);
                if (c.type == CITY){
                  if (checkIfImCityOwner(i-1,j-1)) matrix[i][j] = '.';
                  else matrix[i][j] = 'B';
              } 
              else if (c.type == PATH){
                  if (checkIfImPathOwner(i-1,j-1)) matrix[i][j] = '.';
                  else matrix[i][j] = 'B';
              } 
              else if (c.type == WATER) matrix[i][j] = 'X';
              else if (c.type == GRASS) matrix[i][j] = '.';
              else if (c.type == FOREST) matrix[i][j] = '.';
              else if (c.type == SAND) matrix[i][j] = '.';
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

int generateMove(stack<point>& cami, point p) {

    if (!cami.empty()){
        point p1 = cami.top();
            //cerr << p1.x << " " << p1.y << endl;
        cami.pop();
        if (!cami.empty()){
            point p2 = cami.top();
                //cerr << "test get move" << endl;
            if (p2.x == p.x and p2.y-1 == p.y) return RIGHT;
            if (p2.x == p.x and p2.y+1 == p.y) return LEFT;
            if (p2.x == p.x-1 and p2.y == p.y) return TOP;
            if (p2.x == p.x+1 and p2.y == p.y) return BOTTOM;
        }
    }

    return NONE;
}

point create_point(int x, int y, int xa, int ya) {
    point p;
    p.x = x;
    p.y = y;
    p.xa = xa;
    p.ya = ya;
    return p;
}

bool checkIfImCityOwner(int i, int j){
    Cell c = cell(i, j);
    int z = c.city_id;
    if (z != -1){
      if (city_owner(z) == -1) return false;
      else if (city_owner(z) == me()) return true;
      else return false;
  }
  return false;
}
bool checkIfImPathOwner(int i, int j){
    Cell c = cell(i, j);
    int z = c.path_id;
    if (z != -1){
      if (path_owner(z) == -1) return false;
      else if (path_owner(z) == me()) return true;
      else return false;
  }
  return false;
}

void bfs_ite(VVC& G, vector<point>& v, point& b) {

    int i = 0;
    while (true) {

        point p = v[i];

            ////cerr << p.x << " " << p.y << " " << i << endl;

        if (G[p.x][p.y] == 'B') {
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
        G[p.x][p.y] = 'X';
    }
    ++i;
}
}

int checkSorroundings (Pos pos, Unit u){
    for (int i = BOTTOM; i < NONE; ++i) {
        Dir dir = Dir(i);
        Pos npos = pos + dir;
        Cell cn = cell(npos);

        int aylmaoxd = 0;

        if (cn.type == CITY) aylmaoxd = 0;
        else if (cn.type == PATH) aylmaoxd = 0;
        else if (cn.type == GRASS) aylmaoxd = 1;
        else if (cn.type == FOREST) aylmaoxd = 2;
        else if (cn.type == SAND) aylmaoxd = 3;

        if (cn.unit_id != -1){
            Unit un = unit(cn.unit_id);
            if (un.player != me()){
                int enemylife = un.health;
                int mylife = u.health - aylmaoxd;
                if (mylife > enemylife) return i;
            }
        }

    }

    return -1;
}


    //Cell cell (int i, int j)

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {


        int columnes = cols()+2;
        int files = rows()+2;

        if (round() == 0){

            //cerr << files << " ";
            //cerr << columnes << endl;

            matrix = VVC(files, VC(columnes,'X'));


            generate_matrix();

             for (int i = 0; i < matrix.size(); ++i){
                for (int j = 0; j < matrix.size(); ++j) cerr << matrix[i][j];
                     cerr << endl;
             }


            // Test code to print the matrix xdlol
           /* for (int i = 0; i < rows(); ++i){
                for (int j = 0; j < cols(); ++j) //cerr << matrix[i][j];
                    //cerr << endl;
            }*/
            }


        VI my_orks = orks(me());

        for (int i = 0; i < my_orks.size(); ++i) {

            m2 = matrix;
            generate_matrix_2(m2);

            
            stack<point> cami;   
            vector<point> v;

            Unit u = unit(my_orks[i]);
            Pos pos = u.pos;
            int id = u.id;

            int x = pos.i;
            int z = pos.j;
            int mov;

            int xdlolxd = checkSorroundings(pos, u);
            if (xdlolxd != -1) mov = xdlolxd;
            else{

                point p = create_point(x+1,z+1,-1,-1);

                v.push_back(p);

                point b;
            //cerr << "Start bfs" << endl;
                bfs_ite(m2,v,b);
            //cerr << "end bfs" << endl;
                cami.push(b);
                getPath(b,v,cami);
            //cerr << "end get path" << endl;
                mov = generateMove(cami, p);
            //cerr << "end mov" << endl;
            }
            Dir dir = Dir(mov);
            execute(Command(id, dir));            

            
        }


        /*for (int i = 0; i < test.size(); ++i) {
            int id = test[i];


        }*/
        }
    };


/**
 * Do not modify the following line.
 */
 RegisterPlayer(PLAYER_NAME);

