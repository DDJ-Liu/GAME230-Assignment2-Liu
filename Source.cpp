//SFML includes
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

//other includes
#include <string>
#include <iostream>
#include <fstream>


//namespace
using namespace sf;
using namespace std;

//Game Loop Pattern
void setUp();
void initialize(RenderWindow& window);
void handleInput(RenderWindow& window, Event& e);
void update(RenderWindow& window);
void render(RenderWindow& window);

//functions
Vector2<int> MouseToTileMap(RenderWindow& window);
void SaveLevelToFile();
void LoadLevelFromFile();
void LoadTilesFromFile();
void LoadDefaultTiles();
void defineLevelScale();

//const variables
const int TILES_PIECE_COUNT_DEFAULT = 21;
const int MAX_TILES_COUNT_X = 30;
const int MAX_TILES_COUNT_Y = 30;
const int DEFAULT_TILES_COUNT_X = 10;
const int DEFAULT_TILES_COUNT_Y = 10;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 720;
const string SCREEN_SHOT_NAME = "Assignment 2 Level Screenshot.png";
const string LEVEL_FILE_NAME = "level.txt";


//global variables
Texture* tileTexture;
Sprite* tileSprite;
int tilesPieceCount= TILES_PIECE_COUNT_DEFAULT;
int tilesCountX = DEFAULT_TILES_COUNT_X;
int tilesCountY = DEFAULT_TILES_COUNT_Y;
int tilesWidth = 70;
int tilesHeight = 70;
int tileSelected = 0;

int** level;//Arrays used to store and process level
Sprite** tileMap;//Arrays used to render the level on window

int main() {
	//Process the startup input before defining a window
	setUp();
	
	RenderWindow mainWindow(VideoMode(tilesCountX * tilesWidth + 40, tilesCountY * tilesHeight + 60), "Assignment 2 by Liu");

	//initialize variables & arrays
	initialize(mainWindow);

	//output user guide
	cout << "Editor guide:" << endl;
	cout << "Left mouse click - Places the selected tile on the level map" << endl;
	cout << "Right mouse click - Erase the selected tile" << endl;
	cout << "Up Arrow - Goes to the next available tile in the tileset" << endl;
	cout << "Down Arrow - Goes to the previously available tile in the tileset" << endl;
	cout << "'l' Key - Saves your current level to a file"<<endl;
	cout << "'s' Key - Loads a level from file"<<endl;
	cout << "Space Bar - Take a screenshot of the level and save it to the file " << '"' << SCREEN_SHOT_NAME << '"' << endl;


	while (mainWindow.isOpen())
	{
		// Check for events from the window & handle the input
		Event event;
		while (mainWindow.pollEvent(event))
		{
			handleInput(mainWindow,event);
		}

		update(mainWindow);

		render(mainWindow);
	}
	return 0;
}

//Handle startup input. Set up some basic variables before a window can be initialized
void setUp() {

	//LoadDefaultTiles();
	cout << "please enter the single tileset image along with the width and height of the tiles" << ',' << " example: Grassland-64x64.png 64 64" << '.' << endl;
	cout << " You can also use the default tileset by enter " << '"' << "default" << '"' << endl;
	LoadTilesFromFile();
	cout << "please enter the number of tiles that can be placed in width and height to define the level scale. Example: 10 10" << endl;
	defineLevelScale();
	return;
}

//Initialization
void initialize(RenderWindow& window) {

	tileSelected = 0;
	
	//Intialize Tile arrays based on the loaded tiles

	cout << "Window size: " << window.getSize().x << " " << window.getSize().y << endl;
	
	level = new int*[tilesCountY];
	tileMap = new Sprite * [tilesCountY];
	for (int i = 0; i < tilesCountY; i++) {
		level[i] = new int[tilesCountX];
		tileMap[i] = new Sprite[tilesCountX];
		for (int j = 0; j < tilesCountX; j++) {
			level[i][j] = -1;
			tileMap[i][j].setPosition(j * tilesWidth + (window.getSize().x % tilesWidth) / 2, i * tilesHeight + (window.getSize().y % tilesHeight) / 2);
//			cout << i << " " << j << " Position:" << tileMap[i][j].getPosition().x << " " << tileMap[i][j].getPosition().y << endl;
		}
	}

	return;
}

