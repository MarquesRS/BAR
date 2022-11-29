#include "principal.h"
#include "ui_principal.h"


Principal::Principal(QWidget *parent) : QDialog(parent), ui(new Ui::Principal) {
    ui->setupUi(this);
    QPixmap pixmap(1,1);
    setFixedSize(size());
    pixmap.fill(Qt::transparent);
    this->setWindowIcon(pixmap);
    ui->pushButton_fechar->setFocus();
    this->janela_cadastro_entidade = new cadastro_entidade(this);
    this->janela_cadastro_produto = new cadastro_produto(this);
    this->janela_estoque = new administracao_estoque(this);
    this->janela_pedido = new administracao_pedido(this);
}


Principal::~Principal() {
    delete this->janela_cadastro_entidade;
    delete this->janela_cadastro_produto;
    delete this->janela_estoque;
    delete this->janela_pedido;
    delete ui;
}


void Principal::on_pushButton_cadastro_entidades_clicked() {
    this->janela_cadastro_entidade->show();
}


void Principal::on_pushButton_cadastro_produtos_clicked() {
    this->janela_cadastro_produto->show();
}


void Principal::on_pushButton_estoque_clicked() {
    this->janela_estoque->show();

}


void Principal::on_pushButton_fechar_clicked() {
    this->close();
}


void Principal::on_pushButton_pedidos_clicked() {
    this->janela_pedido->show();
}
