#ifndef PEDIDO_H
#define PEDIDO_H

#include <string>
#include <vector>
#include "datetime.h"
#include "funcionario.h"
#include "produto.h"
#include "pagamento.h"
#include "cliente.h"

class Pedido {
private:
    int id;
    float preco_bruto;
    float preco_liquido;
    Cliente cliente;
    Funcionario funcionario;
    std::vector<Produto> produto_lista;
    Pagamento pagamento;
    Datetime data;

public:
    //Pedido();
    //std::string exibir();
};

#endif // PEDIDO_H
