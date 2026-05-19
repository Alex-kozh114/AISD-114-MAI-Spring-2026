#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
    int key;
    int value;
} data_type;

typedef struct {
    data_type data[MAX_SIZE];
    int size;
} list_type;

void list_create(list_type *l) {
    l->size = 0;
}

void list_push_back(list_type *l, data_type elem) {
    if (l->size == MAX_SIZE) {
        printf("Список переполнен\n");
        return;
    }
    l->data[l->size] = elem;
    l->size++;
}

void list_print(const list_type *l) {
    int i;
    printf("Список (размер %d): ", l->size);
    for (i = 0; i < l->size; i++) {
        printf("(%d,%d) ", l->data[i].key, l->data[i].value);
    }
    printf("\n");
}

void list_concat(list_type *l1, const list_type *l2) {
    int i;
    for (i = 0; i < l2->size; i++) {
        if (l1->size >= MAX_SIZE) {
            printf("Предупреждение: при конкатенации первый список переполнился!\n");
            return;
        }
        l1->data[l1->size] = l2->data[i];
        l1->size++;
    }
}

// Вспомогательная рекурсивная функция для быстрой сортировки Хоара
void quicksort_helper(list_type *l, int low, int high) {
    if (low >= high) return;

    // Выбираем опорный элемент посередине отрезка
    int pivot_key = l->data[low + (high - low) / 2].key;
    int i = low;
    int j = high;

    // Разделение Хоара 
    while (i <= j) {
        while (l->data[i].key < pivot_key) i++;
        while (l->data[j].key > pivot_key) j--;

        if (i <= j) {
            data_type temp = l->data[i];
            l->data[i] = l->data[j];
            l->data[j] = temp;
            i++;
            j--;
        }
    }

    // Рекурсивные вызовы для левой и правой частей
    if (low < j)  quicksort_helper(l, low, j);
    if (i < high) quicksort_helper(l, i, high);
}

// Главная функция сортировки, принимающая только указатель на список
void list_sort(list_type *l) {
    if (l->size > 1) {
        quicksort_helper(l, 0, l->size - 1);
    }
}

int main() {
    list_type l1, l2;
    int n1, n2, i;

    list_create(&l1);
    list_create(&l2);

    printf("Введите количество элементов первого списка: ");
    scanf("%d", &n1);
    printf("Введите key и value для первого списка:\n");
    for (i = 0; i < n1; i++) {
        data_type temp;
        printf("Элемент %d: ", i + 1);
        scanf("%d %d", &temp.key, &temp.value);
        list_push_back(&l1, temp);
    }

    printf("\nВведите количество элементов второго списка: ");
    scanf("%d", &n2);
    printf("Введите key и value для второго списка:\n");
    for (i = 0; i < n2; i++) {
        data_type temp;
        printf("Элемент %d: ", i + 1);
        scanf("%d %d", &temp.key, &temp.value);
        list_push_back(&l2, temp);
    }

    printf("\n--- Исходные списки ---\n");
    printf("Первый "); list_print(&l1);
    printf("Второй "); list_print(&l2);

    printf("\nВыполняем конкатенацию (Первый + Второй)...\n");
    list_concat(&l1, &l2);
    printf("Объединенный "); list_print(&l1);

    printf("\nСортируем объединенный список быстрой сортировкой Хоара...\n");
    list_sort(&l1);
    printf("Итоговый "); list_print(&l1);

    return 0;
}
