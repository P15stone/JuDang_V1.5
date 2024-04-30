#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STOCKS 10// �ùķ��̼��� �ֽ��� ����
#define NUM_DAYS 10// �ùķ��̼��� �� ��

// �ֽ� ����ü
typedef struct {
    char name[20];// �ֽ� �̸�
    double* price;// �Ϻ� �ֽ� ������ ����Ű�� ������
} Stock;

// ���� �ֽ� �ʱ�ȭ
void initializeStocks(Stock* stocks) {
    printf("�ֽ� ����� �ʱ� �ֽ� ������ �Է��ϼ���:\n");
    for (int i = 0; i < NUM_STOCKS; i++) {
        printf("�ֽ� ���� %d: ", i + 1);
        gets_s(stocks[i].name, sizeof(stocks[i].name));
        stocks[i].price = (double*)malloc(NUM_DAYS * sizeof(double));
        if (stocks[i].price == NULL) {
            fprintf(stderr, "�޸� �Ҵ� ����\n");
            exit(1);
        }
        printf("�ʱ� �ֽ� ����: ");
        for (int j = 0; j < NUM_DAYS; j++) {
            scanf_s("%lf", &stocks[i].price[j]);
        }
        while (getchar() != '\n'); // ���� ����
    }
}

// ����ī���� �ùķ��̼��� ���� ��õ �ֽ� ã��
char** recommendStockMonteCarlo(Stock* stocks, int* num_recommendations) {
    double* total = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (total == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }
    double* average = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (average == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }

    // �� �ֽ��� �� ���� ���
    for (int i = 0; i < NUM_STOCKS; i++) {
        total[i] = 0.0;
        for (int j = 0; j < NUM_DAYS; j++) {
            total[i] += stocks[i].price[j];
        }
        average[i] = total[i] / NUM_DAYS;// ��� ���� ���
    }

    // ��� ������ ���� ���� 3���� �ֽ� ��õ
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }

    // ���� ������ �ֽ� ����
    for (int i = 0; i < NUM_STOCKS - 1; i++) {
        for (int j = 0; j < NUM_STOCKS - i - 1; j++) {
            if (average[j] < average[j + 1]) {
                double temp = average[j];
                average[j] = average[j + 1];
                average[j + 1] = temp;
                char temp_name[20];
                strcpy_s(temp_name, sizeof(temp_name), stocks[j].name);
                strcpy_s(stocks[j].name, sizeof(stocks[j].name), stocks[j + 1].name);
                strcpy_s(stocks[j + 1].name, sizeof(stocks[j + 1].name), temp_name);
            }
        }
    }

    // ���� 3�� ��õ
    for (int i = 0; i < 3; i++) {
        recommendations[i] = (char*)malloc((strlen(stocks[i].name) + 1) * sizeof(char));
        if (recommendations[i] == NULL) {
            fprintf(stderr, "�޸� �Ҵ� ����\n");
            exit(1);
        }
        strcpy_s(recommendations[i], strlen(stocks[i].name) + 1, stocks[i].name);
    }

    *num_recommendations = 3;
    free(total);
    free(average);
    return recommendations;
}

// ����� �м��� ����Ͽ� �ֽ� ��õ
char** recommendStockTech(Stock* stocks, int* num_recommendations) {
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }

    // ����� �м��� ���� ��� �߼��� �ֽ� ��õ
    int count = 0;
    for (int i = 0; i < NUM_STOCKS; i++) {
        double trend = 0.0; // �ֽ� ���� �帧

        // �ֱ� 3�ϰ��� �ֽ� ���� ��ȭ�� Ȯ���Ͽ� �帧 ���
        for (int j = NUM_DAYS - 1; j >= NUM_DAYS - 3; j--) {
            trend += stocks[i].price[j] - stocks[i].price[j - 1];
        }

        // �ֱ� 3�ϰ��� �ֽ� ������ ��� ���� ��� �ش� �ֽ��� ��õ
        if (trend > 0 && count < 3) {
            recommendations[count] = (char*)malloc((strlen(stocks[i].name) + 1) * sizeof(char));
            if (recommendations[count] == NULL) {
                fprintf(stderr, "�޸� �Ҵ� ����\n");
                exit(1);
            }
            strcpy_s(recommendations[count], strlen(stocks[i].name) + 1, stocks[i].name);
            count++;
        }
    }

    *num_recommendations = count;
    return recommendations;
}

int main() {
    srand((unsigned int)time(NULL));// ���� �߻��� �ʱ�ȭ

    printf("====================================\n");
    printf("�ֽ� ��õ ���α׷� 'JuDang'�� ���Ű��� ȯ���մϴ� ^^\n");
    printf("====================================\n\n");

    Stock* stocks = (Stock*)malloc(NUM_STOCKS * sizeof(Stock));// �ֽ� �迭 ���� �Ҵ�
    if (stocks == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        return 1;
    }

    initializeStocks(stocks);// �ֽ� �ʱ�ȭ

    // ����ī���� ������� ��õ�� �ֽ� ���
    printf("\n����ī���� ������� ��õ�� �ֽ�:\n");
    int num_recommendations;
    char** monte_carlo_recommendations = recommendStockMonteCarlo(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", monte_carlo_recommendations[i]);
        }
    }
    else {
        printf("����\n");
    }
    for (int i = 0; i < num_recommendations; i++) {
        free(monte_carlo_recommendations[i]);
    }
    free(monte_carlo_recommendations);// �޸� ����

    // ����� �м����� ��õ�� �ֽ� ���
    printf("\n����� �м����� ��õ�� �ֽ�:\n");
    char** tech_recommendations = recommendStockTech(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", tech_recommendations[i]);
        }
    }
    else {
        printf("����\n");
    }
    for (int i = 0; i < num_recommendations; i++) {
        free(tech_recommendations[i]);
    }
    free(tech_recommendations);// �޸� ����

    // �޸� ����
    for (int i = 0; i < NUM_STOCKS; i++) {
        free(stocks[i].price);
    }
    free(stocks);

    return 0;
}