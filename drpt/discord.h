#pragma once

#include "discord-rpc.h"
#include <string>

struct RichPresence
{
	// State
	std::string details;	// What the player is currently doing
	std::string state;		// In-game/party status

	// Timestamps
	int64_t timeStart;		// Start timestamp in epoch seconds (used without end timestamp for elapsed time)
	int64_t timeEnd;		// End timestamp in epoch seconds (needs to be greater than start timestamp for remaining time)

	// Party
	int partySize;			// Number of players in the party
	int partyMax;			// Maximum number of players allowed in the party

	// Icons
	std::string smallKey;	// Small image key (used by Discord API to locate icon to use)
	std::string smallText;	// Tooltip text for the small image key
	std::string largeKey;	// Large image key (used by Discord API to locate icon to use)
	std::string largeText;	// Tooltip text for the large image key
};

// Discord RPC
class Discord
{
public:
	Discord();
	~Discord();

	// Initialise Discord RPC
	void Initialise(std::string appId);

	// Send the Rich Presence update to Discord
	// TO-DO: Handle errors
	bool UpdatePresence(RichPresence &richPresence);

private:
	// Tell Discord that the Discord Rich Presence instance is shutting down
	void Shutdown();

	bool bInitialised;

	DiscordEventHandlers handlers;
	DiscordRichPresence rich;
};