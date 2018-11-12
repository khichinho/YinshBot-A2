#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
// #include<utility>
#include<math.h>
// #include <boost/foreach.hpp>
// #include <boost/algorithm/string/classification.hpp>
// #include <boost/algorithm/string/split.hpp>
#include<ctime>

// Things to do - find successor when the opponent has made consecutive 5 markers of yours

// changes in the code to generalize for board size and initial rings
//map_hex_mysys and map_mysys_hex are same
// see board initialization, rings_removed is renamed as game_rings
// slight change in get_position and set_position. 5 is replaced by board_size.Compare it
// startend, cons_marker and moves_row are updated. 1 helper function endpts is used. Update these functions


using namespace std;
static const std::vector<int> all_directions {1,0,-1,0,0,1,0,-1,-1,-1,1,1};

int endpts5[33][4] = {{-1,4,5,4},
{-5,-1,4,-1},
{-4,-5,-4,1},
{1,-4,1,5},
{-5,-1,1,5},
{-4,-5,5,4},
{-2,3,5,3},
{-5,-2,3,-2},
{-3,-5,-3,2},
{2,-3,2,5},
{-5,-2,2,5},
{-3,-5,5,3},
{-3,2,5,2},
{-5,-3,2,-3},
{-2,-5,-2,3},
{3,-2,3,5},
{-5,-3,3,5},
{-2,-5,5,2},
{-4,1,5,1},
{-5,-4,1,-4},
{-1,-5,-1,4},
{4,-1,4,5},
{-5,-4,4,5},
{-1,-5,5,1},
{-4,0,4,0},
{0,-4,0,4},
{-4,-4,4,4},
{1,5,4,5},
{-4,-5,-1,-5},
{-4,1,-1,4},
{1,-4,4,-1},
{-5,-4,-5,-1},
{5,1,5,4},
};

int endpts6[39][4] = {{-1,5,6,5},
{-6,-1,5,-1},
{-5,-6,-5,1},
{1,-5,1,6},
{-6,-1,1,6},
{-5,-6,6,5},
{-2,4,6,4},
{-6,-2,4,-2},
{-4,-6,-4,2},
{2,-4,2,6},
{-6,-2,2,6},
{-4,-6,6,4},
{-3,3,6,3},
{-6,-3,3,-3},
{-3,-6,-3,3},
{3,-3,3,6},
{-6,-3,3,6},
{-3,-6,6,3},
{-4,2,6,2},
{-6,-4,2,-4},
{-2,-6,-2,4},
{4,-2,4,6},
{-6,-4,4,6},
{-2,-6,6,2},
{-5,1,6,1},
{-6,-5,1,-5},
{-1,-6,-1,5},
{5,-1,5,6},
{-6,-5,5,6},
{-1,-6,6,1},
{-5,0,5,0},
{0,-5,0,5},
{-5,-5,5,5},
{1,6,5,6},
{-5,-6,-1,-6},
{-5,1,-1,5},
{1,-5,5,-1},
{-6,-5,-6,-1},
{6,1,6,5},
};



// used to convert hex coordinate to my coordinate
vector<int> map_hex_mysys(int hexagon, int index){
    vector<int> my_coord;
    my_coord.push_back(0);
    my_coord.push_back(0);
    if(index >=0 && index < hexagon){
        my_coord[0] = index;
        my_coord[1] = hexagon;
    }
    else if(index >= hexagon && index < 2*hexagon){
        my_coord[0] = hexagon;
        my_coord[1] = 2*hexagon - index;
    }
    else if(index >= 2*hexagon && index < 3*hexagon){
        my_coord[0] = 3*hexagon - index;
        my_coord[1] = 2*hexagon - index; 
    }
    else if(index >= 3*hexagon && index < 4*hexagon){
        my_coord[0] = 3*hexagon - index;
        my_coord[1] = (-1)*hexagon;
    }
    else if(index >= 4*hexagon && index < 5*hexagon){
        my_coord[0] = (-1)*hexagon;
        my_coord[1] = index - 5*hexagon;
    }
    else if(index >= 5*hexagon){
        my_coord[0] = index - 6*hexagon;
        my_coord[1] = index - 5*hexagon;
    }
    else{
        my_coord[0] = hexagon;
        my_coord[1] = index;
    }
    return my_coord;
}

// used to convert my coordinate to hex coordinate
vector<int> map_mysys_hex(int abscissa, int ordinate){
    vector<int> hex_coord;
    hex_coord.push_back(0);
    hex_coord.push_back(0);

    if(abscissa >= 0 && ordinate >= 0){
        if(ordinate > abscissa){
            hex_coord[0] = ordinate;
            hex_coord[1] = abscissa;
        }
        else if(ordinate <= abscissa){
            hex_coord[0] = abscissa;
            hex_coord[1] = 2*abscissa - ordinate;
        }
    }
    else if(abscissa >= 0 && ordinate < 0){
            hex_coord[0] = abscissa - ordinate;
            hex_coord[1] = 2*abscissa - 3*ordinate;
    }
    else if(abscissa < 0 && ordinate < 0){
        if(ordinate <= abscissa){
            hex_coord[0] = -1*ordinate;
            hex_coord[1] = -1*(3*ordinate+abscissa);
        }
        else if(ordinate > abscissa){
            hex_coord[0] = -1*abscissa;
            hex_coord[1] = -5*abscissa + ordinate;
        }
    }
    else if(abscissa < 0 && ordinate >= 0){
            hex_coord[0] = ordinate - abscissa;
            hex_coord[1] = 6*ordinate - 5*abscissa;
    }
    else{
        hex_coord[0] = abscissa;
        hex_coord[1] = ordinate;
    }
    return hex_coord;
}

// vector <string> splitstring(string str){
//     vector<string> str_vec;
//     typedef vector<string> Tokens;
//     Tokens tokens;
//     boost::split( tokens, str, boost::is_any_of(" ") );
//     BOOST_FOREACH( const string& i, tokens){
//         str_vec.push_back(i);
//     }
//     return str_vec; 
// }


int call_execute=1;
int call_allmoves=1;
int call_min_alpha=1;
int call_max_alpha=1;
int call_heuristic=1;
int call_cons_marker=1;
int call_startend=1;
int call_is_marker5=1;

double time_execute=0;
double time_allmoves=0;
double time_min_alpha=0;
double time_max_alpha=0;
double time_heuristic=0;
double time_cons_marker=0;
double time_allmoves1=0;
double time_allmoves2=0;
double time_allmoves3=0;
double time_allmoves4=0;
double time_copy=0;
double time_end_pts=0;
double time_startend=0;
double time_moves_row=0;
double time_is_marker5=0;


vector<string> splitstr(string str, char dl){
    string word = "";
    int num = 0;
    str = str + dl;
    int l = str.size();
    vector<string> substr_list;
    
    for (int i = 0; i < l; i++) {
        if (str[i] != dl)
            word = word + str[i];
        else {
            if ((int)word.size() != 0)
                substr_list.push_back(word);
            word = "";
        }
    }
    return substr_list;
}


class Move{
    public:
        string move_type;
        int x;
        int y;
        
        Move(string type, int xcor, int ycor){
            move_type = type;
            x = xcor;
            y = ycor;
        }
};

// vector<Move> movlist(string ply){
//     vector<Move> ret;
//     vector<string> movstr = splitstr(ply);
//     for(int i = 0; i < movstr.size(); i = i+3){
//         int hex = stoi(movstr[i+1]);
//         int ind = stoi(movstr[i+2]);
//         vector<int> coord = map_hex_mysys(hex,ind);
//         Move m1(movstr[i], coord[0], coord[1]);
//         ret.push_back(m1);
//     }
//     return ret;
// }

vector<Move> movlist(string ply){
    vector<string> ply_vector = splitstr(ply, ' ');
    vector<Move> my_move;

    for(int i = 0; i < ply_vector.size()/3; i++){
        vector<int> hex_coordinates;
        hex_coordinates.push_back(stoi(ply_vector[3*i + 1]));
        hex_coordinates.push_back(stoi(ply_vector[3*i + 2]));
        vector<int> mysys_coordinates = map_hex_mysys(hex_coordinates[0],hex_coordinates[1]);

        Move m = Move(ply_vector[3*i], mysys_coordinates[0], mysys_coordinates[1]);
        my_move.push_back(m);
    }
    return(my_move);
}


