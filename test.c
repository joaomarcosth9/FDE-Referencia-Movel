#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

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
    int temp = *b;
    *b = *a;
    *a = temp;
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

void slow_insert(queue* q, node* n) {
    int x = n->rank;
    if (is_empty(q)) {
        q->front = n;
        q->front->nxt = q->back;
        return;
    }
    if (q->back == NULL) {
        q->back = n;
        set_node(q->front, NULL, q->back);
        if (q->back->rank < q->front->rank) {
            swap(&q->back->rank, &q->front->rank);
        }
        return;
    }
    if (x <= q->front->rank) {
        set_node(NULL, q->front, n);
        q->front = n;
        return;
    }
    if (x >= q->back->rank) {
        set_node(q->back, NULL, n);
        q->back = n;
        return;
    }
    q->ref = q->back;  
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
    node* now = q->front;
    while (now != NULL) {
        print_node(now);
        now = now->nxt;
        /* if (now != NULL) printf(", "); */
    }
    printf("\n\n");
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

int main() {
    FILE *fp = fopen("data.csv", "r");
    FILE *out1 = fopen("time_comparison/out1", "w");
    FILE *out2 = fopen("time_comparison/out2", "w");
    char row[10005][100];
    int cnt = 0;
    while (!feof(fp)) {
        fgets(row[cnt], 100, fp);
        cnt++;
    }
    clock_t start, end;
    int ord[] = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000,
7500, 8000, 8500, 9000};
    printf("Com referencia movel\n");
    for (int i = 0; i < 18; i++) {
        queue* q = new_queue();
        double totaltime = 0;
        start = clock();
        for (int j = 0; j < ord[i]; j++) {
            char c;
            int rowit = 0;
            int ptr = 0;
            int it = 0;
            char sep[5][100];
            for (c = row[j][rowit]; rowit < (int)strlen(row[j]); c = row[j][rowit++]) {
                if (c == ',') {
                    sep[ptr][it] = '\0';
                    it = 0;
                    ptr++;
                }
                else {
                    sep[ptr][it++] = c;
                }
            }
            node* n = create_node(sep[0], stoi(sep[1]), stoi(sep[2]), sep[3]);
            insert(q, n);
        }
        end = clock();
        totaltime = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Tempo para inserir %d elementos: %lfms\n", ord[i], totaltime * 1000);
        fprintf(out1, "%d,%lf\n", ord[i], totaltime * 1000);
        destroy_Q(q);
    }
    printf("\n");
    printf("Sem referencia movel\n");
    for (int i = 0; i < 18; i++) {
        queue* q = new_queue();
        double totaltime = 0;
        start = clock();
        for (int j = 0; j < ord[i]; j++) {
            char c;
            int rowit = 0;
            int ptr = 0;
            int it = 0;
            char sep[5][100];
            for (c = row[j][rowit]; rowit < (int)strlen(row[j]); c = row[j][rowit++]) {
                if (c == ',') {
                    sep[ptr][it] = '\0';
                    it = 0;
                    ptr++;
                }
                else {
                    sep[ptr][it++] = c;
                }
            }
            node* n = create_node(sep[0], stoi(sep[1]), stoi(sep[2]), sep[3]);
            slow_insert(q, n);
        }
        end = clock();
        totaltime = (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(out2, "%d,%lf\n", ord[i], totaltime * 1000);
        printf("Tempo para inserir %d elementos: %lfms\n", ord[i], totaltime * 1000);
        destroy_Q(q);
    }
}
