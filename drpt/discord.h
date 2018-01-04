#pragma once

#include "discord-rpc.h"
#include <string>

struct RichPresence
{
	std::string state;		// Party status
	std::string details;	// What the player is currently doing

	int64_t timeStart;
	int64_t timeEnd;

	int partySize;
	int partyMax;

	std::string largeKey;	// Large image key
	std::string largeText;	// Tooltip for the large image key
	std::string smallKey;	// Small image key
	std::string smallText;	// Tooltip for the small image key
};

class Discord
{
public:
	Discord();
	~Discord();

	void Initialise(std::string appId);
	bool UpdatePresence(RichPresence &richPresence);

	void Shutdown();

private:
	DiscordEventHandlers handlers;

	DiscordRichPresence rich;
	bool bInitialised;
};