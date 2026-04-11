#include "Screenshot.h"


#include <QGuiApplication>
#include <QClipboard>
#include <QDir>
#include <QDateTime>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QMessageBox>
#include <QTextEdit>
#include <QLayout>


Screenshot::Screenshot(QObject *parent)
    : QObject{parent}
{
    m_ocr = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path

    QFile file(":/assets/train/eng.traineddata");
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    QString tempPath = QCoreApplication::applicationDirPath() + "/eng.traineddata";
    QFile tempFile(tempPath);
    tempFile.open(QIODevice::WriteOnly);
    tempFile.write(data);
    tempFile.close();


    qDebug() << "initialize tesseract.\n";
    if (m_ocr->Init(QCoreApplication::applicationDirPath().toUtf8(), "eng", tesseract::OEM_DEFAULT)) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        qDebug() << "Could not initialize tesseract.\n";
        exit(1);
    }
}

Screenshot::~Screenshot()
{
    // Destroy used object and release memory
    if (m_ocr != nullptr){
        m_ocr->End();
        delete m_ocr;
        m_ocr = nullptr;
        qDebug() << "Screenshot destroy.\n";
    }
}

void Screenshot::start()
{
    if (m_selectorWindow != nullptr) {
        m_selectorWindow->raise();
        m_selectorWindow->activateWindow();
        m_selectorWindow->setWindowState(Qt::WindowActive);
        m_selectorWindow->show();
        qDebug() << "SelectorWindow exist.\n";
        return;
    }

    qDebug() << "SelectorWindow .\n";
    m_selectorWindow = new SelectorWindow(nullptr);

    connect(m_selectorWindow, &SelectorWindow::captured, this, &Screenshot::onCaptured);
    connect(m_selectorWindow, &SelectorWindow::canceled, this, [this]{
        m_selectorWindow->clearSelectionWindow();

        m_selectorWindow->hide();
    });

    connect(m_selectorWindow, &SelectorWindow::destroyed, this, [this](){
        qDebug() << "SelectorWindow destroyed.\n";
        m_selectorWindow = nullptr;
    });

    connect(this, &Screenshot::ocrResultReady, m_selectorWindow, &SelectorWindow::showOcrResult);

    m_selectorWindow->raise();
    m_selectorWindow->activateWindow();
    m_selectorWindow->setWindowState(Qt::WindowActive);
    m_selectorWindow->show();
}

void cleanOldImages(const QString& folderPath, int maxCount = 20)
{
    QDir dir(folderPath);
    if (!dir.exists()) return;

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files, QDir::Time);

    for (int i = maxCount; i < fileList.size(); ++i) {
        QFile::remove(fileList[i].absoluteFilePath());
    }
}

QString Screenshot::doOCR(const QString &filename)
{
    qDebug() << "OCR starting...";
    Pix *image = pixRead(filename.toUtf8());
    if (!image) return "OCR failed";

    m_ocr->SetImage(image);
    char *outText = m_ocr->GetUTF8Text();
    QString result = QString::fromUtf8(outText).trimmed();

    delete[] outText;
    pixDestroy(&image);
    qDebug() << "OCR finished!";
    return result;
}

void Screenshot::onCaptured(QPixmap pix, bool ocr)
{
    qDebug() << "onCpatued";

    if (pix.isNull())
        return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(pix, QClipboard::Clipboard);

    QString path = QDir::homePath() + "/Pictures/ShineFinch/";
    QDir().mkpath(path);
    cleanOldImages(path, 20);

    QString filename = path + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    pix.save(filename, "PNG", 100);

    if (ocr){
        // OCR
        QString text = doOCR(filename);
        emit ocrResultReady(text);
    }
}
