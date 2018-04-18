//Programmer: Samuel Scheffler
//Description: A basic console based sliding tile puzzle.

#include <iostream>
#include <iomanip> 
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

using namespace std;

//Directional Constants
const int UP = 1;
const int DOWN = 2;
const int RIGHT = 3;
const int LEFT = 4;

//Puzzle Size Constants
const int ROWS = 3;
const int COLS = 3;

//Color Constants
const int GREEN = 10;
const int RED = 12;
const int WHITE = 15;

//Getting current console handle
HANDLE currentConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//PROTOTYPES

void printPuzzle(int puzzle[ROWS][COLS]);
/*
Precondition: puzzle parameter must be a legal 3x3 sliding tile puzzle with a blank space equal to 0.
Postcondition: color coded version of the puzzle gets printed, green elements represent a tile being
in the correct location, red elements represent a tile being in the wrong location
*/
bool slideTile(int(&puzzle)[ROWS][COLS], int direction);
/*
Precondition:  puzzle parameter must be a legal 3x3 sliding tile puzzle with a blank space equal to 0.
Direction must be one of the directional constants.
Postcondition: method returns either true or false depending on whether the move is legal.
Takes a 3x3 puzzle with a blank space,
and swaps the blank space with another element based on the given direction.
*/
void scrambleBoard(int(&puzzle)[ROWS][COLS]);
/*
Precondition: Puzzle parameter must be a legal 3x3 sliding puzzle with a blank space equal to 0.
Postcondition: The puzzle array gets scrambled 10000 times using legal moves, ensuring that it is solvable.
*/
bool isBoardSolved(int puzzle[ROWS][COLS]);
/*
Precondition: Puzzle parameter must be a legal 3x3 sliding puzzle with a blank space equal to 0.
Postcondition: The puzzle array gets evaluated, and a bool value is returned, true if
it is solved, false if it is not.

*/
void updatePuzzle(int(&puzzle)[ROWS][COLS], int puzzleBuffer[ROWS][COLS]);
/*
Precondition: puzzle and puzzle buffer parameters must be legal 3x3 sliding puzzles with a blank space equal to 0.
Postcondition: Elements of puzzle are set equal to elements in puzzleBuffer.
*/
void updatePuzzleOnScreen(int puzzle[ROWS][COLS], int puzzleBuffer[ROWS][COLS]);
/*
Precondition: puzzle and puzzle buffer parameters must be legal 3x3 sliding puzzles with a blank space equal to 0.
Postcondition: Puzzle on screen is updated based on differences in puzzle and puzzleBuffer.
*/
int checkTileLocation(int puzzle[ROWS][COLS], int r, int c);
/*
Precondition: Puzzle parameter must be a legal 3x3 sliding puzzle with a blank space equal to 0. 
r and c must be greater than or equal to 0, and less than 3.
Postcondition: The tile at r and c is evaluated, and an int is returned.
0 indicates that there is a blank space at the given location, 1 indicates element at the given location is in the correct spot, 
and -1 indicates that an element at the given location is in the incorrect spot.  
*/

template<class T>
void printXY(T output, int x, int y);
/*
Precondition: output must be a valid data type. x and y must be coordinated on current console.
Postcondition: Console cursor is temporarily moved to given x and y coordinates to print str,
the cursor then returns to its original location.
*/

