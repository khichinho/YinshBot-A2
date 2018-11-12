#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<utility>
#include<math.h>

using namespace std;

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


static const std::vector<int> all_directions {1,0,-1,0,0,1,0,-1,-1,-1,1,1};
vector<string> split_string(string str, char dl){
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

class Direction{
    public:
        int xchange;
        int ychange;

        Direction(int xc, int yc){
            xchange = xc;
            ychange = yc;
        }
};

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
        int ring1; // rings for player 1 on the board
        int ring2; // rings for player 2 on the board
        int marker1; // markers for player 1 on the board
        int marker2; // markers for player 2 on the board
        int markers_per_ring;  // number of markers, to be removed to remove one ring
        int game_rings; // total_rings to start the game
        int ring1_removed; // rings removed of player1 till now
        int ring2_removed; // rings removed of player2 till now
        int board_size; // understandable

    public:

        vector<vector<int> > board_storage;

        vector<vector<int> > player1_rings;
        vector<vector<int> > player2_rings;

        Board(){
            vector<int> element;
            for(int i = -5; i <= 5; i++)
                element.push_back(0);
            for(int i = -5; i <= 5; i++)
                board_storage.push_back(element);
            
            element.clear();

            board_size = 5;
            markers_per_ring = 5;
            game_rings = 5;
            ring1 = 0;
            ring2 = 0;
            marker1 = 0;
            marker2 = 0;
            ring1_removed = 0;
            ring2_removed = 0;
        }

        Board(int n, int m, int k){
            vector<int> element;
            for(int i = -n; i <= n; i++)
                element.push_back(0);
            for(int i = -n; i <= n; i++)
                board_storage.push_back(element);
            element.clear();

            board_size = n;
            game_rings = m;
            markers_per_ring = k;
            ring1 = 0;
            ring2 = 0;
            marker1 = 0;
            marker2 = 0;
            ring1_removed = 0;
            ring2_removed = 0;
        }

        int get_position(int x, int y);
        void set_position(int x, int y, int value);
        bool check_valid_position(int x, int y);

        vector<Move> get_move(string ply);

        vector<int> map_hex_mysys(int hexagon, int index); 
        vector<int> map_mysys_hex(int abscissa, int ordinate);

        void execute_move(vector<Move>, int player_index);

        void print_board();
        string print_position(int x, int y);

        Board copy_board();

        vector<int> startend(int s1, int f1, int s2, int f2, int value);
        vector<int> moves_row(vector<int> ls);
        vector<int> cons_marker(int value);

        vector<pair<Board, vector<Move> > > all_moves(int player_number);
        vector<pair<Board, vector<Move> > > next_move(int player_number);
        pair<Board, vector<Move> > bot_move(int player_number, int depth, int alpha, int beta);
        pair<Board, vector<Move> > bot_depth(int player_number, int move_number);

        Board adjacent_move(int x, int y, Direction d);

        int row_marker(int x1, int y1, int x2, int y2, int value);

        int board_marker(int value);

        int heuristic();

        string server_output(pair<Board, vector<Move> > server_output_move);

        vector<int> marker_coordinate_feature(int s1,int f1,int s2, int f2, int value); // used to find the coordinates of end points of consecutive markers in a row
        vector<int> marker_set(int s1,int f1, int s2, int f2, int value);   // used to find the number of k-length conseutive markers in a row. k vary from 1 to markers_per_ring-1
        vector<int> total_marker_sets(int value);   // total number of k-set consecutive markers in the board
        vector<int> total_marker_sets_h(int value);
        int num_moves(int player_index);    // it returns total number of moves a player can play

};

string Board::server_output(pair<Board, vector<Move> > server_output_move){
    string move_string; 
    for(int i =0; i< server_output_move.second.size(); i++){
        vector<int> cords_out = server_output_move.first.map_mysys_hex(server_output_move.second[i].x,server_output_move.second[i].y);
        move_string = move_string + server_output_move.second[i].move_type + " " + to_string(cords_out[0]) + " " + to_string(cords_out[1]) + " "; 
    }
    move_string.pop_back();
    return move_string;
}

