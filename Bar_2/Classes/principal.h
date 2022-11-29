#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QMainWindow>
#include "cadastro_endereco.h"

namespace Ui {
class Principal;
}

class Principal : public QMainWindow
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = nullptr);
    ~Principal();

private slots:
    void on_pushButton_cadastro_endereco_clicked();

private:
    Ui::Principal *ui;
    cadastro_endereco * janela_cadastro_endereco;
};

#endif // PRINCIPAL_H
