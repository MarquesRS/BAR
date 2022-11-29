#include "cadastro_produto.h"
#include "ui_cadastro_produto.h"
#include "administracao_pedido.h"


#include <QCloseEvent>
void cadastro_produto::closeEvent(QCloseEvent *event) {
    QString id = ui->lineEdit_id_produto->text();
    if (this->origem == "cadastro_pedido") {
        ((administracao_pedido*)parent())->vincular_produto(id);
    }
    limpar_campos();
    event->accept();
}


cadastro_produto::cadastro_produto(QWidget *parent, QString id_produto, QString origem) : QMainWindow(parent), ui(new Ui::cadastro_produto) {
    ui->setupUi(this);
    setFixedSize(size());
    limpar_campos();
    ui->tabWidget_produtos->setCurrentIndex(0);
    lista_produtos_criar();
    lista_produtos_atualizar();
    if (id_produto != "0") {
        selecionar_produto(-1, id_produto);
    }
    this->origem = origem;
}


cadastro_produto::~cadastro_produto() {
    delete ui;
}


void cadastro_produto::retorno_id_fornecedor(QString id) {
    ui->lineEdit_id_fornecedor->setText(id);
}


void cadastro_produto::limpar_campos(bool limpar1, bool limpar2) {
    if (limpar1) {
        ui->tabWidget_produtos->setCurrentIndex(1);
        ui->lineEdit_preco_custo->setFocus();
        ui->lineEdit_id_produto->setText("0");
        ui->radioButton_ativo->setChecked(true);
        ui->lineEdit_preco_custo->clear();
        ui->lineEdit_preco_venda->clear();
        ui->lineEdit_id_fornecedor->setText("0");
        ui->lineEdit_marca->clear();
        ui->lineEdit_modelo->clear();
        ui->lineEdit_descricao->clear();
        ui->lineEdit_data_fabricacao->clear();
        ui->lineEdit_data_validade->clear();
        ui->lineEdit_data_cadastro->clear();
    }

    if (limpar2) {
        ui->checkBox_bebida->setChecked(false);
        ui->lineEdit_id_bebida->setText("0");
        ui->radioButton_menores->setChecked(false);
        ui->textEdit_ingredientes->clear();
        ui->lineEdit_teor_alcoolico->clear();
        ui->lineEdit_volume->clear();
    }
}


void cadastro_produto::on_pushButton_novo_clicked() {
    limpar_campos();
}


void cadastro_produto::on_pushButton_voltar_clicked() {
    this->close();
}


void cadastro_produto::on_checkBox_bebida_stateChanged(int arg1) {
    if (arg1) {
        ui->tab_bebida->setEnabled(true);
    }
    else {
        ui->tab_bebida->setEnabled(false);
    }
}


void cadastro_produto::lista_produtos_criar() {
    QStringList cabecalho = {"ID", "Marca", "Modelo", "Preço custo", "Preço venda", "Ativo", "Bebida"};
    ui->tableWidget_produtos->setHorizontalHeaderLabels(cabecalho);
    int tamanho_colunas[7] = {20, 100, 100, 80, 80, 50, 50};
    for (int i = 0; i < 7; i++) {
        ui->tableWidget_produtos->setColumnWidth(i, tamanho_colunas[i]);
    }
    ui->tableWidget_produtos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_produtos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_produtos->verticalHeader()->setVisible(false);
}


void cadastro_produto::lista_produtos_atualizar(QString sql) {
    if (sql == "") {
        sql = "select id, marca, modelo, preco_custo, preco_venda, ativo from produto where ativo = 1";
    }
    ui->tableWidget_produtos->setRowCount(0);
    QSqlQuery query_produto, query_bebida;
    QString resultado;
    query_produto.exec(sql);
    for (int linha = 0; query_produto.next(); linha++) {
        ui->tableWidget_produtos->insertRow(linha);
        for (int registro = 0; registro < 6; registro++) {
            if (registro == 5) {
                query_produto.value(registro).toBool() ? resultado = "Sim" : resultado = "Não";
                ui->tableWidget_produtos->setItem(linha, registro, new QTableWidgetItem(resultado));
            }
            else {
                ui->tableWidget_produtos->setItem(linha, registro, new QTableWidgetItem(query_produto.value(registro).toString()));
            }
        }
        ui->tableWidget_produtos->setRowHeight(linha, 20);

        query_bebida.prepare("select id from bebida where id_produto = " + query_produto.value(0).toString());
        (query_bebida.exec() && query_bebida.next() != 0 ) ? resultado = "Sim" : resultado = "Não";
        ui->tableWidget_produtos->setItem(linha, 6, new QTableWidgetItem(resultado));
    }
}


