#include <GX.h>

class Game : public  gx::GameXApp {
public:
	Game(){ }
	~Game() {

	}

};
gx::GameXApp* gx::CreateApp() {
	return new Game();
}