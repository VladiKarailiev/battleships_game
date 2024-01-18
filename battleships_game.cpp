#include <iostream>

using namespace std;


constexpr int FIELD_SIZE = 5;
const int BIGGEST_BOAT_SIZE = 5;

bool isValidIndex(size_t size, int x, int y)
{
	return (x >= 0 && x <= size - 1 && y >= 0 && y <= size - 1);
}


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
void shootPhase(int** playerOneField, char** playerOneView, int** playerTwoField, char** playerTwoView, size_t size)
{
	bool evenTurn = true;
	while (true)
	{
		cout << "Player " << evenTurn + 1 << "'s turn...\n\n";

		char** currPlayerView = nullptr;
		if (evenTurn) currPlayerView = playerOneView;
		else currPlayerView = playerTwoView;

		int** currOpponentField = nullptr;
		if (evenTurn) currOpponentField = playerOneField;
		else currOpponentField = playerTwoField;


		printField(currPlayerView, size);

		int x, y;
		cout << "Shoot at:\n";
		cin >> x >> y;


		currPlayerView[x][y] = shootAt(currOpponentField, size, x, y);
		if (currPlayerView[x][y] == 'X')
		{
			sinkShip(currPlayerView, currOpponentField, size, playerOneField[x][y]); // tuka moje da se optimizira da ne hodi pak prez cqloto
		}

		if (isLoser(currOpponentField, size))
		{
			cout << "gg" << '\n';
			return;
		}


		evenTurn = !evenTurn;
	}
}
/// ---Shooting


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

void printField( int** field, size_t size)
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
		if (field[x][y] == 0)
		{
			if (orientation == 'V') x++;
			else if (orientation == 'H') y++;
			//if (!isValidIndex(size, x, y)) return false;
		}
		else return false;
	}
	return true;
}
void placeBoatsPhase(int** field, size_t size, int* boatCount)
{
	int currBoatId = 1;
	for (int i = BIGGEST_BOAT_SIZE; i > 0; i--)
	{

		if (boatCount[i] == 0) continue;

		printField(field, size);
		cout << "You are currently placing a " << i << " size boat, choose orientation\n[H = Horizontal] [V = Vertical]:\n";

		char orientation = ' ';
		cin >> orientation;

		int x=0, y=0;
		cout << "Enter coordinates: ";
		cin >> x >> y;

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
		system("CLS");
	}

	system("CLS");
}
/// ---Placing Boats

/// Game Modes
void twoPlayerLogic(size_t size)
{
	int** playerOneField = createField(size);
	int** playerTwoField = createField(size);
	
	char** playerOneView = createView(size);
	char** playerTwoView = createView(size);

	///temporary
	int boatCountOne[6] = { 0,0,1,0,1,0 };
	int boatCountTwo[6] = { 0,0,1,0,1,0 };
	///

	placeBoatsPhase(playerOneField, size, boatCountOne); //Player One Places Boats
	placeBoatsPhase(playerTwoField, size, boatCountTwo); //Player Two Places Boats
	

	shootPhase(playerOneField,playerOneView,playerTwoField,playerTwoView, size);




	freeMatrix(playerOneField,size);
	freeMatrix(playerTwoField,size);
	freeMatrix(playerOneView,size);
	freeMatrix(playerTwoView,size);
	
}
/// ---Game Modes


int main()
{
	int size = 0;
	cin >> size;
	twoPlayerLogic(size);

	return 0;
}


/*
indexite pri placevaneto sa za opraqne
da e po izchisteno kato printva
vs cpu logic
da se chetat broq korabi

*/