void cadastro_produto::on_pushButton_remover_clicked() {
    QString id = ui->lineEdit_id_produto->text();
    if (id == "0") {
        QMessageBox::information(this, "Remover", "ERRO: não é possível remover um produto não selecionado.");
        return;
    }
    QSqlQuery query;
    query.exec("delete from bebida where id_produto = " + id);
    query.exec("delete from produto where id = " + id);
    limpar_campos();
    lista_produtos_atualizar();
}


void cadastro_produto::selecionar_produto(int row, QString id_parent) {
    ui->tabWidget_produtos->setCurrentIndex(1);
    ui->lineEdit_preco_custo->setFocus();
    QString id;
    if (row == -1) {
        id = id_parent;

    }
    else {
        id = ui->tableWidget_produtos->item(row, 0)->text();
    }
    QSqlQuery query;

    query.exec("select ativo, preco_custo, preco_venda, id_fornecedor, marca, modelo, descricao, "
               "data_fabricacao, data_vencimento, data_cadastro, id_fornecedor "
               "from produto where id = " + id);
    if (query.next()) {
        ui->lineEdit_id_produto->setText(id);
        ui->radioButton_ativo->setChecked(query.value(0).toBool());
        ui->lineEdit_preco_custo->setText(query.value(1).toString());
        ui->lineEdit_preco_venda->setText(query.value(2).toString());
        ui->lineEdit_id_fornecedor->setText(query.value(3).toString());
        ui->lineEdit_marca->setText(query.value(4).toString());
        ui->lineEdit_modelo->setText(query.value(5).toString());
        ui->lineEdit_descricao->setText(query.value(6).toString());
        ui->lineEdit_data_fabricacao->setText(query.value(7).toString());
        ui->lineEdit_data_validade->setText(query.value(8).toString());
        ui->lineEdit_data_cadastro->setText(query.value(9).toString());
        ui->lineEdit_id_fornecedor->setText(query.value(10).toString());
    }

    query.exec("select id, menores, ingredientes, teor_alcolico, volume from bebida where id_produto = " + id);
    if (query.next()) {
        ui->checkBox_bebida->setChecked(true);
        ui->lineEdit_id_bebida->setText(query.value(0).toString());
        ui->radioButton_menores->setChecked(query.value(1).toBool());
        ui->textEdit_ingredientes->setText(query.value(2).toString());
        ui->lineEdit_teor_alcoolico->setText(query.value(3).toString());
        ui->lineEdit_volume->setText(query.value(4).toString());
    }
    else {
        limpar_campos(false, true); // true é para limpar os campos de bebida
    }

    if (ui->radioButton_menores->isChecked()) {
        ui->lineEdit_teor_alcoolico->setEnabled(false);
    }
    else {
        ui->lineEdit_teor_alcoolico->setEnabled(true);
    }

    lista_produtos_atualizar();
}


void cadastro_produto::on_tableWidget_produtos_cellActivated(int row, int column) {
    selecionar_produto(row);
}


void cadastro_produto::on_pushButton_pesquisar_clicked() {
    QString filtro = ui->lineEdit_pesquisar_descricao->text();
    QString sql = "";
    if (!filtro.isEmpty()) {
        sql = "select id, marca, modelo, preco_custo, preco_venda, ativo where descricao like '%"+filtro+"%'";
    }
    lista_produtos_atualizar(sql);
}


