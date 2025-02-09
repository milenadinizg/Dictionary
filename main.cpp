// Milena Helen Diniz Gomes - 564757
// Yasmin de Lima Marques - 567615

#include "sparse_matrix.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>
#include <limits>

using namespace std;


/* Lê uma matriz esparsa de um arquivo e a armazena na estrutura
 * Abre o arquivo e verifica se foi carregado corretamente
 * Lê as dimensões da matriz e cria uma nova estrutura para armazená-la
 * Percorre o arquivo, lendo os elementos não nulos e inserindo-os na matriz
 * Fecha o arquivo após a leitura e exibe uma mensagem de sucesso.
 */
void readSparseMatrix(SparseMatrix*& m, const string& nomeArquivo) {
    ifstream file(nomeArquivo);

    if(!file.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo.");
    }

    int linhas, colunas;
    file >> linhas >> colunas;

    delete m;
    m = new SparseMatrix(linhas, colunas);

    int i, j;
    double valor;
    while(file >> i >> j >> valor) {
        if(valor != 0) {
            m->inserir(i, j, valor);
        }
    }

    file.close();
    cout << "Matriz de " << linhas << "x" << colunas << " carregada do arquivo " << nomeArquivo << ".\n";
}

/* Soma duas matrizes esparsas e retorna uma nova matriz com o resultado
 * Verifica se as dimensões das matrizes são compatíveis, lançando uma exceção se não forem
 * Percorre a matriz A, copiando seus valores para a matriz resultado
 * Percorre a matriz B, somando seus valores aos já existentes na matriz resultado
 * Retorna a matriz resultante da soma
 */
SparseMatrix* sum(const SparseMatrix* A, const SparseMatrix* B) {
    if(A->getLinhas() != B->getLinhas() || A->getColunas() != B->getColunas()) {
        throw runtime_error("As matrizes tem dimensoes incompativeis para soma.");
    }

    SparseMatrix* C = new SparseMatrix(A->getLinhas(), A->getColunas());

    Node* linha_sentinela = A->getHead()->abaixo;
    for(int i = 1; i <= A->getLinhas(); ++i, linha_sentinela = linha_sentinela->abaixo) {
        Node* linha_atual = linha_sentinela->direita;
        while(linha_atual != linha_sentinela) {
            C->inserir(linha_atual->linha, linha_atual->coluna, linha_atual->valor + B->get(linha_atual->linha, linha_atual->coluna));
            linha_atual = linha_atual->direita;
        }
    }

    linha_sentinela = B->getHead()->abaixo;
    for(int i = 1; i <= B->getLinhas(); ++i, linha_sentinela = linha_sentinela->abaixo) {
        Node* linha_atual = linha_sentinela->direita;
        while(linha_atual != linha_sentinela) {
            if(A->get(linha_atual->linha, linha_atual->coluna) == 0) { 
                C->inserir(linha_atual->linha, linha_atual->coluna, linha_atual->valor);
            }
            linha_atual = linha_atual->direita;
        }
    }

    return C;
}

/* Multiplica duas matrizes esparsas e retorna uma nova matriz com o resultado.
 * Verifica se as dimensões das matrizes são compatíveis para multiplicação, lançando uma exceção se não forem.
 * Cria uma nova matriz esparsa para armazenar o resultado.
 * Percorre os elementos não nulos da matriz A.
 * Para cada elemento de A, percorre a linha correspondente em B para calcular os produtos.
 * Acumula os valores resultantes na matriz C, garantindo que apenas elementos não nulos sejam armazenados.
 * Retorna a matriz resultante da multiplicação.
 */

