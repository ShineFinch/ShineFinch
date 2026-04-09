#ifndef SELECTORWINDOW_H
#define SELECTORWINDOW_H

#include <QWidget>
#include <QScreen>
#include <QPointer>
#include <QQuickWidget>

class SelectorWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SelectorWindow(QWidget *parent = nullptr);

signals:
    void captured(QPixmap pixmap);
    void canceled();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

private slots:
    void onConfirm();
    void onCancel();

private:
    QPoint m_start;
    QPoint m_end;
    bool m_isSelecting = false;
    QRect m_selectionRect;
    QScreen* m_targetScreen = nullptr;
    QQuickWidget* m_qmlControls;

    QScreen* screenAtPos(const QPoint& globalPos);
    void capture();
};

#endif // SELECTORWINDOW_H
