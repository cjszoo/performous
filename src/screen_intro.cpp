#include <screen_intro.h>

CScreenIntro::CScreenIntro(std::string const& name, unsigned int width, unsigned int height): CScreen(name, width, height) {
	CScreenManager* sm = CScreenManager::getSingletonPtr();
	cairo_svg = new CairoSVG(sm->getThemePathFile("intro.svg"), width, height);
	texture = sm->getVideoDriver()->initSurface(cairo_svg->getSDLSurface());
}

CScreenIntro::~CScreenIntro() {
	delete cairo_svg;
}

void CScreenIntro::enter() {
	CScreenManager* sm = CScreenManager::getSingletonPtr();
	sm->getAudio()->playMusic(sm->getThemePathFile("menu.ogg"));
}

void CScreenIntro::exit() {}

void CScreenIntro::manageEvent(SDL_Event event) {
	CScreenManager* sm = CScreenManager::getSingletonPtr();
	if (event.type == SDL_KEYDOWN) {
		int key = event.key.keysym.sym;
		if (key == SDLK_ESCAPE || key == SDLK_q) sm->finished();
		else if (key == SDLK_s) sm->activateScreen("Songs");
		else if (key == SDLK_c) sm->activateScreen("Configuration");
		else if (key == SDLK_p) sm->activateScreen("Practice");
		else if (key == SDLK_SPACE) sm->getAudio()->togglePause();
	}
}

void CScreenIntro::draw() {
	CScreenManager* sm = CScreenManager::getSingletonPtr();
	sm->getVideoDriver()->drawSurface(texture);
}
