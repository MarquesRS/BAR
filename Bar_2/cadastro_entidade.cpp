#include "cadastro_entidade.h"
#include "ui_cadastro_entidade.h"
#include "cadastro_produto.h"
#include "administracao_pedido.h";
#include <QCloseEvent>


void cadastro_entidade::closeEvent(QCloseEvent *event) {
    if (this->origem == "produto") {
        QString id_fornecedor = ui->lineEdit_id_fornecedor->text();
        bool fornecedor = ui->checkBox_fornecedor->isChecked();
        if (fornecedor && !id_fornecedor.isEmpty() && id_fornecedor != "0") {
            ((cadastro_produto*)parent())->retorno_id_fornecedor(id_fornecedor);
        }
        else {
            QMessageBox::information(this, "", "É preciso que a entidade selecionada seja um fornecedor");
        }
    }
    else if (this->origem == "pedido") {
        QString id;
        if (this->retornar_fornecedor) {
            id = ui->lineEdit_id_fornecedor->text();
            bool fornecedor = ui->checkBox_fornecedor->isChecked();
            if (fornecedor && !id.isEmpty() && id != "0") {
                ((administracao_pedido*)parent())->vincular_fornecedor(id);
            }
            else {
                QMessageBox::information(this, "", "É preciso que a entidade selecionada seja um fornecedor.");
            }
        }
        else if (this->retornar_funcionario) {
            id = ui->lineEdit_id_funcionario->text();
            bool funcionario = ui->checkBox_funcionario->isChecked();
            if (funcionario && !id.isEmpty() && id != "0") {
                ((administracao_pedido*)parent())->vincular_funcionario(id);
            }
            else {
                QMessageBox::information(this, "", "É preciso que a entidade selecionada seja um funcionario.");
            }
        }
        else {
            id = ui->lineEdit_id_1->text();
            ((administracao_pedido*)parent())->vincular_cliente(id);
        }
    }

    limpar_campos();
    event->accept();
}


cadastro_entidade::cadastro_entidade(QWidget *parent,
                                     bool origem_cadastro_produto,
                                     bool origem_administracao_pedidos,
                                     bool fornecedor,
                                     bool funcionario)
    : QMainWindow(parent),
      ui(new Ui::cadastro_entidade)
{
    ui->setupUi(this);
    setFixedSize(size());
    limpar_campos();
    ui->tabWidget_entidades->setCurrentIndex(0);
    lista_entidades_criar();
    lista_entidades_atualizar();
    if (origem_cadastro_produto) {
        this->origem = "produto";
    }
    else if (origem_administracao_pedidos) {
        this->origem = "pedido";
        if (fornecedor) {
            this->retornar_fornecedor = true;
        }
        else if (funcionario) {
            this->retornar_funcionario = true;
        }
        else {
            this->retornar_cliente = true;
        }
    }
    else {
        this->origem = "";
    }
}


cadastro_entidade::~cadastro_entidade() {
    delete janela_cadastro_endereco;
    delete ui;
}


void cadastro_entidade::retorno_id_endereco(QString id) {
    ui->lineEdit_id_endereco->setText(id);
}


void cadastro_entidade::on_pushButton_vincular_endereco_clicked() {
    int id = ui->lineEdit_id_endereco->text().toInt();
    this->janela_cadastro_endereco = new Cadastro_endereco(this, id);
    this->janela_cadastro_endereco->show();
}


void cadastro_entidade::on_checkBox_fornecedor_stateChanged(int arg1) {
    if (arg1) {
        ui->tab_fornecedor->setEnabled(true);
    }
    else {
        ui->tab_fornecedor->setEnabled(false);
    }
}


void cadastro_entidade::on_checkBox_funcionario_stateChanged(int arg1) {
    if (arg1) {
        ui->tab_funcionario->setEnabled(true);
    }
    else {
        ui->tab_funcionario->setEnabled(false);
    }
}


void cadastro_entidade::lista_entidades_criar() {
    QStringList cabecalho = {"ID", "Nome", "CPF", "RG", "Fornecedor", "Funcionario"};
    ui->tableWidget_entidades->setHorizontalHeaderLabels(cabecalho);
    int tamanho_colunas[6] = {20, 100, 100, 100, 80, 80};
    for (int i = 0; i < 6; i++) {
        ui->tableWidget_entidades->setColumnWidth(i, tamanho_colunas[i]);
    }
    ui->tableWidget_entidades->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_entidades->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_entidades->verticalHeader()->setVisible(false);
}


