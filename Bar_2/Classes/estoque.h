#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <vector>
#include "produto.h"

class Estoque {
private:
    static int static_id;
    int id;
    bool ativo;
    std::vector<Produto> lista_produtos;

public:
    /*
    Estoque();
    void produto_incluir();
    void produto_remover();
    void produto_alterar();
    void produto_exibir();
    void produto_exibir_id();
    */
};

#endif // ESTOQUE_H
