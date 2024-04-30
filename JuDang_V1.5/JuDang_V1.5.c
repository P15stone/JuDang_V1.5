#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STOCKS 10// 시뮬레이션할 주식의 개수
#define NUM_DAYS 10// 시뮬레이션할 일 수

// 주식 구조체
typedef struct {
    char name[20];// 주식 이름
    double* price;// 일별 주식 가격을 가리키는 포인터
} Stock;

// 가상 주식 초기화
void initializeStocks(Stock* stocks) {
    printf("주식 종목과 초기 주식 가격을 입력하세요:\n");
    for (int i = 0; i < NUM_STOCKS; i++) {
        printf("주식 종목 %d: ", i + 1);
        gets_s(stocks[i].name, sizeof(stocks[i].name));
        stocks[i].price = (double*)malloc(NUM_DAYS * sizeof(double));
        if (stocks[i].price == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            exit(1);
        }
        printf("초기 주식 가격: ");
        for (int j = 0; j < NUM_DAYS; j++) {
            scanf_s("%lf", &stocks[i].price[j]);
        }
        while (getchar() != '\n'); // 버퍼 비우기
    }
}

// 몬테카를로 시뮬레이션을 통해 추천 주식 찾기
char** recommendStockMonteCarlo(Stock* stocks, int* num_recommendations) {
    double* total = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (total == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    double* average = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (average == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    // 각 주식의 총 가격 계산
    for (int i = 0; i < NUM_STOCKS; i++) {
        total[i] = 0.0;
        for (int j = 0; j < NUM_DAYS; j++) {
            total[i] += stocks[i].price[j];
        }
        average[i] = total[i] / NUM_DAYS;// 평균 가격 계산
    }

    // 평균 가격이 높은 상위 3개의 주식 추천
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    // 가격 순으로 주식 정렬
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

    // 상위 3개 추천
    for (int i = 0; i < 3; i++) {
        recommendations[i] = (char*)malloc((strlen(stocks[i].name) + 1) * sizeof(char));
        if (recommendations[i] == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            exit(1);
        }
        strcpy_s(recommendations[i], strlen(stocks[i].name) + 1, stocks[i].name);
    }

    *num_recommendations = 3;
    free(total);
    free(average);
    return recommendations;
}

// 기술적 분석을 사용하여 주식 추천
char** recommendStockTech(Stock* stocks, int* num_recommendations) {
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    // 기술적 분석을 통해 상승 추세인 주식 추천
    int count = 0;
    for (int i = 0; i < NUM_STOCKS; i++) {
        double trend = 0.0; // 주식 가격 흐름

        // 최근 3일간의 주식 가격 변화를 확인하여 흐름 계산
        for (int j = NUM_DAYS - 1; j >= NUM_DAYS - 3; j--) {
            trend += stocks[i].price[j] - stocks[i].price[j - 1];
        }

        // 최근 3일간의 주식 가격이 상승 했을 경우 해당 주식을 추천
        if (trend > 0 && count < 3) {
            recommendations[count] = (char*)malloc((strlen(stocks[i].name) + 1) * sizeof(char));
            if (recommendations[count] == NULL) {
                fprintf(stderr, "메모리 할당 실패\n");
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
    srand((unsigned int)time(NULL));// 난수 발생기 초기화

    printf("====================================\n");
    printf("주식 추천 프로그램 'JuDang'에 오신것을 환영합니다 ^^\n");
    printf("====================================\n\n");

    Stock* stocks = (Stock*)malloc(NUM_STOCKS * sizeof(Stock));// 주식 배열 동적 할당
    if (stocks == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        return 1;
    }

    initializeStocks(stocks);// 주식 초기화

    // 몬테카를로 방법으로 추천된 주식 출력
    printf("\n몬테카를로 방법으로 추천된 주식:\n");
    int num_recommendations;
    char** monte_carlo_recommendations = recommendStockMonteCarlo(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", monte_carlo_recommendations[i]);
        }
    }
    else {
        printf("없음\n");
    }
    for (int i = 0; i < num_recommendations; i++) {
        free(monte_carlo_recommendations[i]);
    }
    free(monte_carlo_recommendations);// 메모리 해제

    // 기술적 분석으로 추천된 주식 출력
    printf("\n기술적 분석으로 추천된 주식:\n");
    char** tech_recommendations = recommendStockTech(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", tech_recommendations[i]);
        }
    }
    else {
        printf("없음\n");
    }
    for (int i = 0; i < num_recommendations; i++) {
        free(tech_recommendations[i]);
    }
    free(tech_recommendations);// 메모리 해제

    // 메모리 해제
    for (int i = 0; i < NUM_STOCKS; i++) {
        free(stocks[i].price);
    }
    free(stocks);

    return 0;
}