vector<pair<Board, vector<Move> > > Board::all_moves(int player_number){
    
    vector<pair<Board, vector<Move> > > possible_moves;
    vector<vector<int> > player_rings;
    int total_rings;

    vector<Direction> directions;
    directions.push_back(Direction(0,1));
    directions.push_back(Direction(1,1));
    directions.push_back(Direction(1,0));
    directions.push_back(Direction(0,-1));
    directions.push_back(Direction(-1,-1));
    directions.push_back(Direction(-1,0));

    if(player_number == -1){ player_rings = player1_rings;}
    else if(player_number == 1){player_rings = player2_rings;}

    if(player_number == -1){ total_rings = ring1 + ring1_removed;}
    else if(player_number == 1){total_rings = ring2 + ring2_removed;}
    
    if(total_rings == game_rings){
        for(int r = 0; r < player_rings.size(); r++){
            for(int i = 0; i< directions.size(); i++){

                int x2 = player_rings[r][0] + directions[i].xchange;
                int y2 = player_rings[r][1] + directions[i].ychange;

                bool over_marker = false;
                bool over_marker_position = false;

                while(check_valid_position(x2,y2)){
                    if(get_position(x2,y2) == -2*player_number || get_position(x2,y2) == 2*player_number){break;}
                    if(get_position(x2,y2) == -1*player_number || get_position(x2,y2) == player_number){over_marker = true;}
                    else if(get_position(x2,y2) == 0 && over_marker_position == false){
                        Board new_board = copy_board();
                        vector<Move> new_moves;
                        new_moves.push_back(Move("S",player_rings[r][0],player_rings[r][1]));
                        new_moves.push_back(Move("M",x2,y2));
                        new_board.execute_move(new_moves,player_number);
                        pair<Board,vector<Move> > bmpair(new_board,new_moves);
                        possible_moves.push_back(bmpair);
                        if(over_marker == true){over_marker_position = true;}
                    }
                    x2 += directions[i].xchange;
                    y2 += directions[i].ychange;
                }
            }
        }
    }
    else{
        for(int i = 0; i <= board_size; i++){
            bool if_exit = false;
            for(int j = i; j >= -i; j--){
                if(check_valid_position(i,j)){
                    if(get_position(i,j) == 0){
                        Board new_board = copy_board();
                        vector<Move> new_move;
                        new_move.push_back(Move("P",i,j));
                        new_board.execute_move(new_move,player_number);
                        pair<Board,vector<Move> > bmpair(new_board,new_move);
                        possible_moves.push_back(bmpair);
                        if_exit = true;
                        break;
                    }
                }
            }
            if(if_exit == true){break;}
        }
    }

    return possible_moves;
}

vector<pair<Board, vector<Move> > > Board::next_move(int player_number){

    vector<int> check_for_x = cons_marker(player_number);
    
    if(check_for_x.size() == 1){
        vector<pair<Board, vector<Move> > > after_sm = all_moves(player_number);

        vector<pair<Board, vector<Move> > > complete_moves;

        for(int i=0; i < after_sm.size(); i++){
            vector<int> remove_markers = after_sm[i].first.cons_marker(player_number);

            if(remove_markers.size() > 1){
                Move rs("RS",remove_markers[0],remove_markers[1]);
                Move re("RE",remove_markers[2],remove_markers[3]);

                vector<vector<int> > player_rings;
                if(player_number == -1) { player_rings = after_sm[i].first.player1_rings;}
                else if(player_number == 1){ player_rings = after_sm[i].first.player2_rings;}

                for(int j = 0; j < player_rings.size(); j++){
                    Move x("X",player_rings[j][0],player_rings[j][1]);
                    vector<Move> rsrex;
                    rsrex.push_back(rs);
                    rsrex.push_back(re);
                    rsrex.push_back(x);

                    Board after_sm_board = after_sm[i].first.copy_board();
                    vector<Move>after_sm_move = after_sm[i].second;

                    after_sm_board.execute_move(rsrex,player_number);
                    after_sm_move.insert(after_sm_move.end(),rsrex.begin(),rsrex.end());

                    pair<Board,vector<Move> > after_x_pair(after_sm_board, after_sm_move);
                    
                    complete_moves.push_back(after_x_pair);
                }
            }
            else{
                complete_moves.push_back(after_sm[i]);
            }
        }
        return complete_moves;
    }

    else if(check_for_x.size() >= 4){

        vector<Move> rsrex;
        vector<pair<Board, vector<Move> > > complete_moves;

        vector<vector<int> > x1_coordinates;
        if(player_number == -1){x1_coordinates = player1_rings;}
        else{x1_coordinates = player2_rings;}

        Move rs("RS",check_for_x[0],check_for_x[1]);
        Move re("RE",check_for_x[2],check_for_x[3]);
        rsrex.push_back(rs);
        rsrex.push_back(re);

        for(int i = 0; i < x1_coordinates.size(); i++){
            Move x("X",x1_coordinates[i][0],x1_coordinates[i][1]);
            rsrex.push_back(x);

            Board board_after_x1 = copy_board();
            board_after_x1.execute_move(rsrex,player_number);

            vector<pair<Board, vector<Move> > > next_move_after_x1 = board_after_x1.next_move(player_number);

            for(int j = 0; j <next_move_after_x1.size(); j++){
                vector<Move> update_rsrex =  rsrex;
                vector<Move> after_x1_move = next_move_after_x1[i].second;
                update_rsrex.insert(update_rsrex.end(),after_x1_move.begin(),after_x1_move.end());
                next_move_after_x1[i].second = update_rsrex;
                complete_moves.push_back(next_move_after_x1[i]);
            }
        }

        return complete_moves;
    }
}

