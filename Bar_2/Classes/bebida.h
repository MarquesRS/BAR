#ifndef BEBIDA_H
#define BEBIDA_H

#include <string>
#include "produto.h"

class Bebida : public Produto{
private:
    int id;
    int id_produto;
    bool menores;
    float volume;
    float teor_alcolico;
    std::string ingredientes;

public:
    //Bebida();
};

#endif // BEBIDA_H
