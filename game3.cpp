#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<utility>
#include<math.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


// Things to do - find successor when the opponent has made consecutive 5 markers of yours

// changes in the code to generalize for board size and initial rings
//map_hex_mysys and map_mysys_hex are same
// see board initialization, rings_removed is renamed as rings_init
// slight change in get_position and set_position. 5 is replaced by board_size.Compare it
// startend, cons_marker and moves_row are updated. 1 helper function endpts is used. Update these functions


using namespace std;
static const std::vector<int> all_directions {1,0,-1,0,0,1,0,-1,-1,-1,1,1};

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

vector <string> splitstr(string str){
    vector<string> str_vec;
    typedef vector<string> Tokens;
    Tokens tokens;
    boost::split( tokens, str, boost::is_any_of(" ") );
    BOOST_FOREACH( const string& i, tokens){
        str_vec.push_back(i);
    }
    return str_vec; 
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

vector<Move> movlist(string ply){
    vector<Move> ret;
    vector<string> movstr = splitstr(ply);
    for(int i = 0; i < movstr.size(); i = i+3){
        int hex = stoi(movstr[i+1]);
        int ind = stoi(movstr[i+2]);
        vector<int> coord = map_hex_mysys(hex,ind);
        Move m1(movstr[i], coord[0], coord[1]);
        ret.push_back(m1);
    }
    return ret;
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
        int score;
        int ring1;
        int ring2;
        int marker1;
        int marker2;
        int markers_removed;  // number of markers, can be removed in one step
        int rings_init;  // no of starting rings
        int ring1_removed;  // rings removed of player1 till now
        int ring2_removed;  // rings removed of player2 till now
        int board_size;

    public:

        vector<vector<int> > board_storage;

        vector<vector<int> > player1_rings;
        vector<vector<int> > player2_rings;
        

        // Board(){
        //     vector<int> element;
        //     for(int i = -5; i <= 5; i++)
        //         element.push_back(0);
        //     for(int i = -5; i <= 5; i++)
        //         board_storage.push_back(element);

        //     element.clear();

        //     board_size = 5;
        //     markers_removed = 5;
        //     rings_init = 5;
        //     ring1 = 0;
        //     ring2 = 0;
        //     marker1 = 0;
        //     marker2 = 0;
        //     ring1_removed = 0;
        //     ring2_removed = 0;
        // }

        Board(int n, int m, int k){     // n,m and k are board_size , initial rings and no of markers to be removed respectively
            vector<int> element;
            for(int i = -n; i <= n; i++)
                element.push_back(0);
            for(int i = -n; i <= n; i++)
                board_storage.push_back(element);
            
            element.clear();

            board_size = n;
            markers_removed = k;
            rings_init = m;
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


        void print_board();
        string print_position(int x, int y);

        Board copy_board();

        vector<int> startend(int s1, int f1, int s2, int f2, int value); // returns coordinates of all the consecutive 5 or more length of same given value  
        // input as starting and ending coordinates
        vector<int> moves_row(vector<int> ls);  // returns possible  5 consecutive length in the same direction. Helper function for cons_marker 
        vector<int> cons_marker(int value);     // it returns all possible 5 consecutive elements of same given value.
        //Element as (x1,y1) and (x2,y2) is stored in vector as [x1,y1,x2,y2]  

        vector<int> cons_marker_old(int value);

        Board adjacent_move(int x, int y, Direction d);

        int row_marker(int x1, int y1, int x2, int y2, int value);

        int board_marker(int value);

        int heuristic();

        string server_output(pair<Board, vector<Move> > server_output_move);

        pair<vector<Move>, int> min_tree(Board b, int player_index);
        pair<vector<Move>, int> max_tree(Board b, int player_index);
};

int Board::get_ring1_removed(){
    return ring1_removed;
}
int Board::get_ring2_removed(){
    return ring2_removed;
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
    vector<int> ls;
    int k = markers_removed;
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
    return ls;
}

vector<int> Board::moves_row(vector<int> ls){
    vector<int> vec;
    int k = markers_removed;
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
    return vec;
}

vector<vector<int> > end_pts(int n){
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
    vector<vector<int> > border_pts = end_pts(board_size);
    vector<int> consmark;
    for(int i=0;i<border_pts.size();i++){
        vector<int> vec = moves_row(startend(border_pts[i][0],border_pts[i][1],border_pts[i][2],border_pts[i][3],value));
        if(vec.size() != 1){
            for(int j=0;j<vec.size(); j++)
                consmark.push_back(vec[j]);
        }
    }
    if(consmark.size() == 0)consmark.push_back(0);
    return consmark;
}

void Board::execute_move(vector<Move> movelist, int player_index){
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
                    for(int i = 0; i < markers_removed; i++)
                        set_position(m1.x+i, m1.y, 0);
                }
                else{
                    for(int i = 0; i < markers_removed; i++)
                        set_position(m1.x-i, m1.y, 0);
                }                
            }
            else if(m1.x == m2.x){
                if(m2.y > m1.y){
                    for(int i = 0; i < markers_removed; i++)
                        set_position(m1.x, m1.y+i, 0);    
                }
                else{
                    for(int i = 0; i < markers_removed; i++){
                         set_position(m1.x, m1.y-i, 0);
                    }
                }
            }
            else if(m2.y-m1.y == m2.x-m1.x){
                if(m2.x > m1.x){
                    for(int i = 0; i < markers_removed; i++)
                        set_position(m1.x+i, m1.y+i, 0);
                }
                else{
                    for(int i = 0; i < markers_removed; i++)
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
}




pair<vector<Move>, int> Board::min_tree(Board b,int player_index){
    // // code for terminal state
    // int utility = 1001001010010;
    // if(player_index == -1){
    //     for(int i=0;i<player1_rings.size();i++){
    //         for(int j=0;j<6;j++){
    //             int k = 1;
    //             int count = 0;
    //             while(true){
    //                 if(check_valid_position(player1_rings[i][0]+k*all_directions[2*j],player1_rings[i][1]+k*all_directions[2*j+1])==false)
    //                     break;
    //                 if(get_position(player1_rings[i][0]+k*all_directions[2*j],player1_rings[i][1]+k*all_directions[2*j+1]) == -2 ,2)break;
    //                 if(get_position(player1_rings[i][0]+k*all_directions[2*j],player1_rings[i][1]+k*all_directions[2*j+1]) == -1 ,1){
    //                     count = 1;
    //                     continue;
    //                 }
    //                 Board temp_board(b);
    //                 vector<Move> temp_move;
    //                 Move m1("S", player1_rings[i][0], player1_rings[i][1]);
    //                 Move m2("M", player1_rings[i][0]+k*all_directions[2*j], player1_rings[i][1]+k*all_directions[2*j+1]);
    //                 temp_move.push_back(m1);
    //                 temp_move.push_back(m2);
    //                 temp_board.execute_move(temp_move,player_index);
    //                 vector<int> all_consecutive = temp_board.cons_marker(player_index);
    //                 if(all_consecutive.size() == 1)
    //                     utility = min(utility,max_tree(temp_board,-1*player_index).second);
    //                 else{
    //                     for(int x=0;x<all_consecutive.size()/4;x++){
    //                         Move temp_m1("RS",all_consecutive[4*x], all_consecutive[4*x+1]);
    //                         Move temp_m2("RE",all_consecutive[4*x+2],all_consecutive[4*x+3]);
    //                         for(int r=0;r<player1_rings.size();r++){
    //                             Move temp_m3("X",player1_rings[r][0], player1_rings[r][1]);
    //                             vector<Move> temp_temp_move;
    //                             temp_temp_move.push_back(temp_m1);
    //                             temp_temp_move.push_back(temp_m2);
    //                             temp_temp_move.push_back(temp_m3);
    //                             Board temp_temp_board(temp_board);
    //                             temp_temp_board.execute_move(temp_temp_move,player_index);
    //                             utility = min(utility, max_tree(temp_temp_board, -1*player_index).second);
    //                         }
    //                     }
    //                 }
                
    //             }
    //         }
    //     }
    
    // }
}


// extract all possible moves of the given player
vector<vector<Move> > all_moves(Board bord,int player_index){
    vector<vector<Move> > possible_moves;
    vector<vector<int> > player_rings;
    Board b = Board(bord);
    vector<int> consecutive_init = b.cons_marker(player_index);
    vector<Move> init_move;
    if(consecutive_init.size() != 1){
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

    // cout << b.player1_rings.size() << "anddfjid";
    for(int i=0;i<player_rings.size();i++){
        int prx = player_rings[i][0];
        int pry = player_rings[i][1];
        // cout << "assds";
        // cout << prx << " " << pry << endl;
        for(int j=0;j<6;j++){
            
            int k=1;
            int count=0;
            while(true){
                
                // cout << "values of k   " << k << "\n";

                if(b.check_valid_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1])==false)break;

                // cout << b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) << "  fksfjk \n";

                if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == -2 )break;
                if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 2 )break;

                if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == -1){
                    count = 1;
                    k++;
                    // cout << "asasasasa" << endl;
                    continue;
                }

                if(b.get_position(prx+k*all_directions[2*j],pry+k*all_directions[2*j+1]) == 1){
                    count = 1;
                    k++;
                    continue;
                }
                // cout << "wahtthefuk" << endl;
                Board temp_board = Board(b);
                vector<Move> temp_move = init_move;
                // vector<Move> temp_move;
                Move m1("S", prx, pry);
                Move m2("M", prx+k*all_directions[2*j], pry+k*all_directions[2*j+1]);
                temp_move.push_back(m1);
                temp_move.push_back(m2);
                temp_board.execute_move(temp_move,player_index);

                k++;

                vector<int> all_consecutive = temp_board.cons_marker(player_index);
                if(all_consecutive.size() == 1){
                    possible_moves.push_back(temp_move);
                }
                else{
                    // cout << "aniefnkdskds   " << k << "a   " << all_consecutive.size() << "   aniefdfj    ";
                    for(int x=0;x<all_consecutive.size()/4;x++){
                        Move temp_m1("RS", all_consecutive[4*x], all_consecutive[4*x+1]);
                        Move temp_m2("RE", all_consecutive[4*x+2], all_consecutive[4*x+3]);
                        vector<vector<int> > play_rings;
                        if(player_index==-1)play_rings = temp_board.player1_rings;
                        else play_rings = temp_board.player2_rings;
                        for(int r=0;r<play_rings.size();r++){
                            Move temp_m3("X", play_rings[r][0], play_rings[r][1]);
                            vector<Move> temp_temp_move;
                            temp_temp_move.push_back(m1);
                            temp_temp_move.push_back(m2);
                            temp_temp_move.push_back(temp_m1);
                            temp_temp_move.push_back(temp_m2);
                            temp_temp_move.push_back(temp_m3);
                            possible_moves.push_back(temp_temp_move);
                        }
                    }
                }
                if(count==1)break;    
            }
        }
    }
    return possible_moves;
}

