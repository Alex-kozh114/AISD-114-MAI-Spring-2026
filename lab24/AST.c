#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TokenKind {
    T_VAL,
    T_OP,
    T_OPEN,
    T_CLOSE
};

typedef struct Token {
    enum TokenKind kind;
    char content[64];
    char symbol;
} Token;

typedef struct ExpressionNode {
    char operation;
    char *rawData;
    struct ExpressionNode *leftSide;
    struct ExpressionNode *rightSide;
} ExpressionNode;

// Прототип для корректной работы рекурсии
void destroyTree(ExpressionNode *node);

ExpressionNode *newLeaf(const char *val) {
    ExpressionNode *node = calloc(1, sizeof(ExpressionNode));
    if (!node) return NULL;
    node->rawData = strdup(val);
    return node;
}

ExpressionNode *newBranch(char op, ExpressionNode *l, ExpressionNode *r) {
    ExpressionNode *node = calloc(1, sizeof(ExpressionNode));
    if (!node) return NULL;
    node->operation = op;
    node->leftSide = l;
    node->rightSide = r;
    return node;
}

// проверяем является ли узел целым числом
int checkInt(ExpressionNode *n) {
    if (!n || !n->rawData) return 0;
    for (int i = 0; n->rawData[i]; i++) 
        if (!isdigit(n->rawData[i])) return 0;
    return 1;
}

// проверяем является ли узел переменной
int checkVar(ExpressionNode *n) {
    if (!n || !n->rawData) return 0;
    return isalpha(n->rawData[0]);
}

// Глубокое копирование поддерева
ExpressionNode *copySubtree(ExpressionNode *src) {
    if (!src) return NULL;
    ExpressionNode *copy = calloc(1, sizeof(ExpressionNode));
    if (src->rawData) copy->rawData = strdup(src->rawData);
    copy->operation = src->operation;
    copy->leftSide = copySubtree(src->leftSide);
    copy->rightSide = copySubtree(src->rightSide);
    return copy;
}

// Редукция умножения
ExpressionNode *reduceMultiplication(ExpressionNode *target, ExpressionNode *varNode, int count) {
    if (count <= 0) return newLeaf("0");
    if (count == 1) return copySubtree(varNode);
    
    // Рекурсивно создаем цепочку сложений: var + (var + (var...))
    return newBranch('+', copySubtree(varNode), reduceMultiplication(NULL, varNode, count - 1));
}

ExpressionNode *applyRules(ExpressionNode *root) {
    if (!root) return NULL;

    root->leftSide = applyRules(root->leftSide);
    root->rightSide = applyRules(root->rightSide);

    // Если нашли умножение
    if (root->operation == '*') {
        ExpressionNode *varPart = NULL;
        int multiplier = 0;

        if (checkVar(root->leftSide) && checkInt(root->rightSide)) {
            varPart = root->leftSide;
            multiplier = atoi(root->rightSide->rawData);
        } else if (checkInt(root->leftSide) && checkVar(root->rightSide)) {
            varPart = root->rightSide;
            multiplier = atoi(root->leftSide->rawData);
        }

        if (varPart && multiplier > 0) {
            ExpressionNode *newSubtree = reduceMultiplication(NULL, varPart, multiplier);
            destroyTree(root);
            return newSubtree;
        }
    }
    return root;
}

void showExpr(const ExpressionNode *n) {
    if (!n) return;
    if (n->rawData) {
        printf("%s", n->rawData);
        return;
    }
    printf("(");
    showExpr(n->leftSide);
    printf(" %c ", n->operation);
    showExpr(n->rightSide);
    printf(")");
}

// Вспомогательные функции парсинга

int getPriority(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

void destroyTree(ExpressionNode *node) {
    if (!node) return;
    destroyTree(node->leftSide);
    destroyTree(node->rightSide);
    free(node->rawData);
    free(node);
}

// Лексический анализатор
int scan(const char *s, Token *res, int *count) {
    int k = 0, i = 0;
    while (s[i]) {
        if (isspace(s[i])) { i++; continue; }
        if (isalnum(s[i])) {
            int p = 0;
            while (isalnum(s[i])) res[k].content[p++] = s[i++];
            res[k].content[p] = '\0';
            res[k++].kind = T_VAL;
        } else {
            res[k].symbol = s[i];
            if (s[i] == '(') res[k].kind = T_OPEN;
            else if (s[i] == ')') res[k].kind = T_CLOSE;
            else res[k].kind = T_OP;
            k++; i++;
        }
    }
    *count = k;
    return 1;
}

// Построение дерева из постфиксной записи
ExpressionNode *parseToTree(Token *pf, int n) {
    ExpressionNode *stack[100];
    int top = -1;
    for (int i = 0; i < n; i++) {
        if (pf[i].kind == T_VAL) {
            stack[++top] = newLeaf(pf[i].content);
        } else {
            ExpressionNode *r = stack[top--];
            ExpressionNode *l = stack[top--];
            stack[++top] = newBranch(pf[i].symbol, l, r);
        }
    }
    return stack[0];
}

int main() {
    char buf[256];
    Token tokens[100], post[100];
    int n = 0, pn = 0;

    printf("Введите выражение (напр. a * 3): ");
    fgets(buf, sizeof(buf), stdin);

    scan(buf, tokens, &n);

    // Упрощенный Shunting-yard для постфикса
    Token stack[100];
    int top = -1;
    for (int i = 0; i < n; i++) {
        if (tokens[i].kind == T_VAL) post[pn++] = tokens[i];
        else if (tokens[i].kind == T_OPEN) stack[++top] = tokens[i];
        else if (tokens[i].kind == T_CLOSE) {
            while (stack[top].kind != T_OPEN) post[pn++] = stack[top--];
            top--;
        } else {
            while (top >= 0 && stack[top].kind == T_OP && getPriority(stack[top].symbol) >= getPriority(tokens[i].symbol))
                post[pn++] = stack[top--];
            stack[++top] = tokens[i];
        }
    }
    while (top >= 0) post[pn++] = stack[top--];

    ExpressionNode *tree = parseToTree(post, pn);
    
    printf("Исходное: ");
    showExpr(tree);
    
    tree = applyRules(tree);

    printf("\nРезультат: ");
    showExpr(tree);
    printf("\n");

    destroyTree(tree);
    return 0;
}
