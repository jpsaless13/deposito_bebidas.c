// ********************************* SISTEMA DE DEPÓ“SITO DE BEBIDAS *********************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//! estrutura que representa uma bebida
typedef struct
{
    char nome[50];  //* nome da bebida
    int quantidade; //* quantidade atual da bebida (em unidades)
    int maxima;     //* quantidade máxima permitida
    int minima;     //* quantidade mí­nima permitida
    float preco;    //* preÃ§o da bebida
} bebida;

//! estrutura que representa uma caixa de bebida
typedef struct
{
    int unidades_por_caixa; //* número de unidades por caixa
    int quantidade_caixas;  //* quantidade de caixas no estoque
    float preco_caixa;      //* preço por caixa
} caixa;

//! estrutura que representa a venda
typedef struct
{
    char nome[50];  //* Nome da bebida vendida
    int quantidade; //* Quantidade vendida
    float preco;    //* Preço da bebida
    int metodo_pagamento;
    char horario[20];
    float precoUnitario;
    float totalSemDesconto;
    float totalComDesconto;
    float descontoAplicado;
    float pagamentoFiado;
    char data[20];
} venda;

//! Estrutura para representar um cliente
typedef struct
{
    int id;
    char nome[25];
    char telefone[15];
    float saldoDevedor;
} Cliente;

//! ponteiros
Cliente *clientes;            //* Ponteiro para representar os clientes
int qtdClientes = 0;          //* Contador de clientes
int capacidadeClientes = 1;   //*Capacidade mÃ­nima de clientes
venda *vendas = NULL;         //* Ponteiro para o registro de vendas
int total_vendas = 0;         //* Contador de vendas
bebida *estoque = NULL;       //* estoque de bebidas
caixa *estoque_caixas = NULL; //* estoque de caixas
int total_bebidas = 0;        //* total de bebidas no estoque
int total_caixas = 0;         //* total de caixas no estoque

//! função auxiliar que ler strings e remove a nova linha
void LerString(char *str, int tamanho)
{
    fgets(str, tamanho, stdin);
    str[strcspn(str, "\n")] = 0; //* remove nova linha
}
//! FunÃ§Ã£o que gera um ID de usuÃ¡rio aleatório
int generateUserId()
{
    return rand(); //* Retorna um número
}

//! Função para registrar a entrada de uma nova bebida no estoque
//* A função verifica se a bebida já existe no estoque e, caso exista,
//* atualiza a quantidade de unidades. Caso contrário, registra a nova bebida
//* com as quantidades e preços fornecidos.

void RegistrarEntrada(char *nome, int quantidade, int maxima, int minima, float preco)
{
    //* Verifica se a quantidade mínima é maior ou igual à quantidade máxima
    //* Se for, exibe um erro e encerra a execução da função.
    if (minima >= maxima)
    {
        printf("erro: a quantidade mínima não pode ser maior ou igual a quantidade máxima.\n");
        return;
    }

    //* Laço para verificar se a bebida já está registrada no estoque
    for (int i = 0; i < total_bebidas; i++)
    {
        //* Verifica se o nome da bebida no estoque é igual ao nome fornecido
        //* pela entrada. Se for, a bebida já está registrada no estoque.
        if (strcmp(estoque[i].nome, nome) == 0)
        {
            //* Se a bebida já existe no estoque, verifica se a nova quantidade
            //* a ser adicionada não ultrapassa a quantidade máxima permitida.
            //* Se ultrapassar, exibe um erro e encerra a execução da função.
            if (estoque[i].quantidade + quantidade > estoque[i].maxima)
            {
                printf("erro: nÃ£o Ã© possÃ­vel adicionar %d unidades de %s. mÃ¡ximo: %d.\n", quantidade, nome, estoque[i].maxima);
                return;
            }

            //* Se a nova quantidade for válida, adiciona a quantidade à quantidade
            //* já existente da bebida no estoque.
            estoque[i].quantidade += quantidade; //* atualiza a quantidade da bebida
            printf("entrada registrada: %d unidades de %s.\n", quantidade, nome);
            return;
        }
    }

    //* verifica se a quantidade inicial não ultrapassa o máximo permitido
    if (quantidade > maxima)
    {
        printf("erro: a quantidade máxima permitida para %s é %d. não é possível adicionar %d unidades.\n", nome, maxima, quantidade);
        return;
    }

    //* aloca memória para a nova bebida no estoque
    //* Como a bebida não existe no estoque, é necessário alocar espaço para
    //* uma nova entrada no vetor `estoque`. A função `realloc` é usada para aumentar
    //* a memória alocada para o estoque, considerando que será adicionado um novo item
    //* (a nova bebida).
    //* `total_bebidas` é incrementado para refletir a quantidade total de bebidas após
    //* o registro da nova bebida.
    estoque = realloc(estoque, (total_bebidas + 1) * sizeof(bebida));
    if (!estoque)
    {
        printf("erro ao alocar memÃ³ria.\n");
        exit(1);
    }

    //* registra a nova bebida no estoque
    strcpy(estoque[total_bebidas].nome, nome);
    estoque[total_bebidas].quantidade = quantidade;
    estoque[total_bebidas].maxima = maxima;
    estoque[total_bebidas].minima = minima;
    estoque[total_bebidas].preco = preco;
    total_bebidas++;
    printf("entrada registrada: %d unidades de %s com mÃ¡ximo %d e mínimo %d. preÃ§o: r$%.2f.\n",
           quantidade, nome, maxima, minima, preco);
}

