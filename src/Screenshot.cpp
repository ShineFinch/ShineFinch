#include "Screenshot.h"


#include <QGuiApplication>
#include <QClipboard>
#include <QDir>
#include <QDateTime>
#include <QtConcurrent>
#include <QFutureWatcher>

Screenshot::Screenshot(QObject *parent)
    : QObject{parent}
{

}

void Screenshot::start()
{
    if (m_selectorWindow != nullptr) {
        m_selectorWindow->raise();
        m_selectorWindow->activateWindow();
        return;
    }

    m_selectorWindow = new SelectorWindow(nullptr);

    connect(m_selectorWindow, &SelectorWindow::captured, this, &Screenshot::onCaptured);
    connect(m_selectorWindow, &SelectorWindow::canceled, this, &QObject::deleteLater);

    connect(m_selectorWindow, &SelectorWindow::destroyed, this, [this](){
        m_selectorWindow = nullptr;
    });

    m_selectorWindow->show();
    m_selectorWindow->raise();
    m_selectorWindow->activateWindow();
    m_selectorWindow->setWindowState(Qt::WindowActive);
}

void Screenshot::onCaptured(QPixmap pix)
{
    qDebug() << "onCpatued";

    if (pix.isNull())
        return;

    QGuiApplication::clipboard()->setPixmap(pix);

    // QString path = QDir::homePath() + "/Pictures/ShineFinch/";
    // QDir().mkpath(path);

    // QString filename = path + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    // pix.save(filename, "PNG", 100);
}
