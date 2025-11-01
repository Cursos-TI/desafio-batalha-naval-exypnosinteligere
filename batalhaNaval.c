#include <stdio.h>

// Definição de constantes para o tamanho do tabuleiro e dos navios
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define AGUA 0
#define NAVIO 3

// ----------------------------------------------------------------------
// FUNÇÃO AUXILIAR: VALIDA E POSICIONA UM NAVIO
// Retorna 0 em caso de sucesso, 1 em caso de erro (limites ou sobreposição)
// ----------------------------------------------------------------------
int posicionar_navio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                     int linha_inicio, int coluna_inicio, int orientacao) {
    
    // 0: Horizontal (Linha constante, Coluna varia)
    // 1: Vertical (Coluna constante, Linha varia)
    // 2: Diagonal Descendente (Linha e Coluna aumentam: (i, j), (i+1, j+1), ...)
    // 3: Diagonal Ascendente (Linha aumenta, Coluna diminui: (i, j), (i+1, j-1), ...)
    
    int i, r, c; // r=linha, c=coluna
    
    // 1. Validação de Limites
    if (linha_inicio < 0 || linha_inicio >= TAMANHO_TABULEIRO || 
        coluna_inicio < 0 || coluna_inicio >= TAMANHO_TABULEIRO) {
        return 1; // Erro: Coordenada inicial fora do limite
    }

    // Verifica se o navio de tamanho 3 cabe na orientação
    switch (orientacao) {
        case 0: // Horizontal
            if (coluna_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 1;
            break;
        case 1: // Vertical
            if (linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 1;
            break;
        case 2: // Diagonal Descendente (Linha e Coluna aumentam)
            if (linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO || 
                coluna_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 1;
            break;
        case 3: // Diagonal Ascendente (Linha aumenta, Coluna diminui)
            if (linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO || 
                coluna_inicio - TAMANHO_NAVIO < -1) return 1; // Coluna deve ser >= 0
            break;
    }
    
    // 2. Validação de Sobreposição (Requisito Funcional)
    // Verifica se alguma posição já está ocupada (valor != AGUA)
    for (i = 0; i < TAMANHO_NAVIO; i++) {
        r = linha_inicio;
        c = coluna_inicio;
        
        switch (orientacao) {
            case 0: c += i; break; // Horizontal
            case 1: r += i; break; // Vertical
            case 2: r += i; c += i; break; // Diagonal Descendente
            case 3: r += i; c -= i; break; // Diagonal Ascendente
        }
        
        if (tabuleiro[r][c] != AGUA) {
            printf("ERRO: Sobreposicao detectada em (%d,%d).\n", r, c);
            return 1; // Erro: Sobreposição
        }
    }
    
    // 3. Posicionamento (Se a validação passou)
    for (i = 0; i < TAMANHO_NAVIO; i++) {
        r = linha_inicio;
        c = coluna_inicio;
        
        switch (orientacao) {
            case 0: c += i; break;
            case 1: r += i; break;
            case 2: r += i; c += i; break;
            case 3: r += i; c -= i; break;
        }
        
        tabuleiro[r][c] = NAVIO;
    }
    
    return 0; // Sucesso
}

// ----------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------------------------
int main() {
    // Requisito: Tabuleiro 10x10. Inicializado com 0 (ÁGUA)
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO] = {0};
    int i, j;
    int erro = 0;

    printf("====================================================\n");
    printf("        POSICIONAMENTO DE NAVIOS DIAGONALES\n");
    printf("====================================================\n");

    // ====================================================================
    // 1. POSICIONAMENTO DOS QUATRO NAVIOS (Definidos para NAO sobrepor)
    // ====================================================================

    printf("Tentando posicionar 4 navios de tamanho %d...\n", TAMANHO_NAVIO);

    // Navio 1: Horizontal (Linha 1, Coluna 2)
    erro |= posicionar_navio(tabuleiro, 1, 2, 0); 
    // Navio 2: Vertical (Linha 6, Coluna 5)
    erro |= posicionar_navio(tabuleiro, 6, 5, 1); 
    
    // Navio 3: Diagonal Descendente (Linha 0, Coluna 0) -> (0,0), (1,1), (2,2)
    erro |= posicionar_navio(tabuleiro, 0, 0, 2); 

    // Navio 4: Diagonal Ascendente (Linha 4, Coluna 8) -> (4,8), (5,7), (6,6)
    erro |= posicionar_navio(tabuleiro, 4, 8, 3); 

    if (erro != 0) {
        printf("\nERRO FATAL: Falha ao posicionar um ou mais navios (limites ou sobreposicao).\n");
        return 1;
    }
    
    // ====================================================================
    // 2. EXIBIÇÃO DO TABULEIRO (Requisito Funcional)
    // ====================================================================
    
    printf("\n--- Tabuleiro Final (0=AGUA, 3=NAVIO) ---\n\n");
    
    // Imprime o cabeçalho das colunas (0 a 9)
    printf("   "); 
    for (j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%2d", j); // Usa %2d para alinhar corretamente
    }
    printf("\n");
    printf("   ----------------------\n");

    // Loops aninhados para percorrer e exibir a matriz (Legibilidade)
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        // Imprime o índice da linha
        printf("%d| ", i);
        
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            // Imprime o valor da posição com %2d para garantir alinhamento
            printf("%2d", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n====================================================\n");

    return 0;
}