#include<iostream>
#include<vector>
#include<string>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <boost/foreach.hpp>
using namespace std;

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
    int hexagon;
    int index;

    Move(string type, int hex, int ind){
        move_type = type;
        hexagon = hex;
        index = ind;
        }
};

vector<Move> movlist(string ply){
    vector<Move> ret;
    vector<string> movstr = splitstr(ply);
    for(int i = 0; i < movstr.size(); i = i+3){
        Move m1(movstr[i], stoi(movstr[i+1]), stoi(movstr[i+2]));
        ret.push_back(m1);
    }
    return ret;
}


class Board {

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

    Board(int board_size, int m, int r) {
        vector<int> el;
        for(int i = 0; i < board_size; i++){
            el.push_back(0);
        }
        for(int i = 0; i <= board_size; i++){
            el.push_back(0);
            board_storage.push_back(el);            
        }
        for(int i = 0; i < board_size; i++){
            el.pop_back();
            board_storage.push_back(el);
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

    int getring1();

    int getring2();

    int getmarker1();

    int getmarker2();

    void setring1(int r1);

    void setring2(int r2);

    void setmarker1(int m1);

    void setmarker2(int m2);
    
    int getpositionValue(int abscissa, int ordinate);

    void setpositionValue(int abscissa,int ordinate, int value);

    vector<int> map_hex_mysys(int hexagon, int index); 

    vector<int> map_mysys_hex(int abscissa, int ordinate);
// player_index can get value -1 or 1, -1 means player1 and 1 means player2
// 1 for marker2-player2, 2 for ring2-player2
    vector<Move> totalmoves(string s);

    void execute_move(vector<Move> movelist, int player_index);

    bool check_valid(Move mv, int player_index);

    void print_board();
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

int Board::getpositionValue(int abscissa, int ordinate){
    int boardsize = (board_storage.size() - 1)/2;
    int horizontal = abscissa + boardsize;
    if(abscissa >= 0)return board_storage[horizontal][boardsize - ordinate];
    else{
        return board_storage[horizontal][horizontal - ordinate];
    } 
}

void Board::setpositionValue(int abscissa, int ordinate, int value){
    int init_value = getpositionValue(abscissa, ordinate);    
    int boardsize = (board_storage.size() - 1)/2;
    int horizontal = abscissa + boardsize;
    if(abscissa >= 0)board_storage[horizontal][boardsize - ordinate] = value;
    else{
        board_storage[horizontal][horizontal - ordinate] = value;
    }
    
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

vector<Move> Board::totalmoves(string move){
}

void Board::execute_move(vector<Move> movelist, int player_index){
    for(int k = 0; k < movelist.size(); k++){
        Move m1 = movelist[k];
        vector<int> mycoord = map_hex_mysys(m1.hexagon, m1.index);
        if(m1.move_type == "P"){
            setpositionValue(mycoord[0], mycoord[1], 2*player_index);
            // if(player_index == -1)ring1++;
            //     else ring2++;
        }
        else if(m1.move_type == "S"){            
            Move m2 = movelist[++k];
            vector<int> mycoord2 = map_hex_mysys(m2.hexagon, m2.index);
            if(m2.move_type != "M")throw "invalid move";
            else {
                setpositionValue(mycoord[0], mycoord[1], player_index);
                setpositionValue(mycoord2[0], mycoord2[1], 2*player_index);
                if(mycoord[1] == mycoord2[1]){
                    if(mycoord2[0] > mycoord[0]){
                        for(int i = mycoord[0]+1; i < mycoord2[0]; i++)
                            setpositionValue(i, mycoord[1], -1* getpositionValue(i, mycoord[1]));
                        //setpositionValue(mycoord2[0], mycoord2[1], 2*player_index);
                    }
                    else{
                        for(int i = mycoord[0]+1; i > mycoord2[0]; i--)
                            setpositionValue(i, mycoord[1], -1*getpositionValue(i, mycoord[1]));
                        //setpositionValue(mycoord2[0], mycoord2[1], 2*player_index);
                    }                
                }

                else if(mycoord[0] == mycoord2[0]){
                    if(mycoord2[1] > mycoord[1]){
                        for(int i = mycoord[1]+1; i < mycoord2[1]; i++)
                            setpositionValue(mycoord[0], i, -1*getpositionValue(mycoord[0], i));
                        //setpositionValue(mycoord2[0], mycoord2[1], 2*player_index);
                    }
                    else{
                        for(int i = mycoord[1]+1; i > mycoord2[1]; i--)
                            setpositionValue(mycoord[0], i, -1*getpositionValue(mycoord[0], i));
                        //setpositionValue(mycoord2[0], mycoord2[1], 2*player_index);
                    }
                }

                else if(mycoord2[1]-mycoord[1] == mycoord2[0]-mycoord[0]){
                    if(mycoord2[0] > mycoord[0]){
                        for(int i = 1; i < mycoord2[0]-mycoord[0]; i++)
                            setpositionValue(mycoord[0]+i, mycoord[1]+i, -1*getpositionValue(mycoord[0]+i, mycoord[1]+i));
                        setpositionValue(mycoord2[0], mycoord2[1], 2*player_index);                        
                    }
                    else{
                        for(int i = 1; i < mycoord[0]-mycoord2[0]; i--)
                            setpositionValue(mycoord[0]-i, mycoord2[1]-i, -1*getpositionValue(mycoord[0]-i,mycoord[1]-i));
                        setpositionValue(mycoord2[0], mycoord2[1], 2*player_index);
                    }
                }

            }
        }

        else if(m1.move_type =="RS"){
            Move m2 = movelist[++k];
            vector<int> mycoord2 = map_hex_mysys(m2.hexagon, m2.index);
            Move m3 = movelist[++k];
            if(player_index == -1)ring1_removed++;
                else ring2_removed++;
            if(mycoord[1] == mycoord2[1]){
                if(mycoord2[0] > mycoord[0]){
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(mycoord[0]+i, mycoord[1], 0);
                    //setpositionValue(mycoord2[0], mycoord2[1], 0);
                }
                else{
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(mycoord[0]-i, mycoord[1], 0);
                    //setpositionValue(mycoord2[0], mycoord2[1], 0);
                }                
            }
            else if(mycoord[0] == mycoord2[0]){
                if(mycoord2[1] > mycoord[1]){
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(mycoord[0], mycoord[1]+i, 0);    
                }
                else{
                    for(int i = 0; i < markers_removed; i++){
                         setpositionValue(mycoord[0], mycoord[1]-i, 0);
                    }
                }
            }
            else if(mycoord2[1]-mycoord[1] == mycoord2[0]-mycoord[0]){
                if(mycoord2[0] > mycoord[0]){
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(mycoord[0]+i, mycoord[1]+i, 0);
                }
                else{
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(mycoord[0]-i,mycoord[1]-i, 0);
                }
            }

            vector<int> mycoord3 = map_hex_mysys(m3.hexagon, m3.index);
            setpositionValue(mycoord3[0], mycoord3[1], 0);

        }
        
    }
}

void Board::print_board(){

    cout << "   " << "     " << " " << "     " << " " << " " << endl;
    cout << " " << "     " << " " << "     " << getpositionValue(-1,4) << "     " << getpositionValue(1,5) << endl;
    cout << "   " << "     " << " " << getpositionValue(-2,3) << "     " << getpositionValue(0,4) << "     " << getpositionValue(2,5) << endl;
    cout << " " << "     " << getpositionValue(-3,2) << "     " << getpositionValue(-1,3) << "     " << getpositionValue(1,4) << "     " << getpositionValue(3,5) << endl;
    cout << "   " << getpositionValue(-4,1) << "     " << getpositionValue(-2,2) << "     " << getpositionValue(0,3) << "     " << getpositionValue(2,4) << "     " << getpositionValue(4,5) << endl;
    cout << " " << "     " << getpositionValue(-3,1) << "     " << getpositionValue(-1,2) << "     " << getpositionValue(1,3) << "     " << getpositionValue(3,4) << "     " << " " << endl;
    cout << "   " << getpositionValue(-4,0) << "     " << getpositionValue(-2,1) << "     " << getpositionValue(0,2) << "     " << getpositionValue(2,3) << "     " << getpositionValue(4,4) << endl;
    cout << getpositionValue(-5,-1) << "     " << getpositionValue(-3,0) << "     " << getpositionValue(-1,1) << "     " << getpositionValue(1,2) << "     " << getpositionValue(3,3) << "     " << getpositionValue(5,4) << endl;
    cout << "   " << getpositionValue(-4,-1) << "     " << getpositionValue(-2,0) << "     " << getpositionValue(0,1) << "     " << getpositionValue(2,2) << "     " << getpositionValue(4,3) << endl;
    cout << getpositionValue(-5,-2) << "     " << getpositionValue(-3,-1) << "     " << getpositionValue(-1,0) << "     " << getpositionValue(1,1) << "     " << getpositionValue(3,2) << "     " << getpositionValue(5,3) << endl;
    cout << "   " << getpositionValue(-4,-2) << "     " << getpositionValue(-2,-1) << "     " << getpositionValue(0,0) << "     " << getpositionValue(2,1) << "     " << getpositionValue(4,2) << endl;
    cout << getpositionValue(-5,-3) << "     " << getpositionValue(-3,-2) << "     " << getpositionValue(-1,-1) << "     " << getpositionValue(1,0) << "     " << getpositionValue(3,1) << "     " << getpositionValue(5,2) << endl;
    cout << "   " << getpositionValue(-4,-3) << "     " << getpositionValue(-2,-2) << "     " << getpositionValue(0,-1) << "     " << getpositionValue(2,0) << "     " << getpositionValue(4,1) << endl;
    cout << getpositionValue(-5,-4) << "     " << getpositionValue(-3,-3) << "     " << getpositionValue(-1,-2) << "     " << getpositionValue(1,-1) << "     " << getpositionValue(3,0) << "     " << getpositionValue(5,1) << endl;
    cout << "   " << getpositionValue(-4,-4) << "     " << getpositionValue(-2,-3) << "     " << getpositionValue(0,-2) << "     " << getpositionValue(2,-1) << "     " << getpositionValue(4,0) << endl;
    cout << " " << "     " << getpositionValue(-3,-4) << "     " << getpositionValue(-1,-3) << "     " << getpositionValue(1,-2) << "     " << getpositionValue(3,-1) << "     " << " " << endl;
    cout << "   " << getpositionValue(-4,-5) << "     " << getpositionValue(-2,-4) << "     " << getpositionValue(0,-3) << "     " << getpositionValue(2,-2) << "     " << getpositionValue(4,-1) << endl;
    cout << " " << "     " << getpositionValue(-3,-5) << "     " << getpositionValue(-1,-4) << "     " << getpositionValue(1,-3) << "     " << getpositionValue(3,-2) << endl;
    cout << "   " << " " << "     " << getpositionValue(-2,-5) << "     " << getpositionValue(0,-4) <<  "     " << getpositionValue(2,-3) <<endl;
    cout << " " << "     " << " " << "     " << getpositionValue(-1,-5) << "     " << getpositionValue(1,-4) << endl;
    cout << "   " << "     " << " " << "     " << " " << " " << endl;

    cout << '\n' << '\n';
    cout << "ring1: " << ring1 << '\n' << "ring2: " << ring2 << '\n';
    cout << "marker1: " << marker1 << '\n' << "marker2: " << marker2 << '\n';
}


int main(){
    Board myBoard = Board(5,5,3);
    int player_number = -1;
    for(int i = 0; i < 100; i++){
        string s;
        getline(cin, s);
        //vector<string> movvlist = splitstr(s);
        vector<Move> movvllist = movlist(s);
        //cout << movvlist.size();
        myBoard.execute_move(movvllist, player_number);
        //int x = myBoard.getpositionValue(1,0);
        cout << 1 ;
        //cout << movvlist.size();
        
        myBoard.print_board();
        player_number *= -1;
    }
    return 1;
}



//  first make a class name Board having data structure
//     vector of vector to store structure of rings,
//     markers and empty spaces.
//     It also contains score of the Board,


// 