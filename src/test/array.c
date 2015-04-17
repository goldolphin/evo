/**
 * 
 * @author goldolphin
 *         2015-04-16 23:37
 */
#include <stdlib.h>
#include <stdio.h>

void sort(int * data, int len) {
    if (data == NULL || len <= 1) return;

    int pivot = data[len-1];

    int wi = 0;
    for (int i = 0; i < len-1; ++i) {
        if (data[i] < pivot) {
            int t = data[wi];
            data[wi++] = data[i];
            data[i] = t;
        }
    }
    data[len-1] = data[wi];
    data[wi] = pivot;

    sort(data, wi);
    sort(data+wi+1, len-wi-1);
}

void shuffle(int * data, int len) {
    for (int i = 0; i < len-1; ++i) {
        int k = (rand() % (len-i)) + i;
        int t = data[k];
        data[k] = data[i];
        data[i] = t;
    }
}

void range(int * data, int start, int end) {
    for (int i = start; i < end; ++i) {
        data[i-start] = i;
    }
}

void print(int * data, int len) {
    printf("array(");
    for (int i = 0; i < len; ++i) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", data[i]);
    }
    printf(")\n");
}

void test(int * data, int len) {
    print(data, len);

    int start = rand()% 5;
    range(data, start, start+len);
    print(data, len);

    shuffle(data, len);
    print(data, len);

    sort(data, len);
    print(data, len);
}

int main() {
    int n = 10;

    for (int i = 0; i < n; ++i) {
        int data[20];
        test(data, 20);
        printf("\n");
    }
    return 0;
}
