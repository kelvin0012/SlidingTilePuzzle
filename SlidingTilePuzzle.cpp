#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>

using namespace std;

struct TileState {
    int board[7]; // board state for tiles
    int g;         
    int h;
    int f;
    int emptySpace; // empty space location
    TileState* parent; // parent node of tilestate
};

// struct for operator overloading f cost
struct CompareCost {
    bool operator()(TileState const& ts1, TileState const& ts2) {
        return ts1.f > ts2.f; 
    }
};

// calculate heuristic for the sum of black tiles on each white tiles left side (so h cost goes lower the closer to goal)
int calcHeuristic(int board[7]) {
    int heuristic = 0;
    for (int i = 0; i < 7; i++) {
        if (board[i] == 1) // find white tile
            for (int j = 0; j < i; j++) // count black tiles on the left side
                if (board[j] == 2) // if found black tile
                    heuristic+=1;    
    }
    return heuristic;
}

// check if goal is reached
bool isGoal (int board[7]) {
    if (calcHeuristic(board) == 0)
        return true;
    return false;
}

vector<TileState> generateSuccessors(TileState currentTile) {
    vector<TileState> successors;
    // get empty space
    int emptySpace = currentTile.emptySpace;

    // initialise possible moves/jumps
    int moves[] = {-3, -2, -1, 1, 2, 3};

    // generate successors / possible moves
    for (int i = 0; i < 6; i++) {
        // move empty space
        int newEmptySpace = emptySpace + moves[i];

        // check within bounds
        if (newEmptySpace >= 0 && newEmptySpace < 7) {
            TileState successor = currentTile; // make successor
            swap(successor.board[emptySpace], successor.board[newEmptySpace]); // swap empty tile with the move
            successor.emptySpace = newEmptySpace;
            successor.parent = new TileState(currentTile); // set parent as currentTile

            // parent distance + distance between parent and successor
            if (abs(moves[i]) == 1 || abs(moves[i]) == 2) // add relevant move cost where 1 and 2 = cost 1 and 3 = cost 2
                successor.g = successor.parent->g + 1; 
            else
                successor.g = successor.parent->g + 2;

            successor.h = calcHeuristic(successor.board);
            successor.f = successor.g + successor.h; // compute total f cost
            
            successors.push_back(successor); // add successor
        }
    }
    return successors;
}

// Store tile board as string to track visits
string stringTileBoard(TileState currentTile) {
    string stringBoard = "";
    for (int i = 0; i < 7; i++)
        stringBoard += to_string(currentTile.board[i]); // convert to int to string and append it
    return stringBoard;
}

// Print tile board
void printBoard(int board[7]) {
    for (int i = 0; i < 7; i++) {
        if (board[i] == 1)
            cout << "W";
        else if (board[i] == 2)
            cout << "B";
        else
            cout << "_";
    }
    cout << endl;
};

// print solution path
void printSolutionPath(TileState currentTile) {
    cout << "Printing Solution Path" << endl;
    // add solution path of the puzzle through parent puzzle nodes
    vector<TileState> solutionPath;
    while (currentTile.parent != nullptr) { // add parent puzzle nodes to a vector until initial node reached
        solutionPath.push_back(currentTile);
        currentTile = *currentTile.parent; 
    }
    solutionPath.push_back(currentTile);

    // print solution path from initial to goal
    for (int i = solutionPath.size() - 1; i >= 0; i--) {
        if (i == solutionPath.size() - 1) 
            cout << "Initial State:" << endl;
        else {
            cout << "Move " << solutionPath.size() - i - 1 << ":" << endl;
            cout << "Heuristic Cost: " << solutionPath[i].h << endl;
        }
        printBoard(solutionPath[i].board);
    }
}

// A* Algorithm
void aStarSearch(int board[7]) {
    TileState initialTile;
    for (int i = 0; i < 7; i++) {
        initialTile.board[i] = board[i];
    }
    initialTile.g = 0;
    initialTile.h = calcHeuristic(initialTile.board);
    initialTile.f = 0; // leave total cost as 0
    initialTile.emptySpace = 3; // initial tile state is the same
    initialTile.parent = nullptr;

    priority_queue<TileState, vector<TileState>, CompareCost> openList;
    unordered_map<string, bool> closedList; // visited
    openList.push(initialTile);

    while (!openList.empty()) {
        TileState currentTile = openList.top();
        openList.pop();
    
        // check if is in goal states
        if (isGoal(currentTile.board)) { 
            int totalCost = currentTile.f;
            printSolutionPath(currentTile);
             cout << "Total Cost: " << totalCost << endl;
            return;
        }
        
        // skip if already visited
        if (closedList[stringTileBoard(currentTile)]) { // convert board to string 
            continue;
        }
        closedList[stringTileBoard(currentTile)] = true;

        // Generate successors and possible moves
        vector<TileState> successors = generateSuccessors(currentTile);
        for (int i = 0; i < successors.size(); i++) {
            TileState successor = successors[i];
            if (!closedList[stringTileBoard(successor)])
                openList.push(successor);
        }
    }
    cout << "No Solution Found" << endl;
    return;
}


int main() {
    // B = 2, W = 1, Empty = 0
    int initial[7] = {2,2,2,0,1,1,1};

    // run algorithm
    aStarSearch(initial);


    return 0;
}