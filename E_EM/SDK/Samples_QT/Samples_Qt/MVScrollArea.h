#pragma once

#include <QEvent>
#include <QScrollArea>
#include <QMouseEvent>
#include <QDebug>

#include "qscrollarea.h"

class MVScrollArea : public QScrollArea
{
public:
	MVScrollArea(QWidget *parent = Q_NULLPTR);
	~MVScrollArea();

	bool eventFilter(QObject *, QEvent *);
};

