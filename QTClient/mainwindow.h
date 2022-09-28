#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
   void slotReadyRead();

private slots:
    void on_pushButton_clicked();
    void sendToServer(QString str);

    void on_pushButton_3_clicked();

    void on_pushButton_disconnected_clicked();


private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    QByteArray Data;
    QString ipStr;
    quint16 portStr,
    nextBlockSize;
};
#endif // MAINWINDOW_H
