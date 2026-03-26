#include "ShortcutManager.h"

QHotkey* ShortcutManager::m_hotkey = nullptr;

void ShortcutManager::registerShortCut(QGuiApplication *app)
{
    if (m_hotkey != nullptr) {
        delete m_hotkey;
        m_hotkey = nullptr;
    }

    m_hotkey = new QHotkey(QKeySequence("F2"), true, app);

    QObject::connect(m_hotkey, &QHotkey::activated, qApp, []() {
        qDebug() << "Hotkey F2 Activated!";
    });

    qDebug() << "Hotkey F2 registered status:" << m_hotkey->isRegistered();
}
