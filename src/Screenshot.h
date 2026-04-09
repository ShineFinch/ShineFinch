#pragma once
#include <QObject>
#include <QPixmap>
#include <QPointer>
#include "SelectorWindow.h"
#include <tesseract/baseapi.h>

class Screenshot : public QObject
{
    Q_OBJECT
public:
    explicit Screenshot(QObject *parent = nullptr);
    ~Screenshot();

    Pix * OCR(QClipboard *clipboard, QString filename);

public slots:
    void start();

private slots:
    void onCaptured(QPixmap pix);

private:
    SelectorWindow* m_selectorWindow = nullptr;
    tesseract::TessBaseAPI* m_ocr = nullptr;
};