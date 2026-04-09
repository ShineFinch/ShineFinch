#pragma once
#include <QObject>
#include <QPixmap>
#include <QPointer>
#include "SelectorWindow.h"

class Screenshot : public QObject
{
    Q_OBJECT
public:
    explicit Screenshot(QObject *parent = nullptr);

public slots:
    void start();

private slots:
    void onCaptured(QPixmap pix);

private:
    SelectorWindow* m_selectorWindow = nullptr;
};