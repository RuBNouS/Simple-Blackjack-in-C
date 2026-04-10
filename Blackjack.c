#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//--------------------informação das cartas
//arrays para imprimir info das cartas
const char* naipes[] = { "Copas", "Ouros", "Paus", "Espadas" };
const char* nomes[] = { "", "As", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Valete", "Dama", "Rei" };

//estrutura
typedef struct {
    int valor;
    int naipe;
} Carta;

//variáveis para o baralho
Carta baralho[52];
int topoBaralho = 0;

//iniciar o baralho
void inicializarBaralho() {
    int i = 0;
    for (int n = 0; n < 4; n++) {
        for (int v = 1; v <= 13; v++) {
            baralho[i].valor = v;
            baralho[i].naipe = n;
            i++;
        }
    }
}

//baralhar as cartas
void baralhar() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 52; i++) {
        int r = rand() % 52;
        Carta temp = baralho[i];
        baralho[i] = baralho[r];
        baralho[r] = temp;
    }
    topoBaralho = 0; //novo baralho sempre - mais simples lol
}

//meter as cartas jogadas para no baralkho
Carta pedirCarta() {
    return baralho[topoBaralho++];
}

//impriomir carta
void imprimirCarta(Carta c) {
    printf("[%s de %s] ", nomes[c.valor], naipes[c.naipe]);
}

//calcular os pontos da mão 
//TALVEZ ADICIONAR NO MENU "DIFICULDADE" QUE TIRA O CALCULO AUTOMATICO DE PONTOS, OU SÓ TIRAR A PARTE DE IMPRIMIR 
// (MAS DEIXAR NO MODO de acessibilidade, OU SÓ DAR A OPÇÃO)
int calcularPontos(Carta mao[], int numCartas) {
    int pontos = 0;
    int ases = 0;

    for (int i = 0; i < numCartas; i++) {
        if (mao[i].valor == 1) {
            pontos += 11;
            ases++;
        }
        else if (mao[i].valor > 10) {
            pontos += 10;
        }
        else {
            pontos += mao[i].valor;
        }
    }

    while (pontos > 21 && ases > 0) {
        pontos -= 10;
        ases--;
    }

    return pontos;
}

