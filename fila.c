#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void line() {
    printf("-----------------------------\n\n");
}

void clear() {
    system(CLEAR);
    line();
}

typedef struct node {
    int rank, matricula;
    char nome[100], curso[100];
    struct node *nxt, *prev;
} node;

typedef struct queue {
    node *ref, *front, *back;
} queue;

int abs(int a) {
    return a < 0 ? -a : a;
}

int stoi(char* s) {
    int n = strlen(s);
    int ans = 0;
    int j = 1;
    for (int i = n - 1; i >= 0; i--) {
        ans += (s[i] - '0') * j;
        j *= 10;
    }
    return ans;
}

void swap(int* a, int* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

queue* new_queue() {
    queue* q = (queue*) malloc(sizeof(queue));
    q->front = NULL;
    q->back = NULL;
    q->ref = NULL;
    return q;
}

void set_node(node* last, node* next, node* me) {
    me->prev = last;
    me->nxt = next;
    if (me->prev != NULL) {
        me->prev->nxt = me;
    }
    if (me->nxt != NULL) {
        me->nxt->prev = me;
    }
}

node* create_node(char* nome, int matricula, int rank, char* curso) {
    node* ans = (node*) malloc(sizeof(node));
    ans->rank = rank;
    strcpy(ans->nome, nome);
    ans->matricula = matricula;
    strcpy(ans->curso, curso);
    return ans;
}

void find(node *n, node* me) {
    int x = me->rank;
    if (n->rank < x) {
        while (n != NULL && n->rank < x) {
            n = n->nxt;
        }
        set_node(n == NULL ? NULL : n->prev, n, me);
    }
    else {
        while (n != NULL && n->rank > x) {
            n = n->prev;
        }
        set_node(n, n == NULL ? NULL : n->nxt, me);
    }

}

int is_empty(queue* q) {
    return q->front == NULL;
}

void insert(queue* q, node* n) {
    int x = n->rank;
    if (is_empty(q)) {
        q->front = n;
        q->front->nxt = q->back;
        q->ref = q->front;
        return;
    }
    if (q->back == NULL) {
        q->back = n;
        set_node(q->front, NULL, q->back);
        if (q->back->rank < q->front->rank) {
            swap(&q->back->rank, &q->front->rank);
        }
        q->ref = q->back;
        return;
    }
    if (x <= q->front->rank) {
        set_node(NULL, q->front, n);
        q->front = n;
        q->ref = q->front;
        return;
    }
    if (x >= q->back->rank) {
        set_node(q->back, NULL, n);
        q->back = n;
        q->ref = q->back;
        return;
    }
    int distfront = abs(q->front->rank - x);
    int distback = abs(q->back->rank - x);
    int distref = abs(q->ref->rank - x);

    if (distref > distfront || distref > distback) {
        if (distfront <= distback) {
            q->ref = q->front;
        }
        else {
            q->ref = q->back;
        }
    }
    find(q->ref, n);
}

void pop_front(queue* q) {
    if (!is_empty(q)) {
        q->front = q->front->nxt;
        q->ref = q->front;
        if (q->front != NULL) {
            free(q->front->prev);
        }
        else {
            free(q->front);
        }
    }
    else {
        printf("Fila vazia\n");
    }
}

void pop_back(queue* q) {
    if (q->back == NULL) {
        pop_front(q);
    }
    else if (!is_empty(q)) {
        q->back = q->back->prev;
        q->ref = q->back;
        if (q->back != NULL) {
            free(q->back->nxt);
        }
    }
    else {
        printf("Fila vazia\n");
    }
}

node* front(queue* q) {
    return q->front;
}

node* back(queue* q) {
    if(q->back == NULL) {
        return front(q);
    }
    return q->back;
}

void print_node(node* n) {
    if(n == NULL) {
        printf("Nó nulo\n");
        return;
    }
    printf("{nome = %s, rank = %d, matricula = %d, curso = %s}\n", n->nome, n->rank, n->matricula, n->curso);
}

void print(queue *q) {
    if (is_empty(q)) {
        printf("Fila vazia\n");
        return;
    }
    node* now = q->front;
    while (now != NULL) {
        print_node(now);
        now = now->nxt;
        /* if (now != NULL) printf(", "); */
    }
    printf("\n");
}

void destroy_Q(queue* q) {
    if (is_empty(q)) {
        free(q);
        return;
    }
    node *n = q->front->nxt;
    while (n != NULL) {
        free(q->front);
        q->front = n;
        n = n->nxt;
    }
    free(q->front);
    free(q);
}

int menu() {
    printf("\n");
    line();
    printf("1 - Inserir\n");
    printf("2 - Pop_front\n");
    printf("3 - Pop_back\n");
    printf("4 - Printar estado atual da fila\n");
    printf("5 - Get_front\n");
    printf("6 - Get_back\n");
    printf("7 - Sair\n");
    printf("\n");
    printf("Digite sua opcao: ");
    int x;
    scanf("%d", &x);
    return x;
}

int main() {
    queue* q = new_queue();
    int x;
    int matricula = 1;
    while (x = menu(), x != 7) {
        clear();
        if (x == 1) {
            // receba nome, rank, matricula e curso
            printf("Digite o nome: ");
            char nome[100];
            scanf("%s", nome);
            printf("Digite o rank: ");
            int rank;
            scanf("%d", &rank);
            printf("Digite o curso: ");
            char curso[100];
            scanf("%s", curso);
            node* n = create_node(nome, matricula++, rank, curso);
            insert(q, n);
        }
        else if (x == 2) {
            pop_front(q);
            printf("Pop_front realizado\n");
        }
        else if (x == 3) {
            pop_back(q);
            printf("Pop_back realizado\n");
        }
        else if (x == 4) {
            print(q);
        }
        else if (x == 5) {
            print_node(front(q));
        }
        else if (x == 6) {
            print_node(back(q));
        } else {
            printf("Opcao invalida\n");
        }
    }
    destroy_Q(q);
    return 0;
}