//! funÃ§Ã£o para exibir o estoque de bebidas
void ExibirEstoque()
{
    printf("\n======================= ESTOQUE ATUAL DE BEBIDAS =======================\n");
    for (int i = 0; i < total_bebidas; i++)
    {
        printf("[%d] bebida: %-30s | quantidade: %3d | máximo: %3d | mí­nimo: %3d | preço: r$%.2f\n",
               i + 1, estoque[i].nome, estoque[i].quantidade,
               estoque[i].maxima, estoque[i].minima, estoque[i].preco);
    }
    printf("========================================================================\n");
}

//! funÃ§Ã£o para editar os dados de uma bebida
void EditarBebida(int indice)
{
    printf("\n======================= EDITAR BEBIDA =======================\n");
    printf("bebida: %s\n", estoque[indice].nome);
    printf("1. nome (atual: %s)\n", estoque[indice].nome);
    printf("2. quantidade mÃ¡xima (atual: %d)\n", estoque[indice].maxima);
    printf("3. quantidade mÃ­nima (atual: %d)\n", estoque[indice].minima);
    printf("4. preÃ§o (atual: r$%.2f)\n", estoque[indice].preco);
    printf("5. manter todos os valores\n");
    printf("escolha o campo que deseja editar (1-5): ");

    int opcao;
    scanf("%d", &opcao);
    getchar(); //! limpa o buffer do teclado

    switch (opcao)
    {
    case 1: //* editar nome
        printf("novo nome da bebida: ");
        char novo_nome[50];
        LerString(novo_nome, sizeof(novo_nome));
        strcpy(estoque[indice].nome, novo_nome);
        break;
    case 2: //* editar quantidade mÃ¡xima
        printf("nova quantidade mÃ¡xima: ");
        scanf("%d", &estoque[indice].maxima);
        break;
    case 3: //* editar quantidade mÃ­nima
        printf("nova quantidade mÃ­nima: ");
        scanf("%d", &estoque[indice].minima);
        break;
    case 4: //* editar preÃ§o
        printf("novo preÃ§o: r$");
        scanf("%f", &estoque[indice].preco);
        break;
    case 5: //* manter todos os valores
        printf("nenhuma alteraÃ§Ã£o foi feita.\n");
        break;
    default:
        printf("opÃ§Ã£o invÃ¡lida. nenhuma alteraÃ§Ã£o foi feita.\n");
        break;
    }
    printf("bebida editada com sucesso!\n");
}

//! funÃ§Ã£o para remover uma bebida do estoque
void RemoverBebida(int indice)
{
    //* desloca as bebidas apÃ³s a removida para preencher o espaÃ§o
    for (int i = indice; i < total_bebidas - 1; i++)
    {
        estoque[i] = estoque[i + 1];
    }
    total_bebidas--;
    estoque = realloc(estoque, total_bebidas * sizeof(bebida));
    if (total_bebidas > 0 && !estoque)
    {
        printf("erro ao alocar memÃ³ria.\n");
        exit(1);
    }
    printf("bebida removida com sucesso!\n");
}

