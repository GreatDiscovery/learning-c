//
// Created by 宋博文 on 2024/1/28.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *array;
    int size;
    int capacity;
} MaxHeap;

MaxHeap *createMaxHeap(int capacity) {
    MaxHeap *maxHeap = (MaxHeap *) malloc(sizeof(MaxHeap));
    maxHeap->array = (int *) malloc(sizeof(int) * capacity);
    maxHeap->size = 0;
    maxHeap->capacity = capacity;
    return maxHeap;
}

void swap(MaxHeap *maxHeap, int i, int j) {
    int temp = maxHeap->array[i];
    maxHeap->array[i] = maxHeap->array[j];
    maxHeap->array[j] = temp;
}

void insert(MaxHeap *maxHeap, int value) {
    if (maxHeap->size == maxHeap->capacity) {
        if (value > maxHeap->array[0]) {
            maxHeap->array[0] = value;
            // 保持最大堆的性质
            int i = 0;
            while (true) {
                int leftChild = 2 * i + 1;
                int rightChild = 2 * i + 2;
                int largest = i;

                if (leftChild < maxHeap->size && maxHeap->array[leftChild] > maxHeap->array[largest]) {
                    largest = leftChild;
                }
                if (rightChild < maxHeap->size && maxHeap->array[rightChild] > maxHeap->array[largest]) {
                    largest = rightChild;
                }

                if (largest != i) {
                    swap(maxHeap, i, largest);
                    i = largest;
                } else {
                    break;
                }
            }
        }
    } else {
        maxHeap->array[maxHeap->size] = value;
        // 保持最大堆的性质
        int i = maxHeap->size;
        while (i != 0 && maxHeap->array[(i - 1) / 2] < maxHeap->array[i]) {
            swap(maxHeap, i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
        maxHeap->size++;
    }
}

int getMax(MaxHeap *maxHeap) {
    if (maxHeap->size <= 0) {
        printf("Heap is empty.\n");
        return -1;
    }
    return maxHeap->array[0];
}

void destroyMaxHeap(MaxHeap *maxHeap) {
    free(maxHeap->array);
    free(maxHeap);
}

void printTopK(MaxHeap *heap) {
    printf("Top-K elements: ");
    for (int i = 0; i < heap->size; ++i) {
        printf("%d ", heap->array[i]);
    }
    printf("\n");
}

int main() {
    int k = 3; // Top-K 的大小
    MaxHeap *maxHeap = createMaxHeap(k);

    // 插入一些元素，模拟Top-K操作
    insert(maxHeap, 4);
    insert(maxHeap, 2);
    insert(maxHeap, 8);
    insert(maxHeap, 5);
    insert(maxHeap, 1);

    // 打印堆顶元素（最大值），即Top-K中的最大值
    printf("Top element: %d\n", getMax(maxHeap));
    printTopK(maxHeap);

    // 销毁堆
    destroyMaxHeap(maxHeap);

    return 0;
}
