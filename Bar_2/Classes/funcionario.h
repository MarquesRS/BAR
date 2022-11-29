#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include "datetime.h"
#include "pessoa.h"
#include <string>
using std::string;

class Funcionario : public Pessoa{
private:
    bool ativo; //
    int conta_corrente;
    string permissoes;
    string usuario;
    string senha;
    Datetime data_admissao;
    Datetime data_recisao;
public:
    //Funcionario();
    string exibir();
    //string permissoes_alterar();
};

#endif // FUNCIONARIO_H
