#include "MainWin.h"
#include "discord-rpc.h"
#include <qdiriterator.h>

//#include "clickablelabel.h"

const QString MainWin::statConnected = "Rich Presence update sent!";

const QString MainWin::appId = "396811586397929472";
const QString MainWin::commentId = "comment";
const QString MainWin::unknownId = "Unknown";
const QString MainWin::noneId = "None";

const QString MainWin::iconPath = ":/icons/Resources/";
const QString MainWin::miscPath = ":/misc/Resources/";

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

	// Connect elapsed time checkbox to remaining time format function
	QObject::connect(ui.checkElapsed, &QCheckBox::clicked, this, &MainWin::FormatTimeRemain);

	// Connect preserve time checkbox to BLARG
	QObject::connect(ui.checkPreserve, &QCheckBox::clicked, this, &MainWin::ToggleTimeUpdate);

	// Connect update button to Discord Rich Presence object
	QObject::connect(ui.butUpdate, &QPushButton::clicked, this, &MainWin::UpdatePresence);

	// Connect comment line texts to validation function
	QObject::connect(ui.lineSmallText, &QLineEdit::textEdited, this, &MainWin::FormatSmallLine);
	QObject::connect(ui.lineLargeText, &QLineEdit::textEdited, this, &MainWin::FormatLargeLine);

	// Connect "about" label to opening the about dialog
	QObject::connect(ui.labelAbout, &ClickableLabel::Clicked, this, &MainWin::OpenAboutDialog);

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

	// Connect icon combo box to updatQComboBoxe icon image
	QObject::connect(ui.comboIcon, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWin::SetImageIcon);

	// Configure the "about" label's mouse event stylesheets
	ui.labelAbout->styleLeave = ui.labelAbout->styleSheet();
	ui.labelAbout->styleEnter = ui.labelAbout->styleSheet() + "color: #000000;";

	// Set the current index, resulting in SetImageIcon being called
	ui.comboIcon->setCurrentIndex(0);
}

void MainWin::UpdatePresence()
{
	//// Details
	richPresence.state = ui.lineState->text().toStdString();
	richPresence.details = ui.lineDetails->text().toStdString();

	//// Timestamp
	// Set the end timestamp
	if (!ui.checkPreserve->isChecked())
	{
		if (ui.checkElapsed->isChecked())
		{
			richPresence.timeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			richPresence.timeEnd = (ui.timeRemain->time().hour() * 3600
				+ ui.timeRemain->time().minute() * 60
				+ ui.timeRemain->time().second());

			if (richPresence.timeEnd > 0)
				richPresence.timeEnd += richPresence.timeStart;
		}
		else
		{
			richPresence.timeStart = 0;
			richPresence.timeEnd = 0;
		}
	}

	//// Party
	richPresence.partySize = ui.spinPartySize->value();
	richPresence.partyMax = ui.spinPartyMax->value();

	//// Icons
	// Text
	if (ui.lineSmallText->isEnabled())
		richPresence.smallText = ui.lineSmallText->text().toStdString();
	else
		richPresence.smallText = "";

	if (ui.lineLargeText->isEnabled())
		richPresence.largeText = ui.lineLargeText->text().toStdString();
	else
		richPresence.largeText = "";

	// Images
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

	discord.UpdatePresence(richPresence);

	statusBar()->showMessage(statConnected, 3000);
}

//void MainWin::OpenIconSelect()
//{
//	if (!iconSelect.isNull())
//		return;
//
//	iconSelect = new IconSelect(this);
//
//	// Remove '?' from title bar
//	iconSelect->setWindowFlags(iconSelect->windowFlags() & ~Qt::WindowContextHelpButtonHint);
//
//	iconSelect->show();
//}

void MainWin::FormatSmallLine()
{
	if (ui.lineSmallText->text().length() > 0)
		ui.lineLargeText->setDisabled(true);
	else
		ui.lineLargeText->setDisabled(false);
}

void MainWin::FormatLargeLine()
{
	if (ui.lineLargeText->text().length() > 0)
		ui.lineSmallText->setDisabled(true);
	else
		ui.lineSmallText->setDisabled(false);
}

void MainWin::FormatTimeRemain()
{
	if (ui.checkElapsed->isChecked() && ui.checkElapsed->isEnabled())
		ui.timeRemain->setEnabled(true);
	else
		ui.timeRemain->setEnabled(false);
}

// TO-DO: Make this less hacky in general
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
	// If the user has selected "Unknown" for the image icon
	else
	{
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

void MainWin::OpenAboutDialog()
{
	if (!about.isNull())
		delete about;

	about = new AboutDialog(this);
	about->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	about->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
	about->exec();
	delete about;
}

//bool MainWin::eventFilter(QObject* object, QEvent* event)
//{
//	if (object == ui.menuBar)
//	{
//		return true;
//	}
//
//	return false;
//}

void MainWin::ToggleTimeUpdate()
{
	if (ui.checkPreserve->isChecked())
		ui.checkElapsed->setDisabled(true);
	else
		ui.checkElapsed->setDisabled(false);
	
	FormatTimeRemain();
}