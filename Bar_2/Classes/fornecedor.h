#ifndef FORNECEDOR_H
#define FORNECEDOR_H

#include "pessoa.h"
#include <string>
using std::string;

class Fornecedor : public Pessoa{
private:
    int codigo_interno;
    int inscricao_estadual;
    string nome_fantasia;
    string razao_social;
    string cnpj;
public:
    //Fornecedor();
    string exibir();
};

#endif // FORNECEDOR_H