SparseMatrix* multiply(const SparseMatrix* A, const SparseMatrix* B) {
    if(A->getColunas() != B->getLinhas()) {
        throw runtime_error("As matrizes tem dimensoes incompativeis para multiplicacao.");
    }

    SparseMatrix* C = new SparseMatrix(A->getLinhas(), B->getColunas());

    for(Node* linhaA = A->getHead()->abaixo; linhaA != A->getHead(); linhaA = linhaA->abaixo) {
        for(Node* elementoA = linhaA->direita; elementoA != linhaA; elementoA = elementoA->direita) {
            Node* linhaB = B->getHead()->abaixo;
            for(int k = 1; k < elementoA->coluna; ++k) {
                linhaB = linhaB->abaixo;
            }
            for(Node* elementoB = linhaB->direita; elementoB != linhaB; elementoB = elementoB->direita) {
                double valor = elementoA->valor * elementoB->valor;
                if (valor != 0) {
                    C->inserir(elementoA->linha, elementoB->coluna, C->get(elementoA->linha, elementoB->coluna) + valor);
                }
            }
        }
    }

    return C;
}

/* Exibe os índices das matrizes armazenadas na lista.
 * Se não houver matrizes, exibe uma mensagem indicando que a lista está vazia.
 * Percorre a lista de matrizes e imprime os índices disponíveis.
 * Formata a saída para melhor visualização.
 */