//! função para verificar o estoque com base na quantidade mÃ­nima
void VerificarEstoque()
{
    printf("\n======================= VERIFICAÃ‡ÃƒO DE ESTOQUE =======================\n");
    for (int i = 0; i < total_bebidas; i++)
    {
        //*alerta se a quantidade estiver abaixo do mÃ­nimo
        if (estoque[i].quantidade < estoque[i].minima)
        {
            printf("**ALERTA**: a quantidade de %s estÃ¡ abaixo do mÃ­nimo (%d). quantidade atual: %d\n",
                   estoque[i].nome, estoque[i].minima, estoque[i].quantidade);
        }
        else if (estoque[i].quantidade == estoque[i].minima)
        {
            printf("**ALERTA**: a quantidade de %s atingiu o limite mÃ­nimo (%d). quantidade atual: %d\n",
                   estoque[i].nome, estoque[i].minima, estoque[i].quantidade);
        }
    }
    printf("========================================================================\n");
}

//! função para cadastrar uma nova caixa de bebida
void CadastrarCaixa(int unidades_por_caixa, int quantidade_caixas, float preco_caixa)
{
    estoque_caixas = realloc(estoque_caixas, (total_caixas + 1) * sizeof(caixa));
    if (estoque_caixas == NULL)
    {
        printf("erro ao alocar memÃ³ria para caixas.\n");
        exit(1);
    }

    //*registra a caixa sem associar a bebida
    estoque_caixas[total_caixas].unidades_por_caixa = unidades_por_caixa;
    estoque_caixas[total_caixas].quantidade_caixas = quantidade_caixas;
    estoque_caixas[total_caixas].preco_caixa = preco_caixa; //* atribui o preÃ§o da caixa
    total_caixas++;

    printf("caixa cadastrada: %d caixas com %d unidades por caixa. preÃ§o por caixa: r$%.2f\n",
           quantidade_caixas, unidades_por_caixa, preco_caixa);
}

//! função para exibir o estoque de caixas
void ExibirEstoqueCx()
{
    printf("\n======================= ESTOQUE ATUAL DE CAIXAS =======================\n");
    for (int i = 0; i < total_caixas; i++)
    {
        printf("[%d] unidades por caixa: %d | quantidade de caixas: %d | preÃ§o por caixa: r$%.2f\n",
               i + 1, estoque_caixas[i].unidades_por_caixa, estoque_caixas[i].quantidade_caixas, estoque_caixas[i].preco_caixa);
    }
    printf("=======================================================================\n");
}

//! função para editar os dados de uma caixa
void EditarCaixa(int indice)
{
    printf("\n======================= EDITAR CAIXA =======================\n");
    printf("caixa: %d unidades por caixa: %d | quantidade de caixas: %d | preÃ§o por caixa: r$%.2f\n",
           estoque_caixas[indice].unidades_por_caixa, estoque_caixas[indice].quantidade_caixas, estoque_caixas[indice].preco_caixa);

    printf("1. unidades por caixa\n");
    printf("2. quantidade de caixas\n");
    printf("3. preÃ§o por caixa\n");
    printf("4. manter todos os valores\n");
    printf("escolha o campo que deseja editar (1-4): ");

    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1: //* editar unidades por caixa
        printf("novas unidades por caixa: ");
        scanf("%d", &estoque_caixas[indice].unidades_por_caixa);
        break;
    case 2: //* editar quantidade de caixas
        printf("nova quantidade de caixas: ");
        scanf("%d", &estoque_caixas[indice].quantidade_caixas);
        break;
    case 3: //* editar preÃ§o por caixa
        printf("novo preÃ§o por caixa: r$");
        scanf("%f", &estoque_caixas[indice].preco_caixa);
        break;
    case 4: //* manter todos os valores
        printf("nenhuma alteraÃ§Ã£o foi feita.\n");
        break;
    default:
        printf("opÃ§Ã£o invÃ¡lida. nenhuma alteraÃ§Ã£o foi feita.\n");
        break;
    }
    printf("caixa editada com sucesso!\n");
}