pair<vector<Move>, int> maxxx_tree(Board b,int player_index, int depth);

pair<vector<Move>, int> minnn_tree(Board b,int player_index,int depth){
    int utility = 100010101;
    vector<Move> movereturn;
    if(depth == 0){
        vector<Move> mv;
        Move m("S",0,0);
        mv.push_back(m);
        pair<vector<Move> , int> reee (mv,b.heuristic());
        return reee;
    }
    vector<vector<Move> > possible_moves = all_moves(b,player_index);
    for(int i=0;i<possible_moves.size();i++){
        Board temp_board = Board(b);
        temp_board.execute_move(possible_moves[i],player_index);
        if(utility > maxxx_tree(temp_board,-1*player_index,depth-1).second){
            movereturn = possible_moves[i];
            utility = maxxx_tree(temp_board,-1*player_index,depth-1).second;
        }
    }
    pair<vector<Move>, int> util(movereturn,utility);
    return util;
}

pair<vector<Move>, int> maxxx_tree(Board b,int player_index, int depth){
    int utility = -1000000;
    vector<Move> movereturn;
    if(depth == 0){
        vector<Move> mv;
        Move m("S",0,0);
        mv.push_back(m);
        pair<vector<Move>, int> reee (mv,b.heuristic());
        return reee;
    }
    vector<vector<Move> > possible_moves = all_moves(b,player_index);
    for(int i=0;i<possible_moves.size();i++){
        Board temp_board = Board(b);
        temp_board.execute_move(possible_moves[i],player_index);
        if(utility < minnn_tree(temp_board,-1*player_index,depth-1).second){
            movereturn = possible_moves[i];
            utility = minnn_tree(temp_board,-1*player_index,depth-1).second;
        }
    }
    pair<vector<Move> ,int> util(movereturn, utility);
    return util;

}

