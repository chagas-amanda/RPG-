#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para os Itens do Inventário
typedef struct {
    char nome[50];
    int tipo_efeito; // 1: Cura, 2: Ataque/Defesa
    int valor;
} Item;

// Estrutura para o Personagem
typedef struct {
    char nome[100];
    int hp;
    int ataque;
    Item *inventario;
    int qtd_itens;
} Personagem;

// Estrutura para a Sala
typedef struct Sala {
    int id;
    int tipo; // 1: Monstro, 2: Tesouro, 3: Armadilha
    struct Sala *proxima;
} Sala;

// Função para criar uma sala dinamicamente
Sala *criar_sala(int id) {
    Sala *nova = malloc(sizeof(Sala));
    nova->id = id;
    nova->proxima = NULL;

    if (id == 7) {
        nova->tipo = 1; // Chefe final na última sala
    }
    else {
        nova->tipo = rand() % 3 + 1;
    }
    return nova;
}

// Função para liberar a masmorra
void liberar_masmorra(Sala *atual) {
    while (atual != NULL) {
        Sala *proxima = atual->proxima;
        free(atual);
        atual = proxima;
    }
}

// Função para usar item
void usar_item(Personagem *heroi, int indice) {
    Item item = heroi->inventario[indice];
    if (item.tipo_efeito == 1) {
        heroi->hp += item.valor;
        printf("Voce usou [%s] e recuperou %d de HP!\n", item.nome, item.valor);
    }
    else {
        printf("Voce usou [%s] e neutralizou o perigo!\n", item.nome);
    }

    for (int i = indice; i < heroi->qtd_itens - 1; i++) {
        heroi->inventario[i] = heroi->inventario[i + 1];
    }
    heroi->qtd_itens--;

    if (heroi->qtd_itens == 0) {
        free(heroi->inventario);
        heroi->inventario = NULL;
    }
    else {
        heroi->inventario =
            realloc(heroi->inventario, heroi->qtd_itens * sizeof(Item));
    }
}

