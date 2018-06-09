#include "mainwindow.h"

#include <QSplashScreen>
#include <QPixmap>
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication salihorskBus(argc, argv);
    salihorskBus.setApplicationName("Salihorsk Bus");
    salihorskBus.setApplicationVersion(QString("%1.%2").arg(MAJOR_VERSION).arg(MINOR_VERSION));
    QPixmap image(":/splashScreen/splashScreen");
    QSplashScreen loadScreen(image);
    loadScreen.show();
    MainWindow mainWindow;
    mainWindow.show();
    loadScreen.finish(&mainWindow);
    return salihorskBus.exec();
}
