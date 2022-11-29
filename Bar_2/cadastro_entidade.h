#ifndef CADASTRO_ENTIDADE_H
#define CADASTRO_ENTIDADE_H

#include <QMainWindow>
#include "cadastro_endereco.h"
#include <QSql>
#include <QSqlQuery>
#include <QDateTime>
#include <QMessageBox>

namespace Ui { class cadastro_entidade; }

class cadastro_entidade : public QMainWindow {
    Q_OBJECT

public:
    explicit cadastro_entidade(QWidget *parent = nullptr,
                               bool origem_cadastro_produto = false,
                               bool origem_administracao_pedidos = false,
                               bool fornecedor = 0,
                               bool funcionario = 0);
    ~cadastro_entidade();
    void retorno_id_endereco(QString id);

private slots:

    void on_pushButton_vincular_endereco_clicked();

    void on_checkBox_fornecedor_stateChanged(int arg1);

    void on_checkBox_funcionario_stateChanged(int arg1);

    void on_pushButton_gravar__clicked();

    void on_pushButton_novo__clicked();

    void on_pushButton_remover_clicked();

    void on_tableWidget_entidades_cellActivated(int row, int column);

    void on_pushButton_pesquisar_clicked();

    void on_pushButton_voltar_clicked();

private:
    Ui::cadastro_entidade *ui;
    Cadastro_endereco * janela_cadastro_endereco;
    void lista_entidades_criar();
    void lista_entidades_atualizar(QString sql = "");
    void limpar_campos(bool limpar_1 = true, bool limpar_2  = true, bool limpar_3 = true);
    QString origem;
    bool retornar_fornecedor;
    bool retornar_cliente;
    bool retornar_funcionario;
    void closeEvent(QCloseEvent *event);
};

#endif // CADASTRO_ENTIDADE_H
