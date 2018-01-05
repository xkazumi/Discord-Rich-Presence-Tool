#pragma once

#include <QLabel>

// Qt label widget that generates click events
class ClickableLabel : public QLabel
{
	Q_OBJECT

public:
	ClickableLabel(QWidget *parent);
	~ClickableLabel();

signals:
	void Clicked();

protected:
	void mousePressEvent(QMouseEvent *event);

	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};
