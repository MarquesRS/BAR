#include "administracao_pedido.h"
#include "ui_administracao_pedido.h"
#include <QCloseEvent>



void administracao_pedido::closeEvent(QCloseEvent *event) {
    limpar_campos();
    event->accept();
}


administracao_pedido::administracao_pedido(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::administracao_pedido)
{
    ui->setupUi(this);
    limpar_campos();
    setFixedSize(size());
    lista_produtos_criar();
    lista_pedidos_criar();
    lista_pedidos_atualizar();
    this->janela_cadastro_entidade = nullptr;
    this->janela_administracao_estoque = nullptr;
    this->janela_cadastro_produto = nullptr;
}


administracao_pedido::~administracao_pedido() {
    delete ui;
}


void administracao_pedido::lista_produtos_criar() {
    QStringList cabecalho = {"Estoque", "Produto", "Quantidade", "Marca", "Modelo", "Preço custo", "Preço venda", "Ativo", "Bebida"};
    ui->tableWidget_produtos->setHorizontalHeaderLabels(cabecalho);
    int tamanho_colunas[9] = {80, 80, 100, 100, 100, 80, 80, 50, 50};
    for (int i = 0; i < 9; i++) {
        ui->tableWidget_produtos->setColumnWidth(i, tamanho_colunas[i]);
    }
    ui->tableWidget_produtos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_produtos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_produtos->verticalHeader()->setVisible(false);
}


void administracao_pedido::lista_pedidos_criar() {
    QStringList cabecalho = {"ID", "Tipo", "Valor", "Cliente", "Fornecedor", "Funcionario", "Data"};
    ui->tableWidget_pedidos->setHorizontalHeaderLabels(cabecalho);
    int tamanho_colunas[7] = {20, 100, 100, 100, 100, 100, 100};
    for (int i = 0; i < 7; i++) {
        ui->tableWidget_pedidos->setColumnWidth(i, tamanho_colunas[i]);
    }
    ui->tableWidget_pedidos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_pedidos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_pedidos->verticalHeader()->setVisible(false);
}


void administracao_pedido::lista_pedidos_atualizar(QString sql) {
    if (sql == "") {
        sql = "select "
                "pedido.id, "
                "pedido.tipo, "
                "pedido.valor, "
                "cliente.nome as cliente, "
                "pessoa_fornecedor.nome as fornecedor, "
                "pessoa_funcionario.nome as funcionario, "
                "data "
            "from pedido "
            "inner join pessoa as cliente on cliente.id = pedido.id_cliente "
            "inner join fornecedor on fornecedor.id = pedido.id_fornecedor "
            "inner join funcionario on funcionario.id = pedido.id_funcionario "
            "inner join pessoa as pessoa_funcionario on pessoa_funcionario.id = funcionario.id_Pessoa "
            "inner join pessoa as pessoa_fornecedor on pessoa_fornecedor.id = fornecedor.id_Pessoa";
    }
    ui->tableWidget_pedidos->setRowCount(0);
    QSqlQuery query_pedido;
    QString resultado;
    query_pedido.exec(sql);
    for (int linha = 0; query_pedido.next() != 0; linha++) {
        ui->tableWidget_pedidos->insertRow(linha);
        for (int registro = 0; registro < 7; registro++) {
            resultado = query_pedido.value(registro).toString();
            if (registro == 1) {
                query_pedido.value(registro).toInt() ? resultado = "Venda" : resultado = "Compra";
            }
            ui->tableWidget_pedidos->setItem(linha, registro, new QTableWidgetItem(resultado));
        }
        ui->tableWidget_pedidos->setRowHeight(linha, 20);
    }
}


void administracao_pedido::vincular_produto_estoque(QString id_produto, QString id_estoque) {
    ui->lineEdit_id_produto->setText(id_produto);
    ui->lineEdit_id_estoque->setText(id_estoque);
}

void administracao_pedido::vincular_produto(QString id_produto) {
    ui->lineEdit_id_produto->setText(id_produto);
}


void administracao_pedido::vincular_cliente(QString id_cliente) {
    ui->lineEdit_id_cliente->setText(id_cliente);
}


void administracao_pedido::vincular_fornecedor(QString id_fornecedor) {
    ui->lineEdit_id_fornecedor->setText(id_fornecedor);
}


void administracao_pedido::vincular_funcionario(QString id_funcionario) {
    ui->lineEdit_id_funcionario->setText(id_funcionario);
}


void administracao_pedido::on_pushButton_cliente_clicked() {
    this->janela_cadastro_entidade = new cadastro_entidade(this, 0, 1, 0, 0);
    this->janela_cadastro_entidade->show();
}


void administracao_pedido::on_pushButton_fornecedor_clicked() {
    this->janela_cadastro_entidade = new cadastro_entidade(this, 0, 1, 1, 0);
    this->janela_cadastro_entidade->show();
}


void administracao_pedido::on_pushButton_funcionario_clicked() {
    this->janela_cadastro_entidade = new cadastro_entidade(this, 0, 1, 0, 1);
    this->janela_cadastro_entidade->show();
}