void showIndexes(const vector<SparseMatrix*>& matriz_list) {
    if(matriz_list.size() == 0) {
        cout << "Nao existe nenhuma matriz.\n";
    }
    
    for(size_t i = 0; i < matriz_list.size(); ++i) {
        cout << i;
        if(i < matriz_list.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

// Exibe os comandos disponíveis
void helper() {
    cout << "----------------------------------------------------------------------------\n";
    cout << ":: HELP\n";
    cout << "----------------------------------------------------------------------------\n";
    cout << "exit ...................................................... fechar o progrma\n";
    cout << "create m n .................. criar uma nova matriz com m linhas e n colunas\n";
    cout << "show i ............................... imprimir a matriz i no terminal print\n";
    cout << "showidx ......................... .... mostrar todos os indices das matrizes\n";
    cout << "sum i j ............................. somar as matrizes i e j da matriz_list\n";
    cout << "multiply i j .................. multiplicar as matrizes i e j da matriz_list\n";
    cout << "clear i ................................................... zerar a matriz i\n";
    cout << "read 'm.txt' ............ ler uma matriz esparsa do arquivo com nome 'm.txt'\n";
    cout << "count i .................... contar quantos elementos não nulos há na matriz\n";
    cout << "update m i j value ........... atualizar o valor da célula (i,j) na matriz m\n";
    cout << "eraseAll .............................. apagar todas as matrizes do programa\n";
    cout << "----------------------------------------------------------------------------\n";
}

int main() {
    vector<SparseMatrix*> matriz_list;
    string comando;

    cout << "Bem-vindo ao gerenciador de Matrizes Esparsas!\n";
    cout << "Digite 'help' para ver os comandos disponíveis.\n";

    while(true) {
        cout << "\n>> ";
        cin >> comando;

        try {
            // Comando para sair do programa
            if(comando == "exit") {
                cout << "Saindo do programa...\n";
                break;
            }
            // Comando para exibir os comandos disponíveis
            else if(comando == "help") {
                helper();
            }
            // Comando para criar uma nova matriz
            else if(comando == "create") {
                int linhas, colunas;
                cin >> linhas >> colunas;
                matriz_list.push_back(new SparseMatrix(linhas, colunas));
                cout << "Matriz adicionada! Indice: " << matriz_list.size() - 1 << "\n";
            }
            // Comando para ler uma matriz de um arquivo
            else if(comando == "read") {
                string nomeArquivo;
                cin >> nomeArquivo;

                SparseMatrix* matriz = new SparseMatrix(1, 1);
                try {
                    readSparseMatrix(matriz, nomeArquivo);
                    matriz_list.push_back(matriz);
                } catch(const exception& e) {
                    cerr << "Erro ao ler arquivo: " << e.what() << endl;
                    delete matriz;
                }
            }
            // Comando para exibir os índices das matrizes armazenadas
            else if(comando == "showidx") {
                cout << "Indices das matrizes: ";
                showIndexes(matriz_list);
            }
            // Comando para exibir uma matriz específica
            else if(comando == "show") {
                int index;
                cin >> index;
                if(index < 0 || index >= static_cast<int>(matriz_list.size())) {
                    throw out_of_range("Erro: Indice invalido.");
                } else {
                    cout << "Imprimindo matriz[" << index << "]:\n";
                    matriz_list[index]->print();
                }
            }
            // Comando para somar duas matrizes
            else if(comando == "sum") {
                int indice1, indice2;
                cin >> indice1 >> indice2;

                if(indice1 < 0 || indice1 >= static_cast<int>(matriz_list.size()) ||
                    indice2 < 0 || indice2 >= static_cast<int>(matriz_list.size())) {
                    throw out_of_range("Erro: Indice invalido.");
                    continue;
                }

                try {
                    SparseMatrix* resultado = sum(matriz_list[indice1], matriz_list[indice2]);
                    cout << "Resultado da soma:\n";
                    resultado->print();

                    cout << "Deseja salvar o resultado? (s/n): ";
                    char resposta;
                    cin >> resposta;

                    if (resposta == 's' || resposta == 'S') {
                        matriz_list.push_back(resultado);
                        cout << "Matriz resultado salva como indice " << matriz_list.size() - 1 << ".\n";
                    } else {
                        delete resultado;
                    }
                } catch(const exception& e) {
                    cerr << "Erro: " << e.what() << endl;
                }
            }
            // Comando para multiplicar duas matrizes
            else if(comando == "multiply") {
                int indice1, indice2;
                cin >> indice1 >> indice2;

                if(indice1 < 0 || indice1 >= static_cast<int>(matriz_list.size()) ||
                    indice2 < 0 || indice2 >= static_cast<int>(matriz_list.size())) {
                    cout << "Indices invalidos.\n";
                    continue;
                }

                try {
                    SparseMatrix* resultado = multiply(matriz_list[indice1], matriz_list[indice2]);
                    cout << "Resultado da multiplicacao:\n";
                    resultado->print();

                    cout << "Deseja salvar o resultado? (s/n): ";
                    char resposta;
                    cin >> resposta;

                    if(resposta == 's' || resposta == 'S') {
                        matriz_list.push_back(resultado);
                        cout << "Matriz resultado salva como indice " << matriz_list.size() - 1 << ".\n";
                    } else {
                        delete resultado;
                    }
                } catch(const exception& e) {
                    cerr << "Erro: " << e.what() << endl;
                }
            }
            // Comando para atualizar um valor na matriz
            else if (comando == "update") {
                int index, i, j;
                double valor;
                cin >> index >> i >> j >> valor;

                if(index < 0 || index >= static_cast<int>(matriz_list.size())) {
                    cout << "Indice invalido.\n";
                    continue;
                }
                matriz_list[index]->inserir(i, j, valor);
                cout << "Valor atualizado na matriz.\n";
            }
            // Comando para limpar uma matriz
            else if(comando == "clear") {
                int index;
                cin >> index;

                if(index < 0 || index >= static_cast<int>(matriz_list.size())) {
                    cout << "Indice invalido.\n";
                    continue;
                }

                matriz_list[index]->clear(); // Suposição de que existe um método clear() na classe SparseMatrix
                cout << "Matriz " << index << " foi zerada.\n";
            }
            // Comando para contar quantos elementos não nulos tem na matriz
            else if (comando == "count") {
                int index;
                cin >> index;

                if (index < 0 || index >= static_cast<int>(matriz_list.size())) {
                    throw out_of_range("Erro: Indice invalido.");
                    continue;
                }

                cout << "A matriz contem " << matriz_list[index]->countNonZero() << " elementos nao nulos.\n";
            }
            // Comando para apagar todas as matrizes
            else if(comando == "eraseAll") {
                for(auto& matriz : matriz_list) {
                    delete matriz;
                }
                matriz_list.clear();
                cout << "Todas as matrizes foram apagadas.\n";
            } 
            else {
                cout << "Comando invalido. Digite 'help' para ver a lista de comandos.\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora o restante da linha
                
            }
        } catch(const exception& e) {
            cerr << e.what() << endl;
        }
    }

    for(auto matriz : matriz_list) {
        delete matriz;
    }

    return 0;
}