#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>


//PLAYING FIELD DIMENSIONS//
const int H = 4; // HIGHT
const int W = 4; // WEIGHT


int field[H+2][W+2] = {
	{-1, -1, -1, -1, -1, -1},
	{-1, 0, 1, 2, 3, -1},
	{-1, 4, 5, 6, 7, -1},
	{-1, 8, 9, 10, 11, -1},
	{-1, 12, 13, 14, 15, -1},
	{-1, -1, -1, -1, -1, -1},
};
int tiles[16][2] = {
	{0, 0}, {0, 1}, {0, 2}, {0, 3}, // 1 2 3 4
	{1, 0}, {1, 1}, {1, 2}, {1, 3}, // 5 6 7 8
	{2, 0}, {2, 1}, {2, 2}, {2, 3}, // 9 10 11 12
	{3, 0}, {3, 1}, {3, 2}, {3, 3}, // 13 14 15 16
};

int main()
{
	// Объект, который, собственно, является главным окном приложения
	sf::RenderWindow window(sf::VideoMode(256, 256), "Fivenashki");

	// shuffling  for field
	srand(time(0));
	for (int i = 1; i <= H; ++i) {
		for (int j = 1; j <= W; ++j) {
			std::swap(field[i][j], field[(std::rand() % H) + 1][(std::rand() % W) + 1]);
		}
	}

	// connectings texture
	sf::Texture texture;
	int w = 64;
	const char* path = "five.png";
	texture.loadFromFile(path);

	// start sprite
	sf::Sprite sprite(texture);

	// mouse
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	int x = (pos.x / w) - 1;
	int y = (pos.y / w) - 1;
	std::swap(field[2][1], field[2][3]);

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		int x = (pos.x / w)+1;
		int y = (pos.y / w)+1;
		// Обрабатываем очередь событий в цикле
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == sf::Event::Closed)
				// тогда закрываем его
				window.close();
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (!(15-field[x - 1][y])) std::swap(field[x][y], field[x - 1][y]);
				else if (!(15 - field[x][y - 1])) std::swap(field[x][y], field[x][y - 1]);
				else if (!(15 - field[x][y + 1])) std::swap(field[x][y], field[x][y + 1]);
				else if (!(15 - field[x + 1][y])) std::swap(field[x][y], field[x + 1][y]);
			}
		}
		// Отрисовка окна
		for (int i = 1; i <= H; ++i)
			for (int j = 1; j <= W; ++j)
			{
				sprite.setTextureRect(sf::IntRect(tiles[field[i][j]][0] * w, tiles[field[i][j]][1] *w, w, w));
				sprite.setPosition((i-1) * w, (j-1) * w);
				window.draw(sprite);
			}

		window.display();
	}

	return 0;
}