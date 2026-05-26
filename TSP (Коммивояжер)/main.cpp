#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("Задача коммивояжёра (TSP)");
    w.resize(1100, 700);
    w.show();
    return app.exec();
}