void cadastro_entidade::lista_entidades_atualizar(QString sql) {
    if (sql == "") {
        sql = "select id, nome, cpf, rg from pessoa ";
    }
    ui->tableWidget_entidades->setRowCount(0);
    QSqlQuery query_pessoa, query_fornecedor, query_funcionario;
    QString resultado;
    if (query_pessoa.exec(sql)) {
        for (int linha = 0; query_pessoa.next() != 0; linha++) {
            ui->tableWidget_entidades->insertRow(linha);
            for (int registro = 0; registro < 4; registro++) {
                ui->tableWidget_entidades->setItem(linha, registro, new QTableWidgetItem(query_pessoa.value(registro).toString()));
            }
            ui->tableWidget_entidades->setRowHeight(linha, 20);

            query_fornecedor.prepare("select id from fornecedor where id_pessoa = " + query_pessoa.value(0).toString());
            (query_fornecedor.exec() && query_fornecedor.next() != 0 ) ? resultado = "Sim" : resultado = "Não";
            ui->tableWidget_entidades->setItem(linha, 4, new QTableWidgetItem(resultado));

            query_funcionario.prepare("select id from funcionario where id_pessoa = " + query_pessoa.value(0).toString());
            (query_funcionario.exec() && query_funcionario.next() != 0 ) ? resultado = "Sim" : resultado = "Não";
            ui->tableWidget_entidades->setItem(linha, 5, new QTableWidgetItem(resultado));
        }
    }
}


void cadastro_entidade::limpar_campos(bool limpar_1, bool limpar_2, bool limpar_3) {
    if (limpar_1) {
        ui->tabWidget_entidades->setCurrentIndex(1);
        ui->lineEdit_nome->setFocus();
        ui->lineEdit_id_1->setText("0");
        ui->lineEdit_nome->clear();
        ui->lineEdit_telefone->clear();
        ui->lineEdit_email->clear();
        ui->lineEdit_cpf->clear();
        ui->lineEdit_data_cadastro->clear();
        ui->lineEdit_data_nascimento->clear();
        ui->lineEdit_rg->clear();
        ui->lineEdit_id_endereco->setText("0");
    }
    if (limpar_2) {
        ui->checkBox_fornecedor->setChecked(false);
        ui->lineEdit_id_fornecedor->clear();
        ui->lineEdit_nome_fantasia->clear();
        ui->lineEdit_razao_social->clear();
        ui->lineEdit_codigo_interno->clear();
        ui->lineEdit_cnpj->clear();
        ui->lineEdit_inscricao_estadual->clear();
    }
    if (limpar_3) {
        ui->checkBox_funcionario->setChecked(false);
        ui->lineEdit_id_funcionario->clear();
        ui->radioButton_ativo->setChecked(true);
        ui->lineEdit_conta_corrente->clear();
        ui->lineEdit_usuario->clear();
        ui->lineEdit_senha->clear();
        ui->lineEdit_permissoes->clear();
        ui->lineEdit_data_admissao->clear();
        ui->lineEdit_data_recisao->clear();
    }
}


void cadastro_entidade::on_pushButton_novo__clicked() {
    limpar_campos();
}


