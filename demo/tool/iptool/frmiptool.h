#ifndef FRMIPTOOL_H
#define FRMIPTOOL_H

#include <QWidget>

namespace Ui {
class frmIPTool;
}

class frmIPTool : public QWidget
{
    Q_OBJECT

public:
    explicit frmIPTool(QWidget *parent = 0);
    ~frmIPTool();

private:
    Ui::frmIPTool *ui;

private:
    bool ipLive(const QString &ip, int port);
    QString getHtml(const QString &url);
    QString getNetIP(const QString &html);
    QString getLocalIP();
    QStringList getLocalIPs();
    bool isIP(const QString &ip);
    bool isWebOk();

private slots:
    void on_btnGetLocalIP_clicked();
    void on_btnGetWebIP_clicked();
    void on_btnWebOk_clicked();
    void on_btnUrlIP_clicked();
};

#endif // FRMIPTOOL_H
