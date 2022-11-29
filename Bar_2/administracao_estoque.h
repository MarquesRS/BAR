#ifndef ADMINISTRACAO_ESTOQUE_H
#define ADMINISTRACAO_ESTOQUE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>
#include "administracao_estoque__remover.h"
#include "cadastro_produto.h"

namespace Ui {
class administracao_estoque;
}

class administracao_estoque : public QMainWindow
{
    Q_OBJECT

public:
    explicit administracao_estoque(QWidget *parent = nullptr, QString origem = "");
    ~administracao_estoque();
    void lista_produtos_atualizar(QString id_estoque = "");

private slots:
    void on_tableWidget_produtos_cellActivated(int row, int column);

    void on_tableWidget_produtos_cellClicked(int row, int column);

    void on_pushButton_remover_clicked();

    void on_pushButton_novo_estoque_clicked();

    void on_pushButton_novo_estoque_2_clicked();

    void on_pushButton_gravar_estoque_clicked();

    void on_pushButton_gravar_estoque_2_clicked();

    void on_tableWidget_estoque_cellActivated(int row, int column);

    void on_tableWidget_estoque_cellClicked(int row, int column);

    void on_pushButton_remover_estoque_clicked();

    void on_pushButton_remover_estoque_2_clicked();

    void on_pushButton_voltar_estoque_2_clicked();

    void on_pushButton_voltar_estoque_clicked();

private:
    Ui::administracao_estoque *ui;
    void lista_produtos_criar();
    void lista_estoques_atualizar(QString sql = "");
    void lista_estoques_criar();
    administracao_estoque__remover * janela_remover;
    int linha;
    void limpar_campos();
    void remover_produto();
    void gravar();
    void remover_estoque();
    cadastro_produto * janela_cadastro_produto;
    QString origem;
    void closeEvent(QCloseEvent *event);

};

#endif // ADMINISTRACAO_ESTOQUE_H
