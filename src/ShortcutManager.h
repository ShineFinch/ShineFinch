#pragma once
#include <QHotkey>
#include <QGuiApplication>

class ShortcutManager
{

public:
   static void registerShortCut(QGuiApplication *app);

private:
   static QHotkey* m_hotkey;

};