void handleInput(RenderWindow& window, Event& e){
	if (e.type == Event::Closed)
		window.close();
	if (e.type == Event::KeyPressed) {
		//Up & Down - change the selected tile
		if (e.key.code == Keyboard::Up) {
			tileSelected = (tileSelected + 1) % tilesPieceCount;
		}
		if (e.key.code == Keyboard::Down) {
			tileSelected = (tileSelected - 1+ tilesPieceCount) % tilesPieceCount;
		}
		//Space - screenshot
		if (e.key.code == Keyboard::Space) {
			Texture screenShot;
			screenShot.create(window.getSize().x, window.getSize().y);
			screenShot.update(window);
			if (screenShot.copyToImage().saveToFile(SCREEN_SHOT_NAME))
			{
				cout << "Screenshot saved to "<< SCREEN_SHOT_NAME << endl;
			}
		}
		//S - save level
		if (e.key.code == Keyboard::S) {
			SaveLevelToFile();
		}
		//L - load level
		if (e.key.code == Keyboard::L) {
			LoadLevelFromFile();
		}

	}
	//Mouse click - add & erase tile
	if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2<int> mousePos = MouseToTileMap(window);
		level[mousePos.y][mousePos.x] = tileSelected;
	}
	if (Mouse::isButtonPressed(Mouse::Right)) {
		Vector2<int> mousePos = MouseToTileMap(window);
		level[mousePos.y][mousePos.x] = -1;
	}
	return;
}

void update(RenderWindow& window) {


	return;
}

void render(RenderWindow& window) {
	window.clear();
	//draw the level based on the int array recorded
	for (int i = 0; i < tilesCountY; i++) {
		for (int j = 0; j < tilesCountX; j++) {
			if (level[i][j] >= 0 && level[i][j] < tilesPieceCount) {
				tileMap[i][j].setTexture(tileTexture[level[i][j]]);
				tileMap[i][j].setScale(1, 1);
				window.draw(tileMap[i][j]);
			}
		}
	}
	//draw the selected tile at the mouse pointed place
	Vector2<int> mousePos = MouseToTileMap(window);
	tileMap[mousePos.y][mousePos.x].setTexture(tileTexture[tileSelected]);
	window.draw(tileMap[mousePos.y][mousePos.x]);
	window.display();
	return;
}

//function to locate the mouse position on the stored int level array
Vector2<int> MouseToTileMap(RenderWindow& window) {
	int x, y;
	x = (int)(Mouse::getPosition(window).x - (window.getSize().x % tilesWidth) / 2) / tilesWidth;
	x = max(0, x);
	//cout << x << endl;
	x = min(x, tilesCountX - 1);
	y = (int)(Mouse::getPosition(window).y - (window.getSize().y % tilesHeight) / 2 ) / tilesHeight;
	y = max(0, y);
	y = min(y, tilesCountY - 1);
	//cout << x << " " << y << endl;
	return Vector2<int>(x, y);

}

//Save the current level to a file
void SaveLevelToFile() {
	ofstream levelFile;
	string fileName = LEVEL_FILE_NAME;

	cout << "Please enter the file name to save the level" << ',' << " or enter " << '"' << "default" << '"' << " to use the default file name " << "'" << LEVEL_FILE_NAME << "'" << endl;

	getline(cin, fileName);
	if (fileName == "default") {
		fileName = LEVEL_FILE_NAME;
	}
	levelFile.open(fileName);
	if (!levelFile.is_open()) {
		cout << "Failed to save the level to file " << fileName << endl;
		return;
	}
	
	//write the scale of the level into the file
	levelFile << tilesCountX << ' ' << tilesCountY << endl;

	//write level tiles into the file one by one
	for (int i = 0; i< tilesCountY; i++) {
		for (int j = 0; j < tilesCountX; j++) {
			levelFile << level[i][j] << " ";
		}
		levelFile << endl;
	}
	levelFile.close();
	cout << "Level saved to file " << fileName << endl;
	return;
}

