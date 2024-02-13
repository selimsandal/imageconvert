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

class ImageConverter : public QWidget {
public:
    ImageConverter(QWidget *parent = nullptr) : QWidget(parent) {
        setAcceptDrops(true); // Enable the widget to accept dropped files

        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(5, 5, 5, 5);
        mainLayout->setSpacing(5);

        // Format selector
        auto *formatLabel = new QLabel(tr("Select Output Format:"), this);
        mainLayout->addWidget(formatLabel);

        formatCombo = new QComboBox(this);
        populateFormats(formatCombo);
        mainLayout->addWidget(formatCombo);

        // Quality slider and spinbox in a horizontal layout
        auto *qualityLayout = new QHBoxLayout();
        auto *qualityLabel = new QLabel(tr("Select Output Quality (0-100):"), this);
        qualityLayout->addWidget(qualityLabel);

        auto *qualitySlider = new QSlider(Qt::Horizontal, this);
        qualitySlider->setRange(0, 100);
        qualitySlider->setValue(100);
        qualityLayout->addWidget(qualitySlider);

        qualitySpinBox = new QSpinBox(this);
        qualitySpinBox->setRange(0, 100);
        qualitySpinBox->setValue(100);
        qualityLayout->addWidget(qualitySpinBox);

        connect(qualitySlider, &QSlider::valueChanged, qualitySpinBox, &QSpinBox::setValue);
        connect(qualitySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), qualitySlider, &QSlider::setValue);

        mainLayout->addLayout(qualityLayout);

        // Save to the same location checkbox
        saveToSameLocationCheckBox = new QCheckBox(tr("Save to the same location with the same name"), this);
        mainLayout->addWidget(saveToSameLocationCheckBox);

        // Convert button
        auto *convertButton = new QPushButton(tr("Convert Image"), this);
        mainLayout->addWidget(convertButton);

        connect(convertButton, &QPushButton::clicked, this, [this]() {
            QString format = formatCombo->currentData().toString();
            int quality = qualitySpinBox->value();
            convertImages(format, quality);
        });

        auto *copyrightLabel = new QLabel(tr("Selim Sandal, 2024"), this);
        mainLayout->addWidget(copyrightLabel, 0, Qt::AlignCenter);
        this->setWindowTitle(tr("Image Converter"));
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
            QString format = formatCombo->currentData().toString();
            int quality = qualitySpinBox->value();
            for (const QUrl &url : urlList) {
                QString filePath = url.toLocalFile();
                convertImage(filePath, format, quality, saveToSameLocationCheckBox->isChecked());
            }
        }
    }

    void showEvent(QShowEvent *event) override {
        if (isExpired()) {
            QMessageBox::warning(this, tr("Update Required"),
                                 tr("This application has expired and requires an update. Please download the latest version."));
            QApplication::quit();
        } else {
            QWidget::showEvent(event);
        }
    }

private:
    QComboBox *formatCombo;
    QSpinBox *qualitySpinBox;
    QCheckBox *saveToSameLocationCheckBox;

    void populateFormats(QComboBox *formatCombo) {
        QList<QByteArray> formats = QImageWriter::supportedImageFormats();
        for (const QByteArray &format : formats) {
            formatCombo->addItem(QString::fromLatin1(format).toUpper(), QString::fromLatin1(format));
        }
    }

    void convertImages(const QString &format, int quality) {
        QStringList inputImagePaths;
        if (!saveToSameLocationCheckBox->isChecked()) {
            // Allow selecting multiple files
            inputImagePaths = QFileDialog::getOpenFileNames(
                    this, tr("Open Images"), "", tr("Image Files (*.*)"),
                    nullptr, QFileDialog::Option::DontUseNativeDialog);
        } else {
            // Allow selecting multiple files even when "Save to the same location" is checked
            inputImagePaths = QFileDialog::getOpenFileNames(
                    this, tr("Open Image"), "", tr("Image Files (*.*)"),
                    nullptr, QFileDialog::Option::DontUseNativeDialog);
        }

        for (const QString &inputImagePath : inputImagePaths) {
            if (!inputImagePath.isEmpty()) {
                convertImage(inputImagePath, format, quality, saveToSameLocationCheckBox->isChecked());
            }
        }
    }




    void convertImage(const QString &inputImagePath, const QString &format, int quality, bool saveToSameLocation) {
        QImage image(inputImagePath);
        if (image.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("Could not load the image."));
            return;
        }

        QString outputImagePath;
        QFileInfo fileInfo(inputImagePath);  // Define fileInfo here, based on the inputImagePath

        if (saveToSameLocation) {
            outputImagePath = fileInfo.absoluteDir().absolutePath() + "/" + fileInfo.completeBaseName() + "." + format.toLower();
        } else {
            // Corrected use of fileInfo within the scope
            outputImagePath = QFileDialog::getSaveFileName(
                    this, tr("Save Image As"), fileInfo.absoluteDir().absolutePath() + "/" + fileInfo.completeBaseName() + "." + format.toLower(),
                    QString("%1 Files (*.%2)").arg(format.toUpper(), format.toLower()),
                    nullptr, QFileDialog::Option::DontUseNativeDialog);
            if (outputImagePath.isEmpty()) return;
        }

        QImageWriter writer(outputImagePath, format.toLatin1());
        writer.setQuality(quality);
        if (!writer.write(image)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not write the image."));
            return;
        }

        //QMessageBox::information(this, tr("Success"), tr("Image converted successfully."));
    }

    bool isExpired() {
        // Set the expiration date to one month from now
        QDate expirationDate = QDate::currentDate().addMonths(1);
        // Check if the current date is past the expiration date
        return QDate::currentDate() > expirationDate;
    }

};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationVersion("1.0");
    ImageConverter converter;
    converter.show();
    return QApplication::exec();
}