pair<Board, vector<Move> > Board::bot_move(int player_number, int depth, int alpha, int beta){
///////////////////////////Level 3
    vector<pair<Board, vector<Move> > > calculated_moves = next_move(player_number);

    int h;
    pair<Board, vector<Move> > move_to_play;

    if(depth == 1){

        if(player_number == -1){h = beta;}
        else{h = alpha;}

        if(player_number == -1){
            for(int i = 0; i< calculated_moves.size(); i++){
                if(calculated_moves[i].first.heuristic() < h){
                    move_to_play.first = calculated_moves[i].first.copy_board();
                    move_to_play.second = calculated_moves[i].second;
                    h = calculated_moves[i].first.heuristic();
                    if(h <= alpha){break;}
                }
            }
        }
        else if(player_number == 1){
            for(int i = 0; i< calculated_moves.size(); i++){
                if(calculated_moves[i].first.heuristic() > h){
                    move_to_play.first = calculated_moves[i].first.copy_board();
                    move_to_play.second = calculated_moves[i].second;
                    h = calculated_moves[i].first.heuristic();
                    if(beta <= h){break;}
                }
            }
        }
    }

    else if(depth > 1){

        if(player_number == -1){h = beta;}
        else{h = alpha;}

        int a = alpha;
        int b = beta;

        if(player_number == -1){
            vector<pair<Board, vector<Move> > > depth_moves;
            for(int i = 0; i< calculated_moves.size(); i++){
                pair<Board, vector<Move> > move_in_depth = calculated_moves[i].first.bot_move(1,depth-1,a,b);
                b = move_in_depth.first.heuristic();
                depth_moves.push_back(move_in_depth);
                if(b <= a){break;}
            }
            for(int i = 0; i< depth_moves.size(); i++){
                if(depth_moves[i].first.heuristic() < h){
                    move_to_play.first = calculated_moves[i].first.copy_board();
                    move_to_play.second = calculated_moves[i].second;
                    h = depth_moves[i].first.heuristic();
                }
            }
        }
        else if(player_number == 1){
            vector<pair<Board, vector<Move> > > depth_moves;
            for(int i = 0; i< calculated_moves.size(); i++){
                pair<Board, vector<Move> > move_in_depth = calculated_moves[i].first.bot_move(-1,depth-1,a,b);
                a = move_in_depth.first.heuristic();
                depth_moves.push_back(move_in_depth);
                if(a <= b){break;}
            }
            for(int i = 0; i< depth_moves.size(); i++){
                if(depth_moves[i].first.heuristic() > h){
                    move_to_play.first = calculated_moves[i].first.copy_board();
                    move_to_play.second = calculated_moves[i].second;
                    h = depth_moves[i].first.heuristic();
                }
            }
        }
    }

    return move_to_play;

///////////////////////////
}

pair<Board, vector<Move> > Board::bot_depth(int player_number, int move_number){
    // if(move_number<=5){ return bot_move(player_number,1, -10000, 10000);}
    // else if(move_number <= 20){ return bot_move(player_number,3, -10000, 10000);}
    // else if(move_number <= 45){ return bot_move(player_number,4, -10000, 10000);}
    // else{ return bot_move(player_number,5, -10000, 10000);}
    return bot_move(player_number,4, -1000000, 1000000);
}

