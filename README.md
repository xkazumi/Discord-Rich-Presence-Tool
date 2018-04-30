# Discord Rich Presence Tool
A C++/Qt program that lets you fill in your own custom Discord Rich Presence information for games and activities away from the PC.

### Notes
* The Discord API only allows you to update your Rich Presence information once every 15 seconds. (https://discordapp.com/developers/docs/topics/rate-limits)

* Due to a limitation of the Discord Rich Presence API, it's not possible to change the *"Custom status"* program name from within the program. If you'd like to change this static name, feel free to [create a new Rich Presence application](https://discordapp.com/developers) and recompile DRPT with your own application ID, name and set of icons.

## Screenshots
![Main window](screenshots/1.png)

![The result in Discord](screenshots/2.png)

## Download
You can find the latest compiled releases here: https://github.com/Rarcoon/Discord-Rich-Presence-Tool/releases

## Requirements (developers only)
- Qt Open Source Edition
https://www.qt.io/

- Discord RPC
https://github.com/discordapp/discord-rpc

## Feedback
If you would like to provide feedback or suggest new icons, you can contact me on Twitter ([@N6T4_](https://twitter.com/n6t4_ "@N6T4_")).
