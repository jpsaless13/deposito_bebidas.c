// ********************************* SISTEMA DE DEPӓSITO DE BEBIDAS *********************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//! estrutura que representa uma bebida
typedef struct
{
    char nome[50];  //* nome da bebida
    int quantidade; //* quantidade atual da bebida (em unidades)
    int maxima;     //* quantidade m�xima permitida
    int minima;     //* quantidade m�nima permitida
    float preco;    //* preço da bebida
} bebida;

//! estrutura que representa uma caixa de bebida
typedef struct
{
    int unidades_por_caixa; //* n�mero de unidades por caixa
    int quantidade_caixas;  //* quantidade de caixas no estoque
    float preco_caixa;      //* pre�o por caixa
} caixa;

//! estrutura que representa a venda
typedef struct
{
    char nome[50];  //* Nome da bebida vendida
    int quantidade; //* Quantidade vendida
    float preco;    //* Pre�o da bebida
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
int capacidadeClientes = 1;   //*Capacidade mínima de clientes
venda *vendas = NULL;         //* Ponteiro para o registro de vendas
int total_vendas = 0;         //* Contador de vendas
bebida *estoque = NULL;       //* estoque de bebidas
caixa *estoque_caixas = NULL; //* estoque de caixas
int total_bebidas = 0;        //* total de bebidas no estoque
int total_caixas = 0;         //* total de caixas no estoque

//! fun��o auxiliar que ler strings e remove a nova linha
void LerString(char *str, int tamanho)
{
    fgets(str, tamanho, stdin);
    str[strcspn(str, "\n")] = 0; //* remove nova linha
}
//! Função que gera um ID de usuário aleat�rio
int generateUserId()
{
    return rand(); //* Retorna um n�mero
}

//! Fun��o para registrar a entrada de uma nova bebida no estoque
//* A fun��o verifica se a bebida j� existe no estoque e, caso exista,
//* atualiza a quantidade de unidades. Caso contr�rio, registra a nova bebida
//* com as quantidades e pre�os fornecidos.

void RegistrarEntrada(char *nome, int quantidade, int maxima, int minima, float preco)
{
    //* Verifica se a quantidade m�nima � maior ou igual � quantidade m�xima
    //* Se for, exibe um erro e encerra a execu��o da fun��o.
    if (minima >= maxima)
    {
        printf("erro: a quantidade m�nima n�o pode ser maior ou igual a quantidade m�xima.\n");
        return;
    }

    //* La�o para verificar se a bebida j� est� registrada no estoque
    for (int i = 0; i < total_bebidas; i++)
    {
        //* Verifica se o nome da bebida no estoque � igual ao nome fornecido
        //* pela entrada. Se for, a bebida j� est� registrada no estoque.
        if (strcmp(estoque[i].nome, nome) == 0)
        {
            //* Se a bebida j� existe no estoque, verifica se a nova quantidade
            //* a ser adicionada n�o ultrapassa a quantidade m�xima permitida.
            //* Se ultrapassar, exibe um erro e encerra a execu��o da fun��o.
            if (estoque[i].quantidade + quantidade > estoque[i].maxima)
            {
                printf("erro: não é possível adicionar %d unidades de %s. máximo: %d.\n", quantidade, nome, estoque[i].maxima);
                return;
            }

            //* Se a nova quantidade for v�lida, adiciona a quantidade � quantidade
            //* j� existente da bebida no estoque.
            estoque[i].quantidade += quantidade; //* atualiza a quantidade da bebida
            printf("entrada registrada: %d unidades de %s.\n", quantidade, nome);
            return;
        }
    }

    //* verifica se a quantidade inicial n�o ultrapassa o m�ximo permitido
    if (quantidade > maxima)
    {
        printf("erro: a quantidade m�xima permitida para %s � %d. n�o � poss�vel adicionar %d unidades.\n", nome, maxima, quantidade);
        return;
    }

    //* aloca mem�ria para a nova bebida no estoque
    //* Como a bebida n�o existe no estoque, � necess�rio alocar espa�o para
    //* uma nova entrada no vetor `estoque`. A fun��o `realloc` � usada para aumentar
    //* a mem�ria alocada para o estoque, considerando que ser� adicionado um novo item
    //* (a nova bebida).
    //* `total_bebidas` � incrementado para refletir a quantidade total de bebidas ap�s
    //* o registro da nova bebida.
    estoque = realloc(estoque, (total_bebidas + 1) * sizeof(bebida));
    if (!estoque)
    {
        printf("erro ao alocar memória.\n");
        exit(1);
    }

    //* registra a nova bebida no estoque
    strcpy(estoque[total_bebidas].nome, nome);
    estoque[total_bebidas].quantidade = quantidade;
    estoque[total_bebidas].maxima = maxima;
    estoque[total_bebidas].minima = minima;
    estoque[total_bebidas].preco = preco;
    total_bebidas++;
    printf("entrada registrada: %d unidades de %s com máximo %d e m�nimo %d. preço: r$%.2f.\n",
           quantidade, nome, maxima, minima, preco);
}

//! função para exibir o estoque de bebidas
void ExibirEstoque()
{
    printf("\n======================= ESTOQUE ATUAL DE BEBIDAS =======================\n");
    for (int i = 0; i < total_bebidas; i++)
    {
        printf("[%d] bebida: %-30s | quantidade: %3d | m�ximo: %3d | m�nimo: %3d | pre�o: r$%.2f\n",
               i + 1, estoque[i].nome, estoque[i].quantidade,
               estoque[i].maxima, estoque[i].minima, estoque[i].preco);
    }
    printf("========================================================================\n");
}

//! função para editar os dados de uma bebida
void EditarBebida(int indice)
{
    printf("\n======================= EDITAR BEBIDA =======================\n");
    printf("bebida: %s\n", estoque[indice].nome);
    printf("1. nome (atual: %s)\n", estoque[indice].nome);
    printf("2. quantidade máxima (atual: %d)\n", estoque[indice].maxima);
    printf("3. quantidade mínima (atual: %d)\n", estoque[indice].minima);
    printf("4. preço (atual: r$%.2f)\n", estoque[indice].preco);
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
    case 2: //* editar quantidade máxima
        printf("nova quantidade máxima: ");
        scanf("%d", &estoque[indice].maxima);
        break;
    case 3: //* editar quantidade mínima
        printf("nova quantidade mínima: ");
        scanf("%d", &estoque[indice].minima);
        break;
    case 4: //* editar preço
        printf("novo preço: r$");
        scanf("%f", &estoque[indice].preco);
        break;
    case 5: //* manter todos os valores
        printf("nenhuma alteração foi feita.\n");
        break;
    default:
        printf("opção inválida. nenhuma alteração foi feita.\n");
        break;
    }
    printf("bebida editada com sucesso!\n");
}