Board Board::copy_board(){

    Board b = Board(board_size, game_rings, markers_per_ring);
    b.ring1 = ring1;
    b.ring2 = ring2;
    b.marker1 = marker1;
    b.marker2= marker2;
    b.ring1_removed = ring1_removed;
    b.ring2_removed = ring2_removed;
    b.game_rings = game_rings;
    b.markers_per_ring = markers_per_ring;

    b.player1_rings = player1_rings;
    b.player2_rings = player2_rings;
    b.board_storage = board_storage;
    return(b);
}

bool Board::check_valid_position(int x, int y){
    if(x == 0){
        if(y >= -(game_rings-1) && y <= (game_rings-1)){ return true;}
        else{return false;}
    }

    else if(x == 1){
        if(y >= -(game_rings-1) && y <= (game_rings)){ return true;}
        else{return false;}
    }
    else if(x == 2){
        if(y >= -(game_rings-2) && y <= (game_rings)){ return true;}
        else{return false;}
    }
    else if(x == 3){
        if(y >= -(game_rings-3) && y <= (game_rings)){ return true;}
        else{return false;}
    }
    else if(x == 4){
        if(y >= -(game_rings-4) && y <= (game_rings)){ return true;}
        else{return false;}
    }
    else if(x == 5){
        if(game_rings == 5){
            if(y >= 1 && y <= 4){ return true;}
            else{return false;}
        }
        else if(game_rings == 6){
            if(y >= -1 && y <= 6){ return true;}
            else{return false;}
        }
    }
    else if(x == 6){
        if(game_rings == 5){return false;}
        else if(game_rings == 6){
            if(y >= 1 && y <= 5){return true;}
            else{return false;}
        }
    }

    else if(x == -1){
        if(y >= -(game_rings) && y <= (game_rings-1)){ return true;}
        else{return false;}
    }
    else if(x == -2){
        if(y >= -(game_rings) && y <= (game_rings-2)){ return true;}
        else{return false;}
    }
    else if(x == -3){
        if(y >= -(game_rings) && y <= (game_rings-3)){ return true;}
        else{return false;}
    }
    else if(x == -4){
        if(y >= -(game_rings) && y <= (game_rings-4)){ return true;}
        else{return false;}
    }
    else if(x == -5){
        if(game_rings == 5){
            if(y >= -4 && y <= -1){ return true;}
            else{return false;}
        }
        else if(game_rings == 6){
            if(y >= -6 && y <= 1){ return true;}
            else{return false;}
        }
    }
    else if(x == -6){
        if(game_rings == 5){return false;}
        else if(game_rings == 6){
            if(y >= -5 && y <= -1){return true;}
            else{return false;}
        }
    }

    else{
        return false;
    }

}

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
    return board_storage[x+game_rings][y+game_rings];
}

