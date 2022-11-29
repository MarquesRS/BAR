#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include <vector>
#include "endereco.h"
#include "datetime.h"

class Pessoa {
protected:
    int id;
    std::string nome; //
    std::string cpf; //
    std::string identidade; //
    std::string email; //
    std::string telefone;
    std::vector<Endereco> endereco;
    Endereco endereco_cobranca;
    Datetime data_nascimento;
    Datetime data_cadastro;

public:
    virtual std::string exibir() = 0; // Se tornou uma classe abstrata
};

#endif // PESSOA_H
