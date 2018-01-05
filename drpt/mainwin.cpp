#include "MainWin.h"

#include <qdiriterator.h>
#include "discord-rpc.h"

const QString MainWin::appId = "396811586397929472";
const QString MainWin::commentId = "comment";
const QString MainWin::iconPath = ":/icons/Resources/";
const QString MainWin::miscPath = ":/misc/Resources/";
const QString MainWin::noneId = "None";
const QString MainWin::statConnected = "Rich Presence update sent!";
const QString MainWin::unknownId = "Unknown";

MainWin::MainWin(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// Initialise Discord RPC
	// TO-DO: Handle errors
	discord.Initialise(appId.toStdString());

	// Initialise Rich Presence struct
	richPresence = {};

	// Prevent window resizing
	// TO-DO: See if this can be done in the editor instead
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

	// Add icon elements to combo list from resource file
	QDirIterator it(iconPath);

	while (it.hasNext())
	{
		it.next();
		ui.comboIcon->addItem(it.fileInfo().baseName());
	}

	ui.comboIcon->model()->sort(0);

	ui.comboIcon->insertItem(0, unknownId);
	ui.comboIcon->insertItem(0, noneId);

	// Connect signals to functions
	QObject::connect(ui.checkElapsed, &QCheckBox::clicked, this, &MainWin::FormatTimeRemain);
	QObject::connect(ui.checkPreserve, &QCheckBox::clicked, this, &MainWin::FormatTimeUpdate);
	QObject::connect(ui.butUpdate, &QPushButton::clicked, this, &MainWin::UpdatePresence);
	QObject::connect(ui.lineSmallText, &QLineEdit::textEdited, this, &MainWin::FormatSmallLine);
	QObject::connect(ui.lineLargeText, &QLineEdit::textEdited, this, &MainWin::FormatLargeLine);
	QObject::connect(ui.labelAbout, &ClickableLabel::Clicked, this, &MainWin::OpenAboutDialog);
	QObject::connect(ui.comboIcon, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWin::SetImageIcon);

	// Configure the "about" label's mouse event stylesheets
	ui.labelAbout->styleLeave = ui.labelAbout->styleSheet();
	ui.labelAbout->styleEnter = ui.labelAbout->styleSheet() + "color: #000000;";

	// Set the current index, resulting in SetImageIcon being called
	ui.comboIcon->setCurrentIndex(0);
}

void MainWin::FormatLargeLine()
{
	if (ui.lineLargeText->text().length() > 0)
		ui.lineSmallText->setDisabled(true);
	else
		ui.lineSmallText->setDisabled(false);
}

void MainWin::FormatSmallLine()
{
	if (ui.lineSmallText->text().length() > 0)
		ui.lineLargeText->setDisabled(true);
	else
		ui.lineLargeText->setDisabled(false);
}

void MainWin::FormatTimeRemain()
{
	if (ui.checkElapsed->isChecked() && ui.checkElapsed->isEnabled())
		ui.timeRemain->setEnabled(true);
	else
		ui.timeRemain->setEnabled(false);
}

void MainWin::FormatTimeUpdate()
{
	if (ui.checkPreserve->isChecked())
		ui.checkElapsed->setDisabled(true);
	else
		ui.checkElapsed->setDisabled(false);

	FormatTimeRemain();
}

void MainWin::OpenAboutDialog()
{
	if (!about.isNull())
		delete about;

	about = new AboutDialog(this);

	// Set the window flags of the dialog window
	about->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	about->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);

	// Enter the event loop of the dialog window in the current thread
	// (prevents interaction with the main window while running)
	about->exec();

	delete about;
}

void MainWin::SetImageIcon()
{
	// NOTE: The icon count offset takes into account the additional "None" option
	if (ui.comboIcon->currentIndex() > ui.comboIcon->count())
		return;

	// If the user selected "None" for the image icon
	if (ui.comboIcon->currentText() == noneId)
	{
		ui.labelPlayPic->setPixmap(QPixmap(miscPath + unknownId + ".png"));
		ui.labelPlayPic->setDisabled(true);

		// Disable comment lines
		ui.lineSmallText->setDisabled(true);
		ui.lineLargeText->setDisabled(true);
	}
	else
	{
		// If the user has selected "Unknown" for the image icon
		if (ui.comboIcon->currentText() == unknownId)
			ui.labelPlayPic->setPixmap(QPixmap(miscPath + unknownId + ".png"));
		else
			ui.labelPlayPic->setPixmap(QPixmap(iconPath + ui.comboIcon->currentText() + ".png"));

		ui.labelPlayPic->setDisabled(false);

		// Enable commment lines
		FormatSmallLine();
		FormatLargeLine();
	}
}

void MainWin::UpdatePresence()
{
	// State //
	richPresence.details = ui.lineDetails->text().toStdString();
	richPresence.state = ui.lineState->text().toStdString();

	// Timestamp //
	// Set the end timestamp if the "preserve time" checkbox is unchecked
	if (!ui.checkPreserve->isChecked())
	{
		// Set the timestamps if the "elapsed time" checkbox is checked
		if (ui.checkElapsed->isChecked())
		{
			richPresence.timeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			richPresence.timeEnd = (ui.timeRemain->time().hour() * 3600
				+ ui.timeRemain->time().minute() * 60
				+ ui.timeRemain->time().second());

			// If the "remaining time" time list is unused (i.e., the user wants elapsed time instead)
			if (richPresence.timeEnd > 0)
				richPresence.timeEnd += richPresence.timeStart;
		}
		// Set timestamps to 0 to preserve current Rich Presence time on Discord
		else
		{
			richPresence.timeStart = 0;
			richPresence.timeEnd = 0;
		}
	}

	// Party //
	richPresence.partySize = ui.spinPartySize->value();
	richPresence.partyMax = ui.spinPartyMax->value();

	// Tooltip text
	if (ui.lineSmallText->isEnabled())
		richPresence.smallText = ui.lineSmallText->text().toStdString();
	else
		richPresence.smallText = "";

	if (ui.lineLargeText->isEnabled())
		richPresence.largeText = ui.lineLargeText->text().toStdString();
	else
		richPresence.largeText = "";

	// Icons //
	// Set comment icon if there is a small text tooltip
	if (richPresence.smallText.length() > 0)
		richPresence.smallKey = commentId.toStdString();
	else
		richPresence.smallKey = "";

	// Replace spaces with underscores and format string to lower-case for Discord icons
	if (ui.comboIcon->currentText() != noneId)
	{
		QString discordKey = ui.comboIcon->currentText();
		discordKey.replace(" ", "_");

		richPresence.largeKey = discordKey.toLower().toStdString();
	}
	else
	{
		richPresence.largeKey = "";
	}

	// Send Rich Presence update to Discord via Discord RPC
	// TO-DO: Error checking
	discord.UpdatePresence(richPresence);

	// Display success message to the user for 3 seconds
	statusBar()->showMessage(statConnected, 3000);
}