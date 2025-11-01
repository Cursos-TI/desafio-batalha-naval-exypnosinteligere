#include <stdio.h>
#include <stdlib.h> // Para uso do abs()

// Definições de constantes
#define TAMANHO_TABULEIRO 10
#define TAMANHO_HABILIDADE 7 // Tamanho 7x7 para as matrizes de habilidade
#define RAIO_HABILIDADE 3    // Raio do centro para a borda (3 posicoes para 7x7)
#define TAMANHO_NAVIO 3

// Códigos de visualização
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5

// ----------------------------------------------------------------------
// ESTRUTURA PARA REPRESENTAR UMA CARTA (Navio)
// ----------------------------------------------------------------------
// Usada apenas para definir a estrutura, mas nao usada no loop de cadastro
// do Batalha Naval. Mantida por consistencia.
typedef struct {
    char estado;
    char codigo[5];
    char nome_cidade[50];
    // ... outros atributos de Super Trunfo
} Carta; 

// ----------------------------------------------------------------------
// FUNÇÕES AUXILIARES DE POSICIONAMENTO
// ----------------------------------------------------------------------

// Função para limpar o buffer (mantida por boa prática, embora não usada no Batalha Naval)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Valida e posiciona um navio (Retorno: 0=Sucesso, 1=Erro)
int posicionar_navio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                     int linha_inicio, int coluna_inicio, int orientacao) {
    
    int i, r, c;
    
    // Validação de Limites (mantida a lógica robusta)
    if (linha_inicio < 0 || linha_inicio >= TAMANHO_TABULEIRO || coluna_inicio < 0 || coluna_inicio >= TAMANHO_TABULEIRO) return 1;

    switch (orientacao) {
        case 0: if (coluna_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 1; break; // Horizontal
        case 1: if (linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 1; break; // Vertical
        case 2: if (linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO || coluna_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 1; break; // Diag. Desc
        case 3: if (linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO || coluna_inicio - TAMANHO_NAVIO < -1) return 1; break; // Diag. Asc
    }
    
    // Validação de Sobreposição (simplificada: verifica se já é NAVIO)
    for (i = 0; i < TAMANHO_NAVIO; i++) {
        r = linha_inicio; c = coluna_inicio;
        switch (orientacao) {
            case 0: c += i; break;
            case 1: r += i; break;
            case 2: r += i; c += i; break;
            case 3: r += i; c -= i; break;
        }
        if (tabuleiro[r][c] != AGUA) return 1; // Já ocupado
    }
    
    // Posicionamento
    for (i = 0; i < TAMANHO_NAVIO; i++) {
        r = linha_inicio; c = coluna_inicio;
        switch (orientacao) {
            case 0: c += i; break;
            case 1: r += i; break;
            case 2: r += i; c += i; break;
            case 3: r += i; c -= i; break;
        }
        tabuleiro[r][c] = NAVIO;
    }
    return 0;
}

// ----------------------------------------------------------------------
// CONSTRUÇÃO DINÂMICA DAS HABILIDADES (Requisito: Condicionais e Loops Aninhados)
// ----------------------------------------------------------------------

// Habilidade 1: CONE (Expande para baixo a partir do centro)
void construir_cone(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int i, j, centro;
    centro = RAIO_HABILIDADE; // 3 para 7x7

    for (i = 0; i < TAMANHO_HABILIDADE; i++) { // Linhas
        for (j = 0; j < TAMANHO_HABILIDADE; j++) { // Colunas
            // O cone é uma forma triangular na metade inferior
            if (i >= centro) { 
                matriz[i][j] = 0;
            } else {
                // A largura do cone aumenta conforme a linha diminui
                // Linha 0: 1 pos (centro); Linha 1: 3 pos; Linha 2: 5 pos;
                if (j >= centro - i && j <= centro + i) {
                    matriz[i][j] = 1; // Área afetada
                } else {
                    matriz[i][j] = 0; // Água
                }
            }
        }
    }
}

// Habilidade 2: CRUZ (Linha e Coluna centrais)
void construir_cruz(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int i, j, centro;
    centro = RAIO_HABILIDADE;

    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Se estiver na linha central OU na coluna central
            if (i == centro || j == centro) {
                matriz[i][j] = 1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }
}

// Habilidade 3: OCTAEDRO/LOSANGO (Distância de Manhattan: |r - rc| + |c - cc|)
void construir_octaedro(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int i, j, centro;
    centro = RAIO_HABILIDADE;

    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Calcula a 'distância de Manhattan' do centro (centro, centro)
            // A distância máxima permitida para um raio 3 é 3 (0 a 3)
            if (abs(i - centro) + abs(j - centro) <= RAIO_HABILIDADE) {
                matriz[i][j] = 1; // Dentro do Losango
            } else {
                matriz[i][j] = 0;
            }
        }
    }
}

// ----------------------------------------------------------------------
// LÓGICA DE SOBREPOSIÇÃO (Requisito: Usar Condicionais)
// ----------------------------------------------------------------------

void sobrepor_habilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                         const int matriz_hab[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], 
                         int linha_origem, int coluna_origem) {
    
    int i, j;
    int linha_tabuleiro, coluna_tabuleiro;
    int offset = RAIO_HABILIDADE; // Metade do tamanho da matriz de habilidade (3)

    for (i = 0; i < TAMANHO_HABILIDADE; i++) { // Percorre as linhas da matriz de habilidade
        for (j = 0; j < TAMANHO_HABILIDADE; j++) { // Percorre as colunas da matriz de habilidade
            
            // Calcula a posição correspondente no tabuleiro
            linha_tabuleiro = linha_origem - offset + i;
            coluna_tabuleiro = coluna_origem - offset + j;

            // Condicional: Verifica se a posição do tabuleiro está dentro dos limites
            if (linha_tabuleiro >= 0 && linha_tabuleiro < TAMANHO_TABULEIRO &&
                coluna_tabuleiro >= 0 && coluna_tabuleiro < TAMANHO_TABULEIRO) {
                
                // Condicional: Se a habilidade AFETA (valor 1 na matriz de habilidade)
                if (matriz_hab[i][j] == 1) {
                    // Marca a posição como afetada, SOMENTE se não for um navio
                    // (Navio tem prioridade visual sobre a área de efeito)
                    if (tabuleiro[linha_tabuleiro][coluna_tabuleiro] != NAVIO) {
                        tabuleiro[linha_tabuleiro][coluna_tabuleiro] = HABILIDADE;
                    }
                }
            }
        }
    }
}

