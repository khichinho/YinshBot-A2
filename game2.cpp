#include<iostream>
#include<vector>
#include<string>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

vector<string> split_string(string str, char dl)
{
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


class Board{

    private:
        int score;
        int ring1;
        int ring2;
        int marker1;
        int marker2;
        int markers_removed;    // number of markers, can be removed in one step
        int rings_removed;  // total rings to be removed
        int ring1_removed;  // rings removed of player1 till now
        int ring2_removed;  // rings removed of player2 till now

        
    

    public:

        vector<vector<int> > player1_rings;
        vector<vector<int> > player2_rings;

        vector<vector<int> > board_storage;

        Board(int board_size, int m, int r){
            vector<int> element;
            for(int i = -board_size; i <= board_size; i++)
                element.push_back(0);
            for(int i = -board_size; i <= board_size; i++)
                board_storage.push_back(element);
            
            element.clear();

            rings_removed = r;
            markers_removed = m;
            ring1 = 0;
            ring2 = 0;
            marker1 = 0;
            marker2 = 0;
            ring1_removed = 0;
            ring2_removed = 0;
        }

        int get_position(int x, int y);
        void set_position(int x, int y, int value);

        vector<Move> get_move(string ply);

        vector<int> map_hex_mysys(int hexagon, int index); 
        vector<int> map_mysys_hex(int abscissa, int ordinate);

        bool check_valid(Move mv, int player_index);
        void execute_move(vector<Move>, int player_index);

        void print_board();
        string print_position(int x, int y);
};


vector<Move> Board::get_move(string ply){
    vector<string> ply_vector = split_string(ply, ' ');
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

string Board::print_position(int x, int y){
    int value = get_position(x,y);
    if(value == -1){ return "a";}
    else if(value == -2){ return "A";}
    else if(value == 1){ return "c";}
    else if(value == 2){ return "C";}
    else if(value == 0){ return ".";}
}

int Board::get_position(int x, int y){
    return board_storage[x+5][y+5];
}

void Board::set_position(int x, int y, int value){

    int init_value = get_position(x, y);

    board_storage[x+5][y+5] = value;

////////////////////////////// picked up from aniket
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
/////////////////////////////
}

vector<int> Board::map_hex_mysys(int hexagon, int index){
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

vector<int> Board::map_mysys_hex(int abscissa, int ordinate){
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


            //}
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

    cout << '\n' << '\n';
    cout << "ring1: " << ring1 << '\n' << "ring2: " << ring2 << '\n';
    cout << "marker1: " << marker1 << '\n' << "marker2: " << marker2 << '\n' << '\n';
}


int main(){
    ifstream infile("thefile.txt");
    string line;
    Board my_board = Board(5,5,3);
    int player_number = -1;
    my_board.print_board();
    
    while (getline(infile, line))
    {
        if(player_number == -1){ cout<<"Player 1 moves : ";}
        else{cout<<"Player 2 moves : ";}
        vector<Move> movvllist = my_board.get_move(line);
        my_board.execute_move(movvllist, player_number);
        my_board.print_board();
        player_number *= -1;
    }
    for(int i = 0;i < my_board.player1_rings.size();i++)
        cout << my_board.player1_rings[i][0] << "  " << my_board.player1_rings[i][1] << '\n';

    for(int i = 0;i < my_board.player2_rings.size();i++)
        cout << my_board.player2_rings[i][0] << "  " << my_board.player2_rings[i][1] << '\n';
    // for(int i = 0; i < 100; i++){
    //      if(player_number == -1){ cout<<"Player 1 moves : ";}
    //      else{cout<<"Player 2 moves : ";}
    //      string s;
    //      getline(cin, s);
    
    //      vector<Move> move_list = my_board.get_move(s);
    
    //      my_board.execute_move(move_list, player_number);
    //      my_board.print_board();
    //      player_number *= -1;
    //  }

    return(1);
}
