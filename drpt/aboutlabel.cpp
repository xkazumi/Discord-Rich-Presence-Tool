#include "aboutlabel.h"

AboutLabel::AboutLabel(QWidget *parent)
	: ClickableLabel(parent)
{
}

AboutLabel::~AboutLabel()
{
}

void AboutLabel::mousePressEvent(QMouseEvent *event)
{
	ClickableLabel::mousePressEvent(event);
}

void AboutLabel::enterEvent(QEvent * event)
{
	setStyleSheet(styleEnter);
}

void AboutLabel::leaveEvent(QEvent * event)
{
	setStyleSheet(styleLeave);
}