void administracao_pedido::limpar_campos(bool limpar_administracao) {
    ui->pushButton_cliente->setEnabled(true);
    ui->pushButton_fornecedor->setEnabled(true);
    ui->pushButton_funcionario->setEnabled(true);
    ui->plainTextEdit_descricao->setReadOnly(false);
    ui->pushButton_selecionar_produto->setEnabled(true);
    ui->pushButton_novo_produto->setEnabled(true);
    ui->pushButton_gravar_produto->setEnabled(true);
    ui->pushButton_remover->setEnabled(true);
    ui->lineEdit_estoque_quantidade->setReadOnly(false);
    ui->pushButton_gravar->setEnabled(true);
    ui->pushButton_gravar_2->setEnabled(true);
    ui->radioButton_compra->setEnabled(true);
    ui->radioButton_venda->setEnabled(true);

    if (limpar_administracao) {
        ui->tabWidget->setCurrentIndex(1);
        ui->lineEdit_id_pedido->setText("0");
        ui->radioButton_venda->setChecked(true);
        ui->lineEdit_id_cliente->setText("0");
        ui->lineEdit_id_fornecedor->setText("0");
        ui->lineEdit_id_funcionario->setText("0");
        ui->lineEdit_data->clear();
        ui->plainTextEdit_descricao->clear();
    }
    ui->pushButton_remover->setEnabled(false);
    ui->lineEdit_id_produto->setText("0");
    ui->lineEdit_id_estoque->setText("0");
    ui->lineEdit_estoque_quantidade->clear();
    ui->tableWidget_produtos->setRowCount(0);
}


void administracao_pedido::on_pushButton_novo_clicked() { limpar_campos(); }


void administracao_pedido::on_pushButton_novo_2_clicked() { limpar_campos(); }


void administracao_pedido::gravar() {
    QString id = ui->lineEdit_id_pedido->text();
    QString id_cliente = ui->lineEdit_id_cliente->text();
    QString id_fornecedor = ui->lineEdit_id_fornecedor->text();
    QString id_funcionario = ui->lineEdit_id_funcionario->text();
    QString descricao = ui->plainTextEdit_descricao->toPlainText();
    QString compra = QString::number(ui->radioButton_compra->isChecked());

    if (compra.toInt()) {
        if (id_fornecedor.isEmpty() || id_fornecedor == "0") {
            QMessageBox::information(this, "Gravar", "Erro: para o tipo \"compra\" é preciso vincular um fornecedor além dos demais campos obrigatórios.");
            return;
        }
    }
    else if (id_cliente.isEmpty() || id_cliente == "0") {
        QMessageBox::information(this, "Gravar", "Erro: para o tipo \"venda\" é preciso vincular um cliente além dos demais campos obrigatórios.");
        return;
    }
    else if (id_funcionario.isEmpty() || id_funcionario == "0") {
        QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
        return;
    }

    QSqlQuery query;
    QString sql;
    if (id == "0") {
        QDateTime date = QDateTime::currentDateTime();
        QString data_cadastro = date.toString("dd/MM/yyyy"); // Pegar data do sistema
        sql = QString("insert into pedido "
                      "(id_cliente, id_fornecedor, id_funcionario, descricao, tipo, data) "
                      "values (%2, %3, %4, '%5', %6, '%7') "
                      "").arg(id_cliente, id_fornecedor, id_funcionario, descricao, compra, data_cadastro);
        query.exec(sql);
        ui->lineEdit_data->setText(data_cadastro);
        query.exec("select id from pedido order by id desc limit 1");
        query.next();
        ui->lineEdit_id_pedido->setText(query.value(0).toString());
    }

    lista_pedidos_atualizar();
}


void administracao_pedido::on_pushButton_gravar_clicked() { gravar(); }


void administracao_pedido::on_pushButton_gravar_2_clicked() { gravar(); }


void administracao_pedido::on_pushButton_novo_produto_clicked() {
    limpar_campos(false);
}

