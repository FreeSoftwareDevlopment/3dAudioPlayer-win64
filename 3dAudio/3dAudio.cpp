#define useWin
// SOUND SHOULD BE ONLY MONO

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cload/loadingBarConsole.h>

#ifdef useWin
#include <Windows.h>
#else
#include <chrono>
#endif

#include <iostream>
#include <utility>
#include <thread>
#include <math.h>

#include <filesystem>
namespace fs = std::filesystem;

class audio {
private:
	const int* windowSize;
	sf::Vector2f px;
	float mX;
	sf::CircleShape* sh;
	sf::Music* m2;
public:
	void audioSet(const sf::Vector2f&);
	const sf::Vector2f& getCurrentPos() { return px; }
	audio(sf::CircleShape* sh2, sf::Music* m, const int* windowSize2) :windowSize(windowSize2), sh(sh2), m2(m) {}
};

void convertMouse(sf::Vector2f* v, const int* windowSize);

void setRect(sf::RectangleShape*, const sf::Color&, const int*, bool = false);

#define toFloat(x) static_cast<float>(x)
#define floatToNondecimal(n, v) static_cast<n>(roundf(v))

void renderArgv(sf::Music* m, CloadingBarConsole* cl, bool* running) {
	while (*running) {
		//SHOW STATUS BAR
		cl->setPercent(floatToNondecimal(int, (m->getPlayingOffset() / m->getDuration()) * 100));
		cl->render();
#ifdef useWin
		Sleep(5);
#else
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
#endif
	}
}

#define rectS 20.f

void showMsgError(const char*, const char* = nullptr, const char* = nullptr);

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		showMsgError("No File Selected\nYou need to Open the File with this Application via Explorer");
		return -1;
	}
	const int windowSize[2]{ 800, 1200 };
#define height windowSize[0]
#define width windowSize[1]
#define heightf toFloat(height)
#define widthf toFloat(width)
	std::cout << "3D Sound\n<c> Shakbyteprojects!\nPRESS ESC TO EXIT\n\nLoad Soundfile" << std::endl;
	if (!fs::exists(argv[1])) {
		showMsgError("File \"", argv[1], "\" not Found!");
		return -2;
	}
	sf::Music music;
	if (!music.openFromFile(argv[1])) {
		showMsgError("Failure Loading Soundfile \"", argv[1], "\"");
		return -1;
	}
	music.setLoop(1);
	// create the window
	bool mono = music.getChannelCount() == 1, running = true;
	music.setRelativeToListener(0);
	CloadingBarConsole l;
	music.setRelativeToListener(true);
	music.play();
	music.setMinDistance(20.f);
	std::cout << "Playing Music: \"" << argv[1] << "\"" << std::endl;
	l.setColorCode(colorcode::SUSPEND);
	std::thread rendererConsole(renderArgv, &music, &l, &running);

	if (mono) {
		sf::RenderWindow window(sf::VideoMode(width, height), "3D Sound Control");

		const float circleRadius = 10.f;
		sf::CircleShape shape(circleRadius);
		audio a(&shape, &music, &windowSize[0]);
		sf::RectangleShape p1(sf::Vector2f(2, 2));
		// set the shape color to green
		shape.setFillColor(sf::Color::Blue);
		shape.setOrigin(circleRadius, circleRadius);
		p1.setPosition(widthf * .5f, heightf * .5f);
		a.audioSet(sf::Vector2f(0, 0));

		bool mouseBTN[2]{ false, false };
		// run the program as long as the window is open
		while (window.isOpen())
		{
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			if (window.hasFocus()) {
				mouseBTN[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				if (mouseBTN[0] != mouseBTN[1]) {
					{
						sf::Cursor c;
						c.loadFromSystem(mouseBTN[0] ? sf::Cursor::Cross : sf::Cursor::Arrow);
						window.setMouseCursor(c);
					}
					if (mouseBTN[0])
						sf::Mouse::setPosition(window.mapCoordsToPixel(a.getCurrentPos()), window);
					mouseBTN[1] = mouseBTN[0];
				}
				if (mouseBTN[0]) {
					auto pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					if (pos.y < height && pos.y>0 && pos.x < width && pos.y>0) {
						convertMouse(&pos, &windowSize[0]);
						a.audioSet(std::move(pos));
					}
				}
			}

			// clear the window with black color
			window.clear(sf::Color::White);

			// draw everything here...
			// window.draw(...);
			setRect(&p1, sf::Color::Color(0xff, 0, 0, 0xff * .7), &windowSize[0], 1);
			window.draw(p1);
			setRect(&p1, sf::Color::Color(0, 0xff, 0, floatToNondecimal(unsigned int, roundf(0xff * .7f))), &windowSize[0]);
			window.draw(p1);
			window.draw(shape);

			// end the current frame
			window.display();
		}
		running = false;
		std::cout << "Stopcode" << std::endl;
	}
	else {
		showMsgError("No Mono Channel Sound in \"", argv[1], "\", ONLY SOUND PLAYING");
	}
	rendererConsole.join();
	std::cout << "\nStop" << std::endl;
	return 0;
}

void audio::audioSet(const sf::Vector2f& p) {
	px.x = (widthf / 2.f) + toFloat(p.x);
	px.y = (heightf / 2.f) + toFloat(p.y);
	sh->setPosition(px.x, px.y);
	m2->setPosition(px.x - widthf * .5f, px.y - heightf * .5f, 0);
}

void convertMouse(sf::Vector2f* v, const int* windowSize) {
	v->x -= (width / 2.f);
	v->y -= (height / 2.f);
}

void setRect(sf::RectangleShape* r, const sf::Color& color, const int* windowSize, bool n) {
	r->setRotation(n ? 90 : 0);
	r->setFillColor(color);
	r->setSize(sf::Vector2f(rectS, n ? widthf : heightf));
	r->setOrigin(rectS * .5f, (n ? widthf : heightf) * .5f);
}

#include "cconcat.h"

void showMsgError(const char* f1, const char* msg, const char* f2) {
	try {
		char* cstr = concat(f1, msg, f2);
		std::cerr << cstr << std::endl;
#ifdef useWin
		MessageBoxA(NULL, cstr, "3D Sound", MB_ICONWARNING);
#endif
		free(cstr);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
