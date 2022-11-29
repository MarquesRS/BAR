#include "cadastro_endereco.h"
#include "ui_cadastro_endereco.h"
#include <QDebug>
#include "cadastro_entidade.h"
#include <QCloseEvent>


void Cadastro_endereco::closeEvent(QCloseEvent *event) {
    QString id = ui->lineEdit_id->text();
    ((cadastro_entidade*)parent())->retorno_id_endereco(id);
    on_pushButton_novo_clicked();
    event->accept();
}


Cadastro_endereco::Cadastro_endereco(cadastro_entidade * parent, int id_parent) : QMainWindow(parent), ui(new Ui::Cadastro_endereco) {
    ui->setupUi(this);
    setFixedSize(size());
    limpar_campos();
    comboBox_pais();
    comboBox_estado();
    lista_enderecos_criar();
    lista_enderecos_atualizar();
    if (id_parent != 0) {
        selecionar_endereco(-1, id_parent);
    }
}


Cadastro_endereco::~Cadastro_endereco() {
    delete ui;
}


void Cadastro_endereco::selecionar_endereco(int row, int id_parent) {
    QString id = "";
    if (row == -1) { // sempre -1 quando não especificada
        id = QString::number(id_parent);
    }
    else {
        id = ui->tableWidget_enderecos->item(row, 0)->text();
    }
    ui->tabWidget->setCurrentIndex(1);
    ui->lineEdit_cep->setFocus();
    QSqlQuery query;
    if (query.exec("select id, id_estado, id_pais, cep, rua, numero, bairro, cidade, complemento, ativo from endereco where id = " + id)) {
        query.first();
        ui->lineEdit_id->setText(query.value(0).toString());
        ui->comboBox_estado->setCurrentIndex(query.value(1).toInt()-1);
        ui->comboBox_pais->setCurrentIndex(query.value(2).toInt()-1);
        ui->lineEdit_cep->setText(query.value(3).toString());
        ui->lineEdit_rua->setText(query.value(4).toString());
        ui->lineEdit_numero->setText(query.value(5).toString());
        ui->lineEdit_bairro->setText(query.value(6).toString());
        ui->lineEdit_cidade->setText(query.value(7).toString());
        ui->lineEdit_complemento->setText(query.value(8).toString());
        ui->radioButton_ativo->setChecked(query.value(9).toBool());
    }
}


void Cadastro_endereco::comboBox_pais() {
    QSqlQuery query;
    if (query.exec("select nome from pais")) {
        query.first();
        QStringList paises;
        do {
            paises.push_back(query.value(0).toString());
        } while(query.next());
        ui->comboBox_pais->addItems(paises);
    }
}


void Cadastro_endereco::comboBox_estado() {
    QSqlQuery query;
    if (query.exec("select nome from estado")) {
        query.first();
        QStringList estados;
        do {
            estados.push_back(query.value(0).toString());
        } while(query.next());
        ui->comboBox_estado->addItems(estados);
    }
}


void Cadastro_endereco::lista_enderecos_criar() {
    QStringList cabecalho = {"ID", "CEP", "Rua", "Numero", "Bairro", "Cidade", "Estado", "Pais", "Ativo"};
    ui->tableWidget_enderecos->setHorizontalHeaderLabels(cabecalho);
    int tamanho_colunas[9] = {20, 70, 130, 60, 100, 100, 50, 50, 20};
    for (int i = 0; i < 9; i++) {
        ui->tableWidget_enderecos->setColumnWidth(i, tamanho_colunas[i]);
    }
    ui->tableWidget_enderecos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_enderecos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_enderecos->verticalHeader()->setVisible(false);
}


void Cadastro_endereco::lista_enderecos_atualizar(QString sql) {
    if (sql == "") {
        sql = "select "
              "endereco.id, "
              "endereco.cep, "
              "endereco.rua, "
              "endereco.numero, "
              "endereco.bairro, "
              "endereco.cidade, "
              "estado.nome, "
              "pais.nome, "
              "endereco.ativo "
              "from endereco, estado, pais "
              "where endereco.id_pais = pais.id and endereco.id_estado = estado.id";
    }
    ui->tableWidget_enderecos->setRowCount(0);
    QSqlQuery query;
    if (query.exec(sql)) {
        for (int linha = 0; query.next() != 0; linha++) {
            ui->tableWidget_enderecos->insertRow(linha);
            for (int registro = 0; registro < 9; registro++) {
                ui->tableWidget_enderecos->setItem(linha, registro, new QTableWidgetItem(query.value(registro).toString()));
            }
            ui->tableWidget_enderecos->setRowHeight(linha, 20);
        }
    }
}


