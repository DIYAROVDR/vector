#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

#include "GUI/mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    QPixmap pixmap(":/images/vector_logo.png");
    QPixmap scaledPixmap = pixmap.scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QSplashScreen splash(scaledPixmap);
    splash.show();
    int time = 500;
    QTimer::singleShot(time, &splash, &QSplashScreen::close);
    MainWindow w;
    QTimer::singleShot(time, &w, &MainWindow::showMaximized);
    return a.exec();
}
