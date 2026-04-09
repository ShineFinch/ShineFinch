#include <QApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QHotkey>
#include "src/ShortcutManager.h"
#include "src/Screenshot.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set Icon
    QString logoPath = ":/assets/ShineFinch.png";
    app.setWindowIcon(QIcon(logoPath));


    // Set TrayIcon
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(logoPath));
    trayIcon.setToolTip("ShineFinch");
    trayIcon.show();




    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ShineFinch", "Main");


    Screenshot *screenshot = new Screenshot(&app);
    engine.setInitialProperties({{"screenshot", QVariant::fromValue(screenshot)}});


    // Register Global shortcut.
    ShortcutManager::registerShortCut(&app);

    return QCoreApplication::exec();
}
