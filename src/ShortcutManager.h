#pragma once
#include <QHotkey>
#include <QGuiApplication>
#include "Screenshot.h"

class ShortcutManager
{

public:
   static void registerShortCut(QGuiApplication *app);

    static Screenshot& getScreenshot(QGuiApplication *app);

private:
   static QHotkey* m_hotkey;
};
