#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace sf;

//PLAYING FIELD DIMENSIONS//
const int H = 20; // HIGHT
const int W = 10; // WEIGHT


int field[H][W] = {}; // PLAYING FIELD

/*
COORDINATES FIGURES :
0   1
2   3
4   5
6   7
(%2,//2) - Translation to coordinates 
*/
int figures[7][4] =
{
	0, 1, 2, 3, //O
	1, 3, 5, 7, //I
	0, 2, 3, 5, //S
	1, 2, 3, 4, //Z
	0, 2, 4, 5, //L
	1, 3, 4, 5, //J
	1, 2, 3, 5  //T
};

// STRUCTURE OF COORDINATES
struct Point 
{
	int x, y;
} a[4], b[4];


// FUNCTION FOR CHECKING THE NORMAL POSITION OF THE FIGURES
bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= W || a[i].y >= H) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

	return 1;
};


int main()
{
	//random
	srand(time(0));

	// Count file
	int CountFile;
	std::ifstream ifCount("count.txt");
	std::ofstream ofCount;
	if (ifCount.is_open())
	{
		ifCount >> CountFile;
	}
	ifCount.close();

	//main window settings
	RenderWindow window(VideoMode(325, 480), "Tetris");

	// connectings texture
	Texture texture, texture_bg, texture_frame;
	std::string path = "D:\\Emil_Programm\\Programming\\Game_Ivan\\Project1\\textures\\";
	texture.loadFromFile(path + "tiles.png");
	texture_bg.loadFromFile(path + "background.png");
	texture_frame.loadFromFile(path + "frame.png");

	// start sprite
	Sprite sprite(texture), sprite_bg(texture_bg), sprite_frame(texture_frame);;
	sprite.setTextureRect(IntRect(18 * rand() % 7, 0, 18, 18));

	// SETTINGS
	int dx = 0;
	bool rotate = 0;
	int colorNum = 1;
	bool beginGame = true;
	int n = rand() % 7;
	int endGame = false;
	int Count = 0;

	// timer
	float timer = 0, delay = 0.3;

	// clock
	Clock clock;

	// font
	Font font;
	font.loadFromFile(path + "arial.ttf");

	// text
	Text CountWindow, HelpCounts, TopCounts;

	CountWindow.setFillColor(Color::Red);
	CountWindow.setCharacterSize(15);
	CountWindow.setFont(font);
	CountWindow.setStyle(sf::Text::Bold);
	TopCounts = HelpCounts = CountWindow;

	TopCounts.setPosition(Vector2f(240, 140));
	HelpCounts.setPosition(Vector2f(50, 405));
	CountWindow.setPosition(Vector2f(238, 50));


	while (window.isOpen())
	{
		// time 
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
				ofCount.open("count.txt"); 
				if (ofCount.is_open()) {
					ofCount << CountFile;
				}
				ofCount.close();
			}

			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Up) rotate = true;
				else if (event.key.code == Keyboard::Left) dx = -1;
				else if (event.key.code == Keyboard::Right) dx = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		//// Gorizontal ////
		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }

		// Stop
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];


		//// Rotate ////
		if (rotate)
		{
			Point p = a[1]; // center
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

		}

		//// down, timer ////
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
			if (!check())
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;
				colorNum = 1 + rand() % 7;
				n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}

			}
			timer = 0;

		}

		//----ACCOUNTING POINTS---//
		int r = 0;
		for (int i = 0; i < H; ++i) {
			bool a = 0;
			for (int j = 0; j < W; ++j) {
				a = a || !bool(field[i][j]);
			}
			if (!a)
				r++;
		}
		if (r) {
			if (r == 1)
				Count += 100;
			else if (r == 2)
				Count += 300;
			else if (r == 3)
				Count += 700;
			else if (r == 4)
				Count += 1500;
			if (Count > CountFile)
				CountFile = Count;

		}

		// CLEANING THE LINE
		int k = H - 1;
		for (int i = H - 1, count; i > 0; i--)
		{
			count = 0;
			for (int j = 0; j < W; j++)
			{
				if (field[i][j]) {
					++count;
				}
				field[k][j] = field[i][j];
			}
			if (count < W) --k;
		}

		// END GAME
		for (int i = 0; i < 10; ++i)
			if (field[0][i]) {
				endGame = true;
				break;
			}

		if (endGame) {
			endGame = false;
			dx = 0;
			rotate = 0;
			colorNum = 1;
			beginGame = true;
			n = rand() % 7;
			timer = 0, delay = 0.3;
			for (int j = 0; j < 10; ++j)
				for (int i = 0; i < 20; ++i)
					field[i][j] = 0;
			if (Count > CountFile) {
				CountFile = Count;
			}
			Count = 0;
			continue;
		}

		// first tetramino
		if (beginGame)
		{
			beginGame = false;
			n = rand() % 7;
			for (int i = 0; i < 4; i++)
			{
				a[i].x = figures[n][i] % 2;
				a[i].y = figures[n][i] / 2;
			}
		}
		dx = 0; rotate = 0; delay = 0.3;

		//Writing//

		window.clear(Color::White);
		window.draw(sprite_bg);
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
			{
				if (field[i][j] == 0) continue;
				sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				sprite.move(28, 31);
				window.draw(sprite);
			}

		for (int i = 0; i < 4; i++)
		{
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));

			sprite.setPosition(a[i].x * 18, a[i].y * 18);

			sprite.move(28, 31);

			window.draw(sprite);
		}
		window.draw(sprite_frame);
		
		// TEXT
		TopCounts.setString(std::to_string(CountFile));
		HelpCounts.setString("1 line - 100\n2 line - 300\n3 line - 700\n4 line - 1500");
		CountWindow.setString(std::to_string(Count));

		window.draw(TopCounts);
		window.draw(HelpCounts);
		window.draw(CountWindow);

		window.display();
	}

	return 0;
}
