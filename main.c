#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void OrdenaVetor(int window[]) {// sÑ ordena o vetor passado
    int temp, i , j;
    for(i = 0; i < 9 ; i++) {
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--) {
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

void mediana(int linhas, int colunas, int matriz[linhas][colunas], int matrizAux[linhas][colunas]) {
    int i,j;
    int mediana;

    int e,f;
    for (e=0; e<linhas; e++) {
        for (f=0; f<colunas; f++)
            matrizAux[e][f]=matriz[e][f];
    }

    for (i=0; i<linhas-2; i++) { // NAO PODE IR ATE O FINAL, SE FOR O QUADRADO SAI DOS LIMITES DA MATRIZ
        for (j=0; j<colunas-2; j++) { // NAO PODE IR ATE O FINAL, SE FOR O QUADRADO SAI DOS LIMITES DA MATRIZ

            int VetorParaOrdenar[9] = { // COLOCA OS DADOS DA MATRIZ 3X3 NO VETOR
                matrizAux[i][j], matrizAux[i][j+1], matrizAux[i][j+2],
                matrizAux[i+1][j],matrizAux[i+1][j+1],matrizAux[i+1][j+2],
                matrizAux[i+2][j], matrizAux[i+2][j+1], matrizAux[i+2][j+2]
            };
            // PARA PERCORRER A MATRIZ E CAPTURAR O LOCAL DA MEDIANA USEI A matrizAux
            // PORQUE SE EU MODIFICAR A MATRIZ PRINCIPAL ACHANDO A MEDIANA E ALTERANDO O VALOR

            // NA PROXIMA VEZ QUE NOS PASSARMOS E A MEDIANA JA ESTIVER ALTERADA VAI DAR ERRO NO VALOR DA MEDIANA
            // ASSIM FIZ AS OPERACOES NA MATRIZAUX E USEI A MATRIZ PRINCIAPL SO PARA INSERIR A MEDIANA NO LOCAL CERTO

            OrdenaVetor(VetorParaOrdenar); // ORDENA MATRIZ 3X3

            mediana = VetorParaOrdenar[4]; // CAPTURA NO VETOR ORDENADO A MEDIANA
            matriz[i+1][j+1] = mediana; // COLOCA NA MATRIZ PRINCIPAL O VALOR DA MEDIANA
        }
    }
}

int lerArquivo(int comprimentoLinha, int linhas, int colunas, int matriz[linhas][colunas]) {
    int linhaAtual = -1/*por que a primeira linha do arquivo é as dimensões*/, colunaAtual = 0; // Posições da matriz para os valores de colorGrayScale serem gravados.
    int index;
    int colorGrayScale; // Vetor para armazenar valores de 0-255
    bool firstLine = true;

    FILE *arq;
    char Linha[comprimentoLinha]; //
    char *result;

    system("clear");

    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("Ruido1.txt", "rt");

    if (arq == NULL) { // Se houve erro na abertura
        printf("Problemas na abertura do arquivo\n");
        return -1;
    }

    while (!feof(arq)) {
        // Lê uma linha (inclusive com o '\n')
        result = fgets(Linha, comprimentoLinha, arq);  // o 'fgets' lê até 1536 caracteres ou até o '\n'
        if (result) {  // Se foi possível ler

            colunaAtual = 0;
            if(firstLine == false && linhaAtual < linhas) {
                //printf("Linha %d: %s",linhaAtual, Linha); // Imprime o conteúdo da linha
                char charToInt[5]; // Cria string de 5 posições

                for(index = 0; index < comprimentoLinha; index = index+4) {
                    int pos;
                    for(pos = 1; pos<=3; pos++) {
                        charToInt[pos] = Linha[index+pos]; // Adicionamos o valor que está em index no meio da string
                    }

                    charToInt[0] = '+'; // Declaramos o valor positivo na string
                    charToInt[4] = '\0'; // Indicador de final de string

                    for(pos = 1; pos <=3; pos++)
                        if(charToInt[pos] < '0' || charToInt[pos] > '9') // Se acontecer de alguma posição que era pra ter valor mas possui espaõ em branco...
                            charToInt[pos] = '0'; // Então dizemos que essa posição é 0 para facilitar a conversão depois

                    if(colunaAtual < colunas) { // Para evitar que uma coluna 385 seja considerada na atribuição se alguma linha for maior
                        sscanf(charToInt, "%d", &colorGrayScale); // Convertemos de string para int
                        matriz[linhaAtual][colunaAtual] = colorGrayScale; // A posição da matriz recebe a cor
                        //printf("(%3d,%3d) = String: %s; colorGrayScale: %3d; 2DArray: %3d\n", linhaAtual, colunaAtual, charToInt, colorGrayScale, matriz[linhaAtual][colunaAtual]);
                        colunaAtual++;
                    }
                }
            }
            else {
                firstLine = false;
            }
        }
        linhaAtual++;
    }
    fclose(arq);
    //int i = 128, j = 383;
    //printf("2DArray(%3d,%3d) = %3d\n", i, j, matriz[i][j]);
    return 0;
}

int gravarArquivo(int comprimentoLinha, int linhas, int colunas, int matriz[linhas][colunas]) {
    char Str[(comprimentoLinha+5)*(linhas+1)];
    FILE *arq;
    char *result;

    arq = fopen("SEMRuido1.txt", "wt");  // Cria um arquivo texto para gravação
    if (arq == NULL) // Se não conseguiu criar
    {
       printf("Problemas na CRIACAO do arquivo\n");
       return -1;
    }
    //RESOLUÇÃO
    int lastPosStr = 0;
    Str[0] = '3';
    Str[1] = '8';
    Str[2] = '4';
    Str[3] = ' ';
    Str[4] = '2';
    Str[5] = '5';
    Str[6] = '6';
    Str[7] = '\n';

    lastPosStr = 8; // Atualizar a última posição não preenchida

    int i, j;
    for(i = 0; i<linhas; i++) {
        for(j = 0; j<colunas; j++) {
            int valor = matriz[i][j];
            char s[4];
            sprintf(s,"%4d", valor); // Formata o valor para ficar em 4 digitos
            //printf("%s", s);
            Str[lastPosStr] = s[0];
            lastPosStr++;
            Str[lastPosStr] = s[1];
            lastPosStr++;
            Str[lastPosStr] = s[2];
            lastPosStr++;
            Str[lastPosStr] = s[3];
            lastPosStr++;
        }
        Str[lastPosStr] = '\n';
        lastPosStr++;
        printf("\n");
    }

    result = fputs(Str, arq);
    if (result == EOF)
        printf("Erro na Gravacao\n");

    fclose(arq);
    return 0;
}

int main()
{
    int linhas = 256, colunas = 384; // Dimensões da matriz e das imagens a serem testadas.
    int comprimentoLinha = 1540; // Número de caracteres máximo dentro de cada linha do arquivo (4 digitos * 384 colunas).
    int matriz[linhas][colunas];
    int matrizAux[linhas][colunas];

    /** ==================================================================
     *   LEITURA DE ARQUIVO E GRAVAÇÃO DE DADOS NA MATRIZ
     *  ==================================================================
     */
    printf("\nINICIADA A LEITURA DO ARQUIVO E GRAVAÇÃO DE DADOS NA MATRIZ\n\n");
    lerArquivo(comprimentoLinha, linhas, colunas, matriz);
    printf("\nFINALIZADA A LEITURA DO ARQUIVO E GRAVAÇÃO DE DADOS NA MATRIZ\n\n");

    /** ==================================================================
     *   APLICAÇÃO DO FILTRA NA MEDIANA NA MATRIZ AUXILIAR
     *  ==================================================================
     */
    printf("\nINICIADA A APLICAÇÃO DO FILTRA NA MEDIANA NA MATRIZ AUXILIAR\n\n");
    mediana(linhas, colunas, matriz, matrizAux);
    printf("\nFINALIZADA A APLICAÇÃO DO FILTRA NA MEDIANA NA MATRIZ AUXILIAR\n\n");

    /** ==================================================================
     *   GRAVAÇÃO DOS DADOS DA MATRIZ NUM ARQUIVO
     *  ==================================================================
     */
    printf("\nINICIADA A GRAVAÇÃO DOS DADOS DA MATRIZ AUXILIAR NUM ARQUIVO\n\n");
    gravarArquivo(comprimentoLinha, linhas, colunas, matriz);
    printf("\nFINALIZADA A GRAVAÇÃO DOS DADOS DA MATRIZ NUM ARQUIVO\n\n");

    return 0;
}
