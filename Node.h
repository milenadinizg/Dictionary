#ifndef NODE_H
#define NODE_H

// Definição da estrutura Node para a matriz esparsa
// Representa um elemento não nulo da matriz 
struct Node {
    Node* direita;  // Próximo nó na mesma linha
    Node* abaixo;   // Próximo nó na mesma coluna
    int linha;      // Índice da linha do elemento
    int coluna;     // Índice da coluna do elemento
    double valor;   // Valor armazenado no nó

    // Construtor do nó.
    // Índice da linha (padrão: -1, nó sentinela)
    // Índice da coluna (padrão: -1, nó sentinela)
    // Valor armazenado no nó (padrão: 0.0)
    Node(int i = -1, int j = -1, double val = 0.0) 
        : direita(this), abaixo(this), linha(i), coluna(j), valor(val) {}
};

#endif