// ----------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------------------------
int main() {
    // Inicializa o tabuleiro 10x10
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO] = {0};
    int i, j;
    int erro = 0;
    
    // Matrizes para as habilidades (7x7)
    int cone[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE] = {0};
    int cruz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE] = {0};
    int octaedro[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE] = {0};
    
    // --- 1. CONSTRUIR AS HABILIDADES DINAMICAMENTE ---
    construir_cone(cone);
    construir_cruz(cruz);
    construir_octaedro(octaedro);
    
    // --- 2. POSICIONAR NAVIOS ---
    printf("--- Posicionamento de Navios ---\n");
    // Navio 1: Horizontal (1, 2) a (1, 4)
    erro |= posicionar_navio(tabuleiro, 1, 2, 0); 
    // Navio 2: Vertical (6, 5) a (8, 5)
    erro |= posicionar_navio(tabuleiro, 6, 5, 1); 
    // Navio 3: Diag. Descendente (0, 0) a (2, 2)
    erro |= posicionar_navio(tabuleiro, 0, 0, 2); 
    // Navio 4: Diag. Ascendente (4, 8) a (6, 6)
    erro |= posicionar_navio(tabuleiro, 4, 8, 3); 
    
    if (erro != 0) {
        printf("ERRO: Falha ao posicionar um ou mais navios.\n");
        return 1;
    }
    
    // --- 3. SOBREPOR HABILIDADES NO TABULEIRO ---
    printf("--- Sobrepondo Habilidades ---\n");
    // Origem do Cone (Linha 8, Coluna 1) - Aponta para o canto inferior esquerdo
    sobrepor_habilidade(tabuleiro, cone, 2, 8); 
    // Origem da Cruz (Linha 4, Coluna 4) - Centro do tabuleiro
    sobrepor_habilidade(tabuleiro, cruz, 4, 4); 
    // Origem do Octaedro (Linha 7, Coluna 2) - Perto da borda
    sobrepor_habilidade(tabuleiro, octaedro, 7, 2); 

    // --- 4. EXIBIÇÃO DO TABULEIRO FINAL ---
    printf("\n====================================================\n");
    printf("        TABULEIRO COM HABILIDADES ESPECIAIS\n");
    printf("        (0=Agua, 3=Navio, 5=Habilidade)\n");
    printf("====================================================\n");
    
    // Imprime o cabeçalho das colunas
    printf("   "); 
    for (j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf(" %d", j);
    }
    printf("\n");
    printf("   --------------------\n");

    // Loops aninhados para exibir a matriz
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d| ", i);
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf(" %d", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("====================================================\n");

    return 0;
}