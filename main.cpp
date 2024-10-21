#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette palette = QApplication::palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    QApplication::setPalette(palette);
    MainWindow w;
    w.show();
    return app.exec();
}