vector<Move> minimax(Board b,int player_index,int depth){
    if(player_index == -1)return minnn_tree(b,player_index,depth).first;
    else return maxxx_tree(b,player_index,depth).first;
}




// heuristic is like positive value when player2 has higher value
int Board::heuristic(){
    // int score = board_marker(1) - board_marker(-1);
    int score = 1000*(ring2_removed - ring1_removed);
    score += 10*(marker2 - marker1);
    return score;
}
//////////////////////////////////

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

// int main1(){
//     ifstream infile("thefile.txt");
//     string line;
//     Board myBoard = Board(5,5,3);
//     int player_number = -1;
//     while (getline(infile, line))
//     {
//         vector<Move> movvllist = movlist(line);
//         myBoard.execute_move(movvllist, player_number);    
        
//         player_number *= -1;
//     }
//     myBoard.print_board();
//     cout << player_number << endl;
    
//     vector<int> consmark = myBoard.cons_marker(player_number);
    
//     for(int i=0;i<consmark.size();i++)
//         cout << consmark[i] << " ";

//     cout << all_directions.size() << endl;
//     // for(int i=0;i<all_directions.size();i++)
//     //     cout << all_directions[i] << "  ";

//     vector<vector<Move> > successor = all_moves(myBoard,player_number);
    
    
//     for(int i=0;i<successor.size();i++){
//         for(int j=0;j<successor[i].size();j++){
//             cout << successor[i][j].move_type << " " << successor[i][j].x << " " << successor[i][j].y << " ";
//         }
//         cout << endl;
//     }
//     cout  << "Player index is " << player_number << "  " <<  successor.size() << " size successor\n";
//     return 0;
// }


