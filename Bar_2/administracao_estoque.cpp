#include "administracao_estoque.h"
#include "ui_administracao_estoque.h"
#include "administracao_pedido.h"
#include <QCloseEvent>


void administracao_estoque::closeEvent(QCloseEvent *event) {
    QString id_estoque = ui->lineEdit_id_estoque->text();
    QString id_produto = "";
    if (origem == "cadastro_pedido") {
        int linha = ui->tableWidget_produtos->currentRow();
        if (id_estoque.isEmpty() || id_estoque == "0") {
            QMessageBox::information(this, "Retornar", "AVISO: nenhum estoque selecionado.");
        }
        else if (linha == -1) {
            QMessageBox::information(this, "Retornar", "AVISO: apenas o estoque foi selecionado.");
        }
        else {
            id_produto = ui->tableWidget_produtos->item(linha, 0)->text();
        }
        ((administracao_pedido*)parent())->vincular_produto_estoque(id_produto, id_estoque);
    }
    limpar_campos();
    event->accept();
}


administracao_estoque::administracao_estoque(QWidget *parent, QString origem)
    : QMainWindow(parent), ui(new Ui::administracao_estoque) {
    ui->setupUi(this);
    setFixedSize(size());
    lista_produtos_criar();
    lista_estoques_criar();
    lista_estoques_atualizar();
    ui->tabWidget->setCurrentIndex(0);
    this->origem = origem;
}

administracao_estoque::~administracao_estoque() {
    limpar_campos();
    delete this->janela_cadastro_produto;
    delete this->janela_remover;
    delete ui;
}


void administracao_estoque::lista_estoques_criar() {
    QStringList cabecalho = {"ID", "Nome", "Finalidade", "Quantidade", "Total custo", "Total venda", "Ativo"};
    ui->tableWidget_estoque->setHorizontalHeaderLabels(cabecalho);
    int tamanho_colunas[6] = {50, 100, 100, 100, 100, 100};
    for (int i = 0; i < 6; i++) {
        ui->tableWidget_estoque->setColumnWidth(i, tamanho_colunas[i]);
    }
    ui->tableWidget_estoque->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_estoque->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_estoque->verticalHeader()->setVisible(false);
}


void administracao_estoque::lista_estoques_atualizar(QString sql) {
    if (sql == "") {
        sql = "select id, nome, finalidade, quantidade, total_custo, total_venda, ativo from estoque";
    }
    ui->tableWidget_estoque->setRowCount(0);
    QSqlQuery query_estoque;
    QString resultado;
    query_estoque.exec(sql);
    for (int linha = 0; query_estoque.next(); linha++) {
        ui->tableWidget_estoque->insertRow(linha);
        for (int registro = 0; registro < 7; registro++) {
            resultado = query_estoque.value(registro).toString();
            if (registro == 6) {
                query_estoque.value(registro).toBool() ? resultado = "Sim" : resultado = "Não";
            }
            ui->tableWidget_estoque->setItem(linha, registro, new QTableWidgetItem(resultado));
        }
        ui->tableWidget_estoque->setRowHeight(linha, 20);
    }
}


void administracao_estoque::lista_produtos_criar() {
    QStringList cabecalho = {"Estoque", "Produto", "Quantidade", "Marca", "Modelo", "Descrição", "Total custo", "Total venda", "Bebida"};
    ui->tableWidget_produtos->setHorizontalHeaderLabels(cabecalho);
    int tamanho_colunas[9] = {80, 80, 100, 100, 100, 100, 100, 100, 80};
    for (int i = 0; i < 9; i++) {
        ui->tableWidget_produtos->setColumnWidth(i, tamanho_colunas[i]);
    }
    ui->tableWidget_produtos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_produtos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_produtos->verticalHeader()->setVisible(false);
}


void administracao_estoque::lista_produtos_atualizar(QString id_estoque) {
    QString sql = "select "
                    "estoque_produto.id_estoque, "
                    "produto.id, "
                    "sum(estoque_produto.quantidade) as quantidade, "
                    "produto.marca, "
                    "produto.modelo, "
                    "produto.descricao, "
                    "produto.total_custo, "
                    "produto.total_venda "
                  "from "
                    "produto "
                  "inner join estoque_produto on "
                    "estoque_produto.id_produto = produto.id and estoque_produto.id_estoque = " + id_estoque + " "
                  "group by estoque_produto.id_produto ";
    ui->tableWidget_produtos->setRowCount(0);
    QSqlQuery query_produto, query_bebida;
    QString resultado;
    query_produto.exec(sql);
    for (int linha = 0; query_produto.next(); linha++) {
        ui->tableWidget_produtos->insertRow(linha);
        for (int registro = 0; registro < 8; registro++) {
            ui->tableWidget_produtos->setItem(linha, registro, new QTableWidgetItem(query_produto.value(registro).toString()));
        }

        query_bebida.exec("select id from bebida where id_produto = " + query_produto.value(1).toString());
        query_bebida.next() ? resultado = "Sim" : resultado = "Não";
        ui->tableWidget_produtos->setItem(linha, 8, new QTableWidgetItem(resultado));

        ui->tableWidget_produtos->setRowHeight(linha, 20);
  }
}