int main() {
    srand(time(NULL));
    Personagem heroi;
    heroi.qtd_itens = 0;
    heroi.inventario = NULL;

    printf("=== RPG DE MASMORRA ===\n");
    printf("Escolha seu personagem:\n");
    printf("[1] LeirbaG Bigode - O Bardo mexicano mais conhecido no mundo, mas "
           "nao o peca para cantar. Jamais.\n");
    printf("[2] Mr Git - O Mago com temperamento mais sensivel da aldeia, nao "
           "roube o lugar dele nas festas.\n");
    printf("[3] El Inhumas - O Ladino referencia em questao de furtos... se "
           "voce nao morar 52km dele.\n");
    printf("Escolha: ");
    int escolha;
    scanf("%d", &escolha);

    // Definindo nome e atributos baseados no RPG original
    if (escolha == 1) {
        strcpy(heroi.nome, "LeirbaG Bigode");
        heroi.hp = 90;
        heroi.ataque = 12;
    }
    else if (escolha == 2) {
        strcpy(heroi.nome, "Mr Git");
        heroi.hp = 80;
        heroi.ataque = 18;
    }
    else {
        strcpy(heroi.nome, "El Inhumas");
        heroi.hp = 100;
        heroi.ataque = 14;
    }

    printf("\nPersonagem escolhido: %s | HP: %d | Ataque: %d\n",
           heroi.nome,
           heroi.hp,
           heroi.ataque);

    // Cria a primeira sala
    Sala *inicio = criar_sala(1);
    Sala *atual = inicio;

    while (heroi.hp > 0 && atual != NULL) {
        printf("\n----------------------------------------\n");
        printf("Voce esta na Sala #%d de 7\n", atual->id);
        printf("HP Atual: %d | Itens: %d\n", heroi.hp, heroi.qtd_itens);

        if (atual->id == 7) {
            printf("FIM DA MASMORRA! O Grande Chefe Final te desafia para um "
                   "duelo mortal!\n");
            int dano = 50;

            if (heroi.qtd_itens > 0) {
                printf("Deseja usar um item contra o chefe? [1-Sim / 0-Nao]: ");
                int usar;
                scanf("%d", &usar);
                if (usar == 1) {
                    for (int i = 0; i < heroi.qtd_itens; i++) {
                        printf("[%d] %s\n", i + 1, heroi.inventario[i].nome);
                    }
                    printf("Escolha o item: ");
                    int idx;
                    scanf("%d", &idx);
                    if (idx > 0 && idx <= heroi.qtd_itens) {
                        usar_item(&heroi, idx - 1);
                        dano = 0;
                    }
                }
            }

            heroi.hp -= dano;
            if (dano > 0)
                printf("Voce lutou com o chefe e sofreu %d de dano!\n", dano);

            if (heroi.hp > 0) {
                printf("\nVITORIA! Voce derrotou o chefe final e escapou da "
                       "masmorra! Fuja, enquanto ha chance...\n");
            }
            else {
                printf("\nDERROTA! O chefe final acabou com voce (vai ser "
                       "caixao fechado)...\n");
            }
            break;
        }

        // Salas de 1 a 6
        if (atual->tipo == 1) {
            int dano = 20;
            printf("Um monstro apareceu e te atacou! (-20 HP)\n");
            heroi.hp -= dano;
        }
        else if (atual->tipo == 2) {
            heroi.qtd_itens++;
            heroi.inventario =
                realloc(heroi.inventario, heroi.qtd_itens * sizeof(Item));
            strcpy(heroi.inventario[heroi.qtd_itens - 1].nome,
                   "Pocao de Cura (+30 HP)");
            heroi.inventario[heroi.qtd_itens - 1].tipo_efeito = 1;
            heroi.inventario[heroi.qtd_itens - 1].valor = 30;
            printf("Voce encontrou um tesouro: Pocao de Cura!\n");
        }
        else {
            printf("Voce caiu em uma armadilha! (*aaaaa* *efeitos sonoros*) "
                   "(-10 HP)\n");
            heroi.hp -= 10;
        }

        if (heroi.hp <= 0) {
            printf("\nVoce morreu na masmorra e nunca mais sera achado... Fim "
                   "de jogo!\n");
            break;
        }

        // Menu de Ações com a opção de fugir
        printf("\nO que deseja fazer?\n");
        if (heroi.qtd_itens > 0) printf("[1] Usar item do inventario\n");
        printf("[2] Avancar para a proxima sala\n");
        printf("[3] Desistir e tentar correr da masmorra\n");
        printf("Escolha: ");
        int acao;
        scanf("%d", &acao);

        if (acao == 1 && heroi.qtd_itens > 0) {
            for (int i = 0; i < heroi.qtd_itens; i++) {
                printf("[%d] %s\n", i + 1, heroi.inventario[i].nome);
            }
            printf("Escolha o item para usar: ");
            int item_idx;
            scanf("%d", &item_idx);
            if (item_idx > 0 && item_idx <= heroi.qtd_itens) {
                usar_item(&heroi, item_idx - 1);
            }
        }
        else if (acao == 3) {
            // Opção de fuga solicitada
            printf("\nVoce saiu correndo desesperadamente da masmorra e "
                   " nao sobreviveu para contar a historia! . . . Voce foi "
                   "derrotado pela sua covardia!\n");

            // Limpa a memória antes de sair com return 0
            liberar_masmorra(inicio);
            if (heroi.inventario != NULL) free(heroi.inventario);
            return 0;
        }

        if (atual->proxima == NULL && atual->id < 7) {
            atual->proxima = criar_sala(atual->id + 1);
        }
        atual = atual->proxima;
    }

    // Limpeza de memória dinâmica ao final normal do jogo
    liberar_masmorra(inicio);
    if (heroi.inventario != NULL) free(heroi.inventario);

    printf("\nAte a proxima!\n");
    return 0;
}