#include<iostream>
#include<vector>
#include<string>
using namespace std;

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
        vector<vector<int> > board_storage;

        Board(int board_size, int m, int r){
            vector<int> element;
            for(int i = -5; i <= 5; i++){
                for(int j = -5; j<= 5; j++){
                    element.push_back(0);
                }
                board_storage.push_back(element);
                element.clear();
            }

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

        vector<int> map_hex_mysys(int hexagon, int index); 

        vector<int> map_mysys_hex(int abscissa, int ordinate);

        bool check_valid(Move mv, int player_index);

        void execute_move(vector<Move>, int player_index);

        void print_board();
        int print_position(int x, int y);
};

int Board::get_position(int x, int y){
    return board_storage[x+5][y+5];
}

int Board::print_position(int x, int y){
    int value = get_position(x,y);
    if(value == -1){ return 1;}
    else if(value == -2){ return 4;}
    else if(value == 1){ return 1;}
    else if(value == 2){ return 3;}
    else if(value == 0){ return 0;}
}

void Board::set_position(int x, int y, int value){

    int init_value = get_position(x, y);

    board_storage[x+5][y+5] = value;

////////////////////////////// picked up from aniket
    if(init_value == 0 && value == 2)ring2++;
    else if(init_value == 0 && value == -2)ring1++;
    else if(init_value == 2 && value == 1)marker2++;
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
        if(m1.move_type == "P"){
            set_position(m1.x, m1.y, 2*player_index);
            if(player_index == -1){ ring1++; }
            else{ ring2++; }
        }
        
        else if(m1.move_type == "S"){
            Move m2 = movelist[++k];
            if(m2.move_type != "M")throw "invalid move";
            else {
                set_position(m1.x, m1.y, player_index);
                set_position(m2.x, m2.y, 2*player_index);
                if(m1.y == m2.y){
                    if(m2.x > m1.x){
                        for(int i = m1.x+1; i < m2.x; i++)
                            set_position(i, m1.y, -1* get_position(i, m1.y));
                        set_position(m2.x, m2.y, 2*player_index);
                    }
                    else{
                        for(int i = m1.x+1; i > m2.x; i--)
                            set_position(i, m1.y, -1*get_position(i, m1.y));
                        //set_position(m2.x, m2.y, 2*player_index);
                    }                
                }

                else if(m1.x == m2.x){
                    if(m2.y > m1.y){
                        for(int i = m1.y+1; i < m2.y; i++)
                            set_position(m1.x, i, -1*get_position(m1.x, i));
                        //set_position(m2.x, m2.y, 2*player_index);
                    }
                    else{
                        for(int i = m1.y+1; i > m2.y; i--)
                            set_position(m1.x, i, -1*get_position(m1.x, i));
                        //set_position(m2.x, m2.y, 2*player_index);
                    }
                }

                else if(m2.y-m1.y == m2.x-m1.x){
                    if(m2.x > m1.x){
                        for(int i = 1; i < m2.x-m1.x; i++)
                            set_position(m1.x+i, m1.y+i, -1*get_position(m1.x+i, m1.y+i));
                        set_position(m2.x, m2.y, 2*player_index);                        
                    }
                    else{
                        for(int i = 1; i < m1.x-m2.x; i--)
                            set_position(m1.x-i, m2.y-i, -1*get_position(m1.x-i,m1.y-i));
                        set_position(m2.x, m2.y, 2*player_index);
                    }
                }

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
                    //set_position(m2.x, m2.y, 0);
                }
                else{
                    for(int i = 0; i < markers_removed; i++)
                        set_position(m1.x-i, m1.y, 0);
                    //set_position(m2.x, m2.y, 0);
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

        }
        
    }
}





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

vector<Move> get_move(string ply){
    vector<string> ply_vector = split_string(ply, ' ');
    vector<Move> my_move;

    for(int i = 0; i < ply_vector.size()/3; i++){
        Move m = Move(ply_vector[3*i], stoi(ply_vector[3*i + 1]), stoi(ply_vector[3*i +2]));
        my_move.push_back(m);
    }
    return(my_move);
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
    cout << "marker1: " << marker1 << '\n' << "marker2: " << marker2 << '\n';
}


int main(){
    Board my_board = Board(5,5,3);
    my_board.print_board();
    
    int player_number = -1;
    for(int i = 0; i < 100; i++){
        string s;
        getline(cin, s);
        //vector<string> movvlist = splitstr(s);
        vector<Move> movvllist = get_move(s);
        //cout << movvlist.size();
        my_board.execute_move(movvllist, player_number);
        //int x = myBoard.getpositionValue(1,0);
        cout << 1 ;
        //cout << movvlist.size();
        
        my_board.print_board();
        player_number *= -1;
    }

    int a = 3;
    int b = 5;
    cout<<a<<a++<<a;
    cout<<b<<++b<<b;

    cout<<endl;
    return(1);
}
