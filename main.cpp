#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QCheckBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStyleFactory>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "DropIndicator.h"

class ImageConverter : public QWidget {
public:
    ImageConverter(QWidget *parent = nullptr) : QWidget(parent) {
        setAcceptDrops(true);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        setupUI(mainLayout);
        fetchCurrentTime();
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override {
        const QMimeData *mimeData = event->mimeData();
        if (mimeData->hasUrls()) {
            QList<QUrl> urlList = mimeData->urls();
            convertImages(urlList, formatCombo->currentData().toString(), qualitySpinBox->value(), saveToSameLocationCheckBox->isChecked());
        }
    }

private:
    QComboBox *formatCombo;
    QSpinBox *qualitySpinBox;
    QCheckBox *saveToSameLocationCheckBox;
    QNetworkAccessManager *networkManager;

    void setupUI(QVBoxLayout *mainLayout) {
        // Label for format selection
        QLabel *formatLabel = new QLabel(tr("Select Output Format:"), this);
        mainLayout->addWidget(formatLabel);

        // Combo box for format selection
        formatCombo = new QComboBox(this);
        populateFormats(); // This function populates the combo box with available formats
        mainLayout->addWidget(formatCombo);

        // Quality selection components
        QHBoxLayout *qualityLayout = new QHBoxLayout();
        QLabel *qualityLabel = new QLabel(tr("Select Output Quality (0-100):"), this);
        qualityLayout->addWidget(qualityLabel);

        // Slider for quality
        QSlider *qualitySlider = new QSlider(Qt::Horizontal, this);
        qualitySlider->setRange(0, 100);
        qualitySlider->setValue(100); // Default value
        qualityLayout->addWidget(qualitySlider);

        // Spin box for quality
        qualitySpinBox = new QSpinBox(this);
        qualitySpinBox->setRange(0, 100);
        qualitySpinBox->setValue(100); // Default value
        qualityLayout->addWidget(qualitySpinBox);

        // Sync slider and spin box values
        connect(qualitySlider, &QSlider::valueChanged, qualitySpinBox, &QSpinBox::setValue);
        connect(qualitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), qualitySlider, &QSlider::setValue);

        // Add the quality controls layout to the main layout
        mainLayout->addLayout(qualityLayout);

        // Button and checkbox layout
        QVBoxLayout *buttonCheckboxLayout = new QVBoxLayout();

        // Convert button
        QPushButton *convertButton = new QPushButton(tr("Convert Image"), this);
        buttonCheckboxLayout->addWidget(convertButton);

        // Checkbox for saving to the same location
        saveToSameLocationCheckBox = new QCheckBox(tr("Save to the same location"), this);
        buttonCheckboxLayout->addWidget(saveToSameLocationCheckBox);

        // Horizontal layout to hold the button/checkbox layout and the drop indicator
        QHBoxLayout *buttonIndicatorLayout = new QHBoxLayout();
        buttonIndicatorLayout->addLayout(buttonCheckboxLayout);

        // Drop indicator component
        DropIndicator *dropIndicator = new DropIndicator(this);
        buttonIndicatorLayout->addWidget(dropIndicator);

        // Add the button and drop indicator layout to the main layout
        mainLayout->addLayout(buttonIndicatorLayout);

        // Connect the convert button signal to the appropriate slot
        connect(convertButton, &QPushButton::clicked, this, &ImageConverter::onConvertButtonClicked);

        // Copyright label
        QLabel *copyrightLabel = new QLabel(tr("Selim Sandal, 2024"), this);
        mainLayout->addWidget(copyrightLabel, 0, Qt::AlignCenter);

        // Set the window title
        this->setWindowTitle(tr("Image Converter"));
    }


    void populateFormats() {
        QList<QByteArray> formats = QImageWriter::supportedImageFormats();
        for (const QByteArray &format : formats) {
            formatCombo->addItem(QString::fromLatin1(format).toUpper(), QString::fromLatin1(format));
        }
    }

    void onConvertButtonClicked() {
        // Generate the filter string for supported image formats using QImageReader
        QStringList formats;
        for (const QByteArray &format : QImageReader::supportedImageFormats()) {
            formats << QString("*.%1").arg(QString::fromLatin1(format).toLower());
        }
        QString filter = tr("Images (%1)").arg(formats.join(' '));

        QList<QUrl> files = QFileDialog::getOpenFileUrls(
                this,
                tr("Select Images"),
                QString(),
                filter
        );

        if (files.isEmpty()) return;

        convertImages(files, formatCombo->currentData().toString(), qualitySpinBox->value(), saveToSameLocationCheckBox->isChecked());
    }


    void convertImages(const QList<QUrl> &urls, const QString &format, int quality, bool saveToSameLocation) {
        for (const QUrl &url : urls) {
            QString inputFilePath = url.toLocalFile();
            QImage image(inputFilePath);
            if (image.isNull()) {
                QMessageBox::critical(this, tr("Error"), tr("Could not load the image: %1").arg(inputFilePath));
                continue;
            }

            QString outputFilePath;
            QFileInfo fileInfo(inputFilePath);
            if (saveToSameLocation) {
                // Change the file extension to the selected format
                outputFilePath = fileInfo.absoluteDir().absolutePath() + "/" + fileInfo.completeBaseName() + "." + format;
            } else {
                // Suggest the new file name with the correct extension
                QString suggestedFileName = fileInfo.absoluteDir().absolutePath() + "/" + fileInfo.completeBaseName() + "." + format;
                // Use the selected format for the save dialog filter
                QString filter = QString("%1 Files (*.%2)").arg(format.toUpper(), format);
                outputFilePath = QFileDialog::getSaveFileName(this, tr("Save Image As"), suggestedFileName, filter);
                if (outputFilePath.isEmpty()) continue;
                // Append the correct file extension if not present
                if (!outputFilePath.endsWith('.' + format, Qt::CaseInsensitive)) {
                    outputFilePath += '.' + format;
                }
            }

            QImageWriter writer(outputFilePath, format.toLatin1());
            writer.setQuality(quality);
            if (!writer.write(image)) {
                QMessageBox::critical(this, tr("Error"), tr("Could not write the image to: %1").arg(outputFilePath));
            }
        }
    }


    void fetchCurrentTime() {
        networkManager = new QNetworkAccessManager(this);
        connect(networkManager, &QNetworkAccessManager::finished, this, &ImageConverter::onTimeFetched);
        networkManager->get(QNetworkRequest(QUrl("http://worldtimeapi.org/api/ip")));
    }

    void onTimeFetched(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseBytes = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseBytes);
            QJsonObject jsonObj = jsonDoc.object();
            QDateTime currentServerTime = QDateTime::fromString(jsonObj["datetime"].toString(), Qt::ISODate);
            checkExpiration(currentServerTime);
        } else {
            QMessageBox::information(this, tr("License Problem"), tr("Error connecting to license server."));
            qApp->quit();
        }
        reply->deleteLater();
    }

    void checkExpiration(const QDateTime &serverTime) {
        QDateTime expirationDate(QDate(2024, 2, 28), QTime(0, 0, 0));
        if (serverTime > expirationDate) {
            QMessageBox::warning(this, tr("Update Required"), tr("This application has expired and requires an update. Please download the latest version."));
            QApplication::quit();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QImageReader::setAllocationLimit(0);
#ifdef Q_OS_WIN
    QApplication::setStyle("fusion");
#endif
    QApplication::setApplicationVersion("beta 2");
    ImageConverter converter;
    converter.show();
    return QApplication::exec();
}
