#include "CommandProcessor.hpp"
#include "BlockClient.hpp"

using namespace std;

void ProcessCommands(sago::SagoCommandQueue &cmdQ, sago::SagoDataHolder &dh, GameStateManager &m) {
	for (size_t i = 0; i < cmdQ.GetCommandQueue().size();i++) {
		string cmd = cmdQ.GetCommandQueue().at(i);
		if (cmd == "SP_ENDLESS") {
			shared_ptr<GameState> game(new BlockClient(dh));
			m.PushState(game);
		}
	}
}