string mov_string(vector<Move> movelist){
    string ret;
    for(int i=0;i<movelist.size()-1;i++){
        vector<int> coord = map_mysys_hex(movelist[i].x,movelist[i].y);
        ret += movelist[i].move_type + " " + to_string(coord[0]) + " " + to_string(coord[1]) + " ";
    }
    int size = movelist.size();
    vector<int> coord1 = map_mysys_hex(movelist[size-1].x,movelist[size-1].y);
    ret += movelist[size-1].move_type + " " + to_string(coord1[0]) + " " + to_string(coord1[1]);
    return ret;
}

class Direction{
    public:
        int xchange;
        int ychange;
    
        Direction(int xc, int yc){
            xchange = xc;
            ychange = yc;
        }
};



class Board{

    private:
        // int score;
        int ring1;
        int ring2;
        int marker1;
        int marker2;
        int markers_per_ring;  // number of markers, can be removed in one step
        int game_rings;  // no of starting rings
        int ring1_removed;  // rings removed of player1 till now
        int ring2_removed;  // rings removed of player2 till now
        int board_size; // size of the board

    public:

        vector<vector<int> > board_storage;

        vector<vector<int> > player1_rings;
        vector<vector<int> > player2_rings;
        
        Board(int n, int m, int k){     // n,m and k are board_size , initial rings and no of markers to be removed respectively
            vector<int> element(2*n+1,0);
            // for(int i = -n; i <= n; i++)
            //     element.push_back(0);
            for(int i = -n; i <= n; i++)
                board_storage.push_back(element);
            
            // element.clear();

            board_size = n;
            markers_per_ring = k;
            game_rings = m;
            ring1 = 0;
            ring2 = 0;
            marker1 = 0;
            marker2 = 0;
            ring1_removed = 0;
            ring2_removed = 0;
        }

        int get_position(int x, int y);     // it is used to find the value of the element at the given position in the board
        void set_position(int x, int y, int value);  // used to set the value in the given position
        bool check_valid_position(int x, int y);    // used if the given position exists in the board

        int get_ring1_removed();
        int get_ring2_removed();
        // vector<Move> get_move(string ply);

        // vector<int> map_hex_mysys(int hexagon, int index); // used to convert hex coordinate to my coordinate
        // vector<int> map_mysys_hex(int abscissa, int ordinate);  // used to convert my coordinate to hex coordinate

        void execute_move(vector<Move>, int player_index); // execute the given move


        vector<int> marker_coordinate_feature(int s1,int f1,int s2, int f2, int value); // used to find the coordinates of end points of consecutive markers in a row
        vector<int> marker_set(int s1,int f1, int s2, int f2, int value);   // used to find the number of k-length conseutive markers in a row. k vary from 1 to markers_per_ring-1
                                                                            // nth index of vector returns the number of n length consecutive markers
        vector<int> total_marker_sets_h(int value);
        
        vector<int> total_marker_sets(int value);   // total number of k-set consecutive markers in the board
        
        int num_moves(int player_index);    // it returns total number of moves a player can play

        void print_board();
        string print_position(int x, int y);

        // Board copy_board();

        vector<int> startend(int s1, int f1, int s2, int f2, int value); // returns coordinates of all the consecutive 5 or more length of same given value  
        // input as starting and ending coordinates
        vector<int> moves_row(vector<int> ls);  // returns possible  5 consecutive length in the same direction. Helper function for cons_marker 
        vector<int> cons_marker(int value);     // it returns all possible 5 consecutive elements of same given value.
        //Element as (x1,y1) and (x2,y2) is stored in vector as [x1,y1,x2,y2]  

        vector<int> cons_marker_old(int value);
        int marker_length_one_dir (int x,int y,int xc,int yc,int value);
        bool is_marker5(int x,int y,int value);
        int heuristic();
        Board copy_board();

};

int Board::get_ring1_removed(){
    return ring1_removed;
}
int Board::get_ring2_removed(){
    return ring2_removed;
}
vector<vector<Move>> all_moves_aa(Board bord,int player_index);
Board Board::copy_board(){

    clock_t t9 = clock();

    Board b = Board(board_size, game_rings, markers_per_ring);
    b.ring1 = ring1;
    b.ring2 = ring2;
    b.marker1 = marker1;
    b.marker2= marker2;
    b.ring1_removed = ring1_removed;
    b.ring2_removed = ring2_removed;
    b.game_rings = game_rings;
    b.board_size = board_size;
    b.markers_per_ring = markers_per_ring;
    b.player1_rings = player1_rings;
    b.player2_rings = player2_rings;
    b.board_storage = board_storage;

    clock_t t10 = clock();
    time_copy += (double)(t10-t9)/CLOCKS_PER_SEC;
    return(b);
}

bool Board::check_valid_position(int x, int y){
    int s = board_size;
    if(x == 0){
        if(y >= -(s-1) && y <= s-1){ return true;}
        else{return false;}
    }

    else if(x == 1){
        if(y >= -(s-1) && y <= s){ return true;}
        else{return false;}
    }
    else if(x == 2){
        if(y >= -(s-2) && y <= s){ return true;}
        else{return false;}
    }
    else if(x == 3){
        if(y >= -(s-3) && y <= s){ return true;}
        else{return false;}
    }
    else if(x == 4){
        if(y >= -(s-4) && y <= s){ return true;}
        else{return false;}
    }
    else if(x == 5){
        if(s==5){
            if(y >= 1 && y <= 4){ return true;}
            else{return false;}
        }
        if(s==6){
            if(y>=-1 && y <= 6)return true;
            return false;
        }
    }

    else if(x == -1){
        if(y >= -s && y <= s-1){ return true;}
        else{return false;}
    }
    else if(x == -2){
        if(y >= -s && y <= s-2){ return true;}
        else{return false;}
    }
    else if(x == -3){
        if(y >= -s && y <= s-3){ return true;}
        else{return false;}
    }
    else if(x == -4){
        if(y >= -s && y <= s-4){ return true;}
        else{return false;}
    }
    else if(x == -5){
        if(s==5){
            if(y >= -4 && y <= -1){ return true;}
            else{return false;}
        }
        if(s==6){
            if(y>=-6 && y <= 1)return true;
            return false;
        }
    }

    if(s==6){
        if(x == 6){
            if(y >= 1 && y <= 5)return true;
            return false;
        }
        if(x == -6){
            if(y >= -5 && y <= -1)return true;
            return false;
        }
    }

    else{
        return false;
    }
}

string Board::print_position(int x, int y){
    int value = get_position(x,y);
    if(value == -1){ return "a";}
    else if(value == -2){ return "A";}
    else if(value == 1){ return "d";}
    else if(value == 2){ return "D";}
    else if(value == 0){ return ".";}
}

int Board::get_position(int x, int y){
    int size = board_size;
    return board_storage[x+size][y+size];
}

void Board::set_position(int x, int y, int value){

    int init_value = get_position(x, y);
    int size = board_size;
    board_storage[x+size][y+size] = value;

    if(init_value == 2 && value == 1)marker2++;
    else if(init_value == -2 && value == -1)marker1++;
    else if(init_value == -1 && value == 1){
        marker1--;
        marker2++;
    }
    else if(init_value == 1 && value == -1){
        marker1++;
        marker2--;
    }
    else if(init_value == 1 && value == 0)marker2--;
    else if(init_value == -1 && value == 0)marker1--;
    else if(init_value == 2 && value == 0)ring2--;
    else if(init_value == -2 && value == 0)ring1--;
}


vector<int> Board::startend(int x1, int y1, int x2, int y2, int value){
    clock_t t1 = clock();
    call_startend ++;
    vector<int> ls;
    int k = markers_per_ring;
    ls.push_back(0);
    ls.push_back(0);
    ls.push_back(0);
    ls.push_back(0);
    int firstx = 0;
    int firsty = 0;
    int lastx = 0;
    int lasty = 0;
    int check = 0;
    if(y1 == y2){
        for(int i = x1; i <= x2; i++){
            if(check==0){
                if(get_position(i,y1) == value){
                    firstx = i;
                    firsty = y1;
                    check = 1;
                }
            }
            if(check==1){
                if(get_position(i,y1) == value){
                    lastx = i;
                    lasty = y1;
                    if(i == x2){
                        if(x2 - firstx >= k-1){
                            ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                        }
                    }
                }
                else if(get_position(i,y1) != value){
                    check = 0;
                    if(i-firstx >= k){
                        ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                    }
                }
            }
        }
    }

    else if(x1 == x2){
        for(int i = y1; i <= y2; i++){
            if(check == 0){
                if(get_position(x1, i) == value){
                    firstx = x1;
                    firsty = i;
                    check = 1;
                }
            }
            else if(check == 1){
                if(get_position(x1, i) == value){
                    lastx = x1;
                    lasty = i;
                    if(i == y2){
                        if(y2 - firsty >= k-1){
                            ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                        }
                    }
                }
                else if(get_position(x1, i) != value){
                    check = 0;
                    if(i-firsty >= k){
                        ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                    }
                }
            }
        }
    }

    else if(y1-x1 == y2-x2){
        for(int i = x1; i<= x2; i++){
            if(check == 0){
                if(get_position(i, y1+i-x1) == value){
                    firstx = i;
                    firsty = y1+i-x1;
                    check = 1;
                }
            }
            else if(check == 1){
                if(get_position(i,y1+i-x1) == value){
                    lastx = i;lasty = y1+i-x1;
                    if(i == x2){
                        if(x2 - firstx >= k-1){
                            ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                        }
                    }
                }
                else if(get_position(i, y1+i-x1) != value){
                    check = 0;
                    if(i-firstx >= k){
                        ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                    }
                }
            }
        }
    }
    clock_t t2 = clock();
    time_startend += (double)(t2-t1)/CLOCKS_PER_SEC;

    return ls;
}

