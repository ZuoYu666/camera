#include "Samples_Qt_TriggerCount.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Samples_Qt_TriggerCount w;
	w.show();
	return a.exec();
}
