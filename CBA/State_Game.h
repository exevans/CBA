#ifndef STATE_GAME_H
#define STATE_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "State.h"
#include "MapManager.h"
#include "QuestManager.h"
#include "DialogueManager.h"
#include "LuaWorldHandle.h"
#include "System.h"
#include "StateManager.h"
#include "LuaManager.h"
#include "SystemManager.h"

struct EventDetails;

enum class GameState {Playing, Loading, Saving};

class State_Game : public State {
public:
	State_Game(StateManager* sManager);

	void onCreate() override;
	void onDestroy() override;

	void activate() override;
	void deactivate() override;

	void update(float) override;
	void draw() override;
private:
	void saveLevel();
	void loadLevel(const std::string&);
	void updateCamera();
	void checkForLoadRequests();
	void checkForDialogueRequests();

	//event callbacks
	void mainMenu(EventDetails*);
	void pause(EventDetails*);
	void activatePlayer(EventDetails*);
	void movePlayer(EventDetails*);
	void playerInteract(EventDetails*);
	void playerPrimaryFire(EventDetails*);
	void toggleConsole(EventDetails*);

	int _player = 0;

	//SystemManager _systemManager;
	//std::vector<std::unique_ptr<System>> _system;	//toremove
	//MapManager _map;
	//KeyBinder _inputManager;
	//QuestManager _questManager;
	//DialogueManager _dialogueManager;
	LuaWorldHandle _luaWorldHandle;
	GameState _gameState;
};

#endif