#include "console.h"
#include "ui_console.h"
#include <QtWidgets>
#include <QByteArray>
#include <QScrollBar>
#include <QRegularExpression>
#include <QtCore/QDebug>
#include <QDebug>
#include <QSqlTableModel>
#include <QListWidgetItem>
#include <QDateTime>

QByteArray buffer;

Console::Console(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    ui->plainTextEdit->document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    ui->plainTextEdit->setPalette(p);
}

void Console::putData(const QByteArray &data)
{
    buffer.append(data);
    if (HexEnabled) {
        ui->plainTextEdit->insertPlainText(data.toHex());
    } else {
        ui->plainTextEdit->insertPlainText(data);
    }
    ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());
    while (!QByteArray(buffer).isEmpty()) {
        qDebug() << buffer;
        uint size = (unsigned char)buffer[1];
        qDebug() << "size" << size << "buffersize" << buffer.length();
        if (buffer.length()>=(size+4)) {
            unsigned short crc16 = Calculate_CRC_CCITT(reinterpret_cast<const unsigned char*>(buffer.mid(0, size+2).data()), size+2);
            QByteArray calc_crc((const char*)&crc16, sizeof(crc16));
            qDebug() << "calc_crc" << calc_crc;
            QByteArray sent_crc = buffer.mid(size+2,2);
            qDebug() << "sent_crc" << sent_crc;
            if (calc_crc==sent_crc) {
                qDebug() << "Validated message" << buffer.mid(2,size) << "address" << buffer.mid(0,1);
                QListWidgetItem *item = new QListWidgetItem();
                QDateTime local(QDateTime::currentDateTime());
                if (HexEnabled) {
                    item->setText(buffer.mid(2,size).toHex());
                    item->setStatusTip("Date:"+local.toString("MM/dd/yyyy")+", Time:"+local.toString("hh:mm:ss.z")+", Address:"+buffer.mid(0,1).toHex()+", Size:"+buffer.mid(1,1).toHex()+", Message:"+buffer.mid(2,size).toHex()+"\n");
                } else {
                    item->setText(buffer.mid(2,size));
                    item->setStatusTip("Date:"+local.toString("MM/dd/yyyy")+", Time:"+local.toString("hh:mm:ss.z")+", Address:"+buffer.mid(0,1).toHex()+", Size:"+buffer.mid(1,1).toHex()+", Message:"+buffer.mid(2,size)+"\n");
                }
                ui->listWidget->addItem(item);
                ui->listWidget->scrollToBottom();
                ui->textBrowser->setText(item->statusTip());
                buffer.remove(0,size+4);
            } else {
                buffer.remove(0, 1);
                qDebug() << "no message";
            }
        } else if (buffer.length()<(size+4)) {
            break;
        } else {
            buffer.remove(0,1);
            qDebug() << "no message";
        }
    }
}

static const unsigned short CRC_CCITT_TABLE[256] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
unsigned short Console::Calculate_CRC_CCITT(const unsigned char* buffer, int size)
{
    unsigned short tmp;
    unsigned short crc = 0xffff;

    for (int i=0; i < size ; i++)
    {
        tmp = (crc >> 8) ^ buffer[i];
        crc = (crc << 8) ^ CRC_CCITT_TABLE[tmp];
    }

    return crc;
}

void Console::setHexEnabled(bool set)
{
    HexEnabled = set;
}

void Console::setPlainEnabled(bool set)
{
    ui->plainTextEdit->setEnabled(set);
}


void Console::clear()
{
    ui->plainTextEdit->clear();
    buffer = "";
}

//void Console::keyPressEvent(QKeyEvent *e)
//{
//    switch (e->key()) {
//    case Qt::Key_Backspace:
//    case Qt::Key_Left:
//    case Qt::Key_Right:
//    case Qt::Key_Up:
//    case Qt::Key_Down:
//        break;
//    default:
//        if (localEchoEnabled)
//            ui->plainTextEdit->QPlainTextEdit::keyPressEvent(e);
//        emit getData(e->text().toLocal8Bit());
//    }
//}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}

bool Console::save()
//! [9] //! [10]
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
//! [10]

//! [11]
bool Console::saveAs()
//! [11] //! [12]
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}
bool Console::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
//#ifndef QT_NO_CURSOR
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//#endif
//    out << ui->plainTextEdit->toPlainText();
//#ifndef QT_NO_CURSOR
//    QApplication::restoreOverrideCursor();
//#endif
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        out << item->statusTip();
    }

    setCurrentFile(fileName);
//    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
void Console::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
//    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "Log.txt";
    setWindowFilePath(shownName);
}

Console::~Console()
{
    delete ui;
}

void Console::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->textBrowser->setText(item->statusTip());
}