void administracao_estoque::on_tableWidget_produtos_cellActivated(int row, int column) {
    QString id = ui->tableWidget_produtos->item(row, 1)->text();
    this->janela_cadastro_produto = new cadastro_produto(this, id, "");
    this->janela_cadastro_produto->show();
}


void administracao_estoque::on_tableWidget_produtos_cellClicked(int row, int column) {
    ui->pushButton_remover->setEnabled(true);
    this->linha = row;
}


void administracao_estoque::remover_produto() {
    QString id_estoque = ui->tableWidget_produtos->item(this->linha, 0)->text();
    QString id_produto = ui->tableWidget_produtos->item(this->linha, 1)->text();
    this->janela_remover = new administracao_estoque__remover(this, id_estoque, id_produto);
    this->janela_remover->show();
}


void administracao_estoque::on_pushButton_remover_clicked() {
    remover_produto();
}


void administracao_estoque::limpar_campos() {
    ui->tabWidget->setCurrentIndex(2);
    ui->lineEdit_id_estoque->setText("0");
    ui->radioButton_ativo->setChecked(true);
    ui->lineEdit_nome->clear();
    ui->lineEdit_finalidade->clear();
}


void administracao_estoque::on_pushButton_novo_estoque_clicked() { limpar_campos(); }


void administracao_estoque::on_pushButton_novo_estoque_2_clicked() { limpar_campos(); }


void administracao_estoque::gravar() {
    QString id = ui->lineEdit_id_estoque->text();
    QString ativo = QString::number(ui->radioButton_ativo->isChecked());
    QString nome = ui->lineEdit_nome->text();
    QString finalidade = ui->lineEdit_finalidade->text();

    if (nome.isEmpty() || finalidade.isEmpty()) {
        QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
        return;
    }

    QString sql;
    QSqlQuery query;
    if (id=="0") {
        sql = QString("insert into estoque (ativo, nome, finalidade) values (%1, '%2', '%3')").arg(ativo, nome, finalidade);
        query.exec(sql);
        QMessageBox::information(this, "Gravar", "Sucesso: estoque cadastrado.");
        query.exec("select id from estoque order by id desc limit 1");
        ui->lineEdit_id_estoque->setText(query.value(0).toString());
    }
    else {
        sql = QString("update estoque set ativo = %1, nome = '%2', finalidade = '%3' where id = %4"
                      "").arg(ativo, nome, finalidade, id);
        query.exec(sql);
        QMessageBox::information(this, "Gravar", "Sucesso: estoque atualizado");
    }

    lista_estoques_atualizar();
}


void administracao_estoque::on_pushButton_gravar_estoque_clicked() { gravar(); }


void administracao_estoque::on_pushButton_gravar_estoque_2_clicked() { gravar(); }


void administracao_estoque::on_tableWidget_estoque_cellActivated(int row, int column) {
    ui->tabWidget->setCurrentIndex(1);
    ui->lista_produtos->setEnabled(true);
    QString id = ui->tableWidget_estoque->item(row, 0)->text();
    lista_produtos_atualizar(id);
    ui->pushButton_remover->setEnabled(false);
}


void administracao_estoque::on_tableWidget_estoque_cellClicked(int row, int column) {
    QString id = ui->tableWidget_estoque->item(row, 0)->text();
    QString nome = ui->tableWidget_estoque->item(row, 1)->text();
    QString finalidade = ui->tableWidget_estoque->item(row, 2)->text();
    QString ativo;

    if (ui->tableWidget_estoque->item(row, 6)->text() == "Sim") {
        ativo = "1";
    }
    else {
        ativo = "0";
    }

    ui->lineEdit_id_estoque->setText(id);
    ui->lineEdit_nome->setText(nome);
    ui->lineEdit_finalidade->setText(finalidade);
    ui->radioButton_ativo->setChecked(ativo.toInt());
}


void administracao_estoque::remover_estoque() {
    QString id = ui->lineEdit_id_estoque->text();
    if (id.isEmpty() || id == "0") {
        QMessageBox::information(this, "Remover", "ERRO: não é possível remover um estoque não selecionado.");
        return;
    }

    QSqlQuery query;
    query.exec("select * from estoque_produto where id_estoque = " + id);
    if (query.next()) {
        QMessageBox::information(this, "Remover", "ERRO: não é possível remover um estoque que contenha produtos vinculados.");
        return;
    }
    else {
        query.exec("delete from estoque where id = " + id);
        QMessageBox::information(this, "Remover", "Sucesso: estoque removido.");
    }
    lista_estoques_atualizar();
    limpar_campos();
    ui->tabWidget->setCurrentIndex(0);
}


void administracao_estoque::on_pushButton_remover_estoque_clicked() { remover_estoque(); }


void administracao_estoque::on_pushButton_remover_estoque_2_clicked() { remover_estoque(); }


void administracao_estoque::on_pushButton_voltar_estoque_2_clicked() {
    this->close();
}


void administracao_estoque::on_pushButton_voltar_estoque_clicked() {
    this->close();
}