//! função para registrar vendas
void RegistrarVenda(int i, int quantidade, int metodo_pagamento)
{
    //* Obtém o horário atual
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    char horario[20];
    strftime(horario, sizeof(horario), "%Y-%m-%d %H:%M:%S", tm_info); //* Horário completo (ano-mês-dia hora:minuto:segundo)

    //* Verifica se a bebida existe no estoque
    for (int i = 0; i < total_bebidas; i++)
    {
        if (estoque[i].quantidade < quantidade)
        {
            printf("Erro: quantidade de %s insuficiente no estoque. Disponível: %d.\n", estoque[i].nome, estoque[i].quantidade);
            return; //* Se a quantidade for insuficiente, retorna sem registrar a venda
        }

        //* Aloca memória para registrar a venda
        vendas = realloc(vendas, (total_vendas + 1) * sizeof(venda));
        if (!vendas)
        {
            printf("Erro ao alocar memória.\n");
            exit(1);
        }

        //! Calcula o preço final da venda
        float precofinal = quantidade * estoque[i].preco;

        //* Registra a venda conforme o método de pagamento
        if (metodo_pagamento == 0)
        { // Dinheiro
            int metd = 0;
            printf("Digite (0) para dinheiro e (1) para pix: ");
            scanf("%d", &metd);
            if (metd == 0)
            {
                printf("\nA opção dinheiro foi selecionada");
                printf("\nA venda será registrada...");
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (dinheiro): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal);
            }
            else if (metd == 1)
            { // Pix
                printf("\nA opção pix foi selecionada");
                printf("\nA venda será registrada...");
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (pix): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal);
            }
        }
        //! Cartão
        else if (metodo_pagamento == 1)
        {
            int metd2;
            printf("Digite (0) para débito e (1) para crédito: ");
            scanf("%d", &metd2);

            //! Débito
            if (metd2 == 0)
            {
                printf("\nA opção débito foi selecionada");
                printf("\nA venda será registrada...");
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (débito): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal);
            }
            //! Crédito
            else if (metd2 == 1)
            {
                float taxa = 0.0;
                printf("\nA opção crédito foi selecionada");
                printf("\nDigite a taxa a ser aplicada sobre o valor total: ");
                scanf("%f", &taxa);
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (crédito): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal + (precofinal * taxa / 100));
            }
        }
        else if (metodo_pagamento == 2)
        { //! Fiado
            printf("Pagamento Fiado\n");
        }

        strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info);
        printf("Horário da venda: %s\n", vendas[total_vendas].data);

        //* Atualiza o estoque
        estoque[i].quantidade -= quantidade;
        total_vendas++; //* Incrementa o contador de vendas
    }
}

//! função para gerar relatório
void gerar_relatorio(int tipo_relatorio)
{
    float total_vendas_periodo = 0.0;
    int vendas_por_periodo = 0;

    //* Variáveis para data
    char data_atual[20];
    time_t t = time(NULL);
    struct tm tm_info;
    localtime_r(&t, &tm_info); //* Obtém a hora local

    //* Formata a data atual como "YYYY-MM-DD"
    strftime(data_atual, sizeof(data_atual), "%d-%m-%Y", &tm_info);

    //* Relatório Diário - Não solicita data, busca as vendas do dia atual
    if (tipo_relatorio == 1)
    {
        printf("\n===== Relatório Diário =====\n");
        printf("Vendas realizadas no dia: %s\n", data_atual);

        for (int i = 0; i < total_vendas; i++)
        {
            //* Verifica se a venda foi realizada no mesmo dia
            if (strcmp(vendas[i].data, data_atual) == 0)
            {
                vendas_por_periodo++;
                total_vendas_periodo += vendas[i].quantidade * vendas[i].preco;
                printf("Venda: %s | Quantidade: %d | Preço unitário: R$%.2f | Total: R$%.2f\n",
                       vendas[i].nome, vendas[i].quantidade, vendas[i].preco,
                       vendas[i].quantidade * vendas[i].preco);
            }
        }
    }

    //* Relatório Mensal - Solicita o mês e o ano (formato MM/AAAA)
    else if (tipo_relatorio == 2)
    {
        int mes, ano;
        printf("\n===== Relatório Mensal =====\n");
        printf("Digite o mês e o ano (MM/AAAA): ");
        scanf("%d/%d", &mes, &ano);

        printf("Vendas realizadas no mês %02d/%d:\n", mes, ano);

        for (int i = 0; i < total_vendas; i++)
        {
            struct tm data_venda_tm = {0};
            strptime(vendas[i].data, "%d-%m-%Y", &data_venda_tm); //* Converte a data para estrutura tm

            //* Verifica se o mês e o ano da venda coincidem com o mês/ano fornecido
            if (data_venda_tm.tm_year + 1900 == ano && data_venda_tm.tm_mon + 1 == mes)
            {
                vendas_por_periodo++;
                total_vendas_periodo += vendas[i].quantidade * vendas[i].preco;
                printf("Venda: %s | Quantidade: %d | Preço unitário: R$%.2f | Total: R$%.2f\n",
                       vendas[i].nome, vendas[i].quantidade, vendas[i].preco,
                       vendas[i].quantidade * vendas[i].preco);
            }
        }
    }

    //* Relatório Anual - Solicita o ano (formato AAAA)
    else if (tipo_relatorio == 3)
    {
        int ano;
        printf("\n===== Relatório Anual =====\n");
        printf("Digite o ano (AAAA): ");
        scanf("%d", &ano);

        printf("Vendas realizadas no ano %d:\n", ano);

        for (int i = 0; i < total_vendas; i++)
        {
            struct tm data_venda_tm = {0};
            strptime(vendas[i].data, "%d-%m-%Y", &data_venda_tm); //* Converte a data para estrutura tm

            //* Verifica se o ano da venda bate com o ano fornecido
            if (data_venda_tm.tm_year + 1900 == ano)
            {
                vendas_por_periodo++;
                total_vendas_periodo += vendas[i].quantidade * vendas[i].preco;
                printf("Venda: %s | Quantidade: %d | Preço unitário: R$%.2f | Total: R$%.2f\n",
                       vendas[i].nome, vendas[i].quantidade, vendas[i].preco,
                       vendas[i].quantidade * vendas[i].preco);
            }
        }
    }

    //* Exibe o total de vendas
    if (vendas_por_periodo > 0)
    {
        printf("\nTotal de vendas: R$%.2f\n", total_vendas_periodo);
    }
    else
    {
        printf("\nNenhuma venda registrada para este período.\n");
    }
}
//! função para inicializar o array (vetor) de clientes

