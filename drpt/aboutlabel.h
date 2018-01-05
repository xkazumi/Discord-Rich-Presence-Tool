#pragma once

#include "clickablelabel.h"

// Clickable "About" label used in the main window
class AboutLabel : public ClickableLabel
{
	Q_OBJECT

public:
	AboutLabel(QWidget *parent);
	~AboutLabel();

	QString styleEnter;
	QString styleLeave;

protected:
	void mousePressEvent(QMouseEvent *event);

	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};
