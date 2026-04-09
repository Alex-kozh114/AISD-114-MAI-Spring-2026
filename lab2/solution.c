#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>

// Перечисление типов самолетов 
typedef enum {
    CESSNA = 1,      // Легкомоторный самолет
    BEECHCRAFT,      // Частный самолет
    ANTONOV,         // Транспортный самолет
    SUKHOI,          // Истребитель 
    AIRBUS,          // Пассажирский средний самолет
    BOEING,          // Пассажирский тяжелый самолет
    CONCORDE         // Сверхзвуковой самолет
} AircraftType;

// Структура узла двоичного дерева 
typedef struct AirNode {
    AircraftType model;
    struct AirNode *leftWing;
    struct AirNode *rightWing;
} AirNode;

const char* getPlaneName(AircraftType t) {
    switch(t) {
        case  AIRBUS:     return "Airbus";
        case BEECHCRAFT: return "Beechcraft";
        case ANTONOV:    return "Antonov";
        case SUKHOI:     return "Sukhoi";
        case: CESSNA:     return "Cessna";
        case BOEING:     return "Boeing";
        case CONCORDE:   return "Concorde";
        default:         return "Unknown Plane";
    }
}

// Создание нового узла
AirNode* createAirNode(AircraftType val) {
    AirNode* node = (AirNode*)malloc(sizeof(AirNode));
    if (!node) return NULL;
    node->model = val;
    node->leftWing = NULL;
    node->rightWing = NULL;
    return node;
}

// Добавляем узел и сохраняем порядок
AirNode* insertPlane(AirNode* root, AircraftType val) {
    if (!root) return createAirNode(val);
    if (val < root->model)
        root->leftWing = insertPlane(root->leftWing, val);
    else if (val > root->model)
        root->rightWing = insertPlane(root->rightWing, val);
    return root;
}

// Поиск минимального узла для удаления [cite: 9]
AirNode* findMinPlane(AirNode* root) {
    while (root && root->leftWing) root = root->leftWing;
    return root;
}

// Удаление узла с освобождением памяти
AirNode* removePlane(AirNode* root, AircraftType val) {
    if (!root) return NULL;
    if (val < root->model)
        root->leftWing = removePlane(root->leftWing, val);
    else if (val > root->model)
        root->rightWing = removePlane(root->rightWing, val);
    else {
        if (!root->leftWing) {
            AirNode* temp = root->rightWing;
            free(root);
            return temp;
        } else if (!root->rightWing) {
            AirNode* temp = root->leftWing;
            free(root);
            return temp;
        }
        AirNode* temp = findMinPlane(root->rightWing);
        root->model = temp->model;
        root->rightWing = removePlane(root->rightWing, temp->model);
    }
    return root;
}

// Визуализация дерева с отступами 
void printAirTree(AirNode* root, int depth) {
    if (!root) return;
    printAirTree(root->rightWing, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%s\n", getPlaneName(root->model));
    printAirTree(root->leftWing, depth + 1);
}

bool checkPlanesInRange(AirNode* root, int minIdx, int maxIdx) {
    if (!root) return true;
    
    // Если узел является листом (терминальной вершиной) [cite: 13]
    if (!root->leftWing && !root->rightWing) {
        return (root->model >= minIdx && root->model <= maxIdx);
    }
    
    // Рекурсивно проверяем все ветви
    return checkPlanesInRange(root->leftWing, minIdx, maxIdx) && 
           checkPlanesInRange(root->rightWing, minIdx, maxIdx);
}

// Освобождение память
void clearAirTree(AirNode* root) {
    if (!root) return;
    clearAirTree(root->leftWing);
    clearAirTree(root->rightWing);
    free(root);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    AirNode* hangar = NULL;
    int choice, typeVal, rMin, rMax;

    while (1) {
        printf("\n--- АВИАДИСПЕТЧЕР (Задание 7) ---\n");
        printf("1. Добавить самолет в ангар\n");
        printf("2. Показать структуру (дерево)\n");
        printf("3. Удалить самолет\n");
        printf("4. Проверить листья на диапазон (Вариант 7)\n");
        printf("5. Выход\n");
        printf("Выбор: ");
        
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("1-Cessna, 2-Beechcraft, 3-Antonov, 4-Sukhoi, 5-Airbus, 6-Boeing, 7-Concorde\n");
                printf("Введите ID типа: ");
                scanf("%d", &typeVal);
                hangar = insertPlane(hangar, (AircraftType)typeVal);
                break;
            case 2:
                if (!hangar) printf("Ангар пуст.\n");
                else printAirTree(hangar, 0);
                break;
            case 3:
                printf("Введите ID для списания: ");
                scanf("%d", &typeVal);
                hangar = removePlane(hangar, (AircraftType)typeVal);
                break;
            case 4:
                if (!hangar) {
                    printf("Дерево не создано.\n");
                } else {
                    printf("Задайте диапазон ID (минимум и максимум): ");
                    scanf("%d %d", &rMin, &rMax);
                    if (checkPlanesInRange(hangar, rMin, rMax))
                        printf("РЕЗУЛЬТАТ: Все листья входят в диапазон.\n");
                    else
                        printf("РЕЗУЛЬТАТ: Найдены листья вне диапазона.\n");
                }
                break;
            case 5:
                clearAirTree(hangar);
                return 0;
            default:
                printf("Неверный пункт.\n");
        }
    }
    return 0;
}
