#ifndef CONSOLE_H
#define CONSOLE_H
#include <QListWidgetItem>
#include <QWidget>

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

public:
    explicit Console(QWidget *parent = 0);
    ~Console();
    void putData(const QByteArray &data);
    void setLocalEchoEnabled(bool set);
    void clear();
    bool save();
    bool saveAs();

protected:
//    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *e) Q_DECL_OVERRIDE;

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::Console *ui;
    bool localEchoEnabled;
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    quint16 CRC(uint16_t crc1, uint8_t data, uint16_t poly);
    unsigned short Calculate_CRC_CCITT(const unsigned char* buffer, int size);
//    QString strippedName(const QString &fullFileName);
//    QPlainTextEdit *textEdit;
    QString curFile;
};

#endif // CONSOLE_H