void inicializarClientes()
{
    clientes = (Cliente *)malloc(capacidadeClientes * sizeof(Cliente));
    if (clientes == NULL)
    {
        fprintf(stderr, "Erro ao alocar memÃ³ria!\n");
        exit(EXIT_FAILURE);
    }
}
//! função para adicionar um cliente
void adicionarCliente()
{
    //* Verifica se Ã© necessÃ¡rio aumentar a capacidade
    if (qtdClientes == capacidadeClientes)
    {
        capacidadeClientes *= 2; //* Dobra a capacidade
        Cliente *novosClientes = (Cliente *)realloc(clientes, capacidadeClientes * sizeof(Cliente));
        if (novosClientes == NULL)
        {
            fprintf(stderr, "Erro ao realocar memÃ³ria!\n");
            free(clientes);
            exit(EXIT_FAILURE);
        }
        clientes = novosClientes;
    }

    //* Adiciona o cliente
    clientes[qtdClientes].id = qtdClientes + 1;

    printf("Digite o nome do cliente: ");
    fgets(clientes[qtdClientes].nome, sizeof(clientes[qtdClientes].nome), stdin);
    clientes[qtdClientes].nome[strcspn(clientes[qtdClientes].nome, "\n")] = 0; //* Remove a nova linha

    printf("Digite o telefone do cliente: ");
    fgets(clientes[qtdClientes].telefone, sizeof(clientes[qtdClientes].telefone), stdin);
    clientes[qtdClientes].telefone[strcspn(clientes[qtdClientes].telefone, "\n")] = 0; //* Remove a nova linha

    clientes[qtdClientes].saldoDevedor = 0.0;
    qtdClientes++;
    printf("Cliente adicionado com sucesso!\n");
}

//! FunÃ§Ã£o para Mostrar Clientes