//! função para remover uma bebida do estoque
void RemoverBebida(int indice)
{
    //* desloca as bebidas após a removida para preencher o espaço
    for (int i = indice; i < total_bebidas - 1; i++)
    {
        estoque[i] = estoque[i + 1];
    }
    total_bebidas--;
    estoque = realloc(estoque, total_bebidas * sizeof(bebida));
    if (total_bebidas > 0 && !estoque)
    {
        printf("erro ao alocar memória.\n");
        exit(1);
    }
    printf("bebida removida com sucesso!\n");
}

//! fun��o para verificar o estoque com base na quantidade mínima
void VerificarEstoque()
{
    printf("\n======================= VERIFICAÇÃO DE ESTOQUE =======================\n");
    for (int i = 0; i < total_bebidas; i++)
    {
        //*alerta se a quantidade estiver abaixo do mínimo
        if (estoque[i].quantidade < estoque[i].minima)
        {
            printf("**ALERTA**: a quantidade de %s está abaixo do mínimo (%d). quantidade atual: %d\n",
                   estoque[i].nome, estoque[i].minima, estoque[i].quantidade);
        }
        else if (estoque[i].quantidade == estoque[i].minima)
        {
            printf("**ALERTA**: a quantidade de %s atingiu o limite mínimo (%d). quantidade atual: %d\n",
                   estoque[i].nome, estoque[i].minima, estoque[i].quantidade);
        }
    }
    printf("========================================================================\n");
}

