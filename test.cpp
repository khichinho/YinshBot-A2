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

    public:
        vector<vector<int> > board_storage;

        Board(int board_size){
            vector<int> element;
            for(int i = -5; i <= 5; i++){
                for(int j = -5; j<= 5; j++){
                    element.push_back(0);
                }
                board_storage.push_back(element);
                element.clear();
            }
        }

        int get_position(int x, int y);

        vector<int> map_hex_mysys(int hexagon, int index); 

        vector<int> map_mysys_hex(int abscissa, int ordinate);

        bool check_valid(Move mv, int player_index);

        void execute_move(vector<Move>);
};

int Board::get_position(int x, int y){
    return board_storage[x+5][y+5];
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

void print_board(Board b){

    cout << "   " << "     " << " " << "     " << " " << " " << endl;
    cout << " " << "     " << " " << "     " << b.get_position(-1,4) << "     " << b.get_position(1,5) << endl;
    cout << "   " << "     " << " " << b.get_position(-2,3) << "     " << b.get_position(0,4) << "     " << b.get_position(2,5) << endl;
    cout << " " << "     " << b.get_position(-3,2) << "     " << b.get_position(-1,3) << "     " << b.get_position(1,4) << "     " << b.get_position(3,5) << endl;
    cout << "   " << b.get_position(-4,1) << "     " << b.get_position(-2,2) << "     " << b.get_position(0,3) << "     " << b.get_position(2,4) << "     " << b.get_position(4,5) << endl;
    cout << " " << "     " << b.get_position(-3,1) << "     " << b.get_position(-1,2) << "     " << b.get_position(1,3) << "     " << b.get_position(3,4) << "     " << " " << endl;
    cout << "   " << b.get_position(-4,0) << "     " << b.get_position(-2,1) << "     " << b.get_position(0,2) << "     " << b.get_position(2,3) << "     " << b.get_position(4,4) << endl;
    cout << b.get_position(-5,-1) << "     " << b.get_position(-3,0) << "     " << b.get_position(-1,1) << "     " << b.get_position(1,2) << "     " << b.get_position(3,3) << "     " << b.get_position(5,4) << endl;
    cout << "   " << b.get_position(-4,-1) << "     " << b.get_position(-2,0) << "     " << b.get_position(0,1) << "     " << b.get_position(2,2) << "     " << b.get_position(4,3) << endl;
    cout << b.get_position(-5,-2) << "     " << b.get_position(-3,-1) << "     " << b.get_position(-1,0) << "     " << b.get_position(1,1) << "     " << b.get_position(3,2) << "     " << b.get_position(5,3) << endl;
    cout << "   " << b.get_position(-4,-2) << "     " << b.get_position(-2,-1) << "     " << b.get_position(0,0) << "     " << b.get_position(2,1) << "     " << b.get_position(4,2) << endl;
    cout << b.get_position(-5,-3) << "     " << b.get_position(-3,-2) << "     " << b.get_position(-1,-1) << "     " << b.get_position(1,0) << "     " << b.get_position(3,1) << "     " << b.get_position(5,2) << endl;
    cout << "   " << b.get_position(-4,-3) << "     " << b.get_position(-2,-2) << "     " << b.get_position(0,-1) << "     " << b.get_position(2,0) << "     " << b.get_position(4,1) << endl;
    cout << b.get_position(-5,-4) << "     " << b.get_position(-3,-3) << "     " << b.get_position(-1,-2) << "     " << b.get_position(1,-1) << "     " << b.get_position(3,0) << "     " << b.get_position(5,1) << endl;
    cout << "   " << b.get_position(-4,-4) << "     " << b.get_position(-2,-3) << "     " << b.get_position(0,-2) << "     " << b.get_position(2,-1) << "     " << b.get_position(4,0) << endl;
    cout << " " << "     " << b.get_position(-3,-4) << "     " << b.get_position(-1,-3) << "     " << b.get_position(1,-2) << "     " << b.get_position(3,-1) << "     " << " " << endl;
    cout << "   " << b.get_position(-4,-5) << "     " << b.get_position(-2,-4) << "     " << b.get_position(0,-3) << "     " << b.get_position(2,-2) << "     " << b.get_position(4,-1) << endl;
    cout << " " << "     " << b.get_position(-3,-5) << "     " << b.get_position(-1,-4) << "     " << b.get_position(1,-3) << "     " << b.get_position(3,-2) << endl;
    cout << "   " << " " << "     " << b.get_position(-2,-5) << "     " << b.get_position(0,-4) <<  "     " << b.get_position(2,-3) <<endl;
    cout << " " << "     " << " " << "     " << b.get_position(-1,-5) << "     " << b.get_position(1,-4) << endl;
    cout << "   " << "     " << " " << "     " << " " << " " << endl;
}


int main(){
    Board my_board = Board(5);
    // print_board(my_board);
    // vector<int> conv = my_board.map_hex_mysys(3,1);
    // cout << conv[0] << " " << conv[1];

    vector<Move> res = get_move("P 1 2");
    for (int i = 0; i< res.size(); i++){
        cout<<res[i].move_type<<res[i].x<<res[i].y<<endl;
    }

    return(1);
}
