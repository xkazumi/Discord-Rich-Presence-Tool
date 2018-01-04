#pragma once

#include <QtWidgets/QMainWindow>
#include <qpointer.h>
#include "ui_mainwin.h"

#include "aboutdialog.h"
#include "discord.h"


class MainWin : public QMainWindow
{
	Q_OBJECT

public:
	MainWin(QWidget *parent = Q_NULLPTR);

	// Discord status strings
	static const QString statConnected;

	static const QString appId;
	static const QString commentId;
	static const QString unknownId;
	static const QString noneId;

	static const QString iconPath;
	static const QString miscPath;

private:
	//bool eventFilter(QObject* object, QEvent* event);
	//static void Test();
	void UpdatePresence();
	//void OpenIconSelect();
	void FormatSmallLine();
	void FormatLargeLine();
	void FormatTimeRemain();
	void SetImageIcon();
	//void CloseIconSelect();
	void OpenAboutDialog();
	void ToggleTimeUpdate();

	//QPointer<IconSelect> iconSelect;
	QPointer<AboutDialog> about;

	Ui::MainWinClass ui;

	RichPresence richPresence;

	Discord discord;
};