#ifndef ADMINISTRACAO_ESTOQUE__REMOVER_H
#define ADMINISTRACAO_ESTOQUE__REMOVER_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>

namespace Ui {
class administracao_estoque__remover;
}

class administracao_estoque__remover : public QDialog
{
    Q_OBJECT

public:
    explicit administracao_estoque__remover(QWidget *parent = nullptr, QString id_estoque = "", QString id_produto = "");
    ~administracao_estoque__remover();

private slots:
    void on_pushButton_cancelar_clicked();

    void on_pushButton_remover_clicked();

private:
    Ui::administracao_estoque__remover *ui;
    void closeEvent(QCloseEvent *event);
    QString id_estoque;

};

#endif // ADMINISTRACAO_ESTOQUE__REMOVER_H
