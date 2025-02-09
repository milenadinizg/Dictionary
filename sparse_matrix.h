#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "Node.h"

// Definição da classe SparseMatrix para manipulação de matrizes esparsas
// Representa uma matriz esparsa usando listas encadeadas circulares
class SparseMatrix {
private:
    int linhas, colunas; // Dimensões da matriz
    Node* m_head; // Nó sentinela principal

    // Libera a memória alocada pela matriz
    void desalocar();

public:
    // Construtor da classe
    SparseMatrix(int m, int n);

    // Destrutor da classe
    ~SparseMatrix();

    // Insere ou atualiza um valor na matriz
    void inserir(int i, int j, double value);

    // Retorna o valor armazenado em (i, j), ou 0 se não existir
    double get(int i, int j) const;

    // Conta os elementos não nulos da matriz
    int countNonZero() const;

    // Remove todos os elementos não nulos da matriz
    void clear();

    // Imprime a matriz completa, incluindo os zeros
    void print() const;

    // Retorna o número de linhas da matriz
    int getLinhas() const { return linhas; }
    
    // Retorna o número de colunas da matriz
    int getColunas() const { return colunas; }
    
    // Retorna o nó sentinela principal da matriz
    Node* getHead() const { return m_head; }
};

#endif