vector<int> Board::moves_row(vector<int> ls){
    // call_cons_marker ++;
    clock_t t1 = clock();
    vector<int> vec;
    int k = markers_per_ring;
    if(!((ls[0] == 0) && (ls[1] == 0) && (ls[2] == 0) && (ls[3] == 0))){
        int x = ls[2] - ls[0];
        int y = ls[3] - ls[1];
        int size = max(x,y)+1;
        if(x != 0)x=1;
        if(y != 0)y=1;
        for(int i = 0; i <= size-k; i++){
            vec.push_back(ls[0]+i*x);
            vec.push_back(ls[1]+i*y);
            vec.push_back(ls[0]+(i+k-1)*x);
            vec.push_back(ls[1]+(i+k-1)*y);
        }
    }
    else vec.push_back(0);
    clock_t t2 = clock();
    time_moves_row += (double)(t2-t1)/CLOCKS_PER_SEC;

    return vec;
}

vector<vector<int> > end_pts(int n){
    // call_cons_marker ++;
    clock_t t1 = clock();

    vector<vector<int> > endcoord;
    
    for(int i=1;i<n;i++){
        vector<int> el1{-i,n-i,n,n-i};
        vector<int> el2{-n,-i,n-i,-i};
        vector<int> el3{i-n,-n,i-n,i};
        vector<int> el4{i,i-n,i,n};
        vector<int> el5{-n,-i,i,n};
        vector<int> el6{i-n,-n,n,n-i};
        endcoord.push_back(el1);
        endcoord.push_back(el2);
        endcoord.push_back(el3);
        endcoord.push_back(el4);
        endcoord.push_back(el5);
        endcoord.push_back(el6);
    }

    vector<int> el1{-(n-1),0,n-1,0};
    vector<int> el2{0,-(n-1),0,n-1};
    vector<int> el3{-(n-1),-(n-1),n-1,n-1};
    vector<int> ea1{1,n,n-1,n};
    vector<int> ea2{-(n-1),-n,-1,-n};
    vector<int> ea3{-(n-1),1,-1,n-1};
    vector<int> ea4{1,-(n-1),(n-1),-1};
    vector<int> ea5{-n,-(n-1),-n,-1};
    vector<int> ea6{n,1,n,(n-1)};
    endcoord.push_back(el1);
    endcoord.push_back(el2);
    endcoord.push_back(el3);
    endcoord.push_back(ea1);
    endcoord.push_back(ea2);
    endcoord.push_back(ea3);
    endcoord.push_back(ea4);
    endcoord.push_back(ea5);
    endcoord.push_back(ea6);

    clock_t t2 = clock();
    time_end_pts += (double)(t2-t1)/CLOCKS_PER_SEC;

    return endcoord;
}


vector<int> Board::cons_marker_old(int value){
    vector<int> cons_mark;

    for(int i = 1; i < 5; i++){
        vector<int> vec = moves_row(startend(-i,5-i,5,5-i,value));
        if(vec.size() != 1){
            for(int j = 0;j < vec.size(); j++)
                cons_mark.push_back(vec[j]);
        }
    }

    vector<int> veco = moves_row(startend(-4,0,4,0,value));
    if(veco.size() != 1){
        for(int j = 0;j < veco.size(); j++)
            cons_mark.push_back(veco[j]);
    }

    for(int i = 1; i < 5; i++){
        vector<int> vec = moves_row(startend(-5,-i,5-i,-i,value));
        if(vec.size() != 1){
            for(int j = 0;j < vec.size(); j++)
               cons_mark.push_back(vec[j]);
        }
    }

    for(int i = 1; i < 5; i++){
        vector<int> vec = moves_row(startend(i-5,-5,i-5,i,value));
        if(vec.size() != 1){
            for(int j = 0;j < vec.size(); j++)
                cons_mark.push_back(vec[j]);
        }
    }

    veco = moves_row(startend(0,-4,0,4,value));
    if(veco.size() != 1){
        for(int j = 0;j < veco.size(); j++)
            cons_mark.push_back(veco[j]);
    }

    for(int i = 1; i < 5; i++){
        vector<int> vec = moves_row(startend(i,i-5,i,5,value));
        if(vec.size() != 1){
            for(int j = 0;j < vec.size(); j++)
            cons_mark.push_back(vec[j]);
        }
    }

    for(int i = 1; i < 5; i++){
        vector<int> vec = moves_row(startend(-5,-i,i,5,value));
        if(vec.size() != 1){
            for(int j = 0;j < vec.size(); j++)
            cons_mark.push_back(vec[j]);
        }
    }

    veco = moves_row(startend(-4,-4,4,4,value));
    if(veco.size() != 1){
        for(int j = 0;j < veco.size(); j++)
            cons_mark.push_back(veco[j]);
    }

    for(int i = 1; i < 5; i++){
        vector<int> vec = moves_row(startend(i-5,-5,5,5-i,value));
        if(vec.size() != 1){
            for(int j = 0;j < vec.size(); j++)
                cons_mark.push_back(vec[j]);
        }
    }
    if(cons_mark.size() == 0)cons_mark.push_back(0);

    return cons_mark;

}

vector<int> Board::cons_marker(int value){
    call_cons_marker ++;
    clock_t t1 = clock();
    // vector<vector<int> > border_pts = end_pts(board_size);
    vector<int> consmark;
    // for(int i=0;i<border_pts.size();i++){
    //     vector<int> vec = moves_row(startend(border_pts[i][0],border_pts[i][1],border_pts[i][2],border_pts[i][3],value));
    //     if(vec.size() != 1){
    //         for(int j=0;j<vec.size(); j++)
    //             consmark.push_back(vec[j]);
    //     }
    // }

    int s = board_size;
    if(s==5){
        for(int i=0;i<33;i++){
            vector<int> vec = moves_row(startend(endpts5[i][0],endpts5[i][1],endpts5[i][2],endpts5[i][3],value));
            if(vec.size() != 1){
                for(int j=0;j<vec.size(); j++)
                    consmark.push_back(vec[j]);
            }
        }
    }
    else if(s==6){
        for(int i=0;i<39;i++){
            vector<int> vec = moves_row(startend(endpts6[i][0],endpts6[i][1],endpts6[i][2],endpts6[i][3],value));
            if(vec.size() != 1){
                for(int j=0;j<vec.size(); j++)
                    consmark.push_back(vec[j]);
            }
        }
    }


    if(consmark.size() == 0)consmark.push_back(0);
    clock_t t2 = clock();
    time_cons_marker += (double)(t2-t1)/CLOCKS_PER_SEC;
    return consmark;
}

