#include "banco.h"


Banco::Banco(QString caminho) {
    alterar_caminho(caminho);
}


void Banco::alterar_caminho(QString caminho) {
    if (caminho == "") {
        caminho = "C:\\Users\\Marques\\Documents\\Projetos\\build-Bar_2-Desktop_Qt_6_3_1_MinGW_64_bit-Release\\release\\database\\banco.db";
    }
    for (int i = 0; i < caminho.size(); i++) {
        if (caminho[i] == '/')
            caminho[i] = '\\';
    }
    this->banco.setDatabaseName(caminho);
}


bool Banco::abrir() {
    if (this->banco.open()) {
        return true;
    }
    return false;
}


void Banco::fechar() {
    this->banco.close();
}
