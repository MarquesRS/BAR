#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QDialog>
#include "cadastro_entidade.h"
#include "cadastro_produto.h"
#include "administracao_estoque.h"
#include "administracao_pedido.h"

namespace Ui { class Principal; }

class Principal : public QDialog {
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = nullptr);
    ~Principal();

private slots:
    void on_pushButton_cadastro_entidades_clicked();

    void on_pushButton_cadastro_produtos_clicked();

    void on_pushButton_estoque_clicked();

    void on_pushButton_fechar_clicked();

    void on_pushButton_pedidos_clicked();

private:
    Ui::Principal *ui;
    cadastro_entidade * janela_cadastro_entidade;
    cadastro_produto * janela_cadastro_produto;
    administracao_estoque * janela_estoque;
    administracao_pedido * janela_pedido;
};

#endif // PRINCIPAL_H
