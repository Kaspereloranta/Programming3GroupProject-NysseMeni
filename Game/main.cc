#include <QApplication>
#include "engine.hh"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(offlinedata);
    StudentSide::Engine(argc,argv);
}
