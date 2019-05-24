#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define PRECO_ECONOMICO 89.90
#define PRECO_INTERMEDIARIO 174.90
#define PRECO_LUXO 289.90

#define FILE_CLIENTE "cliente.dat"
#define FILE_VEICULO "veiculo.dat"
#define FILE_LOCACAO "locacao.dat"
#define FILE_DEVOLUCAO "devolucao.dat"

#define MSG_ERRO_ARQUIVO "\nErro ao abrir o banco de dados. Reinicie o programa e tente novamente.\n"

#define ID_MIN 1000
#define ID_MAX 9999

typedef struct {
    int cliId;
    char nome[50];
    char dataNascimento[50];
    char rg[50];
    char cpf[50];
    char nacionalidade[50];
    char habilitacao[50];
    char validadeCnh[50];
} Cliente;

typedef struct {
    int veiId;
    char renavan[50];
    char marca[50];
    char modelo[50];
    char placa[50];
    int ano[50];
    char combustivel[50];
    char cor[50];
    int categoria;
} Veiculo;

typedef struct {
    int locId;
    Cliente cli;
    Veiculo vei;
    char dataLocacao[50];
    char dataDevolucao[50];
    int devolvido;
} Locacao;

typedef struct {
    int devId;
    Locacao loc;
    char dataDevolucao[50];
    int diasAtraso;
    float valorInicial;
    float valorMulta;
    float valorTotal;
} Devolucao;

typedef struct {
    int d, m, y;
} Date;

const int monthDays[12] = {31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31};

int countLeapYears(Date d) {
    int years = d.y;

    if (d.m <= 2)
        years--;

    return years / 4 - years / 100 + years / 400;
}

int charToInt(char data[], int qtd, int step) {
    char dia[50];
    for (int i = 0; i < 3; i++) {
        dia[i] = "";
    }
    strncpy(dia, &data[step], qtd);
    return atoi(dia);
}

int getDifferenceDays(char data1[], char data2[]) {

    int day1 = charToInt(data1, 2, 0);
    int mon1 = charToInt(data1, 2, 3);
    int year1 = charToInt(data1, 4, 6);

    int day2 = charToInt(data2, 2, 0);
    int mon2 = charToInt(data2, 2, 3);
    int year2 = charToInt(data2, 4, 6);

    Date dt1 = {day1, mon1, year1};
    Date dt2 = {day2, mon2, year2};

    long int n1 = dt1.y * 365 + dt1.d;

    for (int i = 0; i < dt1.m - 1; i++)
        n1 += monthDays[i];

    n1 += countLeapYears(dt1);

    long int n2 = dt2.y * 365 + dt2.d;
    for (int i = 0; i < dt2.m - 1; i++)
        n2 += monthDays[i];
    n2 += countLeapYears(dt2);

    return (n2 - n1);
}

void imprimeCabecalho(char *titulo) {
    system("cls||clear");
    printf("\n----------------------------------------------------------\n%40s\n----------------------------------------------------------\n\n", titulo);
}

int gerarId() {
    srand(time(NULL));
    return rand() % (ID_MAX - ID_MIN + 1) + ID_MIN;
}

void listarClientes() {
    Cliente c;
    FILE *infile;
    int opcao;

    imprimeCabecalho("LISTA DE CLIENTES");

    infile = fopen(FILE_CLIENTE, "r");
    if (infile == NULL) {
        fprintf(stderr, MSG_ERRO_ARQUIVO);
        exit(1);
    }

    printf("    ID %9s %13s", "Nasc.", "Nome\n\n");
    while (fread(&c, sizeof (Cliente), 1, infile)) {
        printf("    %d   %s   %s\n", c.cliId, c.dataNascimento, c.nome);
    }

    fclose(infile);

    printf("\nSelecione uma opção:\n1. Listar novamente\n2. Voltar\n\nOPÇÃO: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            listarClientes();
            break;
        case 2:
            clientes();
            break;
        default:
            printf("\nOpção inválida.\n");
            listarClientes();
            break;
    }
}

