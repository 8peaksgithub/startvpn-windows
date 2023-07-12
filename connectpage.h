#ifndef CONNECTPAGE_H
#define CONNECTPAGE_H

#include <QWidget>

namespace Ui {
class connectpage;
}

class connectpage : public QWidget
{
    Q_OBJECT

public:
    explicit connectpage(QWidget *parent = nullptr);
    ~connectpage();

private slots:
    void on_toolButton_clicked();

private:
    Ui::connectpage *ui;
};

#endif // CONNECTPAGE_H
