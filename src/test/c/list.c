/**
 * 
 * @author goldolphin
 *         2015-04-19 10:44
 */
#include <stdlib.h>
#include <stdio.h>

typedef struct list_st {
    int value;
    struct list_st * tail;
} list;

list * nil = NULL;

void print(list * l) {
    printf("list(");
    for (list * head = l; head != nil; head = head->tail) {
        if (head != l) {
            printf(", ");
        }
        printf("%d", head->value);
    }
    printf(")\n");
}

list * cons(int value, list * tail) {
    list * l = (list *)malloc(sizeof(list));
    l->value = value;
    l->tail = tail;
    return l;
}

list * reverse(list * l) {
    if (l == nil || l->tail == nil) return l;
    list * r = nil;
    for (list * head = l; head != nil; head = head->tail) {
        r = cons(head->value, r);
    }
    return r;
}

list * append(list * left, list * right) {
    if (left == nil) return right;
    return cons(left->value, append(left->tail, right));
}

list * sort(list * l) {
    if (l == nil || l->tail == nil) return l;

    int pivot = l->value;
    list * less = nil;
    list * greater = nil;
    for (list * head = l->tail; head != nil; head = head->tail) {
        if (head->value < pivot) {
            less = cons(head->value, less);
        } else {
            greater = cons(head->value, greater);
        }
    }

    return append(sort(less), cons(pivot, sort(greater)));
}

list * range(int start, int end) {
    list * l = nil;
    for (int i = end-1; i >= start; --i) {
        l = cons(i, l);
    }
    return l;
}

void test(int len) {
    int start = rand()% 5;
    list * l = range(start, start+len);
    print(l);

    list * r = reverse(l);
    print(r);

    list * s = sort(r);
    print(s);
}

int main() {
    int n = 10;

    for (int i = 0; i < n; ++i) {
        test(20);
        printf("\n");
    }
    return 0;
}