int main()
{
	//Used to store user's keystroke
	char userInput;

	//3x3 Solved sliding puzzle with 0 representing the blank space.
	int puzzle[ROWS][COLS] = { { 1, 2, 3 },
	{ 4, 5, 6 },
	{ 7, 8, 0 } };

	//3x3 puzzle buffer, that is used to check if changes were made to the puzzle.
	int puzzleBuffer[ROWS][COLS] = { { 1, 2, 3 },
	{ 4, 5, 6 },
	{ 7, 8, 0 } };

	printPuzzle(puzzle);

	//checking to make sure starting board is solved.
	cout << "isBoardSolved(): ";

	if (isBoardSolved(puzzle)) {
		cout << "true" << endl;
	}

	else {
		cout << "false" << endl;
	}

	cout << "Press any key to begin . . ." << endl;
	_getch();

	cout << "\nScrambling the tiles . . ." << endl;
	scrambleBoard(puzzleBuffer);
	cout << "Scrambling complete, press any key to begin solving." << endl;

	updatePuzzle(puzzle, puzzleBuffer);

	_getch();

	system("CLS");

	printPuzzle(puzzle);

	cout << "isBoardSolved(): ";

	if (isBoardSolved(puzzle)) {
		cout << "true" << endl;
	}

	else {
		cout << "false" << endl;
	}

	cout << "\n(W -> UP | A -> LEFT | S -> DOWN | D-> RIGHT)" << endl;

	//Loop continues until the puzzle is solved.
	while (!isBoardSolved(puzzle)) {
		//Waiting for user input.
		userInput=_getch();

		//puzzleBuffer is modified according to user input.
		switch (userInput) {
			case 'W':
			case 'w':
				slideTile(puzzleBuffer, UP);

				break;
			case 'A':
			case 'a':
				slideTile(puzzleBuffer, LEFT);

				break;
			case 'S':
			case 's':
				slideTile(puzzleBuffer, DOWN);

				break;
			case 'D':
			case 'd':
				slideTile(puzzleBuffer, RIGHT);

				break;
		}
		
		//puzzle is compared to puzzleBuffer, if there are any changes, the screen is updated only in the locations where the changes occurred.
		updatePuzzleOnScreen(puzzle, puzzleBuffer);

		//The elements in puzzle are set equal to the elements in puzzleBuffer.
		updatePuzzle(puzzle, puzzleBuffer);

		//if the board is solved, the ture is output in the correct location in screen.
		if (isBoardSolved(puzzle)){
			printXY("True ", 17, 3);
		}
	}

	system("PAUSE");
	return 0;
}

void printPuzzle(int puzzle[ROWS][COLS]) {
	//Used to check whether tiles are in the correct location.
	int counter = 1;

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			//If a tile is in the correct location, it is marked green.
			if (puzzle[r][c] == counter && puzzle[r][c] != 0) {
				SetConsoleTextAttribute(currentConsole, GREEN);
			}
			//If a tile is in the incorrect location, it is marked red.
			else if (puzzle[r][c] != counter && puzzle[r][c] != 0) {
				SetConsoleTextAttribute(currentConsole, RED);
			}
			//Blank space is left white.
			else {
				SetConsoleTextAttribute(currentConsole, WHITE);
			}

			//Blank space tile is printed as '*'.
			if (puzzle[r][c] != 0) {
				cout << setw(3) << puzzle[r][c];
			}

			else {
				cout << setw(3) << "*";
			}


			counter++;
		}
		cout << endl;
	}

	SetConsoleTextAttribute(currentConsole, WHITE);
}

bool slideTile(int(&puzzle)[ROWS][COLS], int direction) {
	//Hold the Row and Col of the blank space.
	int blankRow;
	int blankCol;

	//Temporary place holder for swapping elements.
	int temp;

	//Locating blank space.
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			if (puzzle[r][c] == 0) {
				blankRow = r;
				blankCol = c;

				break;
			}
		}
	}

	//Swapping blank space location with another element based on given direction.
	switch (direction) {
	case UP:
		if (blankRow - 1 < 0) {
			return false;
		}

		temp = puzzle[blankRow - 1][blankCol];
		puzzle[blankRow - 1][blankCol] = puzzle[blankRow][blankCol];
		puzzle[blankRow][blankCol] = temp;

		break;

	case DOWN:
		if (blankRow + 1 >= ROWS) {
			return false;
		}

		temp = puzzle[blankRow + 1][blankCol];
		puzzle[blankRow + 1][blankCol] = puzzle[blankRow][blankCol];
		puzzle[blankRow][blankCol] = temp;

		break;

	case LEFT:
		if (blankCol - 1 < 0) {
			return false;
		}

		temp = puzzle[blankRow][blankCol - 1];
		puzzle[blankRow][blankCol - 1] = puzzle[blankRow][blankCol];
		puzzle[blankRow][blankCol] = temp;

		break;

	case RIGHT:
		if (blankCol + 1 >= COLS) {
			return false;
		}

		temp = puzzle[blankRow][blankCol + 1];
		puzzle[blankRow][blankCol + 1] = puzzle[blankRow][blankCol];
		puzzle[blankRow][blankCol] = temp;

		break;

	default:
		cout << "Invalid direction" << endl;
		return false;
	}

	return true;
}