//! fun��o para cadastrar uma nova caixa de bebida
void CadastrarCaixa(int unidades_por_caixa, int quantidade_caixas, float preco_caixa)
{
    estoque_caixas = realloc(estoque_caixas, (total_caixas + 1) * sizeof(caixa));
    if (estoque_caixas == NULL)
    {
        printf("erro ao alocar memória para caixas.\n");
        exit(1);
    }

    //*registra a caixa sem associar a bebida
    estoque_caixas[total_caixas].unidades_por_caixa = unidades_por_caixa;
    estoque_caixas[total_caixas].quantidade_caixas = quantidade_caixas;
    estoque_caixas[total_caixas].preco_caixa = preco_caixa; //* atribui o preço da caixa
    total_caixas++;

    printf("caixa cadastrada: %d caixas com %d unidades por caixa. preço por caixa: r$%.2f\n",
           quantidade_caixas, unidades_por_caixa, preco_caixa);
}

//! fun��o para exibir o estoque de caixas
void ExibirEstoqueCx()
{
    printf("\n======================= ESTOQUE ATUAL DE CAIXAS =======================\n");
    for (int i = 0; i < total_caixas; i++)
    {
        printf("[%d] unidades por caixa: %d | quantidade de caixas: %d | preço por caixa: r$%.2f\n",
               i + 1, estoque_caixas[i].unidades_por_caixa, estoque_caixas[i].quantidade_caixas, estoque_caixas[i].preco_caixa);
    }
    printf("=======================================================================\n");
}

//! fun��o para editar os dados de uma caixa
void EditarCaixa(int indice)
{
    printf("\n======================= EDITAR CAIXA =======================\n");
    printf("caixa: %d unidades por caixa: %d | quantidade de caixas: %d | preço por caixa: r$%.2f\n",
           estoque_caixas[indice].unidades_por_caixa, estoque_caixas[indice].quantidade_caixas, estoque_caixas[indice].preco_caixa);

    printf("1. unidades por caixa\n");
    printf("2. quantidade de caixas\n");
    printf("3. preço por caixa\n");
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
    case 3: //* editar preço por caixa
        printf("novo preço por caixa: r$");
        scanf("%f", &estoque_caixas[indice].preco_caixa);
        break;
    case 4: //* manter todos os valores
        printf("nenhuma alteração foi feita.\n");
        break;
    default:
        printf("opção inválida. nenhuma alteração foi feita.\n");
        break;
    }
    printf("caixa editada com sucesso!\n");
}

