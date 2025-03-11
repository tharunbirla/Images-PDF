#include <QCoreApplication>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QDir>
#include <QTextStream>
#include <QList>

void addImages(QList<QString>& imagePaths)
{
    QTextStream(stdout) << "Enter the paths of images to convert (separate with space): ";
    QTextStream stdinStream(stdin);
    QString input = stdinStream.readLine();

    QStringList files = input.split(" ", Qt::SkipEmptyParts);
    for (const QString& file : files) {
        if (QFile::exists(file)) {
            imagePaths.append(file);
            QTextStream(stdout) << "Added: " << file << "\n";
        } else {
            QTextStream(stdout) << "File not found: " << file << "\n";
        }
    }
}

void convertToPdf(const QList<QString>& imagePaths)
{
    if (imagePaths.isEmpty()) {
        QTextStream(stdout) << "No images to convert. Exiting...\n";
        return;
    }

    QTextStream(stdout) << "Enter the output PDF file path: ";
    QTextStream stdinStream(stdin);
    QString outputFile = stdinStream.readLine();

    if (!outputFile.endsWith(".pdf", Qt::CaseInsensitive))
        outputFile += ".pdf";

    QPdfWriter pdfWriter(outputFile);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);

    QTextStream(stdout) << "Converting images to PDF...\n";

    for (int i = 0; i < imagePaths.size(); ++i) {
        QImage image(imagePaths[i]);
        if (image.isNull()) {
            QTextStream(stdout) << "Error loading image: " << imagePaths[i] << "\n";
            continue;
        }

        if (i > 0) {
            pdfWriter.newPage();
        }

        QRectF pageRect = pdfWriter.pageLayout().paintRectPixels(pdfWriter.resolution());
        QImage scaledImage = image.scaled(pageRect.size().toSize(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation);

        QPointF centeredPos((pageRect.width() - scaledImage.width()) / 2,
                            (pageRect.height() - scaledImage.height()) / 2);
        painter.drawImage(centeredPos, scaledImage);
    }

    painter.end();
    QTextStream(stdout) << "PDF created successfully: " << outputFile << "\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<QString> imagePaths;

    addImages(imagePaths);
    convertToPdf(imagePaths);

    return 0;
}