void mostrarClientes()
{
    printf("===================================================================\n");
    printf("| ID | Nome               | Telefone          | Saldo Devedor |\n");
    printf("===================================================================\n");

    for (int i = 0; i < qtdClientes; i++)
    {
        printf("| %3d | %-18s | %-16s | R$ %.2f        |\n",
               clientes[i].id,
               clientes[i].nome,
               clientes[i].telefone,
               clientes[i].saldoDevedor);
    }

    printf("===================================================================\n");
}
//! Função para editar clientes
void editarCliente(int id)
{
    /* printf("\n======================= EDITAR CLIENTE =======================\n");
    printf("bebida: %s\n", estoque[indice].nome);
    printf("1. nome (atual: %s)\n", estoque[indice].nome);
    printf("2. quantidade mÃ¡xima (atual: %d)\n", estoque[indice].maxima);
    printf("3. quantidade mÃ­nima (atual: %d)\n", estoque[indice].minima);
    printf("4. preÃ§o (atual: r$%.2f)\n", estoque[indice].preco);
    printf("5. manter todos os valores\n");
    printf("escolha o campo que deseja editar (1-5): ");

    int id;
    scanf("%d", &id);
    getchar(); //! limpa o buffer do teclado

    switch (opcao)
    {
    case 1: //* editar nome
        printf("novo nome: ");
        char novo_nome[50];
        LerString(clientes[i].nome, sizeof(clientes[i].nome), stdin);
        strcpy(clientes[i].nome, novo_nome);
        break;
    case 2: //* editar quantidade mÃ¡xima
         printf("novo telefone: ");
        char novo_nome[50];
        LerString(clientes[i].telefone, sizeof(clientes[i].telefone), stdin);
        strcpy(clientes[i].telefone, novo_telefone);
        break;
    case 3: //* editar quantidade mÃ­nima
        printf("nova quantidade mÃ­nima: ");
        scanf("%d", &estoque[indice].minima);
        break;
    case 4: //* editar preÃ§o
        printf("novo preÃ§o: r$");
        scanf("%f", &estoque[indice].preco);
        break;
    case 5: //* manter todos os valores
        printf("nenhuma alteraÃ§Ã£o foi feita.\n");
        break;
    default:
        printf("opÃ§Ã£o invÃ¡lida. nenhuma alteraÃ§Ã£o foi feita.\n");
        break;
    }
    printf("bebida editada com sucesso!\n");*/
    for (int i = 0; i < qtdClientes; i++)
    {
        if (clientes[i].id == id)
        {
            printf("Novo nome: ");
            fgets(clientes[i].nome, sizeof(clientes[i].nome), stdin);
            clientes[i].nome[strcspn(clientes[i].nome, "\n")] = 0;

            printf("Novo telefone: ");
            fgets(clientes[i].telefone, sizeof(clientes[i].telefone), stdin);
            clientes[i].telefone[strcspn(clientes[i].telefone, "\n")] = 0;

            printf("Cliente editado com sucesso!\n");
            return;
        }
    }
    printf("Cliente nÃ£o encontrado.\n");
}

//! Função para registrar divida do cliente
void registrarCompra(int idCliente, float valor)
{
    for (int i = 0; i < qtdClientes; i++)
    {
        if (clientes[i].id == idCliente)
        {
            clientes[i].saldoDevedor += valor;
            printf("Compra registrada para o cliente %s. Novo saldo: R$ %.2f\n",
                   clientes[i].nome, clientes[i].saldoDevedor);
            return;
        }
    }
    printf("Cliente nÃ£o encontrado.\n");
}