//! fun��o para registrar vendas
void RegistrarVenda(int i, int quantidade, int metodo_pagamento)
{
    //* Obt�m o hor�rio atual
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    char horario[20];
    strftime(horario, sizeof(horario), "%Y-%m-%d %H:%M:%S", tm_info); //* Hor�rio completo (ano-m�s-dia hora:minuto:segundo)

    //* Verifica se a bebida existe no estoque
    for (int i = 0; i < total_bebidas; i++)
    {
        if (estoque[i].quantidade < quantidade)
        {
            printf("Erro: quantidade de %s insuficiente no estoque. Dispon�vel: %d.\n", estoque[i].nome, estoque[i].quantidade);
            return; //* Se a quantidade for insuficiente, retorna sem registrar a venda
        }

        //* Aloca mem�ria para registrar a venda
        vendas = realloc(vendas, (total_vendas + 1) * sizeof(venda));
        if (!vendas)
        {
            printf("Erro ao alocar mem�ria.\n");
            exit(1);
        }

        //! Calcula o pre�o final da venda
        float precofinal = quantidade * estoque[i].preco;

        //* Registra a venda conforme o m�todo de pagamento
        if (metodo_pagamento == 0)
        { // Dinheiro
            int metd = 0;
            printf("Digite (0) para dinheiro e (1) para pix: ");
            scanf("%d", &metd);
            if (metd == 0)
            {
                printf("\nA op��o dinheiro foi selecionada");
                printf("\nA venda ser� registrada...");
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (dinheiro): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal);
            }
            else if (metd == 1)
            { // Pix
                printf("\nA op��o pix foi selecionada");
                printf("\nA venda ser� registrada...");
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (pix): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal);
            }
        }
        //! Cart�o
        else if (metodo_pagamento == 1)
        {
            int metd2;
            printf("Digite (0) para d�bito e (1) para cr�dito: ");
            scanf("%d", &metd2);

            //! D�bito
            if (metd2 == 0)
            {
                printf("\nA op��o d�bito foi selecionada");
                printf("\nA venda ser� registrada...");
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (d�bito): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal);
            }
            //! Cr�dito
            else if (metd2 == 1)
            {
                float taxa = 0.0;
                printf("\nA op��o cr�dito foi selecionada");
                printf("\nDigite a taxa a ser aplicada sobre o valor total: ");
                scanf("%f", &taxa);
                strcpy(vendas[total_vendas].nome, estoque[i].nome);
                vendas[total_vendas].quantidade = quantidade;
                vendas[total_vendas].preco = estoque[i].preco;
                strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info); //* Formata a data para o formato "YYYY-MM-DD"
                printf("\nVenda registrada (cr�dito): %d unidades de %s a R$%.2f cada. Total: R$%.2f\n",
                       quantidade, estoque[i].nome, estoque[i].preco, precofinal + (precofinal * taxa / 100));
            }
        }
        else if (metodo_pagamento == 2)
        { //! Fiado
            printf("Pagamento Fiado\n");
        }

        strftime(vendas[total_vendas].data, sizeof(vendas[total_vendas].data), "%d-%m-%Y", tm_info);
        printf("Hor�rio da venda: %s\n", vendas[total_vendas].data);

        //* Atualiza o estoque
        estoque[i].quantidade -= quantidade;
        total_vendas++; //* Incrementa o contador de vendas
    }
}