void cadastrarCliente() {
    Cliente c;
    FILE *outfile;

    imprimeCabecalho("CADASTRAR CLIENTE");

    printf("Nome: ");
    scanf(" %[^\n]", &c.nome);

    printf("Data de Nascimento (dd/mm/yyyy): ");
    scanf(" %[^\n]", &c.dataNascimento);

    printf("RG: ");
    scanf(" %[^\n]", &c.rg);

    printf("CPF: ");
    scanf(" %[^\n]", &c.cpf);

    printf("Nacionalidade (Ex.: BR): ");
    scanf(" %[^\n]", &c.nacionalidade);

    printf("Nº Habilitação: ");
    scanf(" %[^\n]", &c.habilitacao);

    printf("Validade CNH (dd/mm/yyyy): ");
    scanf(" %[^\n]", &c.validadeCnh);

    outfile = fopen(FILE_CLIENTE, "a");

    if (outfile == NULL) {
        fprintf(stderr, MSG_ERRO_ARQUIVO);
        exit(1);
    }

    c.cliId = gerarId();
    fwrite(&c, sizeof (Cliente), 1, outfile);

    if (fwrite != 0) {
        printf("\nCliente cadastrado com sucesso! Aguarde...\n");
    } else {
        printf("\nErro ao cadastrar o cliente. Reinicie o programa e tente novamente.\n");
    }

    fclose(outfile);

    sleep(2);
    clientes();

}

void clientes() {
    int opcao;

    imprimeCabecalho("CADASTRO DE CLIENTES");
    printf("Selecione uma opção:\n1. Listar clientes\n2. Cadastrar um cliente\n3. Voltar\n\nOPÇÃO: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            listarClientes();
            break;
        case 2:
            cadastrarCliente();
            break;
        case 3:
            inicio();
            break;
        default:
            printf("\nOpção inválida.\n");
            clientes();
            break;
    }
}

void listarVeiculos() {
    Veiculo c;
    FILE *infile;
    int opcao;

    imprimeCabecalho("LISTA DE VEÍCULOS");

    infile = fopen(FILE_VEICULO, "r");
    if (infile == NULL) {
        fprintf(stderr, MSG_ERRO_ARQUIVO);
        exit(1);
    }

    printf("    ID %9s %11s", "Placa", "Nome\n\n");
    while (fread(&c, sizeof (Veiculo), 1, infile)) {
        printf("    %d   %s   %s - %s\n", c.veiId, c.placa, c.marca, c.modelo);
    }

    fclose(infile);

    printf("\nSelecione uma opção:\n1. Listar novamente\n2. Voltar\n\nOPÇÃO: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            listarVeiculos();
            break;
        case 2:
            veiculos();
            break;
        default:
            printf("\nOpção inválida.\n");
            listarVeiculos();
            break;
    }
}

void cadastrarVeiculo() {
    Veiculo v;
    FILE *outfile;

    imprimeCabecalho("CADASTRAR VEÍCULO");

    printf("Marca: ");
    scanf(" %[^\n]", &v.marca);

    printf("Modelo: ");
    scanf(" %[^\n]", &v.modelo);

    printf("Ano: ");
    scanf(" %[^\n]", &v.ano);

    printf("Cor: ");
    scanf(" %[^\n]", &v.cor);

    printf("Combustível (A)lcool | (G)asolina | (F)lex: ");
    scanf(" %[^\n]", &v.combustivel);

    printf("Placa: ");
    scanf(" %[^\n]", &v.placa);

    printf("Renavan: ");
    scanf(" %[^\n]", &v.renavan);

    printf("Categoria (1) Econômico | (2) Intermediário | (3) Luxo: ");
    scanf("%d", &v.categoria);

    outfile = fopen(FILE_VEICULO, "a");

    if (outfile == NULL) {
        fprintf(stderr, MSG_ERRO_ARQUIVO);
        exit(1);
    }

    v.veiId = gerarId();
    fwrite(&v, sizeof (Veiculo), 1, outfile);

    if (fwrite != 0) {
        printf("\nVeículo cadastrado com sucesso! Aguarde...\n");
    } else {
        printf("\nErro ao cadastrar o veículo. Reinicie o programa e tente novamente.\n");
    }

    fclose(outfile);

    sleep(2);
    veiculos();

}

