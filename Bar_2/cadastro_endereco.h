#ifndef CADASTRO_ENDERECO_H
#define CADASTRO_ENDERECO_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>

class cadastro_entidade;

namespace Ui { class Cadastro_endereco; }

class Cadastro_endereco : public QMainWindow {
    Q_OBJECT

public:
    explicit Cadastro_endereco(cadastro_entidade *parent = nullptr, int id_parent = 0);
    ~Cadastro_endereco();

private slots:
    void on_pushButton_novo_clicked();
    void on_pushButton_gravar_clicked();
    void on_pushButton_voltar_clicked();
    void on_tableWidget_enderecos_cellActivated(int row);
    void on_pushButton_pesquisar_clicked();

private:
    Ui::Cadastro_endereco *ui;
    void comboBox_estado();
    void comboBox_pais();
    void lista_enderecos_criar();
    void lista_enderecos_atualizar(QString sql = "");
    bool verificar_existencia(QStringList parametros);
    void selecionar_endereco(int row = -1, int id_parent = 0);
    void limpar_campos();
    void closeEvent(QCloseEvent *event);
};

#endif // CADASTRO_ENDERECO_H
