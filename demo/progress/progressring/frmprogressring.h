#ifndef FRMPROGRESSRING_H
#define FRMPROGRESSRING_H

#include <QWidget>

namespace Ui {
class frmProgressRing;
}

class frmProgressRing : public QWidget
{
    Q_OBJECT

public:
    explicit frmProgressRing(QWidget *parent = 0);
    ~frmProgressRing();

private:
    Ui::frmProgressRing *ui;

private slots:
    void initForm();
    void on_ckAnimation_stateChanged(int arg1);
    void on_ckClockWise_stateChanged(int arg1);
    void on_cboxStartAngle_currentIndexChanged(int);
};

#endif // FRMPROGRESSRING_H