void Board::execute_move(vector<Move> movelist, int player_index){
    call_execute++;
    clock_t t1 = clock();
    for(int k = 0; k < movelist.size(); k++){
        Move m1 = movelist[k];
        vector<int> ringcoordinate;
        ringcoordinate.push_back(m1.x);
        ringcoordinate.push_back(m1.y);
        if(m1.move_type == "P"){
            set_position(m1.x, m1.y, 2*player_index);
            if(player_index == -1){
                ring1++;
                player1_rings.push_back(ringcoordinate);
            }
            else {
                ring2++;
                player2_rings.push_back(ringcoordinate);
            }
        }
        else if(m1.move_type == "S"){  
            k = k+1;
            Move m2 = movelist[k];
            //if(m2.move_type != "M")throw "invalid move";
            //else {
            vector<int> ring2Coordinate;
            ring2Coordinate.push_back(m2.x);
            ring2Coordinate.push_back(m2.y);
            set_position(m1.x, m1.y, player_index);
            set_position(m2.x, m2.y, 2*player_index);
            if(m1.y == m2.y){
                if(m2.x > m1.x){
                    for(int i = m1.x+1; i < m2.x; i++)
                        set_position(i, m1.y, -1* get_position(i, m1.y));
                }
                else{
                    for(int i = m1.x-1; i > m2.x; i--)
                        set_position(i, m1.y, -1*get_position(i, m1.y));
                }
            }

            else if(m1.x == m2.x){
                if(m2.y > m1.y){
                    for(int i = m1.y+1; i < m2.y; i++)
                        set_position(m1.x, i, -1*get_position(m1.x, i));
                }
                else{
                    for(int i = m1.y-1; i > m2.y; i--)
                        set_position(m1.x, i, -1*get_position(m1.x, i));
                }
            }

            else if(m2.y-m1.y == m2.x-m1.x){
                if(m2.x > m1.x){
                    for(int i = 1; i < m2.x-m1.x; i++)
                        set_position(m1.x+i, m1.y+i, -1*get_position(m1.x+i, m1.y+i));
                }
                else{
                    for(int i = 1; i < m1.x-m2.x; i++)
                        set_position(m1.x-i, m1.y-i, -1*get_position(m1.x-i,m1.y-i));
                }
            }

            if(player_index == -1){
            
                player1_rings.erase(remove(player1_rings.begin(), player1_rings.end(), ringcoordinate), player1_rings.end());
                player1_rings.push_back(ring2Coordinate);
            }
            else{
                player2_rings.erase(remove(player2_rings.begin(), player2_rings.end(), ringcoordinate), player2_rings.end()); 
                player2_rings.push_back(ring2Coordinate);
            }

        }

        else if(m1.move_type =="RS"){
            Move m2 = movelist[++k];
            Move m3 = movelist[++k];
            if(player_index == -1)ring1_removed++;
                else ring2_removed++;
            if(m1.y == m2.y){
                if(m2.x > m1.x){
                    for(int i = 0; i < markers_per_ring; i++)
                        set_position(m1.x+i, m1.y, 0);
                }
                else{
                    for(int i = 0; i < markers_per_ring; i++)
                        set_position(m1.x-i, m1.y, 0);
                }                
            }
            else if(m1.x == m2.x){
                if(m2.y > m1.y){
                    for(int i = 0; i < markers_per_ring; i++)
                        set_position(m1.x, m1.y+i, 0);    
                }
                else{
                    for(int i = 0; i < markers_per_ring; i++){
                         set_position(m1.x, m1.y-i, 0);
                    }
                }
            }
            else if(m2.y-m1.y == m2.x-m1.x){
                if(m2.x > m1.x){
                    for(int i = 0; i < markers_per_ring; i++)
                        set_position(m1.x+i, m1.y+i, 0);
                }
                else{
                    for(int i = 0; i < markers_per_ring; i++)
                        set_position(m1.x-i,m1.y-i, 0);
                }
            }

            set_position(m3.x, m3.y, 0);

            vector<int> ring2Coordinate;
            ring2Coordinate.push_back(m3.x);
            ring2Coordinate.push_back(m3.y);

            if(player_index == -1)
                player1_rings.erase(remove(player1_rings.begin(), player1_rings.end(), ring2Coordinate), player1_rings.end());
            else 
                player2_rings.erase(remove(player2_rings.begin(), player2_rings.end(), ring2Coordinate), player2_rings.end());

        }
        
    }
    clock_t t2 = clock();
    time_execute += (double)(t2-t1)/CLOCKS_PER_SEC;
}


int Board::marker_length_one_dir (int x,int y,int xc,int yc,int value){
    int k=1;
    int count =0;
    while(true){
        if(check_valid_position(x+k*xc,y+k*yc) == false)break;
        if(get_position(x+k*xc,y+k*yc) != value)break;
        count ++;
        k++;
    }
    return count;
}
bool Board::is_marker5(int x,int y,int value){
    call_is_marker5 ++;
    clock_t t1 = clock();
    if(get_position(x,y) != value)return false;
    int k=markers_per_ring;
    if(marker_length_one_dir(x,y,0,1,value) + marker_length_one_dir(x,y,0,-1,value) >= k-1)return true;
    if(marker_length_one_dir(x,y,1,0,value) + marker_length_one_dir(x,y,-1,0,value) >= k-1)return true;
    if(marker_length_one_dir(x,y,1,1,value) + marker_length_one_dir(x,y,-1,-1,value) >= k-1)return true;

    // for(int i=0;i<3;i++){
    //     if(marker_length_one_dir(x,y,all_directions[4*i],all_directions[4*i+1],value)+marker_length_one_dir(x,y,all_directions[4*i+2],all_directions[4*i+3],value) >= k-1)
    //         return true;
    // }
    clock_t t2 = clock();
    time_is_marker5 += (double)(t2-t1)/CLOCKS_PER_SEC;
    return false;
}




// extract all possible moves of the given player
pair<vector<vector<Move> >,vector<Board> >all_moves(Board bord,int player_index){
    call_allmoves ++;
    clock_t t1 = clock();
    vector<vector<Move> > possible_moves;
    vector<Board> corresponding_boards;
    vector<vector<int> > player_rings;
    Board b = bord.copy_board();
    vector<int> consecutive_init = b.cons_marker(player_index);
    vector<Move> init_move;
    if(consecutive_init.size() != 1){
        // clock_t t3 = clock();
        Move rem_ini("RS",consecutive_init[0],consecutive_init[1]);
        Move rem_end("RE",consecutive_init[2],consecutive_init[3]);
        Move rem_ring("X",b.player1_rings[0][0],b.player1_rings[0][1]);
        if(player_index == 1){
            rem_ring.x = b.player2_rings[0][0];
            rem_ring.y = b.player2_rings[0][1];        
        }
        init_move.push_back(rem_ini);
        init_move.push_back(rem_end);
        init_move.push_back(rem_ring);
        b.execute_move(init_move,player_index);

        int r;
        if(player_index == -1)r = b.get_ring1_removed();
        else r = b.get_ring2_removed();
        if(r == 3){
            possible_moves.push_back(init_move);
            corresponding_boards.push_back(b);
            pair<vector<vector<Move> >,vector<Board> > successor(possible_moves,corresponding_boards);
            return successor;
        }
    }
    if(player_index == -1)player_rings = b.player1_rings;
    else {
        player_rings = b.player2_rings;
    }

    // cout << b.player1_rings.size() << "anddfjid";
    clock_t t3 = clock();
    for(int i=0;i<player_rings.size();i++){
        
        int prx = player_rings[i][0];
        int pry = player_rings[i][1];
        // cout << "assds";
        // cout << prx << " " << pry << endl;
        for(int j=0;j<6;j++){
            
            int k=1;
            int count=0;
            while(true){
                clock_t t7 = clock();
                int x = prx+k*all_directions[2*j];
                int y = pry+k*all_directions[2*j+1];

                if(b.check_valid_position(x,y)==false)break;

                // cout << b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) << "  fksfjk \n";

                if(b.get_position(x,y) == -2 || b.get_position(x,y) == 2)break;
                // if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 2 )break;

                if(b.get_position(x,y) == -1 || b.get_position(x,y) == 1){
                    count = 1;
                    k++;
                    // cout << "asasasasa" << endl;
                    continue;
                }

                // if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 1){
                //     count = 1;
                //     k++;
                //     continue;
                // }
                // cout << "wahtthefuk" << endl;
                Board temp_board = b.copy_board();
                // Board temp_board = Board(b);
                vector<Move> temp_move = init_move;
                // vector<Move> temp_move;
                Move m1("S", prx, pry);
                Move m2("M", x, y);
                temp_move.push_back(m1);
                temp_move.push_back(m2);
                temp_board.execute_move(temp_move,player_index);

                k++;
                bool is_exist5 = false;
                int xchange = x-prx;
                int ychange = y-pry;
                int size = max(abs(xchange),abs(ychange));
                int xcc=0;int ycc=0;
                if(xchange != 0)xcc=xchange/abs(xchange);
                if(ychange != 0)ycc=ychange/abs(ychange);
                for(int l=0;l<=size;l++){
                    is_exist5 = temp_board.is_marker5(prx+l*xcc,pry+l*ycc,player_index);
                    if(is_exist5 == true)break;
                }
                // if(temp_board.is_marker5(prx,pry,player_index) == true){
                    if(is_exist5 == true){
                    // cerr << temp_board.is_marker5(prx,pry,player_index);

                vector<int> all_consecutive = temp_board.cons_marker(player_index);
                clock_t t8 = clock();
                time_allmoves4 += (double)(t8-t7)/CLOCKS_PER_SEC;
                if(all_consecutive.size() == 1){
                    possible_moves.push_back(temp_move);
                    corresponding_boards.push_back(temp_board);
                }
                

                else{
                    // cout << "aniefnkdskds   " << k << "a   " << all_consecutive.size() << "   aniefdfj    ";
                    for(int x=0;x<all_consecutive.size()/4;x++){
                        clock_t t5 = clock();
                        Move temp_m1("RS", all_consecutive[4*x], all_consecutive[4*x+1]);
                        Move temp_m2("RE", all_consecutive[4*x+2], all_consecutive[4*x+3]);
                        vector<vector<int> > play_rings;
                        if(player_index==-1)play_rings = temp_board.player1_rings;
                        else play_rings = temp_board.player2_rings;
                        for(int r=0;r<play_rings.size();r++){
                            Move temp_m3("X", play_rings[r][0], play_rings[r][1]);
                            vector<Move> temp_temp_move;
                            Board temp_temp_board = temp_board.copy_board();
                            // Board temp_temp_board = Board(temp_board);
                            temp_temp_move.push_back(m1);
                            temp_temp_move.push_back(m2);
                            temp_temp_move.push_back(temp_m1);
                            temp_temp_move.push_back(temp_m2);
                            temp_temp_move.push_back(temp_m3);
                            temp_temp_board.execute_move(temp_temp_move,player_index);
                            possible_moves.push_back(temp_temp_move);
                            corresponding_boards.push_back(temp_temp_board);
                        }
                        clock_t t6 = clock();
                        time_allmoves2 += (double)(t6-t5)/CLOCKS_PER_SEC;
                    }
                }



                }
                else{possible_moves.push_back(temp_move);
                    corresponding_boards.push_back(temp_board);}
                if(count==1)break; 
            }
        }
    }
    clock_t t4 = clock();
    time_allmoves1 += (double)(t4-t3)/CLOCKS_PER_SEC;
    pair<vector<vector<Move> >,vector<Board> > successor(possible_moves,corresponding_boards);
    clock_t t2 = clock();
    time_allmoves += (double)(t2-t1)/CLOCKS_PER_SEC;
    return successor;
}