void scrambleBoard(int(&puzzle)[ROWS][COLS]) {
	int direction;
	//Used to count the number of successful legal moves.

	int counter = 1;

	//initializing random seed.
	srand(time(NULL));

	while (counter <= 10000) {
		//Random number from 1 to 4 is generated, representing the 4 possible different directions.
		direction = rand() % 4 + 1;

		//Checking if legal move was successful, if so, counter is incremented. 
		if (slideTile(puzzle, direction)) {
			counter++;
		}
	}
}

bool isBoardSolved(int puzzle[ROWS][COLS]) {
	//Used to check whether tiles are in the correct location.
	int counter = 1;

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			//chekcs if a tile is in the wrong location, if so, function return false.
			if (puzzle[r][c] != counter && puzzle[r][c] != 0) {
				return false;
			}

			counter++;
		}
	}

	return true;
}

void updatePuzzle(int(&puzzle)[ROWS][COLS], int puzzleBuffer[ROWS][COLS]) {
	//Setting all elements in puzzle equal to the elements in puzzleBuffer
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			puzzle[r][c] = puzzleBuffer[r][c];
		}
	}
}

void updatePuzzleOnScreen(int puzzle[ROWS][COLS], int puzzleBuffer[ROWS][COLS]) {
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			//Checking if board has changed.
			if (puzzle[r][c] != puzzleBuffer[r][c]) {

				//Checking whether changed tiles are in the correct locations, and changing colors accordingly.
				if (checkTileLocation(puzzleBuffer, r, c) == 1) {
					SetConsoleTextAttribute(currentConsole, GREEN);
				}

				else if (checkTileLocation(puzzleBuffer, r, c) == -1) {
					SetConsoleTextAttribute(currentConsole, RED);
				}

				//Updating board on screen.
				if (puzzleBuffer[r][c] != 0) {
					printXY(puzzleBuffer[r][c], ((c + 1) * 3) - 1, r);
				}

				else {
					printXY("*", ((c + 1) * 3) - 1, r);
				}

				SetConsoleTextAttribute(currentConsole, WHITE);
			}
		}
	}
}

int checkTileLocation(int puzzle[ROWS][COLS], int r, int c) {
	//Ensuring r is in range.
	if (r>=3 || r<0) {
		cout << "Invalid row." << endl;
		return -1;
	}

	//Ensuring c is in range.
	else if (c >= 3 || c < 0) {
		cout << "Invalid col." << endl;
		return -1;
	}

	else {
		//If tile is a blank space, return 0.
		if (puzzle[r][c] == 0) {
			return 0;
		}

		//If tile is in its correct location, return 1.
		else if (puzzle[r][c] == ((r*COLS) + (c+1))) {
			return 1;
		}

		//If tile is incorrect location, return -1.
		else {
			return -1;
		}
	}
	
}

template <class T>
void printXY(T output, int x, int y) {
	//Ensuring coordinates are valid.
	if (x >= 0 && y >= 0) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD originalCoordinates;
		COORD newCoordinates;

		GetConsoleScreenBufferInfo(currentConsole, &csbi);

		//Getting original cursor location.
		originalCoordinates.X = csbi.dwCursorPosition.X;
		originalCoordinates.Y = csbi.dwCursorPosition.Y;

		//Getting newCoordinates from parameters.
		newCoordinates.X = x;
		newCoordinates.Y = y;

		//Setting cursor to new coordinates.
		SetConsoleCursorPosition(currentConsole, newCoordinates);
		cout << output;
		//Setting cursor back to original coordinates.
		SetConsoleCursorPosition(currentConsole, originalCoordinates);
	}

	else {
		cout << "(" << x << ", " << y << ") are invalid coordinates." << endl;
	}
}
