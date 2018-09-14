#include<iostream>
#include<vector>
#include<map>
using namespace std;

class Move{
public:
    string move_type;
    int hexagon;
    int index;
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
};

int Board::get_position(int x, int y){
    return board_storage[x+5][y+5];
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
    print_board(my_board);
    return(1);
}