void veiculos() {
    int opcao;

    imprimeCabecalho("CADASTRO DE VEÍCULOS");
    printf("Selecione uma opção:\n1. Listar veículos\n2. Cadastrar um veículo\n3. Voltar\n\nOPÇÃO: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            listarVeiculos();
            break;
        case 2:
            cadastrarVeiculo();
            break;
        case 3:
            inicio();
            break;
        default:
            printf("\nOpção inválida.\n");
            veiculos();
            break;
    }
}

Cliente procurarCliente() {
    Cliente cliTemp;
    Cliente cliente;
    FILE *infile_cliente;
    char cpf[12];

    cliente.cliId = 0;

    while (cliente.cliId == 0) {
        printf("Busque um cliente por CPF: ");
        scanf(" %[^\n]", &cpf);

        infile_cliente = fopen(FILE_CLIENTE, "r");
        if (infile_cliente == NULL) {
            fprintf(stderr, MSG_ERRO_ARQUIVO);
            exit(1);
        }

        while (fread(&cliTemp, sizeof (Cliente), 1, infile_cliente)) {
            if (strcmp(cliTemp.cpf, cpf) == 0) {
                cliente = cliTemp;
            }
        }

        fclose(infile_cliente);

        if (cliente.cliId == 0) {
            printf("Cliente não encontrado! Tente novamente.\n\n");
        }
    }

    return cliente;
}

Veiculo procurarVeiculo() {
    Veiculo v;
    Veiculo veiTemp;
    Locacao locTemp;
    v.veiId = 0;
    FILE *infile;
    FILE *infile_locacao;
    Veiculo lista[50];
    Locacao listaLoc[50];
    char filtro[50];
    int count = 0;
    int i;

    while (count == 0) {
        strcpy(filtro, "");

        while (strcmp(filtro, "") == 0) {
            int opcao;

            printf("Selecione uma opção para buscar um veículo:\n1. Listar todos os carros disponíveis\n2. Listar todos os econômicos disponíveis\n3. Listar todos os intermediários disponíveis\n4. Listar todos os luxos disponíveis.\n\nOPÇÃO: ");
            scanf("%d", &opcao);

            switch (opcao) {
                case 1:
                    strcpy(filtro, "TODOS");
                    break;
                case 2:
                    strcpy(filtro, "ECONOMICO");
                    break;
                case 3:
                    strcpy(filtro, "INTERMEDIARIO");
                    break;
                case 4:
                    strcpy(filtro, "LUXO");
                    break;
                default:
                    printf("Opção inválida.\n\n");
                    break;
            }
        }

        infile = fopen(FILE_VEICULO, "r");
        infile_locacao = fopen(FILE_LOCACAO, "r");

        if (infile == NULL) {
            fprintf(stderr, MSG_ERRO_ARQUIVO);
            exit(1);
        }

        if (infile_locacao != NULL) {
            while (fread(&locTemp, sizeof (Locacao), 1, infile_locacao)) {
                listaLoc[count] = locTemp;
                count++;
            }
        }

        count = 0;
        while (fread(&veiTemp, sizeof (Veiculo), 1, infile)) {
            int indisponivel = 0;
            for (i = 0; strcmp(listaLoc[i].vei.marca, "") > 0; i++) {
                if (listaLoc[i].vei.veiId == veiTemp.veiId && listaLoc[i].devolvido == 0) {
                    indisponivel = 1;
                    break;
                }
            }

            if (indisponivel == 0) {
                if (strcmp(filtro, "TODOS") == 0) {
                    lista[count] = veiTemp;
                    count++;
                }

                if (strcmp(filtro, "ECONOMICO") == 0 && veiTemp.categoria == 1) {
                    lista[count] = veiTemp;
                    count++;
                }

                if (strcmp(filtro, "INTERMEDIARIO") == 0 && veiTemp.categoria == 2) {
                    lista[count] = veiTemp;
                    count++;
                }

                if (strcmp(filtro, "LUXO") == 0 && veiTemp.categoria == 3) {
                    lista[count] = veiTemp;
                    count++;
                }
            }
        }

        fclose(infile);
        fclose(infile_locacao);

        if (count == 0) {
            printf("\nNenhum veículo disponível!\n\n");
        }
    }

    while (v.veiId == 0) {
        int selecionado;

        for (i = 0; strcmp(lista[i].marca, "") > 0; i++) {
            printf("\n    %d   %s - %s", lista[i].veiId, lista[i].marca, lista[i].modelo);
        }

        printf("\n\nDigite o número do carro selecionado: ");
        scanf("%d", &selecionado);

        for (i = 0; strcmp(lista[i].marca, "") > 0; i++) {
            if (lista[i].veiId == selecionado) {
                v = lista[i];
            }
        }

        if (v.veiId == 0) {
            printf("Veículo não encontrado! Tente novamente.\n\n");
        }
    }

    return v;
}

