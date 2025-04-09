#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_CENTROIDES 4
#define NUM_PONTOS 150

double gerar_random();

void calcular_centroide(double[][3], double[][2]);

int centroide_mais_proximo(double, double, double[][2]);

int main(void) {
    int repeticao = 10;
    char diretorio_arquivoPontos[256], diretorio_arquivoCentroide[256];
    char resultadoDiretorio[50], resultadoNome[50], comando[1500];
    double centroides[NUM_CENTROIDES][2];

    srand(time(NULL)); // semente do aleatório

    for (int i = 0; i < repeticao; i++) {
        int ultima = (i == repeticao - 1);

        // Arquivo dos centroides
        snprintf(diretorio_arquivoCentroide, sizeof(diretorio_arquivoCentroide),
                 "C:\\Users\\marco\\CLionProjects\\TrabalhoAlexDificil\\Resultados\\Centroides\\centroides_iteracao_%d.txt", i + 1);
        FILE *arqCentroide = fopen(diretorio_arquivoCentroide, "w");

        if (i == 0) {
            for (int h = 0; h < NUM_CENTROIDES; h++) {
                centroides[h][0] = gerar_random();
                centroides[h][1] = gerar_random();
                fprintf(arqCentroide, "%.5lf %.5lf\n", centroides[h][0], centroides[h][1]);
            }
        } else {
            char arqAnterior[256];
            snprintf(arqAnterior, sizeof(arqAnterior),
                     "C:\\Users\\marco\\CLionProjects\\TrabalhoAlexDificil\\Resultados\\PopulacaoAgrupadaIteracao\\populacao_agrupada_iteracao_%d.txt", i);
            FILE *arqLeitura = fopen(arqAnterior, "r");

            double pontos[NUM_PONTOS][3];
            for (int k = 0; k < NUM_PONTOS; k++)
                fscanf(arqLeitura, "%lf %lf %lf", &pontos[k][0], &pontos[k][1], &pontos[k][2]);
            fclose(arqLeitura);

            calcular_centroide(pontos, centroides);

            for (int h = 0; h < NUM_CENTROIDES; h++)
                fprintf(arqCentroide, "%.5lf %.5lf\n", centroides[h][0], centroides[h][1]);
        }
        fclose(arqCentroide);

        // Atribuição dos pontos aos centroides
        snprintf(diretorio_arquivoPontos, sizeof(diretorio_arquivoPontos),
                 "C:\\Users\\marco\\CLionProjects\\TrabalhoAlexDificil\\Resultados\\PopulacaoAgrupadaIteracao\\populacao_agrupada_iteracao_%d.txt", i + 1);
        FILE *arqPontos = fopen(diretorio_arquivoPontos, "w");

        if (i == 0) {
            for (int j = 0; j < NUM_PONTOS; j++) {
                double x = gerar_random(), y = gerar_random();
                int grupo = centroide_mais_proximo(x, y, centroides);
                fprintf(arqPontos, "%.5lf %.5lf %d\n", x, y, grupo);
            }
        } else {
            char arqAnterior[256];
            snprintf(arqAnterior, sizeof(arqAnterior),
                     "C:\\Users\\marco\\CLionProjects\\TrabalhoAlexDificil\\Resultados\\PopulacaoAgrupadaIteracao\\populacao_agrupada_iteracao_%d.txt", i);
            FILE *arqLeitura = fopen(arqAnterior, "r");
            for (int j = 0; j < NUM_PONTOS; j++) {
                double x, y;
                int g;
                fscanf(arqLeitura, "%lf %lf %d", &x, &y, &g);
                int novoGrupo = centroide_mais_proximo(x, y, centroides);
                fprintf(arqPontos, "%.5lf %.5lf %d\n", x, y, novoGrupo);
            }
            fclose(arqLeitura);
        }
        fclose(arqPontos);

        // Gráfico
        snprintf(resultadoDiretorio, sizeof(resultadoDiretorio),
                 ultima ? "agrupamento_final" : "interecao_%d", i + 1);
        snprintf(resultadoNome, sizeof(resultadoNome),
                 ultima ? "agrupamento final" : "interecao %d", i + 1);

        snprintf(comando, sizeof(comando),
                 "gnuplot -e \"\
                 set terminal png size 800,800; \
                 set output 'C:/Users/marco/CLionProjects/TrabalhoAlexDificil/Resultados/Grafico/%s.png'; \
                 set title '%s' font 'Arial,25'; \
                 set xlabel 'EIXO X'; set ylabel 'EIXO Y'; \
                 set xrange [0:100]; set yrange [0:100]; \
                 set key off; set grid; \
                 set palette defined(0 'red', 1 'green', 2 'blue', 3 'purple'); \
                 plot '%s' using 1:2:3 with points lc palette pt 7 ps 1.5, \
                      '%s' using 1:2 with points lc rgb 'black' pt 4 ps 2 \
                 \"",
                 resultadoDiretorio, resultadoNome,
                 diretorio_arquivoPontos, diretorio_arquivoCentroide);

        system(comando);
    }

    return 0;
}

double gerar_random() {
    return ((double)rand() / RAND_MAX) * 100.0;
}

void calcular_centroide(double pontos[NUM_PONTOS][3], double centroides[NUM_CENTROIDES][2]) {
    for (int h = 0; h < NUM_CENTROIDES; h++) {
        double somaX = 0, somaY = 0;
        int count = 0;
        for (int k = 0; k < NUM_PONTOS; k++) {
            if ((int)pontos[k][2] == h) {
                somaX += pontos[k][0];
                somaY += pontos[k][1];
                count++;
            }
        }
        if (count == 0) {
            centroides[h][0] = gerar_random();
            centroides[h][1] = gerar_random();
        } else {
            centroides[h][0] = somaX / count;
            centroides[h][1] = somaY / count;
        }
    }
}

int centroide_mais_proximo(double x, double y, double centroides[NUM_CENTROIDES][2]) {
    double menor_dist = 1e9;
    int indice = -1;
    for (int k = 0; k < NUM_CENTROIDES; k++) {
        double dx = x - centroides[k][0];
        double dy = y - centroides[k][1];
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < menor_dist) {
            menor_dist = dist;
            indice = k;
        }
    }
    return indice;
}
