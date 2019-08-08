#include "MVScrollArea.h"

MVScrollArea::MVScrollArea(QWidget *parent) : QScrollArea(parent)
{
	installEventFilter(this);
}

MVScrollArea::~MVScrollArea()
{

}


bool MVScrollArea::eventFilter(QObject *object, QEvent *type)
{
	if (type->type() == QEvent::Wheel)
	{
		return true;
	}
	return QWidget::eventFilter(object, type);
}