void cadastro_produto::on_pushButton_gravar_clicked() {
    QString id = ui->lineEdit_id_produto->text();
    QString ativo = QString::number(ui->radioButton_ativo->isChecked());
    QString preco_custo = ui->lineEdit_preco_custo->text();
    QString preco_venda = ui->lineEdit_preco_venda->text();
    QString id_fornecedor = ui->lineEdit_id_fornecedor->text();
    QString marca = ui->lineEdit_marca->text();
    QString modelo = ui->lineEdit_modelo->text();
    QString descricao = ui->lineEdit_descricao->text();
    QString data_fabricacao = ui->lineEdit_data_fabricacao->text();
    QString data_validade = ui->lineEdit_data_validade->text();
    QString data_cadastro = ui->lineEdit_data_cadastro->text();
    if (preco_custo.isEmpty() || preco_venda.isEmpty() || marca.isEmpty() || modelo.isEmpty() || descricao.isEmpty() || data_fabricacao.isEmpty() || id_fornecedor.isEmpty()) {
        QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
        return;
    }
    else if (!data_validade.isEmpty() && data_validade.size() < 10) {
        QMessageBox::information(this, "Gravar", "Erro: a data de validade informada é inválida.");
        return;
    }
    else if(data_fabricacao.size() < 10) {
        QMessageBox::information(this, "Gravar", "Erro: a data de fabricação informada é inválida.");
        return;
    }

    QString id_bebida, menores, ingredientes, teor_alcoolico, volume;
    id_bebida = menores = ingredientes = teor_alcoolico = volume = "";
    bool bebida = ui->checkBox_bebida->isChecked();
    if (bebida) {
        id_bebida = ui->lineEdit_id_bebida->text();
        menores = QString::number(ui->radioButton_menores->isChecked());
        ingredientes = ui->textEdit_ingredientes->toPlainText();
        teor_alcoolico = ui->lineEdit_teor_alcoolico->text();
        volume = ui->lineEdit_volume->text();
        if (teor_alcoolico.isEmpty() || volume.isEmpty()) {
            QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
            return;
        }
    }

    QString sql = "";
    QSqlQuery query;
    if (id == "0") {
        QDateTime date = QDateTime::currentDateTime();
        QString data_cadastro = date.toString("dd/MM/yyyy"); // Pegar data do sistema
        sql = QString("insert into produto (ativo, preco_custo, preco_venda, marca, modelo, data_fabricacao, data_vencimento, "
                      "data_cadastro, descricao, id_fornecedor, quantidade, total_custo, total_venda) "
                     "values (%2, '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', %11, 0, 0.0, 0.0) "
                     "").arg(ativo, preco_custo, preco_venda, marca, modelo, data_fabricacao, data_validade, data_cadastro, descricao, id_fornecedor);
        query.exec(sql);
        query.exec("select id from produto ORDER BY id desc limit 1");
        query.first();
        QString id_ultimo = query.value(0).toString();
        ui->lineEdit_id_produto->setText(id_ultimo);

        if (ui->checkBox_bebida->isChecked()) {
            sql = QString("insert into bebida (id_produto, menores, volume, teor_alcolico, ingredientes) "
                        "values (%0, %1, '%2', '%3', '%4')").arg(id_ultimo, menores, volume, teor_alcoolico, ingredientes);
            query.exec(sql);
            query.exec("select id from bebida ORDER BY id desc limit 1");
            query.first();
            ui->lineEdit_id_bebida->setText(query.value(0).toString());
        }

        QMessageBox::information(this, "Gravar", "Sucesso: produto cadastrado.");
    }
    else {
        sql = QString("update produto set ativo = %1, preco_custo = '%2', preco_venda = '%3', marca = '%4', modelo = '%5', data_fabricacao = '%6', "
                      "data_vencimento = '%7', descricao = '%8', id_fornecedor = %9 where id = %10 "
                      "").arg(ativo, preco_custo, preco_venda, marca, modelo, data_fabricacao, data_validade, descricao, id_fornecedor, id);
        query.exec(sql);

        if (ui->checkBox_bebida->isChecked()) {
            query.exec("select id from bebida where id_produto = " + id);
            if (query.next()) {
                sql = QString("update bebida set menores = %1, volume = '%2', teor_alcolico = '%3', ingredientes = '%4' "
                              "where id_produto = %5").arg(menores, volume, teor_alcoolico, ingredientes, id);
                query.exec(sql);
            }
            else {
                sql = QString("insert into bebida (id_produto, volume, teor_alcolico, ingredientes, menores) "
                              "values  (%1, '%2', '%3', '%4', %5)").arg(id, volume, teor_alcoolico, ingredientes, menores);
                query.exec(sql);
                query.exec("select id from bebida ORDER BY id desc limit 1");
                query.first();
                ui->lineEdit_id_bebida->setText(query.value(0).toString());
            }
        }
        else {
            query.exec("delete from bebida where id_produto = " + id);
        }

        QMessageBox::information(this, "Gravar", "Sucesso: produto atualizado.");
    }

    lista_produtos_atualizar();
}


void cadastro_produto::on_pushButton_fornecedor_clicked() {
    this->janela_cadastro_entidade = new cadastro_entidade(this, true);
    this->janela_cadastro_entidade->show();
}


void cadastro_produto::on_tableWidget_produtos_cellClicked(int row, int column) {
    selecionar_produto(row);
}



void cadastro_produto::on_radioButton_menores_clicked(bool checked) {
    if (checked) {
        ui->lineEdit_teor_alcoolico->setEnabled(false);
    }
    else {
        ui->lineEdit_teor_alcoolico->setEnabled(true);
    }
}

