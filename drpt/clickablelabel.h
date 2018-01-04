#pragma once

#include <QLabel>

class ClickableLabel : public QLabel
{
	Q_OBJECT

public:
	ClickableLabel(QWidget *parent);
	~ClickableLabel();

	QString styleEnter;
	QString styleLeave;

signals:
	void Clicked();

protected:
	void mousePressEvent(QMouseEvent *event);

	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};
