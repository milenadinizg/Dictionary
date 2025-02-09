#include "sparse_matrix.h"
#include <iostream>
#include <iomanip>

using namespace std;

/* Constrói uma matriz esparsa vazia com m linhas e n colunas
 * Verifica se os valores são válidos, lançando uma exceção se não forem
 * Cria um nó sentinela principal que servirá como referência
 * Inicializa nós sentinelas para cada linha, fazendo uma conexão circular
 * Inicializa nós sentinelas para cada coluna, fazendo uma conexão circular
 */
SparseMatrix::SparseMatrix(int m, int n) : linhas(m), colunas(n) {
    if (m <= 0 || n <= 0) {
        throw invalid_argument("Erro: Dimensoes invalidas! Linhas e colunas devem ser maiores que zero.");
    }
    
    m_head = new Node();

    Node* linha_sentinela = m_head;
    for (int i = 1; i <= m; ++i) {
        Node* nova_linha = new Node(i, 0);
        linha_sentinela->abaixo = nova_linha;
        linha_sentinela = nova_linha;
    }
    linha_sentinela->abaixo = m_head;

    Node* coluna_sentinela = m_head;
    for (int j = 1; j <= n; ++j) {
        Node* nova_coluna = new Node(0, j);
        coluna_sentinela->direita = nova_coluna;
        coluna_sentinela = nova_coluna;
    }
    coluna_sentinela->direita = m_head;
}

/* Destrói a matriz esparsa liberando toda a memória alocada
 * Chama a função auxiliar desalocar() para remover todos os nós
 * Garante que nenhum nó fique alocado após a destruição do objeto
 */
SparseMatrix::~SparseMatrix() {
    desalocar();
}

/* Libera toda a memória alocada pela matriz
 * Percorre todas as linhas, removendo seus elementos e nós sentinelas
 * Percorre todas as colunas, removendo os nós sentinelas das colunas
 * Remove o nó sentinela principal, finalizando a desalocação
 */
void SparseMatrix::desalocar() {
    if (!m_head) return;

    Node* linha_sentinela = m_head->abaixo;
    while (linha_sentinela != m_head) {
        Node* elemento = linha_sentinela->direita;
        while (elemento != linha_sentinela) {
            Node* temp = elemento;
            elemento = elemento->direita;
            delete temp;
        }
        Node* tempLinha = linha_sentinela;
        linha_sentinela = linha_sentinela->abaixo;
        delete tempLinha;
    }

    Node* coluna_sentinela = m_head->direita;
    while (coluna_sentinela != m_head) {
        Node* tempColuna = coluna_sentinela;
        coluna_sentinela = coluna_sentinela->direita;
        delete tempColuna;
    }

    delete m_head;
}

/* Função que insere ou atualiza um valor na matriz
 * Se o valor for zero, a função retorna sem fazer nada
 * Verifica se os índices são válidos, lançando uma exceção se não forem
 * Percorre os nós sentinelas para encontrar a linha correspondente
 * Dentro da linha, avança até a posição correta da coluna
 * Se já existir um elemento na posição (i, j), atualiza seu valor
 * Caso contrário, cria um novo nó e o insere na estrutura
 */
void SparseMatrix::inserir(int i, int j, double value) {
    if (i < 1 || i > linhas || j < 1 || j > colunas) {
        throw out_of_range("Erro: Indices fora dos limites da matriz.");
    }
    if (value == 0) return;

    Node* linha_sentinela = m_head;
    for (int k = 1; k <= i; ++k) {
        linha_sentinela = linha_sentinela->abaixo;
    }

    Node* elemento = linha_sentinela;
    while (elemento->direita != linha_sentinela && elemento->direita->coluna < j) {
        elemento = elemento->direita;
    }

    if (elemento->direita != linha_sentinela && elemento->direita->coluna == j) {
        elemento->direita->valor = value;
    } else {
        Node* novo = new Node(i, j, value);
        novo->direita = elemento->direita;
        elemento->direita = novo;
    }
}

/* Retorna o valor armazenado na posição (i, j) da matriz
 * Lança uma exceção se os índices forem inválidos
 * Percorre os nós sentinelas para localizar a linha correspondente
 * Dentro da linha, avança até encontrar a coluna desejada
 * Retorna o valor encontrado ou 0 caso a posição esteja vazia
 */

double SparseMatrix::get(int i, int j) const {
    if (i < 1 || i > linhas || j < 1 || j > colunas) {
        throw out_of_range("Erro: Indices fora dos limites da matriz.");
    }

    Node* linha_sentinela = m_head;
    for (int k = 1; k <= i; ++k) {
        linha_sentinela = linha_sentinela->abaixo;
    }

    Node* elemento = linha_sentinela->direita;
    while (elemento != linha_sentinela && elemento->coluna < j) {
        elemento = elemento->direita;
    }

    return (elemento != linha_sentinela && elemento->coluna == j) ? elemento->valor : 0.0;
}

/* Conta e retorna a quantidade de elementos não nulos na matriz
 * Percorre todas as linhas da matriz
 * Dentro de cada linha, percorre os elementos não nulos
 * Incrementa o contador para cada elemento encontrado
 */
int SparseMatrix::countNonZero() const {
    int count = 0;
    for (Node* linha = m_head->abaixo; linha != m_head; linha = linha->abaixo) {
        Node* elemento = linha->direita;
        while (elemento != linha) {
            count++;
            elemento = elemento->direita;
        }
    }
    return count;
}

/* Remove todos os elementos não nulos da matriz, mantendo a estrutura
 * Percorre cada linha da matriz e remove seus elementos não nulos
 * Mantém os nós sentinelas para preservar a estrutura da matriz
 * Reseta as conexões das linhas para garantir que fiquem vazias
 */
void SparseMatrix::clear() {
    for (int i = 0; i < this->linhas; ++i) {
        Node* linha_sentinela = this->m_head->abaixo;
        for (int j = 0; j < i; ++j) {
            linha_sentinela = linha_sentinela->abaixo;
        }
        Node* atual = linha_sentinela->direita;
        while (atual != linha_sentinela) {
            Node* temp = atual;
            atual = atual->direita;
            delete temp;
        }

        linha_sentinela->direita = linha_sentinela;
    }
}

/* Imprime a matriz esparsa no formato tradicional, incluindo os zeros
 * Define a largura das colunas para garantir alinhamento na exibição
 * Percorre todas as linhas e colunas, imprimindo os valores existentes
 * Exibe 0.0 para posições vazias, mantendo a estrutura da matriz
 * Utiliza bordas para melhorar a visualização no terminal
 */

void SparseMatrix::print() const {
    int largura_coluna = 6;
    int largura_total = colunas * largura_coluna + 3;

    cout << string(largura_total, '-') << "\n";

    for (Node* linha = m_head->abaixo; linha != m_head; linha = linha->abaixo) {
        cout << "|";
        Node* elemento = linha->direita;
        for (int j = 1; j <= colunas; ++j) {
            if (elemento != linha && elemento->coluna == j) {
                cout << setw(6) << fixed << setprecision(1) << elemento->valor;
                elemento = elemento->direita;
            } else {
                cout << setw(6) << "0.0";
            }
        }
        cout << " |\n";
    }

    cout << string(largura_total, '-') << "\n";
}
