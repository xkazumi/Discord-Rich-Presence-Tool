#include "discord.h"

Discord::Discord()
	: bInitialised(false)
{
	rich = { 0 };
}

Discord::~Discord()
{
	Shutdown();
}

void Discord::Initialise(std::string appId)
{
	if (bInitialised)
		return;

	handlers = { 0 };
	//memset(&handlers, 0, sizeof(handlers));

	handlers.ready = handlers.ready;
	handlers.errored = handlers.errored;
	handlers.disconnected = handlers.disconnected;
	handlers.joinGame = handlers.joinGame;
	handlers.spectateGame = handlers.spectateGame;
	handlers.joinRequest = handlers.joinRequest;

	Discord_Initialize(appId.c_str(), &handlers, 1, nullptr);

	bInitialised = true;
	return;
}

bool Discord::UpdatePresence(RichPresence &richPresence)
{
	if (!bInitialised)
		return false;

	rich.details = richPresence.details.c_str();
	rich.state = richPresence.state.c_str();
	rich.largeImageKey = richPresence.largeKey.c_str();
	rich.largeImageText = richPresence.largeText.c_str();
	rich.partyMax = richPresence.partyMax;
	rich.partySize = richPresence.partySize;
	rich.smallImageKey = richPresence.smallKey.c_str();
	rich.smallImageText = richPresence.smallText.c_str();
	rich.startTimestamp = richPresence.timeStart;
	rich.endTimestamp = richPresence.timeEnd;

	Discord_UpdatePresence(&rich);

	return true;
}

void Discord::Shutdown()
{
	if (bInitialised)
	{
		// Clean up Discord
		Discord_Shutdown();

		bInitialised = false;
	}
}