void locar() {
    Locacao l;
    FILE *outfile;

    imprimeCabecalho("LOCAR VEÍCULO");

    l.cli = procurarCliente();
    printf("Cliente %s selecionado!\n\n", l.cli.nome);

    l.vei = procurarVeiculo();
    printf("Veículo %s %s selecionado!\n\n", l.vei.marca, l.vei.modelo);

    printf("Data da locação (dd/mm/yyyy): ");
    scanf(" %[^\n]", &l.dataLocacao);

    printf("Data da devolução (dd/mm/yyyy): ");
    scanf(" %[^\n]", &l.dataDevolucao);

    outfile = fopen(FILE_LOCACAO, "a");

    if (outfile == NULL) {
        fprintf(stderr, MSG_ERRO_ARQUIVO);
        exit(1);
    }

    l.locId = gerarId();
    l.devolvido = 0;

    fwrite(&l, sizeof (Locacao), 1, outfile);

    if (fwrite != 0) {
        printf("\nLocação cadastrada com sucesso! Aguarde...\n");
    } else {
        printf("\nErro ao cadastrar a locação. Reinicie o programa e tente novamente.\n");
    }

    fclose(outfile);

    sleep(2);
    inicio();
}

Locacao procurarLocacao() {
    Locacao locTemp;
    Locacao locacao;
    FILE *infile;
    char placa[10];

    locacao.locId = 0;

    while (locacao.locId == 0) {
        printf("Digite a placa do veículo: ");
        scanf("%s", &placa);

        infile = fopen(FILE_LOCACAO, "r");
        if (infile == NULL) {
            fprintf(stderr, MSG_ERRO_ARQUIVO);
            exit(1);
        }

        while (fread(&locTemp, sizeof (Locacao), 1, infile)) {
            if (strcmp(locTemp.vei.placa, placa) == 0 && locTemp.devolvido == 0) {
                locacao = locTemp;
            }
        }

        fclose(infile);

        if (locacao.locId == 0) {
            printf("Locação não encontrada! Tente novamente.\n\n");
        }
    }

    return locacao;
}

float getValorVeiculo(Veiculo v) {
    switch (v.categoria) {
        case 1:
            return PRECO_ECONOMICO;
            break;
        case 2:
            return PRECO_INTERMEDIARIO;
            break;
        case 3:
            return PRECO_LUXO;
            break;
        default:
            return 0;
            break;
    }

    return 0;
}

