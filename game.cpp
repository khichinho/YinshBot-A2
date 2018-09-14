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

    vector<vector<int>> board_storage;

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

//    vector<int> map_mysys_hex(int )
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

vector<int> Board::mapping(int hexagon, int index){

}

int main(){
    cout << 10 << endl;
    return 1;
}



//  first make a class name Board having data structure
//     vector of vector to store structure of rings,
//     markers and empty spaces.
//     It also contains score of the Board,


// 