QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    administracao_estoque.cpp \
    administracao_estoque__remover.cpp \
    administracao_pedido.cpp \
    banco.cpp \
    cadastro_endereco.cpp \
    cadastro_entidade.cpp \
    cadastro_produto.cpp \
    main.cpp \
    mainwindow.cpp \
    principal.cpp \
    produto.cpp

HEADERS += \
    administracao_estoque.h \
    administracao_estoque__remover.h \
    administracao_pedido.h \
    banco.h \
    cadastro_endereco.h \
    cadastro_entidade.h \
    cadastro_produto.h \
    mainwindow.h \
    principal.h \
    produto.h

FORMS += \
    administracao_estoque.ui \
    administracao_estoque__remover.ui \
    administracao_pedido.ui \
    cadastro_endereco.ui \
    cadastro_entidade.ui \
    cadastro_produto.ui \
    mainwindow.ui \
    principal.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
