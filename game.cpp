#include<iostream>
#include<vector>
#include<string>
#include <sstream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <boost/foreach.hpp>
#include<algorithm>
using namespace std;


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

    Move(string type, int abs, int ord){
        move_type = type;
        x = abs;
        y = ord;
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

    vector<vector<int> > player1_rings;
    vector<vector<int> > player2_rings;

    vector<vector<int> > board_storage;

    Board(int board_size, int m, int r) {
        
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

    //vector<int> map_hex_mysys(int hexagon, int index); 

    vector<int> map_mysys_hex(int abscissa, int ordinate);
// player_index can get value -1 or 1, -1 means player1 and 1 means player2
// 1 for marker2-player2, 2 for ring2-player2
    vector<Move> totalmoves(string s);

    void execute_move(vector<Move> movelist, int player_index);

    bool check_valid(Move mv, int player_index);

    vector<int> startend(int s1, int f1, int s2, int f2, int value);
    
    vector<int> moves_row(vector<int> ls);
    
    vector<int> cons_marker(int value);

    void print_board();

    int row_marker(int x1, int y1, int x2, int y2, int value);

    int board_marker(int value);

    int heuristics(int player_index);

    vector<int> place_position(int player_index);

    bool check_valid_position(int x, int y);

    bool check_dir(int x1, int y1, int x2, int y2);
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
    // int boardsize = (board_storage.size() - 1)/2;
    // int horizontal = abscissa + boardsize;
    // if(abscissa >= 0)return board_storage[horizontal][boardsize - ordinate];
    // else{
    //     return board_storage[horizontal][horizontal - ordinate];
    // } 
    int boardsize = (board_storage.size() - 1)/2;
    return board_storage[abscissa+boardsize][ordinate+boardsize];
}

void Board::setpositionValue(int abscissa, int ordinate, int value){
    int init_value = getpositionValue(abscissa, ordinate);    
    int boardsize = (board_storage.size() - 1)/2;
    // int horizontal = abscissa + boardsize;
    // if(abscissa >= 0)board_storage[horizontal][boardsize - ordinate] = value;
    // else{
    //     board_storage[horizontal][horizontal - ordinate] = value;
    // }
    
    board_storage[abscissa+boardsize][ordinate+boardsize] = value;

    // if(init_value == 0 && value == 2)ring2++;
    // else if(init_value == 0 && value == -2){
    //     ring1=max(ring1 + 1,5);}
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

vector<int> Board::startend(int x1, int y1, int x2, int y2, int value){
    vector<int> ls;
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
                if(getpositionValue(i,y1) == value){
                    firstx = i;
                    firsty = y1;
                    check = 1;
                }
            }
            if(check==1){
                if(getpositionValue(i,y1) == value){
                    lastx = i;
                    lasty = y1;
                    if(i == x2){
                        if(x2 - firstx >= 4){
                            ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                        }
                    }
                }
                else if(getpositionValue(i,y1) != value){
                    check = 0;
                    if(i-firstx >= 5){
                        ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                    }
                }
            }
        }
    }

    else if(x1 == x2){
        for(int i = y1; i <= y2; i++){
            if(check == 0){
                if(getpositionValue(x1, i) == value){
                    firstx = x1;
                    firsty = i;
                    check = 1;
                }
            }
            else if(check == 1){
                if(getpositionValue(x1, i) == value){
                    lastx = x1;
                    lasty = i;
                    if(i == y2){
                        if(y2 - firsty >= 4){
                            ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                        }
                    }
                }
                else if(getpositionValue(x1, i) != value){
                    check = 0;
                    if(i-firsty >= 5){
                        ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                    }
                }
            }
        }
    }

    else if(y1-x1 == y2-x2){
        for(int i = x1; i<= x2; i++){
            if(check == 0){
                if(getpositionValue(i, y1+i-x1) == value){
                    firstx = i;
                    firsty = y1+i-x1;
                    check = 1;
                }
            }
            else if(check == 1){
                if(getpositionValue(i,y1+i-x1) == value){
                    lastx = i;lasty = y1+i-x1;
                    if(i == x2){
                        if(x2 - firstx >= 4){
                            ls[0] = firstx;ls[1] = firsty; ls[2] = lastx; ls[3] = lasty;
                        }
                    }
                }
                else if(getpositionValue(i, y1+i-x1) != value){
                    check = 0;
                    if(i-firstx >= 5){
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
	if(!((ls[0] == 0) && (ls[1] == 0) && (ls[2] == 0) && (ls[3] == 0))){
		int x = ls[2] - ls[0];
		int y = ls[3] - ls[1];
		int size = max(x,y)+1;
		if(x != 0)x=1;
		if(y != 0)y=1;
		for(int i = 0; i <= size-5; i++){
			vec.push_back(ls[0]+i*x);
			vec.push_back(ls[1]+i*y);
			vec.push_back(ls[0]+(i+4)*x);
			vec.push_back(ls[1]+(i+4)*y);
		}
	}
	else vec.push_back(0);
	return vec;
}



vector<int> Board::cons_marker(int value){
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
        vector<int> vec = moves_row(startend(-5,-i,5,-i,value));
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


void Board::execute_move(vector<Move> movelist, int player_index){
    for(int k = 0; k < movelist.size(); k++){
        Move m1 = movelist[k];
        //vector<int> mycoord = map_hex_mysys(m1.hexagon, m1.index);
        vector<int> ringcoordinate;
        ringcoordinate.push_back(m1.x);
        ringcoordinate.push_back(m1.y);
        if(m1.move_type == "P"){
            setpositionValue(m1.x, m1.y, 2*player_index);
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
            //vector<int> mycoord2 = map_hex_mysys(m2.hexagon, m2.index);
            // MyCoordinate ring2Coordinate = // MyCoordinate(m2.x, m2.y);
            vector<int> ring2Coordinate;
            ring2Coordinate.push_back(m2.x);
            ring2Coordinate.push_back(m2.y);
            //if(m2.move_type != "M")throw "invalid move";
            //else {
            setpositionValue(m1.x, m1.y, player_index);
            setpositionValue(m2.x, m2.y, 2*player_index);
            if(m1.y == m2.y){
                if(m2.x > m1.x){
                    for(int i = m1.x+1; i < m2.x; i++)
                        setpositionValue(i, m1.y, -1* getpositionValue(i, m1.y));
                    //setpositionValue(m2.x, m2.y, 2*player_index);
                }
                else{
                    for(int i = m1.x-1; i > m2.x; i--)
                        setpositionValue(i, m1.y, -1*getpositionValue(i, m1.y));
                    //setpositionValue(m2.x, m2.y, 2*player_index);
                }                
            }

            else if(m1.x == m2.x){
                if(m2.y > m1.y){
                    for(int i = m1.y+1; i < m2.y; i++)
                        setpositionValue(m1.x, i, -1*getpositionValue(m1.x, i));
                    //setpositionValue(m2.x, m2.y, 2*player_index);
                }
                else{
                    for(int i = m1.y-1; i > m2.y; i--)
                        setpositionValue(m1.x, i, -1*getpositionValue(m1.x, i));
                    //setpositionValue(m2.x, m2.y, 2*player_index);
                }
            }

            else if(m2.y-m1.y == m2.x-m1.x){
                if(m2.x > m1.x){
                    for(int i = 1; i < m2.x-m1.x; i++)
                        setpositionValue(m1.x+i, m1.y+i, -1*getpositionValue(m1.x+i, m1.y+i));
                    //setpositionValue(m2.x, m2.y, 2*player_index);                        
                }
                else{
                    for(int i = 1; i < m1.x-m2.x; i++)
                        setpositionValue(m1.x-i, m1.y-i, -1*getpositionValue(m1.x-i,m1.y-i));
                    //setpositionValue(m2.x, m2.y, 2*player_index);
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
            //vector<int> mycoord2 = map_hex_mysys(m2.hexagon, m2.index);
            Move m3 = movelist[++k];
            if(player_index == -1)ring1_removed++;
                else ring2_removed++;
            if(m1.y == m2.y){
                if(m2.x > m1.x){
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(m1.x+i, m1.y, 0);
                    //setpositionValue(m2.x, m2.y, 0);
                }
                else{
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(m1.x-i, m1.y, 0);
                    //setpositionValue(m2.x, m2.y, 0);
                }                
            }
            else if(m1.x == m2.x){
                if(m2.y > m1.y){
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(m1.x, m1.y+i, 0);    
                }
                else{
                    for(int i = 0; i < markers_removed; i++){
                         setpositionValue(m1.x, m1.y-i, 0);
                    }
                }
            }
            else if(m2.y-m1.y == m2.x-m1.x){
                if(m2.x > m1.x){
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(m1.x+i, m1.y+i, 0);
                }
                else{
                    for(int i = 0; i < markers_removed; i++)
                        setpositionValue(m1.x-i,m1.y-i, 0);
                }
            }

            //vector<int> mycoord3 = map_hex_mysys(m3.hexagon, m3.index);
            setpositionValue(m3.x, m3.y, 0);

            // MyCoordinate ring2Coordinate = // MyCoordinate(m3.x, m3.y);
            
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

bool Board::check_valid_position(int x, int y){

    if(x == 0){
        if(y >= -4 && y <= 4){ return true;}
        else{return false;}
    }

    else if(x == 1){
        if(y >= -4 && y <= 5){ return true;}
        else{return false;}
    }
    else if(x == 2){
        if(y >= -3 && y <= 5){ return true;}
        else{return false;}
    }
    else if(x == 3){
        if(y >= -2 && y <= 5){ return true;}
        else{return false;}
    }
    else if(x == 4){
        if(y >= -1 && y <= 5){ return true;}
        else{return false;}
    }
    else if(x == 5){
        if(y >= 1 && y <= 4){ return true;}
        else{return false;}
    }

    else if(x == -1){
        if(y >= -5 && y <= 4){ return true;}
        else{return false;}
    }
    else if(x == -2){
        if(y >= -5 && y <= 3){ return true;}
        else{return false;}
    }
    else if(x == -3){
        if(y >= -5 && y <= 2){ return true;}
        else{return false;}
    }
    else if(x == -4){
        if(y >= -5 && y <= 1){ return true;}
        else{return false;}
    }
    else if(x == -5){
        if(y >= -4 && y <= -1){ return true;}
        else{return false;}
    }

    else{
        return false;
    }
}

void Board::print_board(){

    cout << "   " << "     " << " " << "     " << " " << " " << endl;
    cout << " " << "     " << " " << "     " << getpositionValue(-1,4)+3 << "     " << getpositionValue(1,5)+3 << endl;
    cout << "   " << "     " << " " << getpositionValue(-2,3)+3 << "     " << getpositionValue(0,4)+3 << "     " << getpositionValue(2,5)+3 << endl;
    cout << " " << "     " << getpositionValue(-3,2)+3 << "     " << getpositionValue(-1,3)+3 << "     " << getpositionValue(1,4)+3 << "     " << getpositionValue(3,5)+3 << endl;
    cout << "   " << getpositionValue(-4,1)+3 << "     " << getpositionValue(-2,2)+3 << "     " << getpositionValue(0,3)+3 << "     " << getpositionValue(2,4)+3 << "     " << getpositionValue(4,5)+3 << endl;
    cout << " " << "     " << getpositionValue(-3,1)+3 << "     " << getpositionValue(-1,2)+3 << "     " << getpositionValue(1,3)+3 << "     " << getpositionValue(3,4)+3 << "     " << " " << endl;
    cout << "   " << getpositionValue(-4,0)+3 << "     " << getpositionValue(-2,1)+3 << "     " << getpositionValue(0,2)+3 << "     " << getpositionValue(2,3)+3 << "     " << getpositionValue(4,4)+3 << endl;
    cout << getpositionValue(-5,-1)+3 << "     " << getpositionValue(-3,0)+3 << "     " << getpositionValue(-1,1)+3 << "     " << getpositionValue(1,2)+3 << "     " << getpositionValue(3,3)+3 << "     " << getpositionValue(5,4)+3 << endl;
    cout << "   " << getpositionValue(-4,-1)+3 << "     " << getpositionValue(-2,0)+3 << "     " << getpositionValue(0,1)+3 << "     " << getpositionValue(2,2)+3 << "     " << getpositionValue(4,3)+3 << endl;
    cout << getpositionValue(-5,-2)+3 << "     " << getpositionValue(-3,-1)+3 << "     " << getpositionValue(-1,0)+3 << "     " << getpositionValue(1,1)+3 << "     " << getpositionValue(3,2)+3 << "     " << getpositionValue(5,3)+3 << endl;
    cout << "   " << getpositionValue(-4,-2)+3 << "     " << getpositionValue(-2,-1)+3 << "     " << getpositionValue(0,0)+3 << "     " << getpositionValue(2,1)+3 << "     " << getpositionValue(4,2)+3 << endl;
    cout << getpositionValue(-5,-3)+3 << "     " << getpositionValue(-3,-2)+3 << "     " << getpositionValue(-1,-1)+3 << "     " << getpositionValue(1,0)+3 << "     " << getpositionValue(3,1)+3 << "     " << getpositionValue(5,2)+3 << endl;
    cout << "   " << getpositionValue(-4,-3)+3 << "     " << getpositionValue(-2,-2)+3 << "     " << getpositionValue(0,-1)+3 << "     " << getpositionValue(2,0)+3 << "     " << getpositionValue(4,1)+3 << endl;
    cout << getpositionValue(-5,-4)+3 << "     " << getpositionValue(-3,-3)+3 << "     " << getpositionValue(-1,-2)+3 << "     " << getpositionValue(1,-1)+3 << "     " << getpositionValue(3,0)+3 << "     " << getpositionValue(5,1)+3 << endl;
    cout << "   " << getpositionValue(-4,-4)+3 << "     " << getpositionValue(-2,-3)+3 << "     " << getpositionValue(0,-2)+3 << "     " << getpositionValue(2,-1)+3 << "     " << getpositionValue(4,0)+3 << endl;
    cout << " " << "     " << getpositionValue(-3,-4)+3 << "     " << getpositionValue(-1,-3)+3 << "     " << getpositionValue(1,-2)+3 << "     " << getpositionValue(3,-1)+3 << "     " << " " << endl;
    cout << "   " << getpositionValue(-4,-5)+3 << "     " << getpositionValue(-2,-4)+3 << "     " << getpositionValue(0,-3)+3 << "     " << getpositionValue(2,-2)+3 << "     " << getpositionValue(4,-1)+3 << endl;
    cout << " " << "     " << getpositionValue(-3,-5)+3 << "     " << getpositionValue(-1,-4)+3 << "     " << getpositionValue(1,-3)+3 << "     " << getpositionValue(3,-2)+3 << endl;
    cout << "   " << " " << "     " << getpositionValue(-2,-5)+3 << "     " << getpositionValue(0,-4)+3 <<  "     " << getpositionValue(2,-3)+3 <<endl;
    cout << " " << "     " << " " << "     " << getpositionValue(-1,-5)+3 << "     " << getpositionValue(1,-4)+3 << endl;
    cout << "   " << "     " << " " << "     " << " " << " " << endl;

    cout << '\n' << '\n';
    cout << "ring1: " << ring1 << '\n' << "ring2: " << ring2 << '\n';
    cout << "marker1: " << marker1 << '\n' << "marker2: " << marker2 << '\n';
}


int Board::row_marker(int x1, int y1, int x2, int y2, int value){
    int x = x2 - x1;
    int y = y2 - y1;
    int size = max(x,y);
    if(x != 0)x = 1;
    if(y != 0)y = 1;
    int check = 0;
    int firstx, firsty,lastx,lasty;
    //vector<int> ls;
    int sum = 0;
    for(int i = 0; i <= size; i++){
        int xcurr = x1+i*x;
        int ycurr = y1+i*y;
        if(check==0){
            if(getpositionValue(xcurr,ycurr) == value){
                firstx = xcurr;
                firsty = ycurr;
                check = 1;
            }
        }
        if(check==1){
            if(getpositionValue(xcurr,ycurr) == value){
                lastx = xcurr;
                lasty = ycurr;
                if((xcurr == x2) && (ycurr == y2)){
                    int el = max((x2-firstx+1),(y2-firsty+1));
                    sum = sum + (int)pow(el,3.0);
                }
                    //ls.push_back(max((x2-firstx+1),(y2-firsty+1)));
                
            }
            else if(getpositionValue(xcurr,ycurr) != value){
                check = 0;
                int el = max(xcurr-firstx,ycurr-firsty);
                sum = sum + (int)pow(el,3.0);
                //ls.push_back(max(xcurr-firstx,ycurr-firsty));
            }
        }
    }
//    return ls;
    return sum;
}

int Board::board_marker(int value){
    vector<int> ret;
    int sum = 0;

    //vector<int> veco = row_marker(1,5,4,5,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    int veco = row_marker(1,5,4,5,value);
    sum += veco;
    // for(int i = 1; i < 5; i++){
    //      vector<int> vec = row_marker(-i,5-i,5,5-i,value);
    // 	    for(int j = 0;j < vec.size(); j++)
    // 		    ret.push_back(vec[j]);
    // }

    for(int i = 1; i < 5; i++){
        int vec = row_marker(-i,5-i,5,5-i,value);
        sum += vec;    
    }

    // veco = row_marker(-4,0,4,0,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    veco = row_marker(-4,0,4,0,value);
    sum += veco;

    // for(int i = 1; i < 5; i++){
    //     vector<int> vec = row_marker(-5,-i,5-i,-i,value);
	// 		for(int j = 0;j < vec.size(); j++)
	// 			ret.push_back(vec[j]);
    // }

    for(int i = 1; i < 5; i++){
        int vec = row_marker(-5,-i,5-i,-i,value);
        sum += vec;
    }

    // veco = row_marker(-4,-5,-1,-5,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    veco = row_marker(-4,-5,-1,-5,value);
    sum += veco;

///////////////////////

    // veco = row_marker(-5,-4,-5,-1,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    veco = row_marker(-5,-4,-5,-1,value);
    sum += veco;

    // for(int i = 1; i < 5; i++){
    //     vector<int> vec = row_marker(i-5,-5,i-5,i,value);
	// 		for(int j = 0;j < vec.size(); j++)
	// 			ret.push_back(vec[j]);
    // }

    for(int i = 1; i < 5; i++){
        int vec = row_marker(i-5,-5,i-5,i,value);
        sum += vec;
    }

    // veco = row_marker(0,-4,0,4,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    veco = row_marker(0,-4,0,4,value);
    sum += veco;

    // for(int i = 1; i < 5; i++){
    //     vector<int> vec = row_marker(i,i-5,i,5,value);
	// 		for(int j = 0;j < vec.size(); j++)
	// 			ret.push_back(vec[j]);
    // }

    for(int i = 1; i < 5; i++){
        int vec = row_marker(i,i-5,i,5,value);
        sum += vec;
    }

    // veco = row_marker(5,1,5,4,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);
    
    veco = row_marker(5,1,5,4,value);
    sum += veco;

//////////////////////////

    // veco = row_marker(-4,1,-1,4,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    veco = row_marker(-4,1,-1,4,value);
    sum += veco;

    // for(int i = 1; i < 5; i++){
    //     vector<int> vec = row_marker(-5,-i,i,5,value);
	// 		for(int j = 0;j < vec.size(); j++)
	// 			ret.push_back(vec[j]);
    // }

    for(int i = 1; i < 5; i++){
        int vec = row_marker(-5,-i,i,5,value);
        sum += vec;
    }

    // veco = row_marker(-4,-4,4,4,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    veco = row_marker(-4,-4,4,4,value);
    sum += veco;

    // for(int i = 1; i < 5; i++){
    //     vector<int> vec = row_marker(i-5,-5,5,5-i,value);
	// 		for(int j = 0;j < vec.size(); j++)
	// 			ret.push_back(vec[j]);
    // }

    for(int i = 1; i < 5; i++){
        int vec = row_marker(i-5,-5,5,5-i,value);
        sum += vec;
    }

    // veco = row_marker(1,-4,4,-1,value);
    // for(int j = 0; j < veco.size(); j++)
    //     ret.push_back(veco[j]);

    veco = row_marker(1,-4,4,-1,value);
    sum += veco;

    return sum;

}

int Board::heuristics(int p){
    int score = board_marker(p) - board_marker(-p);
    if(p == -1)
        score += 100*(ring1_removed - ring2_removed);
    else score += 100*(ring2_removed - ring1_removed);
}

bool Board::check_dir(int x1, int y1, int x2, int y2){
    if((x1 == x2) || (y1 == y2) || (x1-x2 == y1-y2))return true;
    return false;
}

vector<int> Board::place_position(int player_index){
    vector<int> direction {0,1,0,-1,1,0,-1,0,1,1,-1,-1};
    vector<int> place;
    vector<vector<int> > player_rings;
    vector<vector<int> > opponent_rings;
    if(player_index == -1){
        player_rings = player1_rings;
        opponent_rings = player2_rings;
    }
    else{
        player_rings = player2_rings;
        opponent_rings = player1_rings;
    }


    if(player_index == -1){
    int n = opponent_rings.size();
    int arr[n];
    for(int i = 0; i < n; ++i)
        arr[i] = i;
    random_shuffle(arr, arr+n);
        for(int i = 0; i < opponent_rings.size(); i++){
            int x = opponent_rings[arr[i]][0];
            int y = opponent_rings[arr[i]][1];
            for(int j = 0; j < 6; i++){
                int xnew = x+direction[2*j];
                int ynew = y+direction[2*j+1];
                int flag = 0;
                if(check_valid_position(xnew,ynew)){
                    if(getpositionValue(xnew,ynew) == 0){
                        for(int k = 0; k < player_rings.size(); k++){
                            if(check_dir(xnew, ynew, player_rings[k][0], player_rings[k][1]) == true){
                                flag = 1;
                                break;
                            }
                        }
                        if(flag == 1){
                            place.push_back(xnew);
                            place.push_back(ynew);
                            return place;
                        }
                    }
                }
            }   
        }
    }
}




int main(){
    // Board myBoard = Board(5,5,3);
    // int player_number = -1;
    // myBoard.print_board();
    // cout << mys[0] << " " << mys[1] << '\n';
    // cout << mys1[0] << " " << mys1[1] << '\n';
    
    // for(int i = 0; i < 100; i++){
    //     string s;
    //     getline(cin, s);
    //     //vector<string> movvlist = splitstr(s);
    //     vector<Move> movvllist = movlist(s);
    //     // for(int i = 0; i < movvllist.size(); i++){
    //     //     Move mv = movvllist[i];
    //     //     cout << 
    //     // }
    //     //cout << movvlist.size();
    //     myBoard.execute_move(movvllist, player_number);
    //     //int x = myBoard.getpositionValue(1,0);
        
    //     //cout << movvlist.size();
        
    //     myBoard.print_board();
    //     player_number *= -1;
    // }

    ifstream infile("thefile.txt");
    string line;
    Board myBoard = Board(5,5,3);
    int player_number = -1;
    while (getline(infile, line))
    {
        vector<Move> movvllist = movlist(line);
        myBoard.execute_move(movvllist, player_number);    
            //myBoard.print_board();
        player_number *= -1;
    }

    myBoard.print_board();
    for(int i = 0;i < myBoard.player1_rings.size();i++)
        cout << myBoard.player1_rings[i][0] << "  " << myBoard.player1_rings[i][1] << '\n';

    for(int i = 0;i < myBoard.player2_rings.size();i++)
        cout << myBoard.player2_rings[i][0] << "  " << myBoard.player2_rings[i][1] << '\n';
    // for(int i = 0; i < 100; i++){
    //      string s;
    //      getline(cin, s);
    
    //      vector<Move> movvllist = movlist(s);
    
    
    //      myBoard.execute_move(movvllist, player_number);
    //      myBoard.print_board();
    //      player_number *= -1;
    //  }








    return 1;
}




//  first make a class name Board having data structure
//     vector of vector to store structure of rings,
//     markers and empty spaces.
//     It also contains score of the Board,


// 