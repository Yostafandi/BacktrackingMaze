//Yousof Hosny
//November 27, 2017
//A maze game where the player must solve the mazes
//The game uses a backtracking algorithm for generating the maze
//The game is designed to be played in a windows console
//(for other operating systems change system("cls") to the operating stystem equivalent)
//Created for ICS4U1 November 27th, 2017


#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
//#include <vector>
//#include <string>
#include <stack>

using namespace std;



class Game {
private:
	bool gameOver;            //Main boolean for ending the game
	bool gameWon = false;     //Boolean for when player completes the maze
	bool backtoMenu = false;
	int WIDTH = 30;
	int HEIGHT = 30;

	int posx, posy;           //The player's x and y position

	int playerCollision[4];

	bool drawGeneration = false; //Boolean to render the maze generation (on or off)

public:

	Game();

	//Main Menu
	void drawMainmenu();
	void drawControls();

	//Game Win
	void drawGamewon();



	//Game map (ROWS)
	int **gameMap = new int*[WIDTH];


	void MainLoop();
	void Draw();
	void Input();
	void Logic();

	void testMap();
	void checkCollision();

	//MAZEEEEE
	void backtrackerMainloop();
	void isGood();
	void backtrackerMapinit();

	int moveHorizontal(int x, int direction);
	int moveVertical(int y, int direction);

	bool isGood(int x, int y, int direction);

	int cellX = 1;
	int cellY = 1;

	stack<int> xVals;
	stack<int> yVals;

};
void Game::drawMainmenu() { //Function that displays the main menu

	while (gameOver == false) {
		system("cls");
		char userInput;
		cout << "Maze Game" << endl;
		cout << "Enter 1 to start the game" << endl;
		cout << "Enter 2 to check the controls" << endl;
		cout << "Enter X to exit the game"<<endl;
		cin >> userInput;
		if (userInput == '1') {
			break;
		}
		else if (userInput == '2') {
			drawControls();
		}
		else if (userInput == 'x' || userInput == 'X') {
			gameOver = true;
			drawGeneration = false;
			break;
		}
		else {
			cout << endl << "Please enter a valid command";
			Sleep(2000);
		}

	}

}
void Game::drawControls() { //A function that draws the controls menu
	while (true){
		system("cls");
		cout << "Controls" << endl;
		cout << "Use the WASD keys to move around (Player is marked as '@' on the map)" << endl;
		cout << "W to move Up" << endl;
		cout << "A to move Left" << endl;
		cout << "S to move Down" << endl;
		cout << "D to move Right" << endl;
		cout << endl << "The goal is to reach the end point (marked as '$' on the map)"<<endl;
		cout << endl << "Enter X at any time to exit the game" << endl;
		cout << "Enter C to toggle the maze generation (off by default)" << endl;
		//cout << "Enter N to change the maze grid size" << endl;
		cout << endl << "Enter B at any time to go back to the main menu" <<endl;

		char userInput;
		cin >> userInput;

		if (userInput == 'x' || userInput == 'X') {
			gameOver = true;
			drawGeneration = false;
			break;
		}
		else if (userInput == 'b' || userInput == 'B') {
			break;
		}
		else if (userInput == 'c' || userInput == 'C') {
			drawGeneration = !drawGeneration;
			if (drawGeneration) {
				cout << endl << "Rendering maze generation has been toggled on";
				cout << endl << "This may cause slower loading speed";
				Sleep(2000);
			}
			else {
				cout << endl << "Maze generation toggled off";
				Sleep(2000);
			}

		}
		else {
			cout << endl << "Please enter a valid command";
			Sleep(2000);

		}
	}



}

