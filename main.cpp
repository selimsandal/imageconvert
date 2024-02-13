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
#include <QTranslator>
#include <QLocale>

class ImageConverter : public QWidget {
public:
    ImageConverter(QWidget *parent = nullptr) : QWidget(parent) {
        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(5, 5, 5, 5); // Reduce window border margins
        mainLayout->setSpacing(5); // Reduce spacing between widgets

        // Format selector
        auto *formatLabel = new QLabel(tr("Select Format:"), this);
        mainLayout->addWidget(formatLabel);

        auto *formatCombo = new QComboBox(this);
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

        auto *qualitySpinBox = new QSpinBox(this);
        qualitySpinBox->setRange(0, 100);
        qualitySpinBox->setValue(100);
        qualityLayout->addWidget(qualitySpinBox);

        // Sync slider and spinbox
        connect(qualitySlider, &QSlider::valueChanged, qualitySpinBox, &QSpinBox::setValue);
        connect(qualitySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), qualitySlider, &QSlider::setValue);

        mainLayout->addLayout(qualityLayout); // Add the horizontal layout to the main vertical layout

        // Convert button
        auto *convertButton = new QPushButton(tr("Convert Image"), this);
        mainLayout->addWidget(convertButton);

        // Connect convert button click signal to the slot
        connect(convertButton, &QPushButton::clicked, this, [this, formatCombo, qualitySpinBox]() {
            QString format = formatCombo->currentData().toString();
            int quality = qualitySpinBox->value();
            convertImage(format, quality);
        });

        auto *copyrightlabel = new QLabel(tr("Selim Sandal, 2024"), this);
        mainLayout->addWidget(copyrightlabel,0 , Qt::AlignCenter);
        this->setWindowTitle("Image Convert");
    }

private:
    void populateFormats(QComboBox *formatCombo) {
        QList<QByteArray> formats = QImageWriter::supportedImageFormats();
        for (const QByteArray &format : formats) {
            formatCombo->addItem(QString::fromLatin1(format).toUpper(), QString::fromLatin1(format));
        }
    }

    void convertImage(const QString &format, int quality) {
        QString inputImagePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.*)"));
        if (inputImagePath.isEmpty()) return;

        QImage image(inputImagePath);
        if (image.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("Could not load the image."));
            return;
        }

        QString outputImagePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", QString("%1 Files (*.%2)").arg(format, format.toLower()));
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
