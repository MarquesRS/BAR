#ifndef BANCO_H
#define BANCO_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class Banco {
private:
    QSqlDatabase banco = QSqlDatabase::addDatabase("QSQLITE");

public:
    Banco(QString caminho = "");
    bool abrir();
    void fechar();
    void alterar_caminho(QString caminho);
};

#endif // BANCO_H
