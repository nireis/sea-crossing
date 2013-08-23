#include "conf.h"
#include "game/game.h"
#include "engine/renderHub.h"

#include <thread>

using namespace std;

int main(){

	Conf conf;
	getConfFromFile("config", conf);

	MessageReceiver *testReceiver;
	RenderHub testRenderer(testReceiver);
	//if(testRenderer.init())
	//{
	//	testRenderer.addScene();
	//	testRenderer.setActiveScene(0);
	//	testReceiver->pushLoadSceneMessages();
	//	std::thread renderThread(&RenderHub::run,&testRenderer);
	//}
	std::thread renderThread(&RenderHub::init,&testRenderer);
	testReceiver->pushLoadSceneMessages();
	
	Board board;
	RandomAIPlayer player1("Spongebob", board);
	RandomAIPlayer player2("Patrick", board);
	
	SimpleGUIGame g(&player1, &player2, board, testReceiver);
	std::thread gameThread(&SimpleGUIGame::start,&g);

	gameThread.join();
	renderThread.join();
}
