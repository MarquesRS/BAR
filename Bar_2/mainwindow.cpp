#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFixedSize(size());
    QPixmap pixmap(1,1);
    pixmap.fill(Qt::transparent);
    this->setWindowIcon(pixmap);
    QString caminho;
    caminho = qApp->applicationDirPath() + "/database/banco.db";
    this->banco = new Banco(caminho);
    qDebug() << caminho;
}


MainWindow::~MainWindow() {
    this->banco->fechar();
    delete banco;
    delete ui;
}


void MainWindow::on_pushButton_entrar_clicked() {
    QString usuario = ui->lineEdit_login->text();
    QString senha = ui->lineEdit_senha->text();
    if (this->banco->abrir()) {
        QSqlQuery query;
        if (query.exec("select id from funcionario where usuario = '" + usuario + "' and senha = '" + senha + "'")) {
            if (query.next() != 0) {
                this->close();
                Principal principal(this);
                principal.exec();
            }
            else {
                ui->statusbar->showMessage("Usuário não localizado");
            }
        }
    }
}
