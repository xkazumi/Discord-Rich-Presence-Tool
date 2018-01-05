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
}

void ClickableLabel::leaveEvent(QEvent * event)
{
}