void Cadastro_endereco::limpar_campos() {
    ui->tabWidget->setCurrentIndex(1);
    ui->lineEdit_cep->setFocus();
    ui->lineEdit_id->setText("0");
    ui->radioButton_ativo->setChecked(true);
    ui->lineEdit_cep->clear();
    ui->lineEdit_rua->clear();
    ui->lineEdit_numero->clear();
    ui->lineEdit_bairro->clear();
    ui->lineEdit_cidade->clear();
    ui->lineEdit_complemento->clear();
    ui->comboBox_estado->setCurrentIndex(0);
    ui->comboBox_pais->setCurrentIndex(0);
}


void Cadastro_endereco::on_pushButton_novo_clicked() {
    limpar_campos();
}


bool Cadastro_endereco::verificar_existencia(QStringList parametros) {
    QString sql = QString("select id from endereco where cep = '%1' and rua = '%2' and numero = %3 and "
                          "bairro = '%4' and cidade = '%5' and complemento = '%6' and id_estado = %7 and id_pais = %8 and ativo = %9"
                          "").arg(parametros[0], parametros[1], parametros[2], parametros[3], parametros[4], parametros[5], parametros[6], parametros[7], parametros[8]);
    QSqlQuery query;
    if (query.exec(sql)) {
        if (query.next() != 0) {
            QMessageBox::information(this, "Gravar", "Erro: endereço já cadastrado. ID: " + query.value(0).toString());
            return true;
        }
        return false;
    }
    return false;
}


void Cadastro_endereco::on_pushButton_gravar_clicked() {
    if (ui->lineEdit_cep->text().isEmpty()|| ui->lineEdit_rua->text().isEmpty() || ui->lineEdit_numero->text().isEmpty()
        || ui->lineEdit_bairro->text().isEmpty() || ui->lineEdit_cidade->text().isEmpty()) {
        QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
        return;
    }
    if (ui->lineEdit_cep->text().size() < 9) {
        QMessageBox::information(this, "Gravar", "Erro: o CEP informado é inválido.");
        return;
    }
    QString cep = ui->lineEdit_cep->text();
    QString rua = ui->lineEdit_rua->text();
    QString numero = ui->lineEdit_numero->text();
    QString bairro = ui->lineEdit_bairro->text();
    QString cidade = ui->lineEdit_cidade->text();
    QString id = ui->lineEdit_id->text();
    QString ativo = QString::number(ui->radioButton_ativo->isChecked());
    QString complemento = ui->lineEdit_complemento->text();
    QString estado = QString::number(ui->comboBox_estado->currentIndex()+1);
    QString pais = QString::number(ui->comboBox_pais->currentIndex()+1);
    QString sql;
    QSqlQuery query;
    QStringList parametros = {cep, rua, numero, bairro, cidade, complemento, estado, pais, ativo};
    if (verificar_existencia(parametros)) {
        return;
    }
    if (id == "0") {
        sql = QString("insert into endereco (ativo, numero, id_estado, id_pais, rua, bairro, cidade, cep, complemento) "
                      "values (%1, '%2', %3, %4, '%5', '%6', '%7', '%8', '%9')"
                      "").arg(ativo, numero, estado, pais, rua, bairro, cidade, cep, complemento);
        if (query.exec(sql)){
            QMessageBox::information(this, "Gravar", "Sucesso: endereço cadastrado.");
            query.exec("select id from endereco ORDER BY id desc limit 1");
            query.first();
            ui->lineEdit_id->setText(query.value(0).toString());
        }
    }
    else {
        sql = QString("update endereco set ativo = %1, cep = '%2', rua = '%3', numero = %4, bairro = '%5', "
                      "cidade = '%6', id_estado = %7, complemento = '%8', id_pais = %9 where id = %10 "
                      "").arg(ativo, cep, rua, numero, bairro, cidade, estado, complemento, pais, id);
        if (query.exec(sql)) {
            QMessageBox::information(this, "Gravar", "Sucesso: endereço atualizado.");
        }
    }
    lista_enderecos_atualizar();
}


void Cadastro_endereco::on_pushButton_voltar_clicked() {
    this->close();
}


void Cadastro_endereco::on_tableWidget_enderecos_cellActivated(int row) {
    selecionar_endereco(row);
}


void Cadastro_endereco::on_pushButton_pesquisar_clicked() {
    QString filtro = ui->lineEdit_pesquisar_cep->text();
    QString sql = "";
    if (!filtro.isEmpty()) {
        sql = "select id, cep, rua, numero, bairro, cidade, id_estado, id_pais, ativo from endereco where cep like '%"+filtro+"%'";
    }
    lista_enderecos_atualizar(sql);
}