void Game::drawGamewon() {
	while (true) {
		cout << "Congrats you beat the maze!" << endl;
		cout << "Enter B to go back to the main menu"<<endl;

		char userInput;
		cin >> userInput;

		if (userInput == 'b' || userInput == 'B') {
			break;
		}
		else if (userInput == 'x' || userInput == 'X') {
			gameOver = true;
			drawGeneration = false;
			break;
		}
		else {
			cout << endl << "Please enter a valid command";
			Sleep(2000);

		}

	}
}
void Game::backtrackerMainloop() {
	backtrackerMapinit();
	srand(time(0));
	int goodMoveCount = 0;
	int direction = 0;

	while (true){
		//find n good moves
		for (int i = 0; i < 4; i++) {
			if (isGood(cellX, cellY, i))
				goodMoveCount++;
		}

		// if only 1 good move, move there
		//Check if a direction is possible move
		//if so, move horizontal or vertically there
		if (goodMoveCount== 1) {
			if (isGood(cellX, cellY, 0))
				cellY = moveVertical(cellY, 0);
			else if (isGood(cellX, cellY, 2))
				cellY = moveVertical(cellY, 2);
			else if (isGood(cellX, cellY, 1))
				cellX = moveHorizontal(cellX, 1);
			else if (isGood(cellX, cellY, 3))
				cellX = moveHorizontal(cellX, 3);
		}

		// if no good moves, move back in stack
		else if (goodMoveCount == 0) {
			cellX = xVals.top();
			cellY = yVals.top();
			xVals.pop();
			yVals.pop();
		}

		//if more than 1 good move, push stack
		else if (goodMoveCount > 1) {
			xVals.push(cellX);
			yVals.push(cellY);

			//direction to move randomly chosen
			direction = rand() % 4;
			while (!isGood(cellX, cellY, direction)){
                direction = rand() % 4;
			}

			cellX = moveHorizontal(cellX, direction);
			cellY = moveVertical(cellY, direction);
		}

		// set grid
		gameMap[cellY][cellX] = 0;
		//output grid to show creation
		if (drawGeneration) {
			Draw();
		}
        if(xVals.empty()){
            break;
        }

		//reset goodMoveCount value
		goodMoveCount = 0;

	}

	//This sets the games endpoint to the bottom right corner of the map
	for(int i = HEIGHT -2; i >1; i--){
        if(gameMap[i][WIDTH -2]==0){
            gameMap[i][WIDTH -2] = 2;
            break;
        }
	}

}
void Game::backtrackerMapinit() {
    //This function initializes the game map so that every cell is a wall
    //this is so that the backtracker algorithm can work as it checks if a cell
    //is good by checking if its neighbours are walls
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j< WIDTH; j++) {
			gameMap[i][j] = 1;

		}
	}
	Draw();
}
int Game::moveHorizontal(int x, int direction) {
	//1 = right, 3 = left
	if (direction == 1)
		return(x + 1);
	else if (direction == 3)
		return(x - 1);
	else
		return(x);
}
int Game::moveVertical(int y, int direction) {
	//0 = up, 2 = down
	//Reminder: up = y-1
	if (direction == 0)
		return(y - 1);
	else if (direction == 2)
		return(y + 1);
	else
		return(y);
}
bool Game::isGood(int x, int y, int direction) {
	x = moveHorizontal(x, direction);
	y = moveVertical(y, direction);


	//checking if current is already a space or if it is the outer boundary
	if (gameMap[y][x] == 0 || x >= (WIDTH - 1) || x <= 0 || y <= 0 || y >= (HEIGHT - 1)) {
		return false;
	}

	//This checks if the neighbouring cells are all walls (depending on the direction the neighbouring cells will differ)
	else if (direction == 0) { //up
		if (gameMap[y][x - 1] != 0 && gameMap[y][x + 1] != 0 && gameMap[y - 1][x] != 0 && gameMap[y - 1][x - 1] != 0 && gameMap[y - 1][x + 1] != 0) {
			return true;

		}
	}
	else if (direction == 2) { //down
		if (gameMap[y][x - 1] != 0 && gameMap[y][x + 1] != 0 && gameMap[y + 1][x] != 0 && gameMap[y + 1][x - 1] != 0 && gameMap[y + 1][x + 1] != 0) {
			return true;
		}
	}
	else if (direction == 1) { //right
		if (gameMap[y - 1][x] != 0 && gameMap[y + 1][x] != 0 && gameMap[y][x + 1] != 0 && gameMap[y - 1][x + 1] != 0 && gameMap[y + 1][x + 1] != 0) {
			return true;
		}
	}
	else if (direction == 3) { //left
		if (gameMap[y - 1][x] != 0 && gameMap[y + 1][x] != 0 && gameMap[y][x - 1] != 0 && gameMap[y - 1][x - 1] != 0 && gameMap[y + 1][x - 1] != 0) {
			return true;
		}
	}
	return false;

}
Game::Game() {
	gameOver = false;

	posx = 1;
	posy = 1;

	//Game Map (COLUMNS)
	for (int i = 0; i<WIDTH; i++) {
		gameMap[i] = new int[HEIGHT];
	}

	MainLoop();
}
void Game::MainLoop() {

	while (gameOver == false) {

		//Resetting everything for more than one round
		posx = 1;
		posy = 1;
		gameWon = false;
		backtoMenu = false;

		drawMainmenu();
		backtrackerMainloop();
		if (gameOver == false) {
			Draw();
		}
		while (gameOver == false && backtoMenu == false) {

			checkCollision();
			Input();
			if (gameWon) {
				drawGamewon();
				break;
			}
		}
	}
}


