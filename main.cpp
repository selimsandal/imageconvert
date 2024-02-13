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
#include <QImage>
#include <QImageWriter>
#include <QMessageBox>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

class ImageConverter : public QWidget {
public:
    ImageConverter(QWidget *parent = nullptr) : QWidget(parent) {
        setAcceptDrops(true); // Enable the widget to accept dropped files

        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(5, 5, 5, 5);
        mainLayout->setSpacing(5);

        // Format selector
        auto *formatLabel = new QLabel(tr("Select Format:"), this);
        mainLayout->addWidget(formatLabel);

        formatCombo = new QComboBox(this); // Use member variable
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

        qualitySpinBox = new QSpinBox(this); // Use member variable
        qualitySpinBox->setRange(0, 100);
        qualitySpinBox->setValue(100);
        qualityLayout->addWidget(qualitySpinBox);

        connect(qualitySlider, &QSlider::valueChanged, qualitySpinBox, &QSpinBox::setValue);
        connect(qualitySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), qualitySlider, &QSlider::setValue);

        mainLayout->addLayout(qualityLayout);

        // Convert button
        auto *convertButton = new QPushButton(tr("Convert Image"), this);
        mainLayout->addWidget(convertButton);

        connect(convertButton, &QPushButton::clicked, this, [this]() {
            QString format = formatCombo->currentData().toString();
            int quality = qualitySpinBox->value();
            convertImages(format, quality);
        });

        auto *copyrightlabel = new QLabel(tr("Selim Sandal, 2024"), this);
        mainLayout->addWidget(copyrightlabel, 0, Qt::AlignCenter);
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
                convertImage(filePath, format, quality);
            }
        }
    }

private:
    QComboBox *formatCombo; // Member variable for format combo box
    QSpinBox *qualitySpinBox; // Member variable for quality spin box

    void populateFormats(QComboBox *formatCombo) {
        QList<QByteArray> formats = QImageWriter::supportedImageFormats();
        for (const QByteArray &format : formats) {
            formatCombo->addItem(QString::fromLatin1(format).toUpper(), QString::fromLatin1(format));
        }
    }

    void convertImages(const QString &format, int quality) {
        QStringList inputImagePaths = QFileDialog::getOpenFileNames(
                this, tr("Open Images"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp *.gif)"),
                nullptr, QFileDialog::Option::DontUseNativeDialog);

        for (const QString &inputImagePath : inputImagePaths) {
            if (!inputImagePath.isEmpty()) {
                convertImage(inputImagePath, format, quality);
            }
        }
    }

    void convertImage(const QString &inputImagePath, const QString &format, int quality) {
        QImage image(inputImagePath);
        if (image.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("Could not load the image."));
            return;
        }

        QString outputImagePath = QFileDialog::getSaveFileName(
                this, tr("Save Image"), "", QString("%1 Files (*.%2)").arg(format, format.toLower()),
                nullptr, QFileDialog::Option::DontUseNativeDialog);
        if (outputImagePath.isEmpty()) return;

        QImageWriter writer(outputImagePath, format.toLatin1());
        writer.setQuality(quality);
        if (!writer.write(image)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not write the image."));
            return;
        }

        QMessageBox::information(this, tr("Success"), tr("Image converted successfully."));
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QApplication::setApplicationVersion("beta 1");

    ImageConverter converter;
    converter.show();

    return QApplication::exec();
}
