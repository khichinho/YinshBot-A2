vector<int> remove_markers;
int ring_remove_amount = 0;

vector<vector<int> > player_rings;
if(player_number == -1) { player_rings = player1_rings;}
else if(player_number == 1){ player_rings = player2_rings;}

vector<pair<Board, vector<Move> > > to_move;

remove_markers = cons_marker(player_number);

if(remove_markers.size > 1){
    for(int i = 0; i < 4; i++){
        Move rs("RS",remove_markers[0],remove_markers[1]);
        Move re("RE",remove_markers[2],remove_markers[3]);
        
        for(int j = 0; j < player_rings.size(); j++){
            Move x("X",player_rings[j][0],player_rings[j][1]);
            vector<Move> rsrex;
            rsrex.push_back(rs);
            rsrex.push_back(re);
            rsrex.push_back(x);

            Board after_remove = copy_board();
            after_remove.execute_move(rsrex,player_number);

            vector<pair<Board, vector<Move> > > after_remove_moves = after_remove.all_moves(player_number);

            for(int k = 0; k < after_remove_moves.size(); k++){
                vector<Move> till_remove = rsrex;
                till_remove.insert(till_remove.end(), after_remove_moves[k].second.begin(), after_remove_moves[k].second.end());
                after_remove_moves[k].second = till_remove;
            }

            for(int l = 0; l < after_remove_moves.size(); l++){
                vector<int> remove_markers2;
                remove_markers2 = after_remove_moves[l].first.cons_marker(player_number);
            }
            // to_move.insert(to_move.end(), after_remove_moves.begin(), after_remove_moves.end());
        }
    }
}