void Game::testMap() {
    //This function is not used in the main program (created for testing collisions)
    //the function just generates a square map with outer boundaries
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j< WIDTH; j++) {

			if (j == 0) {
				gameMap[i][j] = 1;
			}
			else if (i == 0) {
				gameMap[i][j] = 1;
			}
			else if (j == WIDTH - 1) {
				gameMap[i][j] = 1;
			}
			else if (i == HEIGHT - 1) {
				gameMap[i][j] = 1;
			}
			else {
				gameMap[i][j] = 0;
			}
		}
	}
	Draw();//TEST

}
void Game::Draw() {

	system("cls"); //For linux type ("clear")
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j< WIDTH; j++) {
			if ((posx == j) && (posy == i)) { //player
				cout << '@';
			}
			else if (gameMap[i][j] == 1) { //wall
				cout << '#';
			}
			else if (gameMap[i][j] == 0) { //floor
				cout << ' ';

			}
			else if (gameMap[i][j] == 2) { //finish
				cout << '$';

			}
		}
		cout << endl;

	}
	Sleep(WIDTH / 2);
}
void Game::checkCollision() {

	//returns a number based on
	//0=up, 1=right, 2=down, 3=left
	if (gameMap[posy - 1][posx] == 1) {
		playerCollision[0] = 0;
	}
	else {
		playerCollision[0] = 1;
	}

	if (gameMap[posy + 1][posx] == 1) {
		playerCollision[2] = 0;
	}
	else {
		playerCollision[2] = 1;
	}
	if (gameMap[posy][posx + 1] == 1) {
		playerCollision[1] = 0;
	}
	else {
		playerCollision[1] = 1;
	}
	if (gameMap[posy][posx - 1] == 1) {
		playerCollision[3] = 0;
	}
	else {
		playerCollision[3] = 1;
	}
	if (gameMap[posy][posx] == 2) {
		gameWon = true;
	}
}
void Game::Input() {
	if (_kbhit()) { //This checks if an input is in the input buffer
		switch (_getch()) //This gets the character for the switch statement
		{
		case 'w': //up
			if (playerCollision[0] != 0) { posy--; }

			break;

		case 'a': //left
			if (playerCollision[3] != 0) { posx--; }
			break;

		case 's': //down reversed because of the printing system
			if (playerCollision[2] != 0) { posy++; }
			break;

		case 'd': //right
			if (playerCollision[1] != 0) { posx++; }
			break;
		case 'x': //Exiting the game
			gameOver = true;
			break;
		case 'b':
			backtoMenu = true;
			break;

		case 'W': //up
			if (playerCollision[0] != 0) { posy--; }

			break;

		case 'A': //left
			if (playerCollision[3] != 0) { posx--; }
			break;

		case 'S': //down reversed because of the printing system
			if (playerCollision[2] != 0) { posy++; }
			break;

		case 'D': //right
			if (playerCollision[1] != 0) { posx++; }
			break;
		case 'X': //Exiting the game
			gameOver = true;
			break;
		case 'B':
			backtoMenu = true;
			break;

		default:
			break;
		}
		Draw();
	}
}



void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

int main()
{
    //The following line makes the menu system not format correctly, however reduces flicker on the maze generation
    //ios::sync_with_stdio(false);


	ShowConsoleCursor(false);//this sets the cursor to false
	Game game;

	return 0;
}
