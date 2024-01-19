#include <iostream>

using namespace std;


constexpr int FIELD_SIZE = 5;
const int BIGGEST_BOAT_SIZE = 5;

bool isValidIndex(size_t size, int x, int y)
{
	return (x >= 0 && x <= size - 1 && y >= 0 && y <= size - 1);
}

/// Basic Matrix Functions
void fillRowWith(char* arr, size_t size, char ch)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = ch;
	}
}
int** createField(size_t size)
{
	int** field = new int* [size];
	for (int i = 0; i < size; i++)
	{
		field[i] = new int[size] {0};
	}


	return field;
}
char** createView(size_t size)
{
	char** field = new char* [size];
	for (int i = 0; i < size; i++)
	{
		field[i] = new char[size];
		fillRowWith(field[i], size, '-');
	}
	return field;
}

void freeMatrix(int** mtx, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		delete[] mtx[i];
	}
	delete[] mtx;
}
void freeMatrix(char** mtx, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		delete[] mtx[i];
	}
	delete[] mtx;
}

void printField(int** field, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			cout << field[i][j] << ' ';
		}
		cout << endl;
	}

}
void printField(char** field, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			cout << field[i][j] << ' ';
		}
		cout << endl;
	}

}
/// ---Basic Matrix Functions

/// Sinking Ships
bool checkIfSunkStep(int** field, size_t size, int x, int y, int shipId, int stepX, int stepY)
{
	while (isValidIndex(size, x+stepX, y+stepY) && field[x][y] == field[x + stepX][y + stepY])
	{
		x += stepX;
		y += stepY;
	}
	
	return !isValidIndex(size, x + stepX, y + stepY) || field[x][y] != -field[x + stepX][y + stepY];

}
bool checkIfSunk(int** field, size_t size, int x, int y, int shipId)
{

	if (!isValidIndex(size, x, y)) return false;

	return
		(checkIfSunkStep(field, size, x, y, shipId, 1, 0)) &&
		(checkIfSunkStep(field, size, x, y, shipId, -1, 0)) &&
		(checkIfSunkStep(field, size, x, y, shipId, 0, 1)) &&
		(checkIfSunkStep(field, size, x, y, shipId, 0, -1));


}
void sinkShip(char** view, int** field, size_t size, int shipId)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (field[i][j] == shipId) view[i][j] = 'X';
		}
	}
}
///


/// Shooting
char shootAt(int** field, size_t size, int x, int y) /// return codes: {0=>miss,*=>hit,X=>sunk}
{
	if(field[x][y]>0)
		field[x][y] = -field[x][y];
	
	if (field[x][y] < 0)
	{
		if (checkIfSunk(field, size, x, y, field[x][y]))
		{
			return 'X';
		}
		return '*';
	}
	return '0';
}
bool isLoser(int** field, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (field[i][j] > 0) return false;
		}
	}
	return true;
}
void shootPhase(int** playerOneField, char** playerOneView, int** playerTwoField, char** playerTwoView, size_t size, bool isPlayerTwoHuman)
{
	bool evenTurn = false;
	while (true)
	{
		system("CLS");
		cout << "Player " << evenTurn + 1 << "'s turn...\n\n";

		char** currPlayerView = nullptr;
		if (!evenTurn) currPlayerView = playerOneView;
		else currPlayerView = playerTwoView;

		int** currOpponentField = nullptr;
		if (!evenTurn) currOpponentField = playerTwoField;
		else currOpponentField = playerOneField;


		printField(currPlayerView, size);

		int x, y;

		if (isPlayerTwoHuman || !evenTurn)
		{
			cout << "Shoot at:\n";
			cin >> x >> y;
			x--; y--;
		}

		else
		{
			x = rand() % size;
			y = rand() % size;
		}

		currPlayerView[x][y] = shootAt(currOpponentField, size, x, y);
		if (currPlayerView[x][y] == 'X')
		{
			sinkShip(currPlayerView, currOpponentField, size,  currOpponentField[x][y]); // tuka moje da se optimizira da ne hodi pak prez cqloto
		}

		if (isLoser(currOpponentField, size))
		{
			system("CLS");
			cout << "Player " << evenTurn + 1 << " WON!\n";
			printField(currPlayerView,size);
			cout << "Good game...\n";
			return;
		}


		system("CLS");
		cout << "Player " << evenTurn + 1 << "'s turn...\n\n";
		printField(currPlayerView, size);

		switch (currPlayerView[x][y])
		{
		case '0':
			cout << "Miss...\n";
			break;
		case '*':
			cout << "HIT!\n";
			break;
		case 'X':
			cout << "SHIP SUNK!\n";
			break;
		}

		cout << "Press enter to continue...";
		getchar();
		if(!isPlayerTwoHuman && !evenTurn)
			getchar();

		evenTurn = !evenTurn;
	}
}
/// ---Shooting




