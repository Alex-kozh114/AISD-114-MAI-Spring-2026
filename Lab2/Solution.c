#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>

typedef enum {
    RU = 1,
    US,
    MU,
    EG,
    IL,
    TR,
    AE
} LocationCode;

// Структура узла 
typedef struct TreeEntry {
    LocationCode pointId;
    struct TreeEntry *childLeft;
    struct TreeEntry *childRight;
} TreeEntry;

const char* getLocName(LocationCode code) {
    switch(code) {
        case RU: return "Russia";
        case US: return "USA";
        case MU: return "Mauritius";
        case EG: return "Egypt";
        case IL: return "Israel";
        case TR: return "Turkey";
        case AE: return "UAE";
        default: return "Unknown";
    }
}

// Создание нового элемента с выделением памяти
TreeEntry* makeEntry(LocationCode val) {
    TreeEntry* entry = (TreeEntry*)malloc(sizeof(TreeEntry));
    if (!entry) return NULL;
    entry->pointId = val;
    entry->childLeft = NULL;
    entry->childRight = NULL;
    return entry;
}

TreeEntry* insertEntry(TreeEntry* root, LocationCode val) {
    if (root == NULL) return makeEntry(val);
    if (val < root->pointId)
        root->childLeft = insertEntry(root->childLeft, val);
    else if (val > root->pointId)
        root->childRight = insertEntry(root->childRight, val);
    return root;
}

// Поиск самого левого узла 
TreeEntry* getLeftmost(TreeEntry* root) {
    while (root && root->childLeft) root = root->childLeft;
    return root;
}

// Удаление узла с освобождением памяти
TreeEntry* removeEntry(TreeEntry* root, LocationCode val) {
    if (!root) return NULL;
    if (val < root->pointId)
        root->childLeft = removeEntry(root->childLeft, val);
    else if (val > root->pointId)
        root->childRight = removeEntry(root->childRight, val);
    else {
        if (!root->childLeft) {
            TreeEntry* tmp = root->childRight;
            free(root);
            return tmp;
        } else if (!root->childRight) {
            TreeEntry* tmp = root->childLeft;
            free(root);
            return tmp;
        }
        TreeEntry* tmp = getLeftmost(root->childRight);
        root->pointId = tmp->pointId;
        root->childRight = removeEntry(root->childRight, tmp->pointId);
    }
    return root;
}

void showTree(TreeEntry* root, int depth) {
    if (!root) return;
    showTree(root->childRight, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%s\n", getLocName(root->pointId));
    showTree(root->childLeft, depth + 1);
}

bool validateLeaves(TreeEntry* root, int lower, int upper) {
    if (!root) return true;
    
    // Определение листа (терминальной вершины) [cite: 13]
    if (root->childLeft == NULL && root->childRight == NULL) {
        return (root->pointId >= lower && root->pointId <= upper);
    }
    
    return validateLeaves(root->childLeft, lower, upper) && 
           validateLeaves(root->childRight, lower, upper);
}

// Очистка памяти
void clearTree(TreeEntry* root) {
    if (!root) return;
    clearTree(root->childLeft);
    clearTree(root->childRight);
    free(root);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    TreeEntry* mainRoot = NULL;
    int userCmd, inputVal, minLimit, maxLimit;

    while (1) {
        printf("\n--- СИСТЕМА УПРАВЛЕНИЯ ДЕРЕВОМ ---\n");
        printf("1. Добавить локацию\n");
        printf("2. Построить схему (визуализация)\n");
        printf("3. Исключить локацию\n");
        printf("4. Проверить диапазон листьев (Задание 7)\n");
        printf("5. Выход\n");
        printf("Действие: ");
        
        if (scanf("%d", &userCmd) != 1) break;

        switch (userCmd) {
            case 1:
                printf("Коды: 1-RU, 2-US, 3-MU, 4-EG, 5-IL, 6-TR, 7-AE\nВведите код: ");
                scanf("%d", &inputVal);
                mainRoot = insertEntry(mainRoot, (LocationCode)inputVal);
                break;
            case 2:
                if (!mainRoot) printf("Дерево не сформировано.\n");
                else showTree(mainRoot, 0);
                break;
            case 3:
                printf("Укажите код для удаления: ");
                scanf("%d", &inputVal);
                mainRoot = removeEntry(mainRoot, (LocationCode)inputVal);
                break;
            case 4:
                if (!mainRoot) {
                    printf("Дерево пусто!\n");
                } else {
                    printf("Задайте границы диапазона через пробел: ");
                    scanf("%d %d", &minLimit, &maxLimit);
                    if (validateLeaves(mainRoot, minLimit, maxLimit))
                        printf("Результат: Все листья входят в диапазон.\n");
                    else
                        printf("Результат: Обнаружены листья вне диапазона.\n");
                }
                break;
            case 5:
                clearTree(mainRoot);
                return 0;
            default:
                printf("Команда не распознана.\n");
        }
    }
    return 0;
}
