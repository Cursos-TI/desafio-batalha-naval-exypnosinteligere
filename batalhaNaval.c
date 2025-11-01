#include <stdio.h>

// Definição de constantes para o tamanho do tabuleiro e dos navios
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define AGUA 0
#define NAVIO 3

// ----------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------------------------
int main() {
    // Requisito: Tabuleiro 10x10. Inicializado implicitamente com 0 (ÁGUA)
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO] = {0};
    
    // Variáveis para coordenadas dos navios
    int linha_navio_horiz, coluna_navio_horiz;
    int linha_navio_vert, coluna_navio_vert;
    
    // Variáveis de controle para os loops
    int i, j;
    
    // ====================================================================
    // 1. DEFINIÇÃO DAS COORDENADAS (Requisito: Definir diretamente no código)
    // ====================================================================
    
    // Navio Horizontal: Posicionado na Linha 1, a partir da Coluna 2 (posições: (1,2), (1,3), (1,4))
    linha_navio_horiz = 1;
    coluna_navio_horiz = 2;
    
    // Navio Vertical: Posicionado na Coluna 5, a partir da Linha 6 (posições: (6,5), (7,5), (8,5))
    // Nota: As coordenadas são escolhidas para garantir que os navios não se sobreponham.
    linha_navio_vert = 6;
    coluna_navio_vert = 5;

    // ====================================================================
    // 2. VALIDAÇÃO DE LIMITES (Requisito Funcional)
    // ====================================================================
    
    // Verifica se o Navio Horizontal cabe no tabuleiro
    if (coluna_navio_horiz + TAMANHO_NAVIO > TAMANHO_TABULEIRO || 
        linha_navio_horiz < 0 || linha_navio_horiz >= TAMANHO_TABULEIRO) {
        printf("ERRO: Coordenadas invalidas para o navio horizontal. Fora dos limites.\n");
        return 1; // Encerra o programa com erro
    }
    
    // Verifica se o Navio Vertical cabe no tabuleiro
    if (linha_navio_vert + TAMANHO_NAVIO > TAMANHO_TABULEIRO ||
        coluna_navio_vert < 0 || coluna_navio_vert >= TAMANHO_TABULEIRO) {
        printf("ERRO: Coordenadas invalidas para o navio vertical. Fora dos limites.\n");
        return 1;
    }
    
    // No Nível Novato, a não sobreposição é garantida pela escolha manual das coordenadas.
    
    // ====================================================================
    // 3. POSICIONAMENTO DOS NAVIOS (Requisito: Usar valor 3)
    // ====================================================================
    
    // --- Posicionamento do Navio Horizontal ---
    // O navio ocupa a mesma linha e se estende por TAMANHO_NAVIO colunas
    for (i = 0; i < TAMANHO_NAVIO; i++) {
        // tabuleiro[Linha Fixa][Coluna Inicial + Deslocamento]
        tabuleiro[linha_navio_horiz][coluna_navio_horiz + i] = NAVIO;
    }

    // --- Posicionamento do Navio Vertical ---
    // O navio ocupa a mesma coluna e se estende por TAMANHO_NAVIO linhas
    for (i = 0; i < TAMANHO_NAVIO; i++) {
        // tabuleiro[Linha Inicial + Deslocamento][Coluna Fixa]
        tabuleiro[linha_navio_vert + i][coluna_navio_vert] = NAVIO;
    }

    // ====================================================================
    // 4. EXIBIÇÃO DO TABULEIRO (Requisito Funcional)
    // ====================================================================
    
    printf("\n--- Tabuleiro de Batalha Naval (10x10) ---\n");
    printf("0 = AGUA | 3 = NAVIO\n\n");
    
    // Imprime o cabeçalho das colunas (0 a 9)
    printf("   "); // Espaco para a coluna das linhas
    for (j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%d ", j);
    }
    printf("\n");
    printf("   --------------------\n");

    // Usa loops aninhados para percorrer a matriz (Requisito: Loops aninhados)
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        // Imprime o índice da linha
        printf("%d| ", i);
        
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            // Imprime o valor da posição com um espaço para separação (Legibilidade)
            printf("%d ", tabuleiro[i][j]);
        }
        printf("\n"); // Quebra de linha após cada linha do tabuleiro
    }

    printf("\nPosicoes do Navio Horizontal: (%d,%d) a (%d,%d)\n", 
           linha_navio_horiz, coluna_navio_horiz, 
           linha_navio_horiz, coluna_navio_horiz + TAMANHO_NAVIO - 1);
    
    printf("Posicoes do Navio Vertical:   (%d,%d) a (%d,%d)\n", 
           linha_navio_vert, coluna_navio_vert, 
           linha_navio_vert + TAMANHO_NAVIO - 1, coluna_navio_vert);

    return 0;
}