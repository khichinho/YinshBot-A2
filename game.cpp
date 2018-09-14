#include<iostream>
#include<vector>
using namespace std;

class Board {

private:
    int score;
    int ring1;
    int ring2;
    int marker1;
    int marker2;
public:

    vector<vector<int> > board_storage;

    Board(int board_size) {
        vector<int> el;
        el.push_back(0);
        board_storage.push_back(el);
        el.clear();
        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < 6; j++)
                el.push_back(0);
            board_storage.push_back(el);
        }
    }

    int getring1();

    int getring2();

    int getmarker1();

    int getmarker2();

    void setring1(int r1);

    void setring2(int r2);

    void setmarker1(int m1);

    void setmarker2(int m2);
    
    vector<int> map_hex_mysys(int hexagon, int index); 

    vector<int> map_mysys_hex(int abscissa, int ordinate);
// player_index can get value 0 or 1, 0 means player1 and 1 means player2

    void execute_move(string move, int player_index);

    bool check_valid(string move, int player_index);
};

int Board::getring1()
    {return ring1;}
void Board::setring1(int r1)
    {ring1 = r1;}


int Board::getring2()
    {return ring2;}
void Board::setring2(int r2)
    {ring2 = r2;}

int Board::getmarker1()
    {return marker1;}
void Board::setmarker1(int m1)
    {marker1 = m1;}

int Board::getmarker2()
    {return marker2;}
void Board::setmarker2(int m2)
    {marker2 = m2;}

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


int main(){
    Board myBoard = Board(5);
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6*i; j++){
            vector<int> conv = myBoard.map_hex_mysys(i,j);
            vector<int> revconv = myBoard.map_mysys_hex(conv[0], conv[1]);

            if(i != revconv[0] || j != revconv[1]){
                cout << i << " " << j << ": " << conv[0] << " " << conv[1] << ": " << revconv[0] << " " << revconv[1] << endl;

            }
        }
    }
    return 1;
}



//  first make a class name Board having data structure
//     vector of vector to store structure of rings,
//     markers and empty spaces.
//     It also contains score of the Board,


// 