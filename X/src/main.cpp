#include <GX.h>

class Game : public  gx::GameX {
public:
	Game(){ }
	~Game() {

	}

};

gx::GameX* gx::CreateApp() {
	return new Game();
}