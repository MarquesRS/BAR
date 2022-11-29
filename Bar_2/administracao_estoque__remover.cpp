#include "administracao_estoque__remover.h"
#include "ui_administracao_estoque__remover.h"
#include "administracao_estoque.h"
#include <QMessageBox>
#include <QCloseEvent>


void administracao_estoque__remover::closeEvent(QCloseEvent *event) {
    ((administracao_estoque*)parent())->lista_produtos_atualizar(this->id_estoque);
    event->accept();
}


administracao_estoque__remover::administracao_estoque__remover(QWidget *parent, QString id_estoque, QString id_produto) : QDialog(parent), ui(new Ui::administracao_estoque__remover) {
    ui->setupUi(this);
    setFixedSize(size());
    ui->lineEdit_id->setText(id_produto);
    this->id_estoque = id_estoque;
}


administracao_estoque__remover::~administracao_estoque__remover() {
    delete ui;
}


void administracao_estoque__remover::on_pushButton_cancelar_clicked() {
    this->close();
}


void administracao_estoque__remover::on_pushButton_remover_clicked() {
    QSqlQuery query;
    QString id_produto = ui->lineEdit_id->text();
    QString quantidade = ui->lineEdit_quantidade->text();
    QString quantidade_em_estoque = "0";
    query.exec("select quantidade from estoque_produto where id_estoque = " + this->id_estoque + " and id_produto = " + id_produto);
    if (query.next()) {
        quantidade_em_estoque = query.value(0).toString();
    }

    if (ui->radioButton_remover_tudo->isChecked()) {
        query.exec("delete from estoque_produto where id_estoque = " + this->id_estoque + " and id_produto = " + id_produto);
        quantidade = quantidade_em_estoque;
    }
    else {
        if (quantidade.isEmpty() || quantidade.toInt() <= 0) {
            QMessageBox::information(this, "Remover", "Erro: a quantidade informada deve ser maior que zero.");
             return;
        }
        else if (quantidade.toInt() >= quantidade_em_estoque.toInt() || quantidade.toInt() <= 0) {
            QMessageBox::information(this, "Remover", "Erro: a quantidade informada é inválida.");
            return;
        }
        QString sql = QString("update estoque_produto set quantidade = quantidade - %1 where id_estoque = %2 and id_produto = %3 "
                              "").arg(quantidade, this->id_estoque, id_produto);
        query.exec(sql);
    }

    query.exec("update produto set quantidade = quantidade - " + quantidade + " "
               "where id_produto = " + id_produto);
    query.exec("update produto set total_custo = preco_custo * quantidade, total_venda = preco_venda * quantidade "
               "where id_produto = " + id_produto);
    QMessageBox::information(this, "Remover", "Sucesso: quantidade atualizada.");
    this->close();
}