pair<vector<vector<Move> >,vector<Board> >all_moves_exp(Board b,int player_index){
    call_allmoves ++;
    clock_t t1 = clock();
    vector<vector<Move> > possible_moves;
    vector<Board> corresponding_boards;
    vector<int> consecutive_init = b.cons_marker(player_index);
    vector<Move> init_move;
    if(consecutive_init.size() != 1){
        // clock_t t3 = clock();
        Move rem_ini("RS",consecutive_init[0],consecutive_init[1]);
        Move rem_end("RE",consecutive_init[2],consecutive_init[3]);
        Move rem_ring("X",b.player1_rings[0][0],b.player1_rings[0][1]);
        if(player_index == 1){
            rem_ring.x = b.player2_rings[0][0];
            rem_ring.y = b.player2_rings[0][1];        
        }
        init_move.push_back(rem_ini);
        init_move.push_back(rem_end);
        init_move.push_back(rem_ring);
        b.execute_move(init_move,player_index);

        int r;
        if(player_index == -1)r = b.get_ring1_removed();
        else r = b.get_ring2_removed();
        if(r == 3){
            possible_moves.push_back(init_move);
            corresponding_boards.push_back(b);
            pair<vector<vector<Move> >,vector<Board> > successor(possible_moves,corresponding_boards);
            return successor;
        }
    }
    if(player_index == -1){

    // cout << b.player1_rings.size() << "anddfjid";
    clock_t t3 = clock();
    for(int i=0;i<b.player1_rings.size();i++){
        
        int prx = b.player1_rings[i][0];
        int pry = b.player1_rings[i][1];
        // cout << "assds";
        // cout << prx << " " << pry << endl;
        for(int j=0;j<6;j++){
            
            int k=1;
            int count=0;
            while(true){
                clock_t t7 = clock();
                int x = prx+k*all_directions[2*j];
                int y = pry+k*all_directions[2*j+1];

                if(b.check_valid_position(x,y)==false)break;

                // cout << b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) << "  fksfjk \n";

                if(b.get_position(x,y) == -2 || b.get_position(x,y) == 2)break;
                // if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 2 )break;

                if(b.get_position(x,y) == -1 || b.get_position(x,y) == 1){
                    count = 1;
                    k++;
                    // cout << "asasasasa" << endl;
                    continue;
                }

                // if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 1){
                //     count = 1;
                //     k++;
                //     continue;
                // }
                // cout << "wahtthefuk" << endl;
                Board temp_board = b.copy_board();
                // Board temp_board = Board(b);
                vector<Move> temp_move = init_move;
                // vector<Move> temp_move;
                Move m1("S", prx, pry);
                Move m2("M", x, y);
                temp_move.push_back(m1);
                temp_move.push_back(m2);
                temp_board.execute_move(temp_move,player_index);

                k++;

                if(temp_board.is_marker5(prx,pry,player_index) == true){
                    // cerr << temp_board.is_marker5(prx,pry,player_index);

                vector<int> all_consecutive = temp_board.cons_marker(player_index);
                clock_t t8 = clock();
                time_allmoves4 += (double)(t8-t7)/CLOCKS_PER_SEC;
                if(all_consecutive.size() == 1){
                    possible_moves.push_back(temp_move);
                    corresponding_boards.push_back(temp_board);
                }
                

                else{
                    // cout << "aniefnkdskds   " << k << "a   " << all_consecutive.size() << "   aniefdfj    ";
                    for(int x=0;x<all_consecutive.size()/4;x++){
                        clock_t t5 = clock();
                        Move temp_m1("RS", all_consecutive[4*x], all_consecutive[4*x+1]);
                        Move temp_m2("RE", all_consecutive[4*x+2], all_consecutive[4*x+3]);
                        vector<vector<int> > play_rings;
                        if(player_index==-1)play_rings = temp_board.player1_rings;
                        else play_rings = temp_board.player2_rings;
                        for(int r=0;r<play_rings.size();r++){
                            Move temp_m3("X", play_rings[r][0], play_rings[r][1]);
                            vector<Move> temp_temp_move;
                            Board temp_temp_board = temp_board.copy_board();
                            // Board temp_temp_board = Board(temp_board);
                            temp_temp_move.push_back(m1);
                            temp_temp_move.push_back(m2);
                            temp_temp_move.push_back(temp_m1);
                            temp_temp_move.push_back(temp_m2);
                            temp_temp_move.push_back(temp_m3);
                            temp_temp_board.execute_move(temp_temp_move,player_index);
                            possible_moves.push_back(temp_temp_move);
                            corresponding_boards.push_back(temp_temp_board);
                        }
                        clock_t t6 = clock();
                        time_allmoves2 += (double)(t6-t5)/CLOCKS_PER_SEC;
                    }
                }



                }
                else{possible_moves.push_back(temp_move);
                    corresponding_boards.push_back(temp_board);}
                if(count==1)break; 
            }
        }
    }
    clock_t t4 = clock();
    time_allmoves1 += (double)(t4-t3)/CLOCKS_PER_SEC;
    pair<vector<vector<Move> >,vector<Board> > successor(possible_moves,corresponding_boards);
    clock_t t2 = clock();
    time_allmoves += (double)(t2-t1)/CLOCKS_PER_SEC;
    return successor;
    }

    if(player_index == 1){

    // cout << b.player1_rings.size() << "anddfjid";
    clock_t t3 = clock();
    for(int i=0;i<b.player2_rings.size();i++){
        
        int prx = b.player2_rings[i][0];
        int pry = b.player2_rings[i][1];
        // cout << "assds";
        // cout << prx << " " << pry << endl;
        for(int j=0;j<6;j++){
            
            int k=1;
            int count=0;
            while(true){
                clock_t t7 = clock();
                int x = prx+k*all_directions[2*j];
                int y = pry+k*all_directions[2*j+1];

                if(b.check_valid_position(x,y)==false)break;

                // cout << b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) << "  fksfjk \n";

                if(b.get_position(x,y) == -2 || b.get_position(x,y) == 2)break;
                // if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 2 )break;

                if(b.get_position(x,y) == -1 || b.get_position(x,y) == 1){
                    count = 1;
                    k++;
                    // cout << "asasasasa" << endl;
                    continue;
                }

                // if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 1){
                //     count = 1;
                //     k++;
                //     continue;
                // }
                // cout << "wahtthefuk" << endl;
                Board temp_board = b.copy_board();
                // Board temp_board = Board(b);
                vector<Move> temp_move = init_move;
                // vector<Move> temp_move;
                Move m1("S", prx, pry);
                Move m2("M", x, y);
                temp_move.push_back(m1);
                temp_move.push_back(m2);
                temp_board.execute_move(temp_move,player_index);

                k++;

                if(temp_board.is_marker5(prx,pry,player_index) == true){
                    // cerr << temp_board.is_marker5(prx,pry,player_index);

                vector<int> all_consecutive = temp_board.cons_marker(player_index);
                clock_t t8 = clock();
                time_allmoves4 += (double)(t8-t7)/CLOCKS_PER_SEC;
                if(all_consecutive.size() == 1){
                    possible_moves.push_back(temp_move);
                    corresponding_boards.push_back(temp_board);
                }
                

                else{
                    // cout << "aniefnkdskds   " << k << "a   " << all_consecutive.size() << "   aniefdfj    ";
                    for(int x=0;x<all_consecutive.size()/4;x++){
                        clock_t t5 = clock();
                        Move temp_m1("RS", all_consecutive[4*x], all_consecutive[4*x+1]);
                        Move temp_m2("RE", all_consecutive[4*x+2], all_consecutive[4*x+3]);
                        vector<vector<int> > play_rings;
                        if(player_index==-1)play_rings = temp_board.player1_rings;
                        else play_rings = temp_board.player2_rings;
                        for(int r=0;r<play_rings.size();r++){
                            Move temp_m3("X", play_rings[r][0], play_rings[r][1]);
                            vector<Move> temp_temp_move;
                            Board temp_temp_board = temp_board.copy_board();
                            // Board temp_temp_board = Board(temp_board);
                            temp_temp_move.push_back(m1);
                            temp_temp_move.push_back(m2);
                            temp_temp_move.push_back(temp_m1);
                            temp_temp_move.push_back(temp_m2);
                            temp_temp_move.push_back(temp_m3);
                            temp_temp_board.execute_move(temp_temp_move,player_index);
                            possible_moves.push_back(temp_temp_move);
                            corresponding_boards.push_back(temp_temp_board);
                        }
                        clock_t t6 = clock();
                        time_allmoves2 += (double)(t6-t5)/CLOCKS_PER_SEC;
                    }
                }



                }
                else{possible_moves.push_back(temp_move);
                    corresponding_boards.push_back(temp_board);}
                if(count==1)break; 
            }
        }
    }
    clock_t t4 = clock();
    time_allmoves1 += (double)(t4-t3)/CLOCKS_PER_SEC;
    pair<vector<vector<Move> >,vector<Board> > successor(possible_moves,corresponding_boards);
    clock_t t2 = clock();
    time_allmoves += (double)(t2-t1)/CLOCKS_PER_SEC;
    return successor;
    }


}

