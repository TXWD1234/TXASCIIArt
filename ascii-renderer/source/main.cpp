#include "MainClass.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


	txInputWindow iw;
	iw.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
	iw.setWindowTitle("Enter Information");
	iw.show();

	
	







	
    //Main window;
    //window.show();
    return app.exec();
}
