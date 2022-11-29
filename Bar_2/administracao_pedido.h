#ifndef ADMINISTRACAO_PEDIDO_H
#define ADMINISTRACAO_PEDIDO_H

#include <QMainWindow>
#include "cadastro_entidade.h"
#include "administracao_estoque.h"
#include "cadastro_produto.h"
#include "produto.h"
#include <vector>


namespace Ui {
class administracao_pedido;
}

class administracao_pedido : public QMainWindow
{
    Q_OBJECT

public:
    explicit administracao_pedido(QWidget *parent = nullptr);
    ~administracao_pedido();
    void vincular_produto_estoque(QString id_produto, QString id_estoque);
    void vincular_produto(QString id_produto);
    void vincular_cliente(QString id_cliente);
    void vincular_fornecedor(QString id_fornecedor);
    void vincular_funcionario(QString id_funcionario);

private slots:

    void on_pushButton_cliente_clicked();

    void on_pushButton_fornecedor_clicked();

    void on_pushButton_funcionario_clicked();

    void on_pushButton_novo_2_clicked();

    void on_pushButton_gravar_2_clicked();

    void on_pushButton_novo_clicked();

    void on_pushButton_gravar_clicked();

    void on_pushButton_novo_produto_clicked();

    void on_pushButton_gravar_produto_clicked();

    void on_tableWidget_produtos_cellClicked(int row, int column);

    void on_pushButton_remover_clicked();

    void on_pushButton_selecionar_produto_clicked();

    void on_radioButton_venda_clicked(bool checked);

    void on_radioButton_compra_clicked(bool checked);

    void on_pushButton_voltar_clicked();

    void on_pushButton_voltar_2_clicked();

    void on_tableWidget_pedidos_cellClicked(int row, int column);

private:
    int linha_produto;
    Ui::administracao_pedido *ui;
    void lista_produtos_criar();
    void lista_produtos_atualizar(QString sql = "");
    void lista_pedidos_criar();
    void lista_pedidos_atualizar(QString sql = "");
    void closeEvent(QCloseEvent *event);
    void limpar_campos(bool limpar_administracao = true);

    cadastro_entidade * janela_cadastro_entidade;
    administracao_estoque * janela_administracao_estoque;
    cadastro_produto * janela_cadastro_produto;

    void novo();
    void gravar();

    std::vector<produto*> produtos;
};

#endif // ADMINISTRACAO_PEDIDO_H