void devolver() {
    Devolucao d;
    Locacao l;
    Locacao locTemp;
    char dataDevolucao[12];
    int difDatas;
    int difExtra;
    int difExtraTemp;
    int opcao = 0;
    int index = 0;
    float valorInicial;
    float valorMulta;
    float valorTotal;
    FILE *outfile;
    FILE *outfile_locacao;

    imprimeCabecalho("DEVOLVER VEÍCULO");

    l = procurarLocacao();
    printf("Veículo %s %s alugado para %s no período de %s a %s encontrado!\n\n", l.vei.marca, l.vei.modelo, l.cli.nome, l.dataLocacao, l.dataDevolucao);

    printf("Data de devolução (dd/mm/yyyy): ");
    scanf("%s", &dataDevolucao);

    difDatas = getDifferenceDays(l.dataLocacao, l.dataDevolucao);
    difExtra = getDifferenceDays(l.dataDevolucao, dataDevolucao);

    valorInicial = difDatas * getValorVeiculo(l.vei);

    if (difExtra > 0) {
        valorMulta = (difExtra * getValorVeiculo(l.vei)) * 2;
        difExtraTemp = difExtra;
    } else {
        valorMulta = 0;
        difExtraTemp = 0;
    }

    valorTotal = valorInicial + valorMulta;

    printf("\nDias Locados: %d\n", difDatas);
    printf("Dias Extras: %d\n", difExtraTemp);

    printf("\nInicial: %7.2f\n", valorInicial);
    printf("Multas:  %7.2f\n", valorMulta);
    printf("TOTAL:   %7.2f\n", valorTotal);

    while (opcao == 0) {
        printf("\nSelecione uma opção:\n1. Confirmar devolução e pagamento\n2. Cancelar\n\nOPÇÃO: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                d.devId = gerarId();
                strcpy(d.dataDevolucao, dataDevolucao);
                d.loc = l;
                d.valorTotal = valorTotal;
                d.valorInicial = valorInicial;
                d.valorMulta = valorMulta;
                d.diasAtraso = difExtraTemp;

                outfile = fopen(FILE_DEVOLUCAO, "a");
                outfile_locacao = fopen(FILE_LOCACAO, "r+");

                if (outfile == NULL || outfile_locacao == NULL) {
                    fprintf(stderr, MSG_ERRO_ARQUIVO);
                    exit(1);
                }

                while (fread(&locTemp, sizeof (Locacao), 1, outfile_locacao)) {
                    if (locTemp.locId == l.locId) {
                        locTemp.devolvido = 1;
                        break;
                    } else {
                        index++;
                    }
                }

                fseek(outfile_locacao, index * sizeof (Locacao), SEEK_SET);
                fwrite(&locTemp, sizeof (Locacao), 1, outfile_locacao);
                fwrite(&d, sizeof (Devolucao), 1, outfile);

                if (fwrite != 0) {
                    printf("\nVeículo devolvido com sucesso! Aguarde...\n");
                } else {
                    printf("\nErro ao devolver o veículo. Reinicie o programa e tente novamente.\n");
                }

                fclose(outfile);
                fclose(outfile_locacao);

                sleep(2);
                inicio();
                break;
            case 2:
                inicio();
                break;
            default:
                opcao = 0;
                printf("Opção inválida!\n\n");
                break;
        }
    }
}

void inicio() {
    int opcao;

    imprimeCabecalho("LOCADORA DE VEÍCULOS");
    printf("Selecione uma opção:\n1. Cadastro de Clientes\n2. Cadastro de Veículos\n3. Locar\n4. Devolver\n5. Sair\n\nOPÇÃO: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            clientes();
            break;
        case 2:
            veiculos();
            break;
        case 3:
            locar();
            break;
        case 4:
            devolver();
            break;
        case 5:
            exit(1);
            break;
        default:
            printf("\nOpção inválida.");
            inicio();
            break;
    }
}

int main() {
    inicio();
}