/// Placing Boats
void placeBoat(int** field, size_t size, int x, int y, int boatSize, char orientation, int boatId)
{
	for (int i = 0; i < boatSize; i++)
	{
		if (orientation == 'V')
		{
			field[x][y] = boatId;
			x++;
		}
		else
		{
			field[x][y] = boatId;
			y++;
		}
	}
}
bool isSpaceFree(int** field, size_t size, int x, int y, int boatSize, char orientation)
{
	for (int i = 0; i < boatSize; i++)
	{
		if (!isValidIndex(size, x, y)) return false;
		if (field[x][y] == 0)
		{
			if (orientation == 'V') x++;
			else if (orientation == 'H') y++;
			
		}
		else return false;
	}
	return true;
}
void placeBoatsPhase(int** field, size_t size,int* boatCount, int playerId)
{

	int currBoatId = 1;
	for (int i = BIGGEST_BOAT_SIZE; i > 0; i--)
	{

		system("CLS");
		cout << "Player " << playerId << "'s turn\n";
		if (boatCount[i] == 0) continue;

		printField(field, size);
		cout << "You are currently placing a " << i << " size boat, choose orientation\n[H = Horizontal] [V = Vertical]:\n";

		char orientation = ' ';
		cin >> orientation;

		int x=0, y=0;

		cout << "Enter coordinates: ";
		cin >> x >> y;
		x--; y--;

		if (!isSpaceFree(field, size, x, y, i, orientation))
		{
			cout << "Not enough space try again...\n";
		}


		else
		{
			boatCount[i]--;
			placeBoat(field, size, x, y, i, orientation, currBoatId++);
		}



		if (boatCount[i] != 0) i++;
	}


	system("CLS");

}
void placeBoatsPhaseComputer(int** field, size_t size, int* boatCount)
{
	int currBoatId = 1;
	for (int i = BIGGEST_BOAT_SIZE; i > 0; i--)
	{

		if (boatCount[i] == 0) continue;

		char choicesForOrientation[2] = { 'V','H' };
		char orientation = choicesForOrientation[rand() % 2];

		int x = 0, y = 0;
		x = rand() % size;
		y = rand() % size;

		if (isSpaceFree(field, size, x, y, i, orientation))
		{
			boatCount[i]--;
			placeBoat(field, size, x, y, i, orientation, currBoatId++);
		}

		if (boatCount[i] != 0) i++;
	}
}
/// ---Placing Boats

/// Game Logic
void gameLogic(size_t size,bool isPlayerTwoHuman, int* boatCountOne, int* boatCountTwo)
{
	int** playerOneField = createField(size);
	int** playerTwoField = createField(size);
	
	char** playerOneView = createView(size);
	char** playerTwoView = createView(size);


	placeBoatsPhase(playerOneField, size, boatCountOne,1); //Player One Places Boats

	if (isPlayerTwoHuman)
		placeBoatsPhase(playerTwoField, size, boatCountTwo,2); //Player Two Places Boats
	else
		placeBoatsPhaseComputer(playerTwoField, size, boatCountTwo);

	shootPhase(playerOneField,playerOneView,playerTwoField,playerTwoView, size,isPlayerTwoHuman);




	freeMatrix(playerOneField,size);
	freeMatrix(playerTwoField,size);
	freeMatrix(playerOneView,size);
	freeMatrix(playerTwoView,size);
	
}
/// ---Game Modes

void getGamemode(int& mode)
{
	cout << "Choose gamemode:\n";
	cout << "1. PLAYER vs COMPUTER\n";
	cout << "2. PLAYER vs PLAYER\n";
	mode = 0;
	while (mode != 1 && mode != 2) cin >> mode;
}
void getFieldSize(int& size)
{
	cout << "Enter field size: ";
	size = 0;
	cin >> size;
}
void getBoatCounts(int* boatCountOne, int* boatCountTwo)
{
	boatCountOne[0] = boatCountTwo[0] = 0;
	boatCountOne[1] = boatCountTwo[1] = 0;

	int temp;
	cout << "How many boats of each size:\n";
	cout << "Size 2:\n";
	cin >> temp;
	boatCountOne[2] = boatCountTwo[2] = temp;
	cout << "Size 3: ";
	cin >> temp;
	boatCountOne[3] = boatCountTwo[3] = temp;
	cout << "Size 4: ";
	cin >> temp;
	boatCountOne[4] = boatCountTwo[4] = temp;
	cout << "Size 5: ";
	cin >> temp;
	boatCountOne[5] = boatCountTwo[5] = temp;
}


int main()
{
	srand(time(0));
	int mode = 0,size=0;
	getGamemode(mode);
	system("CLS");
	getFieldSize(size);
	system("CLS");
	int boatCountOne[6], boatCountTwo[6];
	getBoatCounts(boatCountOne, boatCountTwo);
	system("CLS");
	gameLogic(size,mode-1,boatCountOne,boatCountTwo);

	return 0;
}


/*
indexite pri placevaneto sa za opraqne - Done
da e po izchisteno kato printva - Done
vs cpu logic // isplayertwo human i trqq da napraq da si slaga korabite na random - Done
da se chetat broq korabi - Done

Bugs:
*sunkvaneto ne go promeni vs na X - Done

*/