void cadastro_entidade::on_pushButton_gravar__clicked() {
    // Validações pagina de cadastro
    QString id = ui->lineEdit_id_1->text();
    QString nome = ui->lineEdit_nome->text();
    QString telefone = ui->lineEdit_telefone->text();
    QString email = ui->lineEdit_email->text();
    QString cpf = ui->lineEdit_cpf->text();
    QString data_nascimento = ui->lineEdit_data_nascimento->text();
    QString rg = ui->lineEdit_rg->text();
    if (nome.isEmpty() || telefone.isEmpty() || email.isEmpty() || cpf.isEmpty() || rg.isEmpty()) {
        QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
        return;
    }
    else if (!data_nascimento.isEmpty() && data_nascimento.size() < 10) {
        QMessageBox::information(this, "Gravar", "Erro: a data de nascimento informada é inválida.");
        return;
    }
    else if (telefone.size() < 14) {
        QMessageBox::information(this, "Gravar", "Erro: o número de telefone informado é inválido.");
        return;
    }
    else if (!(email.contains('@') && email.contains('.'))) {
        QMessageBox::information(this, "Gravar", "Erro: o e-mail informado é inválido.");
        return;
    }
    else if (rg.size() < 6) {
        QMessageBox::information(this, "Gravar", "Erro: o RG informado é inválido.");
        return;
    }
    else if (cpf.size() < 14) {
        QMessageBox::information(this, "Gravar", "Erro: o CPF informado é inválido.");
        return;
    }

    // Validações pagina de fornecedor
    QString nome_fantasia, razao_social, codigo_interno, cnpj, inscricao_estadual;
    nome_fantasia = razao_social = codigo_interno = cnpj = inscricao_estadual = "";
    if (ui->checkBox_fornecedor->isChecked()) {
        nome_fantasia  = ui->lineEdit_nome_fantasia->text();
        razao_social = ui->lineEdit_razao_social->text();
        codigo_interno = ui->lineEdit_codigo_interno->text();
        cnpj = ui->lineEdit_cnpj->text();
        inscricao_estadual = ui->lineEdit_inscricao_estadual->text();
        if (nome_fantasia.isEmpty() || razao_social.isEmpty() || cnpj.isEmpty() || inscricao_estadual.isEmpty()) {
            QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
            return;
        }
        else if (cnpj.size() < 18) {
            QMessageBox::information(this, "Gravar", "Erro: o CNPJ informado é inválido.");
            return;
        }
        else if (inscricao_estadual.size() < 10) {
            QMessageBox::information(this, "Gravar", "Erro: a inscrição estadual informada é inválida.");
            return;
        }
    }

    // Validações pagina de funcionário
    QString ativo, conta_corrente, usuario, senha, permissoes, data_admissao, data_recisao;
    conta_corrente = usuario = senha = permissoes = data_admissao = data_recisao = "";
    if (ui->checkBox_funcionario->isChecked()) {
        ativo = QString::number(ui->radioButton_ativo->isChecked());
        conta_corrente = ui->lineEdit_conta_corrente->text();
        usuario = ui->lineEdit_usuario->text();
        senha = ui->lineEdit_senha->text();
        permissoes = ui->lineEdit_permissoes->text();
        data_admissao = ui->lineEdit_data_admissao->text();
        data_recisao = ui->lineEdit_data_recisao->text();
        if (conta_corrente.isEmpty() || usuario.isEmpty() || senha.isEmpty() || data_admissao.isEmpty()) {
            QMessageBox::information(this, "Gravar", "Erro: verifique se os campos obrigatórios foram preenchidos.");
            return;
        }
        else if (conta_corrente.size() < 19) {
            QMessageBox::information(this, "Gravar", "Erro: a conta corrente informada é inválida.");
            return;
        }
        else if (data_recisao.isEmpty() && data_recisao.size() < 10) {
            QMessageBox::information(this, "Gravar", "Erro: a data de recisão informada é inválida.");
            return;
        }
        else if (data_admissao.size() < 10) {
            QMessageBox::information(this, "Gravar", "Erro: a data de admissão informada é inválida.");
            return;
        }
        // validar as permissões no futuro
    }

    // Validações pagina de endereço
    QString id_endereco = ui->lineEdit_id_endereco->text();
    if (id_endereco == "0") {
        QMessageBox::information(this, "Gravar", "Erro: nenhum endereço foi vinculado.");
        return;
    }

    // Validações de insert e update
    QString sql;
    QSqlQuery query;
    if (id == "0") {
        QDateTime date = QDateTime::currentDateTime();
        QString data_cadastro = date.toString("dd/MM/yyyy"); // Pegar data do sistema
        sql = QString("insert into pessoa (nome, cpf, rg, email, data_nascimento, data_cadastro, telefone) "
                      "values ('%1', '%2', '%3', '%4', '%5', '%6', '%7')"
                      "").arg(nome, cpf, rg, email, data_nascimento, data_cadastro, telefone);
        if (query.exec(sql)){
            QMessageBox::information(this, "Gravar", "Sucesso: entidade cadastrada.");
            query.exec("select id from pessoa ORDER BY id desc limit 1");
            query.first();
            QString ultimo_id = query.value(0).toString();
            ui->lineEdit_id_1->setText(ultimo_id);

            query.exec("insert into pessoa_endereco (id_pessoa, id_endereco) values (" + ultimo_id + ", " + id_endereco + ")");

            // Se for um fornecedor, grava os dados
            if (ui->checkBox_fornecedor->isChecked()) {
                sql = QString("insert into fornecedor (id_pessoa, codigo_interno, inscricao_estadual, nome_fantasia, razao_social, cnpj) "
                              "values (%1, %2, %3, '%4', '%5', '%6')").arg(ultimo_id, codigo_interno, inscricao_estadual, nome_fantasia, razao_social, cnpj);
                query.exec(sql);
                query.exec("select id from fornecedor ORDER BY id desc limit 1");
                query.first();
                ui->lineEdit_id_fornecedor->setText(query.value(0).toString());
            }

            // Se for um funcionario, grava os dados
            if (ui->checkBox_funcionario->isChecked()) {
                sql = QString("insert into funcionario (id_pessoa, ativo, conta_corrente, permissoes, usuario, senha, data_admissao, data_recisao) "
                              "values (%1, %2, '%3', '%4', '%5', '%6', '%7', '%8')").arg(ultimo_id, ativo, conta_corrente, permissoes, usuario, senha, data_admissao, data_recisao);
                query.exec(sql);
                query.exec("select id from funcionario ORDER BY id desc limit 1");
                query.first();
                ui->lineEdit_id_funcionario->setText(query.value(0).toString());
            }
        }
    }
    else {
        sql = QString("update pessoa set nome = '%1', cpf = '%2', rg = '%3', email = '%4', data_nascimento = '%5', "
                      "telefone = '%7' where id = %8"
                      "").arg(nome, cpf, rg, email, data_nascimento, telefone, id);
        if (query.exec(sql)) {
            QMessageBox::information(this, "Gravar", "Sucesso: entidade atualizada.");

            query.exec("update pessoa_endereco set id_endereco = " + id_endereco + " where id_pessoa = " + id);

            // Se for um fornecedor, grava os dados
            if (ui->checkBox_fornecedor->isChecked()) { //
                query.exec("select id from fornecedor where id_pessoa = " + id);
                if (query.next()) { // Se tiver atualiza os dados existentes
                    sql = QString("update fornecedor set codigo_interno = %1, inscricao_estadual = %2, nome_fantasia = '%3', "
                                                  "razao_social = '%4', cnpj = '%5' where id_pessoa = %6"
                                                  "").arg(codigo_interno, inscricao_estadual, nome_fantasia, razao_social, cnpj, id);
                    query.exec(sql);
                }
                else { // Se não tiver, atualiza inserindo novos dados
                    sql = QString("insert into fornecedor (id_pessoa, codigo_interno, inscricao_estadual, nome_fantasia, razao_social, cnpj) "
                                  "values (%1, %2, %3, '%4', '%5', '%6')").arg(id, codigo_interno, inscricao_estadual, nome_fantasia, razao_social, cnpj);
                    query.exec(sql);
                    query.exec("select id from fornecedor ORDER BY id desc limit 1");
                    query.first();
                    ui->lineEdit_id_fornecedor->setText(query.value(0).toString());
                }

            }
            else { // Suponha que a entidade foi cadastrada como fornecedor, ao atualizar, desmarcaram a propriedade. É necessário apagar a associação
                query.exec("delete from fornecedor where id_pessoa = " + id);
            }

            // Se for um funcionario, grava os dados
            if (ui->checkBox_funcionario->isChecked()) { //
                query.exec("select id from funcionario where id_pessoa = " + id);
                if (query.next()) {
                    sql = QString("update funcionario set ativo = %1, conta_corrente = '%2', permissoes = '%3', usuario = '%4', "
                                  "senha = '%5', data_admissao = '%6', data_recisao = '%7' where id_pessoa = %8"
                                  "").arg(ativo, conta_corrente, permissoes, usuario, senha, data_admissao, data_recisao, id);
                    query.exec(sql);
                }
                else {
                    sql = QString("insert into funcionario (id_pessoa, ativo, conta_corrente, permissoes, usuario, senha, data_admissao, data_recisao) "
                                  "values (%1, %2, '%3', '%4', '%5', '%6', '%7', '%8')").arg(id, ativo, conta_corrente, permissoes, usuario, senha, data_admissao, data_recisao);
                    query.exec(sql);
                    query.exec("select id from funcionario ORDER BY id desc limit 1");
                    query.first();
                    ui->lineEdit_id_funcionario->setText(query.value(0).toString());
                }
            }
            else {
                query.exec("delete from funcionario where id_pessoa = " + id);
            }
        }
    }
    lista_entidades_atualizar();
}