//! fun��o para gerar relat�rio
void gerar_relatorio(int tipo_relatorio)
{
    float total_vendas_periodo = 0.0;
    int vendas_por_periodo = 0;

    //* Vari�veis para data
    char data_atual[20];
    time_t t = time(NULL);
    struct tm tm_info;
    localtime_r(&t, &tm_info); //* Obt�m a hora local

    //* Formata a data atual como "YYYY-MM-DD"
    strftime(data_atual, sizeof(data_atual), "%d-%m-%Y", &tm_info);

    //* Relat�rio Di�rio - N�o solicita data, busca as vendas do dia atual
    if (tipo_relatorio == 1)
    {
        printf("\n===== Relat�rio Di�rio =====\n");
        printf("Vendas realizadas no dia: %s\n", data_atual);

        for (int i = 0; i < total_vendas; i++)
        {
            //* Verifica se a venda foi realizada no mesmo dia
            if (strcmp(vendas[i].data, data_atual) == 0)
            {
                vendas_por_periodo++;
                total_vendas_periodo += vendas[i].quantidade * vendas[i].preco;
                printf("Venda: %s | Quantidade: %d | Pre�o unit�rio: R$%.2f | Total: R$%.2f\n",
                       vendas[i].nome, vendas[i].quantidade, vendas[i].preco,
                       vendas[i].quantidade * vendas[i].preco);
            }
        }
    }

    //* Relat�rio Mensal - Solicita o m�s e o ano (formato MM/AAAA)
    else if (tipo_relatorio == 2)
    {
        int mes, ano;
        printf("\n===== Relat�rio Mensal =====\n");
        printf("Digite o m�s e o ano (MM/AAAA): ");
        scanf("%d/%d", &mes, &ano);

        printf("Vendas realizadas no m�s %02d/%d:\n", mes, ano);

        for (int i = 0; i < total_vendas; i++)
        {
            struct tm data_venda_tm = {0};
            strptime(vendas[i].data, "%d-%m-%Y", &data_venda_tm); //* Converte a data para estrutura tm

            //* Verifica se o m�s e o ano da venda coincidem com o m�s/ano fornecido
            if (data_venda_tm.tm_year + 1900 == ano && data_venda_tm.tm_mon + 1 == mes)
            {
                vendas_por_periodo++;
                total_vendas_periodo += vendas[i].quantidade * vendas[i].preco;
                printf("Venda: %s | Quantidade: %d | Pre�o unit�rio: R$%.2f | Total: R$%.2f\n",
                       vendas[i].nome, vendas[i].quantidade, vendas[i].preco,
                       vendas[i].quantidade * vendas[i].preco);
            }
        }
    }

    //* Relat�rio Anual - Solicita o ano (formato AAAA)
    else if (tipo_relatorio == 3)
    {
        int ano;
        printf("\n===== Relat�rio Anual =====\n");
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
                printf("Venda: %s | Quantidade: %d | Pre�o unit�rio: R$%.2f | Total: R$%.2f\n",
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
        printf("\nNenhuma venda registrada para este per�odo.\n");
    }
}
//! fun��o para inicializar o array (vetor) de clientes

void inicializarClientes()
{
    clientes = (Cliente *)malloc(capacidadeClientes * sizeof(Cliente));
    if (clientes == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória!\n");
        exit(EXIT_FAILURE);
    }
}
//! fun��o para adicionar um cliente
void adicionarCliente()
{
    //* Verifica se é necessário aumentar a capacidade
    if (qtdClientes == capacidadeClientes)
    {
        capacidadeClientes *= 2; //* Dobra a capacidade
        Cliente *novosClientes = (Cliente *)realloc(clientes, capacidadeClientes * sizeof(Cliente));
        if (novosClientes == NULL)
        {
            fprintf(stderr, "Erro ao realocar memória!\n");
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

//! Função para Mostrar Clientes

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
//! Fun��o para editar clientes
void editarCliente(int id)
{
    /* printf("\n======================= EDITAR CLIENTE =======================\n");
    printf("bebida: %s\n", estoque[indice].nome);
    printf("1. nome (atual: %s)\n", estoque[indice].nome);
    printf("2. quantidade máxima (atual: %d)\n", estoque[indice].maxima);
    printf("3. quantidade mínima (atual: %d)\n", estoque[indice].minima);
    printf("4. preço (atual: r$%.2f)\n", estoque[indice].preco);
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
    case 2: //* editar quantidade máxima
         printf("novo telefone: ");
        char novo_nome[50];
        LerString(clientes[i].telefone, sizeof(clientes[i].telefone), stdin);
        strcpy(clientes[i].telefone, novo_telefone);
        break;
    case 3: //* editar quantidade mínima
        printf("nova quantidade mínima: ");
        scanf("%d", &estoque[indice].minima);
        break;
    case 4: //* editar preço
        printf("novo preço: r$");
        scanf("%f", &estoque[indice].preco);
        break;
    case 5: //* manter todos os valores
        printf("nenhuma alteração foi feita.\n");
        break;
    default:
        printf("opção inválida. nenhuma alteração foi feita.\n");
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
    printf("Cliente não encontrado.\n");
}

//! Fun��o para registrar divida do cliente
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
    printf("Cliente não encontrado.\n");
}

//* fun��o principal
int main()
{
    inicializarClientes();                                                //* Fun��o inicializadora de clientes
    int opcao;                                                            //* vari�vel para armazenar a opção do menu
    char nome[50];                                                        //* vari�vel para armazenar o nome da bebida
    int quantidade = 0, maxima, minima, metodo_pagamento = 0, indice = 0; //** vari�veis para quantidade, máxima , mínima e id do cliente
    float preco, preco_caixa;                                             //* vari�veis para preço da bebida e da caixa
    int unidades_por_caixa, quantidade_caixas;                            //* vari�veis para unidades e quantidade de caixas
    int id;
    float valor = 0; //* vari�veis para id e valor devedor do cliente

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
        printf("14. Gerar relat�rio\n");
        printf("15. Sair do sistema\n");
        printf("escolha uma opção (1-14): ");
        scanf("%d", &opcao);
        getchar(); //* limpa o buffer do teclado (limpa área da memória usada)

        switch (opcao)
        {
        case 1: //*registrar entrada de bebida
            printf("nome da bebida: ");
            LerString(nome, sizeof(nome));
            printf("quantidade: ");
            scanf("%d", &quantidade);
            printf("máxima: ");
            scanf("%d", &maxima);
            printf("mi�nima: ");
            scanf("%d", &minima);
            printf("preço: r$");
            scanf("%f", &preco);
            RegistrarEntrada(nome, quantidade, maxima, minima, preco);
            break;
        case 2: //* exibir estoque de bebidas
            ExibirEstoque();
            break;
        case 3: //* editar bebida
            ExibirEstoque();
            printf("escolha o número da bebida a ser editada: ");
            int num; //* vari�vel para armazenar o n�mero da bebida
            scanf("%d", &num);
            if (num < 1 || num > total_bebidas)
            {
                printf("n�mero inv�lido.\n");
            }
            else
            {
                EditarBebida(num - 1); //* chama a função para editar a bebida
            }
            break;
        case 4: //* remover bebida
            ExibirEstoque();
            printf("escolha o n�mero da bebida a ser removida: ");
            scanf("%d", &num);
            if (num < 1 || num > total_bebidas)
            {
                printf("número inválido.\n");
            }
            else
            {
                RemoverBebida(num - 1); //* chama a função para remover a bebida
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
            printf("preço por caixa: r$");
            scanf("%f", &preco_caixa);
            CadastrarCaixa(unidades_por_caixa, quantidade_caixas, preco_caixa);
            break;
        case 7: //* exibir estoque de caixas
            ExibirEstoqueCx();
            break;
        case 8: //* editar caixa
            ExibirEstoqueCx();
            printf("escolha o número da caixa a ser editada: ");
            scanf("%d", &num);
            if (num < 1 || num > total_caixas)
            {
                printf("número inválido.\n");
            }
            else
            {
                EditarCaixa(num - 1); //* chama a função para editar a caixa
            }
            break;
        case 9: //*Vender bebida
            ExibirEstoque();
            printf("Digite o indice da bebida");
            scanf("%d", &indice);
            printf("Quantidade: ");
            scanf("%d", &quantidade);
            printf("selecione o metodo de pagamento (0) para a vista e (1) para cartão");
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
        case 14: //* gerar relat�rios
            int tipo_relatorio;
            printf("Escolha o tipo de relat�rio:\n");
            printf("1. Relat�rio Di�rio\n");
            printf("2. Relat�rio Mensal\n");
            printf("3. Relat�rio Anual\n");
            printf("Digite o n�mero do tipo de relat�rio: ");
            scanf("%d", &tipo_relatorio);
            gerar_relatorio(tipo_relatorio); //* Gera o relat�rio com base no tipo
            break;
        case 15: //* sair do sistema
            printf("saindo do sistema...\n");
            break;
        default:
            printf("opção inv�lida. tente novamente.\n");
            break;
        }
    } while (opcao != 15); //! condi��o de parada do loop do menu principal

    //* libera mem�ria alocada nos estoques
    free(estoque);
    free(estoque_caixas);

    return 0;
}
// TODO: ORGANIZAR MENUS
// TODO: IMPLEMENTAR FUN��O PARA CALCULO DA RENDA LIQUIDA INCLUINDO TODAS DESPESAS