//--------------------logica do jogo em si
void jogarBlackjack() {
    //info default
    Carta maoJogador[10];
    Carta maoDealer[10];
    int cartasJogador = 0;
    int cartasDealer = 0;
    int escolha;

    //-----começar o jogo
    printf("\n===== A Distribuir Cartas =====\n");

    //iniciar e baralhar
    inicializarBaralho();
    baralhar();

    //distribuir cartas (2 para cada)
    maoJogador[cartasJogador++] = pedirCarta();
    maoDealer[cartasDealer++] = pedirCarta();
    maoJogador[cartasJogador++] = pedirCarta();
    maoDealer[cartasDealer++] = pedirCarta();

    //turno do jogador

	int turnoJogador = 1; //1 para continuar, 0 para parar

    while (turnoJogador) {
        int pontosJogador = calcularPontos(maoJogador, cartasJogador); //calcular pontos

        int pontosVisiveisDealer = calcularPontos(&maoDealer[0], 1); //calcular os pontos apenas da carta visível do Dealer

        //mostrar mão dealer com os pontos visíveis
        printf("\nMão do Dealer (%d pontos visíveis):\n", pontosVisiveisDealer);
        imprimirCarta(maoDealer[0]);
        printf("[Carta Escondida]\n");

        printf("\nSua mão (%d pontos):\n", pontosJogador); //mostrar os pontos
        //ciclo para mostrar as cartas na mão do jogador
        for (int i = 0; i < cartasJogador; i++) {
            imprimirCarta(maoJogador[i]); 
        }
        printf("\n");
        printf("\n");

        //calculo do max 21
        if (pontosJogador > 21) {
            printf("Estourou. Passaste dos 21.\n");
            break; 
        }
        else if (pontosJogador == 21) {
            printf("Conseguiste 21.\n");
            break; //passa para turno do dealer
        }

        //pedir escolha ao jogador
        do{
            printf("========== O que queres fazer?    1 - Pedir carta (Hit)   2 - Parar (Stand) ===== Escolha: ");
            if (scanf("%d", &escolha) != 1) {
                printf("Entrada inválida! Por favor, introduza um número (1 ou 2).\n");
			    while (getchar() != '\n'); //limpa o buffer do teclado
                escolha = 0;
                continue;
            }
		    switch (escolha) {
		    case 1:
                maoJogador[cartasJogador++] = pedirCarta();
                break;
            case 2:
				turnoJogador = 0; //sair do ciclo do jogador
            default:
                printf("Entrada inválida! Por favor, introduza um número (1 ou 2).\n");
                break;
            }
        }while (escolha != 1 && escolha != 2);

    }

    //turno do dealer
    printf("\n===== Turno do Dealer =====\n");
    while (1) {
        int pontosDealer = calcularPontos(maoDealer, cartasDealer);

        printf("\nMão do Dealer (%d pontos):\n", pontosDealer);
        for (int i = 0; i < cartasDealer; i++) {
            imprimirCarta(maoDealer[i]);
        }
        printf("\n");

        if (pontosDealer > 21) {
            printf("\nDealer Estourou. Passou dos 21.\n");
            break;
        }
        else if (pontosDealer >= 17) {
            printf("\nO Dealer para com %d pontos.\n", pontosDealer);
            break;
        }
        else {
            printf("\nO Dealer pede mais uma carta...\n");
            maoDealer[cartasDealer++] = pedirCarta();
        }
    }

    //determinar o Vencedor
    int pontosJogadorFinais = calcularPontos(maoJogador, cartasJogador);
    int pontosDealerFinais = calcularPontos(maoDealer, cartasDealer);

    printf("\n======================================== Resultado Final ========================================\n");
    printf("Tu: %d pontos | Dealer: %d pontos\n", pontosJogadorFinais, pontosDealerFinais);

    //decidir se ganhou, perdeu ou empatou
    if (pontosJogadorFinais > 21) {
        printf("Passaste de 21! O Dealer Ganhou.\n");
        return;
    }
    else if (pontosDealerFinais > 21) {
        printf("O Dealer rebentou! Ganhaste.\n");
    }
    else if (pontosJogadorFinais > pontosDealerFinais) {
        printf("Ganhaste.\n");
    }
    else if (pontosDealerFinais > pontosJogadorFinais) {
        printf("O Dealer Ganhou.\n");
    }
    else { //mesmo pontitos
        printf("Empate (Push).\n");
    }
}

//função para mostrar as regras
void mostrarRegras() {
    printf("\n===== Regras do Blackjack ===================================================\n");
    printf("Objetivo: Vencer o Dealer, obtendo uma pontuação maior sem passar de 21.\n");
    printf("1. Cartas numéricas valem o seu valor normal.\n");
    printf("2. Valete, Dama e Rei valem 10 pontos.\n");
    printf("3. O Ás vale 1 ou 11 pontos, dependendo do que for mais favorável para a mão.\n");
    printf("4. O Dealer é obrigado a parar se tiver 17 pontos ou mais.\n");
    printf("=============================================================================\n");
}

int main() {
    SetConsoleOutputCP(CP_UTF8); //muda a janela para UTF-8 para conseguir meter acentos e etc...

    int opcao;
    int modoAcessibilidade = 1; //ligado-1, desligado-0. TEM DE IR LIGADO POR DEFAULT

    do {
        printf("\n===== Blackjack ======================================\n");
        printf("1 - Jogar\n");
        printf("2 - Ler Regras\n");
        printf("3 - Sair\n");
        printf("Escolha uma opção: ");

		//apenas aceitar valores validos e lidar com entradas inválidas
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada inválida! Por favor, introduza um número entre 1 e 4.\n");
            while (getchar() != '\n'); //limpa o buffer do teclado
            opcao = 0; //reinicia a opção para forçar o ciclo a continuar
            continue;
        }

        switch (opcao) {
        case 1:
            jogarBlackjack();
            break;
        case 2:
            mostrarRegras();
            break;
        case 3:
            printf("\nA fechar programa\nOuve: Alice In Chains - Would?\n");
            break;
        default:
            printf("\nEntrada inválida! Por favor, introduza um número entre 1 e 33.\n");
        }
    } while (opcao != 3);

    return 0;
}