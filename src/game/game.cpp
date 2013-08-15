#include "game.h"

using namespace std;

void DebugGame::start()
{
	string command("");
	bool game_finished(false);
	DebugGameData d(Board(), ConsolePlayer("Spongebob"), ConsolePlayer("Patrick"));

	cout << "==- START DEBUG GAME -==" << endl << endl;
	cout << "The possible commands are: player1 (p1), player2 (p2), marker_player1 (mp1),"
		<< endl;
	cout <<	"                           marker_player2 (mp2), back (b), forward (f), exit (e)."
		<< endl;

	while(!game_finished){
		cout << endl << "Pass a command: ";
		cin >> command;

		if(command == "player1" || command == "p1"){
			game_finished = command_player(d, 0);
		}
		else if(command == "player2" || command == "p2"){
			game_finished = command_player(d, 1);
		}
		else if(command == "marker_player1" || command == "mp1"){
			game_finished = command_marker_player(d, 0);
		}
		else if(command == "marker_player2" || command == "mp2"){
			game_finished = command_marker_player(d, 1);
		}
		else if(command == "back" || command == "b"){
			command_back(d);
		}
		else if(command == "forward" || command == "f"){
			command_forward(d);
		}
		else if(command == "exit" || command == "e"){
			game_finished = true;
		}
		else{
			cout << "ERROR: The command you entered is not known." << endl;
		}
	}

	cout << endl << "==- END DEBUG GAME -==" << endl;
}

bool DebugGame::command_player(DebugGameData& d, PlayerID player_id)
{
	NodeLabel label;

	d.player[player_id].getNextMove(label);

	if(d.board.isNodeLabel(label))
		if(!d.board.nodeHasOwner(label)){
			placeGem(d, player_id, label);
			reverted_moves.clear();

			if(d.board.checkVictoryCondition(player_id)){
				cout << d.player[player_id].getName() << " wins!" << endl;
				return true;
			}
		}
		else{
			cout << "ERROR: This node is already taken." << endl;
		}
	else
		cout << "ERROR: The passed string is not a node label." << endl;

	return false;
}

void DebugGame::placeGem(DebugGameData& d, PlayerID player_id, NodeLabel label)
{
	vector<FaceLabel> new_markers;

	cout << "Placed a gem on " << label << "." << endl;
	d.board.placeGem(label, player_id, new_markers);
	moves.push_back(Move(label, player_id, true));

	for(unsigned int i=0; i<new_markers.size(); i++){
		cout << "A new marker of " << d.player[player_id].getName()
			<< " has been placed on " << new_markers[i] << "."
			<< endl;
	}
}

bool DebugGame::command_marker_player(DebugGameData& d, PlayerID player_id)
{
	FaceLabel label;

	d.player[player_id].getMarkerMove(label);

	if(d.board.isFaceLabel(label)){
		if(!d.board.faceHasOwner(label)){
			placeMarker(d, player_id, label);
			reverted_moves.clear();

			if(d.board.checkVictoryCondition(player_id)){
				cout << d.player[player_id].getName() << " wins!" << endl;
				return true;
			}
		}
		else{
			cout << "ERROR: This face is already taken." << endl;
		}
	}
	else{
		cout << "ERROR: The passed string is not a face label." << endl;
	}

	return false;
}

void DebugGame::placeMarker(DebugGameData& d, PlayerID player_id, FaceLabel label)
{
	cout << "Placed a marker on " << label << "." << endl;
	d.board.placeMarker(label, player_id);
	moves.push_back(Move(label, player_id, false));
}

void DebugGame::command_back(DebugGameData& d)
{
	if(!moves.empty()){
		Move const& last_move(moves.back());

		if(last_move.is_gem){
			removeGem(d, last_move);
		}
		else{
			removeMarker(d, last_move);
		}

		reverted_moves.push_back(last_move);
		moves.pop_back();
	}
	else{
		cout << "ERROR: There exists no move that can be reverted." << endl;
	}
}

void DebugGame::removeGem(DebugGameData& d, Move const& move)
{
	vector<FaceLabel> removed_markers;

	cout << "Removed the gem of " << d.player[move.owner_id].getName()
		<< " from " << move.label << "." << endl;
	d.board.removeGem(move.label, removed_markers);

	if(!removed_markers.empty()){
		cout << "Also removed the following markers: ";

		for(unsigned int i=0; i<removed_markers.size(); i++){
			cout << removed_markers[i] << " ";
		}
		cout << endl;
	}
}
	
void DebugGame::removeMarker(DebugGameData& d, Move const& move)
{
	cout << "Removed the marker of " << d.player[move.owner_id].getName()
		<< " from " << move.label << "." << endl;
	d.board.removeMarker(move.label);
}

void DebugGame::command_forward(DebugGameData& d)
{
	if(!reverted_moves.empty()){
		Move const& last_undone_move(reverted_moves.back());

		if(last_undone_move.is_gem){
			placeGem(d, last_undone_move.owner_id, last_undone_move.label);
		}
		else{
			placeMarker(d, last_undone_move.owner_id, last_undone_move.label);
		}

		reverted_moves.pop_back();
	}
	else{
		cout << "ERROR: There exists no move that can be redone." << endl;
	}
}
