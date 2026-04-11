#include "SelectorWindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QObject>
#include <QQuickItem>
#include <QTimer>


SelectorWindow::SelectorWindow(QWidget *parent)
    : QWidget{parent}
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint |
                   Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);

    QRect totalRect;
    for (auto* screen : QGuiApplication::screens()) {
        totalRect = totalRect.united(screen->geometry());
    }
    setGeometry(totalRect);
    show();

    m_qmlControls = new QQuickWidget(this);
    m_qmlControls->setSource(QUrl("qrc:/qt/qml/ShineFinch/SelectorWindowOverlay.qml"));
    m_qmlControls->hide();

    QQuickItem* qmlItem = m_qmlControls->rootObject();
    if (!qmlItem) {
        qCritical() << "QML root object is null! Check SelectorWindowOverlay.qml path";
        return;
    }

    QObject* qmlRoot = static_cast<QObject*>(qmlItem);

    connect(qmlRoot, SIGNAL(tiggerConfirmed()), this, SLOT(onConfirm()));
    connect(qmlRoot, SIGNAL(tiggerCanceled()), this, SLOT(onCancel()));
    connect(qmlRoot, SIGNAL(triggerOCR()), this, SLOT(onOCR()));
}


QScreen* SelectorWindow::screenAtPos(const QPoint& globalPos)
{
    for (auto* screen : QGuiApplication::screens()) {
        if (screen->geometry().contains(globalPos)) {
            return screen;
        }
    }
    return QGuiApplication::primaryScreen();
}

void SelectorWindow::mousePressEvent(QMouseEvent* e)
{
    if (m_selectionFinished) {
        return;
    }

    if (e->button() == Qt::LeftButton) {
        m_start = e->globalPosition().toPoint();
        m_targetScreen = screenAtPos(m_start);
        m_isSelecting = true;
        m_qmlControls->hide();
    }
}

void SelectorWindow::mouseMoveEvent(QMouseEvent* e)
{
    if (m_isSelecting) {
        m_end = e->globalPosition().toPoint();
        m_selectionRect = QRect(m_start, m_end).normalized();
        update();
    }
}

void SelectorWindow::mouseReleaseEvent(QMouseEvent*)
{
    m_isSelecting = false;
    update();

    if (m_selectionRect.width() > 5 && m_selectionRect.height() > 5) {
        m_qmlControls->move(m_selectionRect.left(), m_selectionRect.bottom() + 10);
        m_qmlControls->adjustSize();
        m_qmlControls->show();
        m_selectionFinished = true;
    }
}

void SelectorWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(0, 0, 0, 160));

    if (!m_selectionRect.isEmpty() && m_targetScreen) {
        p.setCompositionMode(QPainter::CompositionMode_Clear);
        p.fillRect(m_selectionRect, Qt::transparent);

        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        p.setPen(QPen(Qt::white, 2));
        p.drawRect(m_selectionRect);
    }
}

void SelectorWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape) {
        qDebug() << "escape clicked";
        emit canceled();
    }
}

void SelectorWindow::onConfirm()
{
    capture(false);
    qDebug() << "onConfirm \n";
     emit canceled();

}

void SelectorWindow::onCancel()
{
    emit canceled();
    qDebug() << "onCancel \n";
    // close();
    // QTimer::singleShot(100, this, &SelectorWindow::close);
}

void SelectorWindow::onOCR()
{
    capture(true);
    qDebug() << "onOCR \n";
}

void SelectorWindow::showOcrResult(const QString &text)
{
    QObject *root = m_qmlControls->rootObject();

    bool connected = QMetaObject::invokeMethod(
        root,
        "setOcrResult",
        Qt::AutoConnection,
        Q_ARG(QVariant, text)
        );
}

void SelectorWindow::resetOcrResult()
{
    QObject *root = m_qmlControls->rootObject();

    bool connected = QMetaObject::invokeMethod(
        root,
        "resetOcrResult",
        Qt::AutoConnection
        );
}

void SelectorWindow::clearSelectionWindow()
{
    m_selectionRect = QRect();
    m_start = QPoint();
    m_end = QPoint();
    m_isSelecting = false;
    m_selectionFinished = false;
    m_targetScreen = nullptr;

    if (m_qmlControls && m_qmlControls->isVisible()) {
        m_qmlControls->hide();
    }

    resetOcrResult();

    this->activateWindow();
    this->setFocus(Qt::ActiveWindowFocusReason);
    this->raise();

    update();

    qDebug() << "clearSelectionWindow";
}


void SelectorWindow::capture(bool ocr)
{
    if (!m_targetScreen || m_selectionRect.isEmpty())
        return;

    QRect screenRect = m_targetScreen->geometry();
    QRect localRect = m_selectionRect.translated(-screenRect.topLeft());
    if (!ocr){
        this->hide();
    }

    QGuiApplication::processEvents();
    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    QPixmap pix = primaryScreen->grabWindow(
        0,
        localRect.x(),
        localRect.y(),
        localRect.width(),
        localRect.height()
        );

    if (!pix.isNull()) {
        emit captured(pix, ocr);
    }


}