vector<vector<Move> > all_moves_aa(Board bord,int player_index){
    vector<vector<Move> > possible_moves;
    vector<vector<int> > player_rings;
    Board b = Board(bord);
    vector<int> consecutive_init = b.cons_marker(player_index);
    vector<Move> init_move;
    if(consecutive_init.size() != 1){
        // clock_t t3 = clock();
        Move rem_ini("RS",consecutive_init[0],consecutive_init[1]);
        Move rem_end("RE",consecutive_init[2],consecutive_init[3]);
        Move rem_ring("X",b.player1_rings[0][0],b.player1_rings[0][1]);
        if(player_index == 1){
            rem_ring.x = b.player2_rings[0][0];
            rem_ring.y = b.player2_rings[0][1];        
        }
        init_move.push_back(rem_ini);
        init_move.push_back(rem_end);
        init_move.push_back(rem_ring);
        b.execute_move(init_move,player_index);

        int r;
        if(player_index == -1)r = b.get_ring1_removed();
        else r = b.get_ring2_removed();
        if(r == 3){
            possible_moves.push_back(init_move);
            return possible_moves;
        }
    }
    if(player_index == -1)player_rings = b.player1_rings;
    else {
        player_rings = b.player2_rings;
    }

    for(int i=0;i<player_rings.size();i++){
        
        int prx = player_rings[i][0];
        int pry = player_rings[i][1];
        for(int j=0;j<6;j++){
            
            int k=1;
            int count=0;
            while(true){
                clock_t t7 = clock();
                int x = prx+k*all_directions[2*j];
                int y = pry+k*all_directions[(2*j)+1];

                if(b.check_valid_position(x,y)==false)break;

                if(b.get_position(x,y) == -2)break;
                if(b.get_position(x,y) == 2)break;

                if(b.get_position(x,y) == -1 || b.get_position(x,y) == 1){
                    count = 1;
                    k++;
                    continue;
                }

                // Board temp_board = b.copy_board();
                Board temp_board = Board(b);
                vector<Move> temp_move = init_move;
                Move m1("S", prx, pry);
                Move m2("M", x, y);
                temp_move.push_back(m1);
                temp_move.push_back(m2);
                temp_board.execute_move(temp_move,player_index);

                k++;
                bool is_exist5 = false;
                int xchange = x-prx;
                int ychange = y-pry;
                int size = max(abs(xchange),abs(ychange));
                int xcc=0;int ycc=0;
                if(xchange != 0)xcc=xchange/abs(xchange);
                if(ychange != 0)ycc=ychange/abs(ychange);
                for(int l=0;l<=size;l++){
                    is_exist5 = temp_board.is_marker5(prx+l*xcc,pry+l*ycc,player_index);
                    if(is_exist5 == true)break;
                }
                // if(temp_board.is_marker5(prx,pry,player_index) == true){
                if(is_exist5 == true){

                    vector<int> all_consecutive = temp_board.cons_marker(player_index);
                    clock_t t8 = clock();
                    time_allmoves4 += (double)(t8-t7)/CLOCKS_PER_SEC;
                    if(all_consecutive.size() == 1){
                        possible_moves.push_back(temp_move);
                    }
                

                    else{
                        for(int x=0;x<all_consecutive.size()/4;x++){
                            Move temp_m1("RS", all_consecutive[4*x], all_consecutive[4*x+1]);
                            Move temp_m2("RE", all_consecutive[4*x+2], all_consecutive[4*x+3]);
                            vector<vector<int> > play_rings;
                            if(player_index==-1)play_rings = temp_board.player1_rings;
                            else play_rings = temp_board.player2_rings;
                            for(int r=0;r<play_rings.size();r++){
                                Move temp_m3("X", play_rings[r][0], play_rings[r][1]);
                                vector<Move> temp_temp_move;
                                Board temp_temp_board = temp_board.copy_board();
                                temp_temp_move.push_back(m1);
                                temp_temp_move.push_back(m2);
                                temp_temp_move.push_back(temp_m1);
                                temp_temp_move.push_back(temp_m2);
                                temp_temp_move.push_back(temp_m3);
                                temp_temp_board.execute_move(temp_temp_move,player_index);
                                possible_moves.push_back(temp_temp_move);
                            }
                        }
                    }

                }
                else{possible_moves.push_back(temp_move);}
                if(count==1)break; 
            }
        }
    }
    return possible_moves;
}

pair<vector<Move>, int> max_alpha(Board b,int player_index, int depth,int alpha,int beta);
pair<vector<Move>, int> min_alpha(Board b,int player_index,int depth,int alpha,int beta){
    call_min_alpha ++;
    clock_t t1 = clock();
    // int utility = 100010101;
    vector<Move> movereturn;
    if(depth == 0){
        vector<Move> mv;
        Move m("S",0,0);
        mv.push_back(m);
        pair<vector<Move> , int> reee (mv,b.heuristic());
        return reee;
    }
    pair<vector<vector<Move> >,vector<Board> > successor = all_moves(b,player_index);
    for(int i=0;i<successor.second.size();i++){
        Board temp_board = successor.second[i];
        // temp_board.execute_move(possible_moves[i],player_index);
        pair<vector<Move>, int> child = max_alpha(temp_board,-1*player_index,depth-1,alpha,beta);
        // if(utility > max_alpha(temp_board,-1*player_index,depth-1,alpha,beta).second){
        //     movereturn = possible_moves[i];
        //     utility = max_alpha(temp_board,-1*player_index,depth-1,alpha,beta).second;
        // }
        if(beta > child.second){
            movereturn = successor.first[i];
            beta = child.second;
        }
        // beta = min(beta,child.second);
        
        pair<vector<Move>, int> util(movereturn,beta);
        if(alpha >= beta) return util;
    }
    pair<vector<Move>, int> util(movereturn,beta);
    clock_t t2 = clock();
    time_min_alpha += (double)(t2-t1)/CLOCKS_PER_SEC;
    return util;
}