//* função principal
int main()
{
    inicializarClientes();                                                //* Função inicializadora de clientes
    int opcao;                                                            //* variável para armazenar a opÃ§Ã£o do menu
    char nome[50];                                                        //* variável para armazenar o nome da bebida
    int quantidade = 0, maxima, minima, metodo_pagamento = 0, indice = 0; //** variáveis para quantidade, mÃ¡xima , mÃ­nima e id do cliente
    float preco, preco_caixa;                                             //* variáveis para preÃ§o da bebida e da caixa
    int unidades_por_caixa, quantidade_caixas;                            //* variáveis para unidades e quantidade de caixas
    int id;
    float valor = 0; //* variáveis para id e valor devedor do cliente

    do
    {
        printf("\n======================= MENU =======================\n");
        printf("1. registrar entrada de bebida\n");
        printf("2. exibir estoque de bebidas\n");
        printf("3. editar bebida\n");
        printf("4. remover bebida\n");
        printf("5. verificar estoque\n");
        printf("6. cadastrar nova caixa\n");
        printf("7. exibir estoque de caixas\n");
        printf("8. editar caixa\n");
        printf("9. vender bebida\n");
        printf("10. Adicionar Cliente\n");
        printf("11. Mostrar clientes\n");
        printf("12. Editar clientes\n");
        printf("13. Registrar Compra fiado\n");
        printf("14. Gerar relatório\n");
        printf("15. Sair do sistema\n");
        printf("escolha uma opÃ§Ã£o (1-14): ");
        scanf("%d", &opcao);
        getchar(); //* limpa o buffer do teclado (limpa Ã¡rea da memÃ³ria usada)

        switch (opcao)
        {
        case 1: //*registrar entrada de bebida
            printf("nome da bebida: ");
            LerString(nome, sizeof(nome));
            printf("quantidade: ");
            scanf("%d", &quantidade);
            printf("mÃ¡xima: ");
            scanf("%d", &maxima);
            printf("mi­nima: ");
            scanf("%d", &minima);
            printf("preÃ§o: r$");
            scanf("%f", &preco);
            RegistrarEntrada(nome, quantidade, maxima, minima, preco);
            break;
        case 2: //* exibir estoque de bebidas
            ExibirEstoque();
            break;
        case 3: //* editar bebida
            ExibirEstoque();
            printf("escolha o nÃºmero da bebida a ser editada: ");
            int num; //* variável para armazenar o número da bebida
            scanf("%d", &num);
            if (num < 1 || num > total_bebidas)
            {
                printf("número inválido.\n");
            }
            else
            {
                EditarBebida(num - 1); //* chama a funÃ§Ã£o para editar a bebida
            }
            break;
        case 4: //* remover bebida
            ExibirEstoque();
            printf("escolha o número da bebida a ser removida: ");
            scanf("%d", &num);
            if (num < 1 || num > total_bebidas)
            {
                printf("nÃºmero invÃ¡lido.\n");
            }
            else
            {
                RemoverBebida(num - 1); //* chama a funÃ§Ã£o para remover a bebida
            }
            break;
        case 5: //*verificar estoque
            VerificarEstoque();
            break;
        case 6: //*cadastrar nova caixa
            printf("unidades por caixa: ");
            scanf("%d", &unidades_por_caixa);
            printf("quantidade de caixas: ");
            scanf("%d", &quantidade_caixas);
            printf("preÃ§o por caixa: r$");
            scanf("%f", &preco_caixa);
            CadastrarCaixa(unidades_por_caixa, quantidade_caixas, preco_caixa);
            break;
        case 7: //* exibir estoque de caixas
            ExibirEstoqueCx();
            break;
        case 8: //* editar caixa
            ExibirEstoqueCx();
            printf("escolha o nÃºmero da caixa a ser editada: ");
            scanf("%d", &num);
            if (num < 1 || num > total_caixas)
            {
                printf("nÃºmero invÃ¡lido.\n");
            }
            else
            {
                EditarCaixa(num - 1); //* chama a funÃ§Ã£o para editar a caixa
            }
            break;
        case 9: //*Vender bebida
            ExibirEstoque();
            printf("Digite o indice da bebida");
            scanf("%d", &indice);
            printf("Quantidade: ");
            scanf("%d", &quantidade);
            printf("selecione o metodo de pagamento (0) para a vista e (1) para cartÃ£o");
            scanf("%d", &metodo_pagamento);
            RegistrarVenda(indice, quantidade, metodo_pagamento);
            break;
        case 10: //* adicionar Clientes
            adicionarCliente();
            break;
        case 11: //* mostrar clientes
            mostrarClientes();
            break;
        case 12: //* editar clientes
            printf("Digite o ID do cliente a ser editado: ");
            scanf("%d", &id);
            editarCliente(id);
            break;
        case 13: //* registrar compra fiado(temporario)
            printf("Digite o ID do cliente: ");
            scanf("%d", &id);
            printf("Digite o valor da compra: ");
            scanf("%f", &valor);
            registrarCompra(id, valor);
            break;
        case 14: //* gerar relatórios
            int tipo_relatorio;
            printf("Escolha o tipo de relatório:\n");
            printf("1. Relatório Diário\n");
            printf("2. Relatório Mensal\n");
            printf("3. Relatório Anual\n");
            printf("Digite o número do tipo de relatório: ");
            scanf("%d", &tipo_relatorio);
            gerar_relatorio(tipo_relatorio); //* Gera o relatório com base no tipo
            break;
        case 15: //* sair do sistema
            printf("saindo do sistema...\n");
            break;
        default:
            printf("opÃ§Ã£o inválida. tente novamente.\n");
            break;
        }
    } while (opcao != 15); //! condição de parada do loop do menu principal

    //* libera memória alocada nos estoques
    free(estoque);
    free(estoque_caixas);

    return 0;
}
// TODO: ORGANIZAR MENUS
// TODO: IMPLEMENTAR FUNÇÃO PARA CALCULO DA RENDA LIQUIDA INCLUINDO TODAS DESPESAS


