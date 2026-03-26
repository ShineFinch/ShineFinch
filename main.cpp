#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QHotkey>
#include "src/ShortcutManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Set Icon
    QString logoPath = ":/assets/ShineFinch.png";
    app.setWindowIcon(QIcon(logoPath));


    // Set TrayIcon
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(logoPath));
    trayIcon.setToolTip("ShineFinch");
    trayIcon.show();

    // Register Global shortcut.
    ShortcutManager::registerShortCut(&app);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ShineFinch", "Main");

    return QCoreApplication::exec();
}
