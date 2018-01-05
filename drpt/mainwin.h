#pragma once

#include <QtWidgets/QMainWindow>
#include <qpointer.h>

#include "ui_mainwin.h"

#include "aboutdialog.h"
#include "discord.h"

// The main Qt window of the application
class MainWin : public QMainWindow
{
	Q_OBJECT

public:
	MainWin(QWidget *parent = Q_NULLPTR);

	static const QString appId;
	static const QString commentId;
	static const QString iconPath;
	static const QString miscPath;
	static const QString noneId;
	static const QString statConnected;
	static const QString unknownId;

private:
	// Set the small comment box's disabled state
	void FormatLargeLine();

	// Set the large comment box's disabled state
	void FormatSmallLine();

	// Set the time remaining list's disabled state
	void FormatTimeRemain();

	// Set the elapsed time check box's disabled state
	void FormatTimeUpdate();

	// Open the "About" dialog in the same thread
	void OpenAboutDialog();

	// Update the icon preview image
	// TO-DO: Make this less hacky in general
	void SetImageIcon();

	// Send a Discord Rich Presence update
	void UpdatePresence();

	// "About" dialog's pointer
	QPointer<AboutDialog> about;

	// Instance of Discord RPC
	Discord discord;

	// Discord Rich Presence struct 
	RichPresence richPresence;

	// Qt UI for the main window
	Ui::MainWinClass ui;
};