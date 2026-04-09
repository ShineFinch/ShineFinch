#include "ShortcutManager.h"


#include <QDir>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QPixmap>
#include <QScreen>

QHotkey* ShortcutManager::m_hotkey = nullptr;




void ShortcutManager::registerShortCut(QGuiApplication *app)
{
    if (m_hotkey != nullptr) {
        delete m_hotkey;
        m_hotkey = nullptr;
    }

    m_hotkey = new QHotkey(QKeySequence("F2"), true, app);

    Screenshot& screenshot = ShortcutManager::getScreenshot(app);
    QObject::connect(m_hotkey, &QHotkey::activated, app, [&]() {
        // qDebug() << "Hotkey F2 Activated!";
       screenshot.start();
    });

}

Screenshot& ShortcutManager::getScreenshot(QGuiApplication *app)
{

    static Screenshot instance(nullptr);
    return instance;
}