//Load an existing level file
void LoadLevelFromFile() {
	ifstream levelFile;
	string fileName = LEVEL_FILE_NAME;

	cout << "Please enter the file name to load the level" << ',' << " or enter "<<'"'<<"default"<<'"'<< " to use the default file name " << "'" << LEVEL_FILE_NAME << "'" << endl;

	getline(cin,fileName);
	if (fileName == "default") {
		fileName = LEVEL_FILE_NAME;
	}
	levelFile.open(fileName);
	if (!levelFile.is_open()) {
		cout << "Failed to load level from file " << fileName << endl;
		return;
	}
	int fileX = DEFAULT_TILES_COUNT_X, fileY = DEFAULT_TILES_COUNT_Y;

	//read saved level scale
	levelFile >> fileX >> fileY;
//	cout << fileX << " " << fileY << endl;
	
	//create a new array to buffer the file information
	int** fileLevel=new int*[fileY];

	for (int i = 0; i < fileY; i++) {
		fileLevel[i] = new int[fileX];
		for (int j = 0; j < fileX; j++) {
			levelFile >> fileLevel[i][j];
		}
	}

	//if the file level scale is smaller than current scale, fill "-1" to the empty tiles. Or abandon the overflowed tiles if reversed.
	for (int i = 0; i < tilesCountY; i++) {
		for (int j = 0; j < tilesCountX; j++) {
			if (i < fileY && j < fileX) {
				level[i][j] = fileLevel[i][j];
			}
			else {
				level[i][j] = -1;
			}
		}
	}

	levelFile.close();
	cout << "Level loaded from file " << fileName << endl;
	return;
}

//Load a tileset image and split it, based on the user input
void LoadTilesFromFile() {

	string tileSetName;
	cin >> tileSetName ;
	if (tileSetName == "default") {
		LoadDefaultTiles();
		return;
	}
	cin >> tilesWidth >> tilesHeight;
	Image tileset;
	if (!tileset.loadFromFile(tileSetName)) {
		cout << "Can't load image file " << tileSetName << endl;
		cout << "Please enter again." << endl;
		LoadTilesFromFile();
		return;
	}
	Vector2u tileSetSize = tileset.getSize();

	//check if the image scale is divisible by the input width and height
	if ((tileSetSize.x % tilesWidth) | (tileSetSize.y % tilesHeight)) {
		cout << "The tileset dimensions are not divisible by the specified tile width and height!" << endl;
		cout << "Please enter again." << endl;
		LoadTilesFromFile();
		return;
	}

	//define the tile Array and read tiles from the image
	tilesPieceCount = (tileSetSize.x / tilesWidth) * (tileSetSize.y / tilesHeight);
	tileTexture = new Texture[tilesPieceCount];
	tileSprite = new Sprite[tilesPieceCount];
	int index = 0;
	for (int y = 0; y < tileSetSize.y; y += tilesHeight) {
		for (int x = 0; x < tileSetSize.x; x += tilesWidth) {
			if (index >= tilesPieceCount) {
				cout << "index expiration" << endl;
				return;
			}
			if (!tileTexture[index].loadFromImage(tileset, IntRect(x, y, tilesWidth, tilesHeight))) {
				cout << "errors when load tiles";
				return;
			}
			tileSprite[index].setTexture(tileTexture[index]);
			index++;
		}
	}
	cout << "Loaded from image file " << tileSetName << endl;
	cout << "The image is devided into " << tilesPieceCount << " tiles in total" << endl;
	return;
}

void LoadDefaultTiles() {
	tilesPieceCount = TILES_PIECE_COUNT_DEFAULT;
	tileTexture = new Texture[tilesPieceCount];
	tileSprite = new Sprite[tilesPieceCount];
	int tilesCountX = DEFAULT_TILES_COUNT_X;
	int tilesCountY = DEFAULT_TILES_COUNT_Y;

	//Set default variables
	tilesWidth = 70;
	tilesHeight = 70;

	//Load default tiles
	cout << "Loading default tiles" << endl;
	for (int i = 0; i < tilesPieceCount; i++) {
		if (!tileTexture[i].loadFromFile("Tilesets/Platformer-" + to_string(i) + ".png")) {
			cout << "Failed to load tiles" << endl;
			return;
		}
		cout << "load tile " << "Tilesets/Platformer-" << to_string(i) << ".png" << endl;
		tileSprite[i].setTexture(tileTexture[i]);
	}


}

//define the level scale at the startup, based on the user input
void defineLevelScale() {
//	cout << "enter function defineLevelScale()" << endl;
	cin >> tilesCountX >> tilesCountY;
	if ((tilesCountX < 1) || (tilesCountX > MAX_TILES_COUNT_X) || (tilesCountY < 1) || (tilesCountY > MAX_TILES_COUNT_Y)) {
		cout << "Wrong level scale" << ',' << " please enter again." << endl;
		defineLevelScale();
		return;
	}
	return;
}