void Board::set_position(int x, int y, int value){

    int init_value = get_position(x, y);

    board_storage[x+game_rings][y+game_rings] = value;

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

vector<int> Board::startend(int x1, int y1, int x2, int y2, int value){
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
    return ls;
}

vector<int> Board::moves_row(vector<int> ls){
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
// it gives the number of sets of 1,2,3 to board_size-1
vector<int> Board::marker_set(int s1,int f1, int s2, int f2, int value){
    vector<int> coord = marker_coordinate_feature(s1,f1,s2,f2,value);
    int n = markers_per_ring;
    vector<int> marker_fet(n,0);
    if(coord.size()==1)return marker_fet;
    for(int i=0;i<coord.size()/4;i++){
        int ydiff = coord[i+3] - coord[i+1];
        int xdiff = coord[i+2] - coord[i];
        int size = max(xdiff,ydiff)+1;
        if(size>=n-1)marker_fet[n-1] += 1;
        else marker_fet[size] += 1;
    }
    return marker_fet;
}

vector<int> Board::total_marker_sets(int value){
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

int eval(vector<int> feat){
    int n = feat.size();
    int score;
    if(n==5){
        score = feat[1]*1+feat[2]*3+feat[3]*25+feat[4]*300;
    }
    if(n==6){
        score = feat[1]*1+feat[2]*3+feat[3]*25+feat[4]*300+feat[5]*4000;
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
    clock_t t1 = clock();
    vector<int> feature1 = total_marker_sets_h(-1);
    vector<int> feature2 = total_marker_sets_h(1);
    int score = eval(feature2) - eval(feature1);
    score += 10000*(ring2_removed - ring1_removed);
    // score += 1000*(num_moves(1) - num_moves(-1));
    return score;
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
}

////////////////////OLD HEURISTIC
    int Board::row_marker(int x1, int y1, int x2, int y2, int value){
        int x = x2 - x1;
        int y = y2 - y1;
        int size = max(x,y);
        if(x != 0)x = 1;
        if(y != 0)y = 1;
        int check = 0;
        int firstx, firsty,lastx,lasty;
        int sum = 0;
        for(int i = 0; i <= size; i++){
            int xcurr = x1+i*x;
            int ycurr = y1+i*y;
            if(check==0){
                if(get_position(xcurr,ycurr) == value || get_position(xcurr,ycurr) == 2*value){
                    firstx = xcurr;
                    firsty = ycurr;
                    check = 1;
                }
            }
            if(check==1){
                if(get_position(xcurr,ycurr) == value || get_position(xcurr,ycurr) == 2*value){
                    lastx = xcurr;
                    lasty = ycurr;
                    if((xcurr == x2) && (ycurr == y2)){
                        int el = max((x2-firstx+1),(y2-firsty+1));
                        sum = sum + (int)pow(el,3.0);
                    }
                    
                }
                else if(get_position(xcurr,ycurr) != value){
                    check = 0;
                    int el = max(xcurr-firstx,ycurr-firsty);
                    sum = sum + (int)pow(el,3.0);
                    //ls.push_back(max(xcurr-firstx,ycurr-firsty));
                }
            }
        }
        return sum;
    }

    int Board::board_marker(int value){
        int sum = 0;

        int veco = row_marker(1,5,4,5,value);
        sum += veco;

        for(int i = 1; i < 5; i++){
            int vec = row_marker(-i,5-i,5,5-i,value);
            sum += vec;    
        }

        veco = row_marker(-4,0,4,0,value);
        sum += veco;

        for(int i = 1; i < 5; i++){
            int vec = row_marker(-5,-i,5-i,-i,value);
            sum += vec;
        }

        veco = row_marker(-4,-5,-1,-5,value);
        sum += veco;

        //////////////////////////

        veco = row_marker(-5,-4,-5,-1,value);
        sum += veco;

        for(int i = 1; i < 5; i++){
            int vec = row_marker(i-5,-5,i-5,i,value);
            sum += vec;
        }

        veco = row_marker(0,-4,0,4,value);
        sum += veco;

        for(int i = 1; i < 5; i++){
            int vec = row_marker(i,i-5,i,5,value);
            sum += vec;
        }
        
        veco = row_marker(5,1,5,4,value);
        sum += veco;

        //////////////////////////

        veco = row_marker(-4,1,-1,4,value);
        sum += veco;

        for(int i = 1; i < 5; i++){
            int vec = row_marker(-5,-i,i,5,value);
            sum += vec;
        }

        veco = row_marker(-4,-4,4,4,value);
        sum += veco;

        for(int i = 1; i < 5; i++){
            int vec = row_marker(i-5,-5,5,5-i,value);
            sum += vec;
        }

        veco = row_marker(1,-4,4,-1,value);
        sum += veco;

        return sum;

    }

    // int Board::heuristic(){
    //     int score = board_marker(1) - board_marker(-1);
    //     score += 3000*(ring2_removed - ring1_removed);
    //     return score;
    // }
//////////////////////////////////

void Board::print_board(){

    if(game_rings == 5){
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
    }

    cout << '\n';
    cout << "player 1 rings: " << ring1 << '\n' << "player 2 rings: " << ring2 << '\n';
    cout << "player 1 markers: " << marker1 << '\n' << "player 2 markers: " << marker2 << '\n';
    cout << "player 1 rings removed: " << ring1_removed << '\n' << "player 2 rings removed: " << ring2_removed << '\n' << '\n';
}

int main(){
//////////////// SERVER ///////////
    string game_data;
    getline(cin, game_data);

    vector<string> game_data_vector;

    stringstream data(game_data);
    string line;

    while(getline(data,line,' ')){
        game_data_vector.push_back(line);
    }

    int player_number;

    if(stoi(game_data_vector[0]) == 1){ player_number = -1;}
    else{player_number = 1;}
    
    int number_of_rings = stoi(game_data_vector[1]);
    int game_time = stoi(game_data_vector[2]);

    // Board(N, M, K)
    // N=board size, M=starting rings, K=consecutive markers to remove ring.
    Board my_board = Board(5,5,5);

    ofstream ofs;
    ofs.open("log.txt", ofstream::out | ofstream::trunc);
    ofs.close();

    ofstream myfile;
    myfile.open ("log.txt",ios::app);
    
    string opponent_move;
    int ply_number = 0;

    if(player_number == 1){
        getline(cin, opponent_move);
        vector<Move> op_move_list = my_board.get_move(opponent_move);
        myfile << opponent_move <<"\n";
        my_board.execute_move(op_move_list, -1);
    }

    myfile.close();

    while(true){
        ply_number += 1;
        myfile.open ("log.txt",ios::app);
        pair<Board, vector<Move> > my_move = my_board.bot_depth(player_number,ply_number);
        my_board.execute_move(my_move.second,player_number);
        myfile << my_board.server_output(my_move) << "\n";
        cout << my_board.server_output(my_move) <<endl;

        getline(cin, opponent_move);
        vector<Move> op_move_list = my_board.get_move(opponent_move);
        myfile << opponent_move <<"\n";
        my_board.execute_move(op_move_list, -1*player_number);

        myfile.close();
    }
///////////////////////////////////
    return(1);


    // ifstream infile("thefile.txt");
    // string line;
    // Board my_board = Board(5,5,5);
    // int player_number = -1;
    // my_board.print_board();

//////////////// FILE INPUT////////
    // while (getline(infile, line))
    // {
    //     if(player_number == -1){ cout<<"Player 1 moves : ";}
    //     else{cout<<"Player 2 moves : ";}
    //     vector<Move> movvllist = my_board.get_move(line);
    //     my_board.execute_move(movvllist, player_number);
    //     my_board.print_board();
    //     player_number *= -1;
    // }
///////////////////////////////////

//////////////// BOT///////////////

    // pair<Board, vector<Move> > b;

    // for(int i = 0; i < 100; i++){
    //     if(player_number == -1){ cout<<"Player 1 moves : ";}
    //     else{cout<<"Player 2 moves : ";}

    //     b = my_board.bot_move(player_number,100);
    //     my_board.execute_move(b.second,player_number);
    //     cout<< my_board.server_output(b) << endl;

    //     string s;
    //     getline(cin, s);
    //     if(s != "a"){break;}
    //     my_board.print_board();
    //     player_number *= -1;
    //  }
///////////////////////////////////

//////////////// DEBUG/////////////
    // vector<Board> possible_moves;

    // vector<Direction> directions;
    // directions.push_back(Direction(0,1));
    // directions.push_back(Direction(1,1));
    // directions.push_back(Direction(1,0));
    // directions.push_back(Direction(0,-1));
    // directions.push_back(Direction(-1,-1));
    // directions.push_back(Direction(-1,0));
    
    // for(int r = 0; r < my_board.player1_rings.size(); r++){
    //     cout << my_board.player1_rings[r][0] << "," << my_board.player1_rings[r][1] << " -> \n";

    //     for(int i = 0; i < directions.size(); i++){
            
    //         int x2 = my_board.player1_rings[r][0] + directions[i].xchange;
    //         int y2 = my_board.player1_rings[r][1] + directions[i].ychange;

    //         while(my_board.check_valid_position(x2, y2)){

    //             if(my_board.get_position(x2,y2) == -2*player_number || my_board.get_position(x2,y2) == 2*player_number){break;}
    //             else if(my_board.get_position(x2,y2) == 0){
    //                 Board new_board = my_board.copy_board();
    //                 vector<Move> new_moves;
    //                 new_moves.push_back(Move("S", my_board.player1_rings[r][0], my_board.player1_rings[r][1]));
    //                 new_moves.push_back(Move("M", x2, y2));
    //                 new_board.execute_move(new_moves, player_number);
    //                 possible_moves.push_back(new_board);
    //             }
    //             cout << "\t" <<x2 << "," << y2 << "\n";

    //             x2 += directions[i].xchange;
    //             y2 += directions[i].ychange;
    //         }
    //     }
    // }

    // for(int i = 0; i < possible_moves.size(); i++){
    //     possible_moves[i].print_board();
    // }
///////////////////////////////////

//////////////// PLAY GAME MANUAL//
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
///////////////////////////////////
}