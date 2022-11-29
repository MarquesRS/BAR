#ifndef CADASTRO_PRODUTO_H
#define CADASTRO_PRODUTO_H

#include <QMainWindow>
#include <QSql>
#include <QSqlQuery>
#include <QDateTime>
#include <QMessageBox>
#include "cadastro_entidade.h"

namespace Ui { class cadastro_produto; }

class cadastro_produto : public QMainWindow {
    Q_OBJECT

public:
    explicit cadastro_produto(QWidget *parent = nullptr, QString id_produto = "0", QString origem = "");
    ~cadastro_produto();
    void retorno_id_fornecedor(QString id);

private slots:
    void on_pushButton_novo_clicked();

    void on_pushButton_voltar_clicked();

    void on_checkBox_bebida_stateChanged(int arg1);

    void on_pushButton_remover_clicked();

    void on_tableWidget_produtos_cellActivated(int row, int column);

    void on_pushButton_pesquisar_clicked();

    void on_pushButton_gravar_clicked();

    void on_pushButton_fornecedor_clicked();

    void on_tableWidget_produtos_cellClicked(int row, int column);

    void on_radioButton_menores_clicked(bool checked);

private:
    Ui::cadastro_produto *ui;
    void limpar_campos(bool limpar1 = true, bool limpar2 = true);
    void lista_produtos_criar();
    void lista_produtos_atualizar(QString sql = "");
    cadastro_entidade * janela_cadastro_entidade;
    void closeEvent(QCloseEvent *event);
    void selecionar_produto(int row = -1, QString id_parent = "0");
    QString origem;
};

#endif // CADASTRO_PRODUTO_H
