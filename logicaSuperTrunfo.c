#include <stdio.h>
#include <string.h>

#define MAX_NOME 64
#define NUM_CARTAS 5

typedef struct {
    char nome[MAX_NOME];
    long long populacao;   // habitantes
    double area;           // km^2
    double pib;            // em bilhões
    double densidade;      // hab/km^2
} Carta;

/* --- Dados e utilitários --- */
const char* nome_atributo(int codigo) {
    switch (codigo) {
        case 1: return "Populacao";
        case 2: return "Area";
        case 3: return "PIB";
        case 4: return "Densidade Demografica";
        default: return "Desconhecido";
    }
}

double valor_atributo(const Carta *c, int codigo) {
    switch (codigo) {
        case 1: return (double)c->populacao;
        case 2: return c->area;
        case 3: return c->pib;
        case 4: return c->densidade;
        default: return 0.0;
    }
}

void limpar_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* --- Interface / seleção --- */
void mostrar_cartas(const Carta cartas[], int n) {
    printf("Cartas disponiveis:\n");
    for (int i = 0; i < n; ++i) {
        printf(" %d) %s\n", i + 1, cartas[i].nome);
    }
}

int escolher_indice(const char *prompt, int n, int proibido) {
    int idx = 0;
    while (1) {
        printf("%s (1-%d): ", prompt, n);
        if (scanf("%d", &idx) != 1) {
            printf("Entrada invalida. Digite um numero.\n");
            limpar_buffer();
            continue;
        }
        if (idx < 1 || idx > n) {
            printf("Opcao fora do intervalo. Tente novamente.\n");
            continue;
        }
        if (proibido != -1 && idx == proibido) {
            printf("Opcao proibida (ja escolhida). Escolha outra.\n");
            continue;
        }
        limpar_buffer();
        return idx - 1; // retorna 0-based
    }
}

int escolher_atributo(int excluir) {
    int attr = 0;
    while (1) {
        printf("\nEscolha um atributo para comparacao:\n");
        for (int i = 1; i <= 4; ++i) {
            if (i == excluir) continue;
            switch (i) {
                case 1: printf(" %d) Populacao (maior vence)\n"); break;
                case 2: printf(" %d) Area (maior vence)\n"); break;
                case 3: printf(" %d) PIB (maior vence)\n"); break;
                case 4: printf(" %d) Densidade Demografica (MENOR vence)\n"); break;
            }
        }
        printf("Opcao: ");
        if (scanf("%d", &attr) != 1) {
            printf("Entrada invalida. Digite um numero valido.\n");
            limpar_buffer();
            continue;
        }
        if (attr < 1 || attr > 4 || attr == excluir) {
            printf("Opcao invalida. Tente novamente.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        return attr;
    }
}

/* --- Comparacao --- */
/* retorna: 1 se c1 vence, -1 se c2 vence, 0 empate (para um atributo) */
int comparar_um_atributo(double v1, double v2, int atributo) {
    if (atributo == 4) { // densidade: menor vence
        if (v1 < v2) return 1;
        if (v2 < v1) return -1;
        return 0;
    } else { // maior vence
        if (v1 > v2) return 1;
        if (v2 > v1) return -1;
        return 0;
    }
}

/* --- Exibição --- */
void imprimir_valor_atributo(const Carta *c, int attr) {
    if (attr == 1) printf("%lld", c->populacao);
    else printf("%.2f", valor_atributo(c, attr));
}

void imprimir_resultado(const Carta cartas[], int i1, int i2, int a1, int a2,
                        double v1a1, double v2a1, double v1a2, double v2a2,
                        int pontos1, int pontos2, double soma1, double soma2) {
    printf("\n=== Resultado da Comparacao ===\n");
    printf("Carta 1: %s\n", cartas[i1].nome);
    printf("Carta 2: %s\n", cartas[i2].nome);

    printf("\nAtributos utilizados:\n");
    printf(" 1) %s\n", nome_atributo(a1));
    printf("    %s: ", cartas[i1].nome); imprimir_valor_atributo(&cartas[i1], a1); printf("\n");
    printf("    %s: ", cartas[i2].nome); imprimir_valor_atributo(&cartas[i2], a1); printf("\n");
    printf(" 2) %s\n", nome_atributo(a2));
    printf("    %s: ", cartas[i1].nome); imprimir_valor_atributo(&cartas[i1], a2); printf("\n");
    printf("    %s: ", cartas[i2].nome); imprimir_valor_atributo(&cartas[i2], a2); printf("\n");

    printf("\nSoma dos atributos:\n");
    printf(" %s: %.2f\n", cartas[i1].nome, soma1);
    printf(" %s: %.2f\n", cartas[i2].nome, soma2);

    if (soma1 > soma2) printf("\nVencedor (pela soma): %s\n", cartas[i1].nome);
    else if (soma2 > soma1) printf("\nVencedor (pela soma): %s\n", cartas[i2].nome);
    else printf("\nEmpate!\n");

    printf("\nComparacao por atributos (pontos): %s %d x %d %s\n",
           cartas[i1].nome, pontos1, pontos2, cartas[i2].nome);
    printf("%s\n", (pontos1 > pontos2) ? "Por atributo: Carta 1 venceu." : (pontos2 > pontos1) ? "Por atributo: Carta 2 venceu." : "Por atributo: Empate.");
}

/* --- main --- */
int main(void) {
    Carta cartas[NUM_CARTAS] = {
        {"Brasil", 214326223LL, 8515767.0, 1445.0, 214326223.0 / 8515767.0},
        {"Estados Unidos", 334998398LL, 9833517.0, 25500.0, 334998398.0 / 9833517.0},
        {"China", 1415045928LL, 9596961.0, 17734.0, 1415045928.0 / 9596961.0},
        {"India", 1419361000LL, 3287263.0, 3730.0, 1419361000.0 / 3287263.0},
        {"Japao", 123951692LL, 377975.0, 4872.0, 123951692.0 / 377975.0}
    };

    printf("=== Desafio Super Trunfo - Comparacao Avancada (Atributos Multiples) ===\n\n");
    mostrar_cartas(cartas, NUM_CARTAS);

    int idx1 = escolher_indice("\nEscolha o numero da primeira carta", NUM_CARTAS, -1);
    int idx2 = escolher_indice("Escolha o numero da segunda carta (diferente da primeira)", NUM_CARTAS, idx1 + 1);

    int attr1 = escolher_atributo(-1);
    int attr2 = escolher_atributo(attr1);

    double v1_attr1 = valor_atributo(&cartas[idx1], attr1);
    double v2_attr1 = valor_atributo(&cartas[idx2], attr1);
    double v1_attr2 = valor_atributo(&cartas[idx1], attr2);
    double v2_attr2 = valor_atributo(&cartas[idx2], attr2);

    int res1 = comparar_um_atributo(v1_attr1, v2_attr1, attr1);
    int res2 = comparar_um_atributo(v1_attr2, v2_attr2, attr2);

    int pontos1 = (res1 == 1) + (res2 == 1);
    int pontos2 = (res1 == -1) + (res2 == -1);

    double soma1 = v1_attr1 + v1_attr2;
    double soma2 = v2_attr1 + v2_attr2;

    imprimir_resultado(cartas, idx1, idx2, attr1, attr2,
                       v1_attr1, v2_attr1, v1_attr2, v2_attr2,
                       pontos1, pontos2, soma1, soma2);

    printf("\nObrigado por jogar.\n");
    return 0;
}