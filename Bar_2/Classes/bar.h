#ifndef BAR_H
#define BAR_H

#include "estoque.h"
#include "pedido.h"
#include "cliente.h"
#include "funcionario.h"

class Bar {
private:
    std::vector<Estoque> estoque_lista;
    std::vector<Pedido> pedido_lista;
    std::vector<Cliente> cliente_lista;
    std::vector<Funcionario> funcionario_lista;

public:
    /*
    void estoque_administrar();
    void cliente_administrar();
    void funcionario_administrar();
    void menu_administrar();
    */
};

#endif // BAR_H
