#ifndef CLIENTE_H
#define CLIENTE_H

#include "pessoa.h"
#include <vector>
#include <string>
using std::string;

class Pedido;

class Cliente {
private:
    std::vector<Pedido> pedidos_lista;

public:
    string exibir();
    /*
    Cliente();
    void pedido_incluir();
    void pedido_exibir();
    void pedido_exibir_id();
    */
};

#endif // CLIENTE_H
