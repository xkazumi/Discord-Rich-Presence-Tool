#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent)
	: QLabel(parent)
{
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent *event) 
{
	emit Clicked();
}

void ClickableLabel::enterEvent(QEvent * event)
{
	setStyleSheet(styleEnter);
	//QPalette palette;
	//palette.setColor(QPalette::WindowText, Qt::white);
	//setStyleSheet("");
	//setPalette(palette);
}

void ClickableLabel::leaveEvent(QEvent * event)
{
	setStyleSheet(styleLeave);
}