void cadastro_entidade::on_pushButton_remover_clicked() {
    QString id = ui->lineEdit_id_1->text();
    if (id == "0") {
        QMessageBox::information(this, "Remover", "ERRO: não é possível remover uma entidade não selecionada.");
        return;
    }
    QSqlQuery query;
    query.exec("delete from pessoa_endereco where id_pessoa = " + id);
    query.exec("delete from funcionario where id_pessoa = " + id);
    query.exec("delete from fornecedor where id_pessoa = " + id);
    query.exec("delete from pessoa where id = " + id);
    limpar_campos();
    lista_entidades_atualizar();
}


void cadastro_entidade::on_tableWidget_entidades_cellActivated(int row, int column) {
    ui->tabWidget_entidades->setCurrentIndex(1);
    ui->lineEdit_nome->setFocus();
    QString id = ui->tableWidget_entidades->item(row, 0)->text();
    QSqlQuery query;

    // Carrega os dados gerais da entidade
    if (query.exec("select nome, telefone, email, cpf, data_cadastro, data_nascimento, rg from pessoa where id = " + id)) {
        if (query.next() != 0) {
            ui->lineEdit_id_1->setText(id);
            ui->lineEdit_nome->setText(query.value(0).toString());
            ui->lineEdit_telefone->setText(query.value(1).toString());
            ui->lineEdit_email->setText(query.value(2).toString());
            ui->lineEdit_cpf->setText(query.value(3).toString());
            ui->lineEdit_data_cadastro->setText(query.value(4).toString());
            ui->lineEdit_data_nascimento->setText(query.value(5).toString());
            ui->lineEdit_rg->setText(query.value(6).toString());
        }

        if (query.exec("select id_endereco from pessoa_endereco where id_pessoa = " + id)) {
            query.first();
            ui->lineEdit_id_endereco->setText(query.value(0).toString());
        }

        // Verifica se a entidade é um fornecedor e se for carrega seus dados
        if (query.exec("select nome_fantasia, razao_social, codigo_interno, cnpj, inscricao_estadual, id from fornecedor where id_pessoa = " + id)) {
            if (query.next()) {
                ui->checkBox_fornecedor->setChecked(true);
                ui->lineEdit_nome_fantasia->setText(query.value(0).toString());
                ui->lineEdit_razao_social->setText(query.value(1).toString());
                ui->lineEdit_codigo_interno->setText(query.value(2).toString());
                ui->lineEdit_cnpj->setText(query.value(3).toString());
                ui->lineEdit_inscricao_estadual->setText(query.value(4).toString());
                ui->lineEdit_id_fornecedor->setText(query.value(5).toString());
            }
            else {
                limpar_campos(false, true, false); // reseta só os campos de fornecedor
            }
        }

        // Verifica se a entidade é um funcionario e se for carrega seus dados
        if (query.exec("select ativo, conta_corrente, usuario, senha, permissoes, data_admissao, data_recisao, id from funcionario where id_pessoa = " + id)) {
            if (query.next()) {
                ui->checkBox_funcionario->setChecked(true);
                ui->radioButton_ativo->setChecked(query.value(0).toBool());
                ui->lineEdit_conta_corrente->setText(query.value(1).toString());
                ui->lineEdit_usuario->setText(query.value(2).toString());
                ui->lineEdit_senha->setText(query.value(3).toString());
                ui->lineEdit_permissoes->setText(query.value(4).toString());
                ui->lineEdit_data_admissao->setText(query.value(5).toString());
                ui->lineEdit_data_recisao->setText(query.value(6).toString());
                ui->lineEdit_id_funcionario->setText(query.value(7).toString());
            }
            else {
                limpar_campos(false, false, true); // reseta só os campos de funcionário
            }
        }
    }
}


void cadastro_entidade::on_pushButton_pesquisar_clicked() {
    QString filtro = ui->lineEdit_pesquisar_nome->text();
    QString sql = "";
    if (!filtro.isEmpty()) {
        sql = "select id, nome, cpf, rg from pessoa where nome like '%"+filtro+"%'";
    }
    lista_entidades_atualizar(sql);
}


void cadastro_entidade::on_pushButton_voltar_clicked() {
    this->close();
}

