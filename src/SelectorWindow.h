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
    void captured(QPixmap pixmap, bool ocr);
    void canceled();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

public slots:
    void onConfirm();
    void onCancel();
    void onOCR();
    void showOcrResult(const QString &text);
    void resetOcrResult();
    void clearSelectionWindow();

private:
    QPoint m_start;
    QPoint m_end;
    bool m_isSelecting = false;
    bool m_selectionFinished = false;
    QRect m_selectionRect;
    QScreen* m_targetScreen = nullptr;
    QQuickWidget* m_qmlControls;

    QScreen* screenAtPos(const QPoint& globalPos);
    void capture(bool ocr);
};

#endif // SELECTORWINDOW_H