void administracao_pedido::on_pushButton_gravar_produto_clicked() {
    QString id_produto = ui->lineEdit_id_produto->text();
    QString id_estoque = ui->lineEdit_id_estoque->text();
    QString quantidade = ui->lineEdit_estoque_quantidade->text();
    QString compra = QString::number(ui->radioButton_compra->isChecked());
    QString sql;
    QSqlQuery query;
    bool existe = false;

    if (quantidade.toInt() <= 0) {
        QMessageBox::information(this, "Gravar", "Erro: a quantidade informada é menor ou igual a zero.");
        return;
    }

    for (auto elemento : this->produtos) {
        if ( (elemento->id_produto == id_produto.toInt()) && (elemento->id_estoque == id_estoque.toInt()) ) {
            elemento->quantidade += quantidade.toInt();
            existe = true;
        }
    }
    if (!existe) {
        produto * novo = new produto(id_produto.toInt(), id_estoque.toInt(), quantidade.toInt());
        this->produtos.push_back(novo);
    }

    if (compra.toInt()) {
        if (existe) {
            query.exec(QString("update estoque_produto set quantidade = quantidade + %0 where id_produto = %1 and id_estoque = %2").arg(quantidade, id_produto, id_estoque));
        }
        else {
            query.exec(QString("insert into estoque_produto (id_produto, id_estoque, quantidade) values (%1, %2, %3)").arg(id_produto, id_estoque, quantidade));
        }
    }
    else {

    }

    QString resultado;
    int linha = ui->tableWidget_produtos->rowCount();
    ui->tableWidget_produtos->insertRow(linha);
    query.exec("select id, marca, modelo, preco_custo, preco_venda, ativo from produto where id = " + id_produto);
    query.next();
    for (int registro = -1; registro < 7; registro++) {
        switch(registro) {
            case -1:
                resultado = id_estoque;
                break;
            case 5:
                query.value(registro).toInt() ? resultado = "Sim" : resultado = "Não";
                break;
            case 6:
                query.exec("select id from bebida where id_produto = " + id_produto);
                query.next() ? resultado = "Sim" : resultado = "Não";
                break;
            default:
                resultado = query.value(registro).toString();
                break;
        }
        ui->tableWidget_produtos->setItem(linha, registro+1, new QTableWidgetItem(resultado));
    }
    ui->tableWidget_produtos->setRowHeight(linha, 20);

    ui->lineEdit_id_produto->setText("0");
    ui->lineEdit_id_estoque->setText("0");
    ui->lineEdit_estoque_quantidade->clear();
}


void administracao_pedido::on_tableWidget_produtos_cellClicked(int row, int column) {
    this->linha_produto = row;
    ui->pushButton_remover->setEnabled(true);
}


void administracao_pedido::on_pushButton_remover_clicked() {
    if (this->linha_produto != -1) {
        ui->tableWidget_produtos->removeRow(this->linha_produto);
        this->linha_produto  = -1;
    }
}


void administracao_pedido::on_pushButton_selecionar_produto_clicked() {
    if (ui->radioButton_compra->isChecked()) {
        this->janela_cadastro_produto = new cadastro_produto(this, "0", "cadastro_pedido");
        this->janela_cadastro_produto->show();
    }
    this->janela_administracao_estoque = new administracao_estoque(this, "cadastro_pedido");
    this->janela_administracao_estoque->show();
}


void administracao_pedido::on_radioButton_venda_clicked(bool checked) {
    if (checked) {
        ui->pushButton_fornecedor->setEnabled(false);
        ui->lineEdit_id_fornecedor->setText("0");
        ui->pushButton_cliente->setEnabled(true);
    }
}


void administracao_pedido::on_radioButton_compra_clicked(bool checked) {
    if (checked) {
        ui->pushButton_fornecedor->setEnabled(true);
        ui->pushButton_cliente->setEnabled(false);
        ui->lineEdit_id_cliente->setText("0");
    }
}


void administracao_pedido::on_pushButton_voltar_clicked() { this->close(); }


void administracao_pedido::on_pushButton_voltar_2_clicked() { this->close(); }


void administracao_pedido::on_tableWidget_pedidos_cellClicked(int row, int column) {
    QString id = ui->tableWidget_pedidos->item(row, 0)->text();
    ui->tabWidget->setCurrentIndex(1);
    QSqlQuery query;
    QString sql;
    query.exec("select id, id_cliente, id_fornecedor, id_funcionario, descricao, tipo, valor, data from pedido where id = " + id);
    if (query.next()) {

        ui->lineEdit_id_pedido->setText(query.value(0).toString());
        ui->lineEdit_id_cliente->setText(query.value(1).toString());
        ui->lineEdit_id_fornecedor->setText(query.value(2).toString());
        ui->lineEdit_id_funcionario->setText(query.value(3).toString());
        ui->plainTextEdit_descricao->setPlainText(query.value(4).toString());
        if (query.value(5).toInt()) {
            ui->radioButton_venda->setChecked(true);
        }
        else {
            ui->radioButton_compra->setChecked(true);
        }
        ui->lineEdit_valor_pedido->setText(query.value(6).toString());
        ui->lineEdit_data->setText(query.value(7).toString());

        ui->radioButton_compra->setEnabled(false);
        ui->radioButton_venda->setEnabled(false);
        ui->pushButton_cliente->setEnabled(false);
        ui->pushButton_fornecedor->setEnabled(false);
        ui->pushButton_funcionario->setEnabled(false);
        ui->plainTextEdit_descricao->setReadOnly(true);
        ui->pushButton_selecionar_produto->setEnabled(false);
        ui->lineEdit_estoque_quantidade->setReadOnly(true);
        ui->pushButton_novo_produto->setEnabled(false);
        ui->pushButton_gravar_produto->setEnabled(false);
        ui->pushButton_remover->setEnabled(false);
        ui->pushButton_gravar->setEnabled(false);
        ui->pushButton_gravar_2->setEnabled(false);
    }
}

