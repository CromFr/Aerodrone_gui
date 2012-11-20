#include <iostream>

#include <QtGui>

#include "Window.hpp"

using namespace std;

int main(int argc, char** argv)
{

    QApplication qtapp(argc, argv);


    Window win;
    qtapp.exec();

    return 0;
}