int main(){
    int player_number;
    int n;  // n is initial number of rings and size of board
    int game_time;
    int k;
    

    

    // string game_data;
    // getline(cin, game_data);

    // vector<string> game_data_vector;

    // stringstream data(game_data);
    // string line;

    // while(getline(data,line,' ')){
    //     game_data_vector.push_back(line);
    // }

    // int player_number;

    // if(stoi(game_data_vector[0]) == 1){ player_number = -1;}
    // else{player_number = 1;}
    
    // int number_of_rings = stoi(game_data_vector[1]);
    // int game_time = stoi(game_data_vector[2]);

    // Board my_board = Board(5,number_of_rings,3);






    cin >> player_number >> n >> game_time >> k;
    if(player_number == 1)player_number = -1;
    else {player_number = 1;}
    cin.ignore();
    Board my_board = Board(n,n,k);

    // player_number = 1;
    // Board my_board = Board(5,3,3);
    // Move ma("P",2,2);
    // vector<Move> mlist;
    // mlist.push_back(ma);
    // cout << mov_string(mlist) << endl;

    // string opp = "P 4 8";
    // vector<Move> opp_move = movlist(opp);
    // my_board.execute_move(opp_move,-1);
    // my_board.print_board();
    // return 0;


    

    string opponent_move;
    // opponent is player1
    if(player_number == 1){
        getline(cin, opponent_move);
        vector<Move> opp_move = movlist(opponent_move);
        my_board.execute_move(opp_move,-1);
    }


    for(int i=0;i<n;i++){
        int x = rand()%(n+1) - n;
        int y = rand()%(n+1) - n;
        
        while(my_board.check_valid_position(x,y)==false || my_board.get_position(x,y)!=0){
            x = rand()%(n+1) - n;
            y = rand()%(n+1) - n;
        }
        Move m("P",x,y);
        vector<Move> next_move;
        next_move.push_back(m);
        my_board.execute_move(next_move,player_number);
        cout << mov_string(next_move) << endl;

        
        getline(cin,opponent_move);
        vector<Move> opp_move = movlist(opponent_move);
        my_board.execute_move(opp_move,-1*player_number);
    }
    
    // cout << "11";
    int x = 0;
    while(x<10){
        vector<Move> next_move = minimax(my_board,player_number,2);
        my_board.execute_move(next_move,player_number);
        cout << mov_string(next_move) << endl;

        getline(cin,opponent_move);
        vector<Move> opp_move = movlist(opponent_move);
        my_board.execute_move(opp_move,-1*player_number);
        x++;
    }
    while(true){
        vector<Move> next_move = minimax(my_board,player_number,3);
        my_board.execute_move(next_move,player_number);
        cout << mov_string(next_move) << endl;

        getline(cin,opponent_move);
        vector<Move> opp_move = movlist(opponent_move);
        my_board.execute_move(opp_move,-1*player_number);

    }
    return 0;
}