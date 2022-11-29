#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>
#include "fornecedor.h"
#include "datetime.h"


class Produto {
private:
    int id;
    bool ativo;
    float preco;
    std::string marca;
    std::string modelo;
    Datetime data_fabricacao;
    Datetime data_vencimento;
    Datetime data_cadastro;
    Fornecedor fornecedor;
public:
    //Produto(Fornecedor fornecedor, std::string marca, std::string modelo, float preco,
    //        Datetime data_fabricacao, Datetime data_vencimento, Datetime data_cadastro);
    //std::string exibir();
    //template<typename T>
    //void alterar(std::string campo, T dado);
};

#endif // PRODUTO_H