pair<vector<Move>, int> max_alpha(Board b,int player_index, int depth,int alpha,int beta){
    // int utility = -1000000;
    call_max_alpha ++;
    clock_t t1 = clock();
    vector<Move> movereturn;
    if(depth == 0){
        vector<Move> mv;
        Move m("S",0,0);
        mv.push_back(m);
        pair<vector<Move>, int> reee (mv,b.heuristic());
        return reee;
    }
    
    pair<vector<vector<Move> >,vector<Board> > successor = all_moves(b,player_index);
    for(int i=0;i<successor.second.size();i++){
        Board temp_board = successor.second[i];
        // temp_board.execute_move(possible_moves[i],player_index);
        pair<vector<Move>, int> child = min_alpha(temp_board,-1*player_index,depth-1,alpha,beta);
        if(alpha < child.second){
            movereturn = successor.first[i];
            alpha = child.second;
        }
        // alpha = max(alpha,utility);
        pair<vector<Move>, int> util(movereturn, alpha);
        if(alpha >= beta)return util;
    }
    pair<vector<Move> ,int> util(movereturn, alpha);
    clock_t t2 = clock();
    time_max_alpha += (double)(t2-t1)/CLOCKS_PER_SEC;
    return util;
}

vector<Move> alpha_beta(Board b,int player_index,int depth,int alpha, int beta){
    if(player_index == -1)return min_alpha(b,player_index,depth,alpha,beta).first;
    else return max_alpha(b,player_index,depth,alpha,beta).first;
}



pair<vector<Move>, int> max_alpha1(Board b,int player_index, int depth,int alpha,int beta);
pair<vector<Move>, int> min_alpha1(Board b,int player_index,int depth,int alpha,int beta){
    call_min_alpha ++;
    clock_t t1 = clock();
    // int utility = 100010101;
    vector<Move> movereturn;
    if(depth == 0){
        vector<Move> mv;
        Move m("S",0,0);
        mv.push_back(m);
        pair<vector<Move> , int> reee (mv,b.heuristic());
        return reee;
    }
    vector<vector<Move> > possible_moves = all_moves_aa(b,player_index);
    for(int i=0;i<possible_moves.size();i++){
        vector<Move> temp_move = possible_moves[i];
        Board temp_board = b.copy_board();
        temp_board.execute_move(possible_moves[i],player_index);
        pair<vector<Move>, int> child = max_alpha(temp_board,-1*player_index,depth-1,alpha,beta);
        if(beta > child.second){
            movereturn = possible_moves[i];
            beta = child.second;
        }
        pair<vector<Move>, int> util(movereturn,beta);
        if(alpha >= beta) return util;
    }
    pair<vector<Move>, int> util(movereturn,beta);
    return util;
}

pair<vector<Move>, int> max_alpha1(Board b,int player_index, int depth,int alpha,int beta){
    vector<Move> movereturn;
    if(depth == 0){
        vector<Move> mv;
        Move m("S",0,0);
        mv.push_back(m);
        pair<vector<Move>, int> reee (mv,b.heuristic());
        return reee;
    }
    
    vector<vector<Move> > successor = all_moves_aa(b,player_index);
    for(int i=0;i<successor.size();i++){
        vector<Move> temp_move = successor[i];
        Board temp_board = b.copy_board();
        temp_board.execute_move(successor[i],player_index);
        pair<vector<Move>, int> child = min_alpha(temp_board,-1*player_index,depth-1,alpha,beta);
        if(alpha < child.second){
            movereturn = successor[i];
            alpha = child.second;
        }
        pair<vector<Move>, int> util(movereturn, alpha);
        if(alpha >= beta)return util;
    }
    pair<vector<Move> ,int> util(movereturn, alpha);
    return util;
}

vector<Move> alpha_beta1(Board b,int player_index,int depth,int alpha, int beta){
    if(player_index == -1)return min_alpha(b,player_index,depth,alpha,beta).first;
    else return max_alpha(b,player_index,depth,alpha,beta).first;
}












// it gives all length of given value in a row
vector<int> Board::marker_coordinate_feature( int s1,int f1,int s2,int f2,int value){
    vector<int> coord;
    int x = s2-s1;
    int y = f2-f1;
    int diff = max(x,y);
    int check = 0;
    if(x != 0)x=1;
    if(y != 0)y=1;
    for(int i=0;i<=diff;i++){
        int xcoord = s1+x*i;
        int ycoord = f1+y*i;
        
        if(check == 0){
            if(get_position(xcoord,ycoord) == value){
                if(i==diff){
                    coord.push_back(s2);
                    coord.push_back(f2);
                    coord.push_back(s2);
                    coord.push_back(f2);
                    continue;
                }
            
                coord.push_back(xcoord);
                coord.push_back(ycoord);
                check = 1;
                continue;
            }
        }
        if(check == 1){
            if(get_position(xcoord,ycoord) != value){
                coord.push_back(xcoord-x);
                coord.push_back(ycoord-y);
                check = 0;
                continue;
            }
            if(i == diff){
                coord.push_back(s2);
                coord.push_back(f2);
            }
        }
    }
    if(coord.size()==0)coord.push_back(0);
    return coord;
}

bool is_gap(int s1,int f1,int s2,int f2){
    if(abs(f2-f1)==2)return true;
    if(abs(s2-s1)==2)return true;
}


// it gives the number of sets of 1,2,3 to board_size-1
vector<int> Board::marker_set(int s1,int f1, int s2, int f2, int value){
    vector<int> coord = marker_coordinate_feature(s1,f1,s2,f2,value);
    int n = markers_per_ring;
    vector<int> marker_fet(n,0);
    if(coord.size()==1)return marker_fet;
    for(int i=0;i<coord.size();i=i+4){
        int ydiff = coord[i+3] - coord[i+1];
        int xdiff = coord[i+2] - coord[i];
        int size = max(xdiff,ydiff)+1;
        if(size>=n-1)marker_fet[n-1] = marker_fet[n-1] + 1;
        else marker_fet[size] = marker_fet[size]+1;
    }
    return marker_fet;
}

vector<int> Board::total_marker_sets(int value){
    int size = board_size;
    // vector<vector<int> > endpts = end_pts(size);
    int n = markers_per_ring;
    vector<int> marker_feat(n,0);
    // for(int i=0;i<endpts.size();i++){
    //     vector<int> temp = marker_set(endpts[i][0],endpts[i][1],endpts[i][2],endpts[i][3],value);
    //     for(int j=1;j<n;j++)
    //         marker_feat[j] += temp[j];
    // }

    if(size == 5){
        for(int i=0;i<33;i++){
            vector<int> temp = marker_set(endpts5[i][0],endpts5[i][1],endpts5[i][2],endpts5[i][3],value);
            for(int j=1;j<n;j++)
                marker_feat[j] += temp[j];
        }
    }

    else if(size == 6){
        for(int i=0;i<39;i++){
            vector<int> temp = marker_set(endpts6[i][0],endpts6[i][1],endpts6[i][2],endpts6[i][3],value);
            for(int j=1;j<n;j++)
                marker_feat[j] += temp[j];
        }
    }

    return marker_feat;
}

vector<int> Board::total_marker_sets_h(int value){
    int size = board_size;
    vector<vector<int> > endpts = end_pts(size);
    int n = markers_per_ring;
    vector<int> marker_feat(n,0);
    for(int i=0;i<endpts.size();i++){
        vector<int> temp = marker_set(endpts[i][0],endpts[i][1],endpts[i][2],endpts[i][3],value);
        for(int j=1;j<n;j++)
            marker_feat[j] += temp[j];
    }
    return marker_feat;
}

