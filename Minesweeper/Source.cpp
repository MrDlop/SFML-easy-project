#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

using namespace sf;

//PLAYING FIELD DIMENSIONS//
const int H = 14; // HIGHT
const int W = 14; // WEIGHT
const int HG = 12;
const int WG = 12;
const int h = 12;


int fieldLogicGen[H][W] = {};
int fieldLogic[HG][WG] = {};// PLAYING FIELD
int fieldGrafic[HG][WG] = {};

int c;

void generationField() {
	//random
	srand(time(0));
	for (int i = 0; i < HG; ++i) {
		for (int j = 0; j < WG; ++j) {
			fieldGrafic[i][j] = 10;
		}
	}
	for (int i = 0; i < HG; ++i) {
		for (int j = 0; j < WG; ++j) {
			fieldLogic[i][j] = 0;
		}
	}
	for (int i = 0; i < HG; ++i) {
		for (int j = 0; j < WG; ++j) {
			fieldLogicGen[i][j] = -1;
		}
	}
	// Бомбы
	for (int i = 0, n, k; i < 20; ++i) {
		n = 1 + rand() % h, k = 1 + rand() % h;
		if (fieldLogicGen[n][k] == 9) --c;
		fieldLogicGen[n][k] = 9;
	}
	//заполнение цифрами
	for (int i = 1; i < H-1; ++i) {
		for (int j = 1; j < W-1; ++j) {
			if (fieldLogicGen[i][j] != 9) {
				int num = 0;
				for (int l = i - 1; l < i + 2; ++l) {
					for (int k = j - 1; k < j + 2; ++k) {
						if (fieldLogicGen[l][k] == 9) ++num;
					}
				}
				fieldLogicGen[i][j] = num;
			}
		}
	}
	//перенос в массив без невидимых границ
	for (int i = 0; i < HG; ++i) {
		for (int j = 0; j < WG; ++j) {
			fieldLogic[i][j] = fieldLogicGen[i + 1][j + 1];
		}
	}
}

int main() {
	c = HG * WG;
	int cUser = 0;
	bool first = true;
	generationField();
	//main window settings
	RenderWindow window(VideoMode(450, 450), "Sapper");

	// connectings texture
	Texture texture;
	int w = 32;
	std::string path = ".\\textures\\";
	texture.loadFromFile(path + "tiles.jpg");

	// start sprite
	Sprite sprite(texture);

	// timer
	float timer = 0;
	
	// font
	Font font;
	font.loadFromFile(path + "arial.ttf");

	// text
	Text timeText;
	timeText.setFillColor(Color::Red);
	timeText.setCharacterSize(20);
	timeText.setFont(font);
	timeText.setStyle(sf::Text::Bold);
	timeText.setPosition(Vector2f(200, 10));

	// clock
	Clock clock;

	while (window.isOpen())
	{
		if (cUser == c) {
			RenderWindow win(VideoMode(450, 450), "Your win!");
			while (win.isOpen()) {
				Event event;
				while (win.pollEvent(event)) {
					if (event.type == Event::Closed) {
						win.close();
						window.close();
						main();
					}
				}
			}

		}
		// time 
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// mouse
		Vector2i pos = Mouse::getPosition(window);
		int x = (pos.x / w) - 1;
		int y = (pos.y / w) - 1;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
				return 0;
			}
			else if (event.type == Event::MouseButtonPressed) {
				if (event.type == Event::MouseButtonPressed)
					if (event.key.code == Mouse::Left) {
						if (first) {
							first = false;
							int num = 0;
							for (int l = y - 1; l < y + 2; ++l) {
								for (int k = x - 1; k < x + 2; ++k) {
									if (fieldLogic[l][k] == 9) ++num;
								}
							}
							fieldLogic[y][x] = num;
						}
						fieldGrafic[x][y] = fieldLogic[x][y];
						if (fieldGrafic[x][y] == 9) {
							for (int i = 1; i <= 11; ++i)
								for (int j = 1; j <= 11; ++j)
								{
									sprite.setTextureRect(IntRect(fieldLogic[i][j] * w, 0, w, w));
									sprite.setPosition(i * w, j * w);
									window.draw(sprite);
								}
							window.close();
							main();
						}
						++cUser;
					}
					else if (event.key.code == Mouse::Right && (fieldGrafic[x][y] == 10 || fieldGrafic[x][y] == 11)) {
						if (fieldGrafic[x][y] == 11) fieldGrafic[x][y] = 10;
						else fieldGrafic[x][y] = 11;
					}
			}
		}

		//Writing//
		window.clear(Color(100,100,100));

		for (int i = 0; i <= 11; ++i)
			for (int j = 0; j <= 11; ++j)
			{
				sprite.setTextureRect(IntRect(fieldGrafic[i][j] * w, 0, w, w));
				sprite.setPosition((i+1) * w, (j+1) * w);
				window.draw(sprite);
			}

		timeText.setString(std::to_string(int(timer)));
		window.draw(timeText);
		window.display();
	}
}