void Board::print_board(){

    cout << "   " << "     " << " " << "     " << " " << " " << endl;
    cout << " " << "     " << " " << "     " << print_position(-1,4) << "     " << print_position(1,5) << endl;
    cout << "   " << "     " << " " << print_position(-2,3) << "     " << print_position(0,4) << "     " << print_position(2,5) << endl;
    cout << " " << "     " << print_position(-3,2) << "     " << print_position(-1,3) << "     " << print_position(1,4) << "     " << print_position(3,5) << endl;
    cout << "   " << print_position(-4,1) << "     " << print_position(-2,2) << "     " << print_position(0,3) << "     " << print_position(2,4) << "     " << print_position(4,5) << endl;
    cout << " " << "     " << print_position(-3,1) << "     " << print_position(-1,2) << "     " << print_position(1,3) << "     " << print_position(3,4) << "     " << " " << endl;
    cout << "   " << print_position(-4,0) << "     " << print_position(-2,1) << "     " << print_position(0,2) << "     " << print_position(2,3) << "     " << print_position(4,4) << endl;
    cout << print_position(-5,-1) << "     " << print_position(-3,0) << "     " << print_position(-1,1) << "     " << print_position(1,2) << "     " << print_position(3,3) << "     " << print_position(5,4) << endl;
    cout << "   " << print_position(-4,-1) << "     " << print_position(-2,0) << "     " << print_position(0,1) << "     " << print_position(2,2) << "     " << print_position(4,3) << endl;
    cout << print_position(-5,-2) << "     " << print_position(-3,-1) << "     " << print_position(-1,0) << "     " << print_position(1,1) << "     " << print_position(3,2) << "     " << print_position(5,3) << endl;
    cout << "   " << print_position(-4,-2) << "     " << print_position(-2,-1) << "     " << print_position(0,0) << "     " << print_position(2,1) << "     " << print_position(4,2) << endl;
    cout << print_position(-5,-3) << "     " << print_position(-3,-2) << "     " << print_position(-1,-1) << "     " << print_position(1,0) << "     " << print_position(3,1) << "     " << print_position(5,2) << endl;
    cout << "   " << print_position(-4,-3) << "     " << print_position(-2,-2) << "     " << print_position(0,-1) << "     " << print_position(2,0) << "     " << print_position(4,1) << endl;
    cout << print_position(-5,-4) << "     " << print_position(-3,-3) << "     " << print_position(-1,-2) << "     " << print_position(1,-1) << "     " << print_position(3,0) << "     " << print_position(5,1) << endl;
    cout << "   " << print_position(-4,-4) << "     " << print_position(-2,-3) << "     " << print_position(0,-2) << "     " << print_position(2,-1) << "     " << print_position(4,0) << endl;
    cout << " " << "     " << print_position(-3,-4) << "     " << print_position(-1,-3) << "     " << print_position(1,-2) << "     " << print_position(3,-1) << "     " << " " << endl;
    cout << "   " << print_position(-4,-5) << "     " << print_position(-2,-4) << "     " << print_position(0,-3) << "     " << print_position(2,-2) << "     " << print_position(4,-1) << endl;
    cout << " " << "     " << print_position(-3,-5) << "     " << print_position(-1,-4) << "     " << print_position(1,-3) << "     " << print_position(3,-2) << endl;
    cout << "   " << " " << "     " << print_position(-2,-5) << "     " << print_position(0,-4) <<  "     " << print_position(2,-3) <<endl;
    cout << " " << "     " << " " << "     " << print_position(-1,-5) << "     " << print_position(1,-4) << endl;
    cout << "   " << "     " << " " << "     " << " " << " " << endl;

    cout << '\n';
    cout << "player 1 rings: " << ring1 << '\n' << "player 2 rings: " << ring2 << '\n';
    cout << "player 1 markers: " << marker1 << '\n' << "player 2 markers: " << marker2 << '\n';
    cout << "player 1 rings removed: " << ring1_removed << '\n' << "player 2 rings removed: " << ring2_removed << '\n' << '\n';
}




int eval(vector<int> feat){
    int n = feat.size();
    int score;
    if(n==5){
        score = feat[1]*3+feat[2]*80+feat[3]*850+feat[4]*10000;
    }
    if(n==6){
        score = feat[1]*3+feat[2]*85+feat[3]*850+feat[4]*10000+feat[5]*85000;
    }
    return score;
}

// include line number 24 for this function
// this function gives the number of moves of given player
int Board::num_moves(int player_index){
    vector<vector<int> > player_rings;
    if(player_index == -1)player_rings = player1_rings;
    else player_rings = player2_rings;
    int moves_num=0;
    for(int i=0;i<player_rings.size();i++){
        int prx = player_rings[i][0];
        int pry = player_rings[i][1];
        for(int j=0;j<6;j++){
            // int count=0;
            bool check = true;
            int k=1;
            while(true){
                int a = prx+k*all_directions[2*j];
                int b = pry+k*all_directions[2*j+1];
                if(check_valid_position(a,b) == false)break;
                if(get_position(a,b) == -2 || get_position(a,b) == 2)break;
                if(get_position(a,b) == -1 || get_position(a,b) == 1){
                    check = false;
                    k++;
                    continue;
                }
                moves_num++;
                if(check == false)break;
                k++;
            }
            // moves_num += count;
        }
    }
    return moves_num;
}

// heuristic is like positive value when player2 has higher value
int Board::heuristic(){
    // int score = 1000*(ring2_removed - ring1_removed);
    // score += 10*(marker2 - marker1);

    vector<int> feature1 = total_marker_sets(-1);
    vector<int> feature2 = total_marker_sets(1);
    int score = eval(feature2) - eval(feature1);
    score += 800000*(ring2_removed - ring1_removed);
    score += 10*(num_moves(1) - num_moves(-1));
    
    return score;
}



// possible features
// a). 2 in a row
// b). 3 in a row
// c). 4 in a row
// d). 5 in a row
// e). 4 in a row and ring next to it
// f). 5 in a row including ring
// g). In 5 consecutive 4 are of my markers and 1 is ring
// h). reduce number of possible moves for opponent

int main(){
    int player_number;
    int n;  // n is initial number of rings and size of board
    int game_time;
    int k;
    
    cin >> player_number >> n >> game_time >> k;
    if(player_number == 1)player_number = -1;
    else {player_number = 1;}
    cin.ignore();
    Board my_board = Board(n,n,k);


    string opponent_move;
    // opponent is player1
    if(player_number == 1){
        getline(cin, opponent_move);
        vector<Move> opp_move = movlist(opponent_move);
        my_board.execute_move(opp_move,-1);
    }


    for(int i=0;i<n;i++){
        srand(time(0));
        // int x = rand()%(n+1) - n;
        // int y = rand()%(n+1) - n;
        int hex=rand()%4;
        int ind=rand()%4;
        int mul = rand()%4;
        while(mul*ind >= 6*hex){
            hex=rand()%3;
            ind=rand()%3;
            mul = rand()%3;
        }

        vector<int> coord=map_hex_mysys(hex,ind*mul);
        // while(my_board.check_valid_position(x,y)==false || my_board.get_position(x,y)!=0){
        //     x = rand()%(n+1) - n;
        //     y = rand()%(n+1) - n;
        // }
        while(my_board.check_valid_position(coord[0],coord[1]) ==false || my_board.get_position(coord[0],coord[1]) !=0){
            hex = rand()%3;
            ind = rand()%3;
            mul = rand()%3;
            // coord = map_hex_mysys(hex,ind*)
            while(mul*ind >= 6*hex){
                hex=rand()%3;
                ind=rand()%3;
                mul = rand()%3;
            }
            coord = map_hex_mysys(hex,ind*mul);
        }
        // Move m("P",x,y);
        Move m("P",coord[0],coord[1]);
        vector<Move> next_move;
        next_move.push_back(m);
        my_board.execute_move(next_move,player_number);
        cout << mov_string(next_move) << endl;

        
        getline(cin,opponent_move);
        vector<Move> opp_move = movlist(opponent_move);
        my_board.execute_move(opp_move,-1*player_number);
    }

    int x = 0;
    if(n == 6){
        // int x=0;
        // while(x<10){
        //     vector<Move> next_move = alpha_beta(my_board,player_number,2,-10000000,10000000);
        //     // vector<Move> next_move = minimax(my_board,player_number,3,);
        //     my_board.execute_move(next_move,player_number);
        //     cout << mov_string(next_move) << endl;

        //     getline(cin,opponent_move);
        //     vector<Move> opp_move = movlist(opponent_move);
        //     my_board.execute_move(opp_move,-1*player_number);
        //     x++;
        // }

        while(true){
            vector<Move> next_move = alpha_beta(my_board,player_number,2,-10000000,10000000);
            // vector<Move> next_move = minimax(my_board,player_number,3,);
            my_board.execute_move(next_move,player_number);
            cout << mov_string(next_move) << endl;

            getline(cin,opponent_move);
            vector<Move> opp_move = movlist(opponent_move);
            my_board.execute_move(opp_move,-1*player_number);
        } 

        
    }

    if(n==5){
        while(true){
            vector<Move> next_move = alpha_beta(my_board,player_number,3,-10000000,10000000);
            // vector<Move> next_move = minimax(my_board,player_number,3,);
            my_board.execute_move(next_move,player_number);
            cout << mov_string(next_move) << endl;

            getline(cin,opponent_move);
            vector<Move> opp_move = movlist(opponent_move);
            my_board.execute_move(opp_move,-1*player_number);
        }
    
    }
    return 0;
}