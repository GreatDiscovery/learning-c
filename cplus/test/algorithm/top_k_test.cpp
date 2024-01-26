//
// Created by 贾芸 on 2024/1/26.
//

#include "../basic.h"
#include <unistd.h>

// 最小堆结构体
typedef struct {
    int *array;
    int capacity;
    int size;
} MinHeap;

// 初始化最小堆
MinHeap *initMinHeap(int capacity) {
    MinHeap *heap = (MinHeap *) malloc(sizeof(MinHeap));
    heap->array = (int *) malloc(sizeof(int) * capacity);
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

// 交换堆中的两个元素
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 向最小堆中插入元素
void insert(MinHeap *heap, int value) {
    if (heap->size < heap->capacity) {
        heap->array[heap->size++] = value;
        int index = heap->size - 1;
        while (index > 0 && heap->array[index] < heap->array[(index - 1) / 2]) {
            swap(&heap->array[index], &heap->array[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    } else if (value > heap->array[0]) {
        heap->array[0] = value;
        int index = 0;
        while (1) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;
            if (leftChild < heap->size && heap->array[leftChild] < heap->array[smallest]) {
                smallest = leftChild;
            }
            if (rightChild < heap->size && heap->array[rightChild] < heap->array[smallest]) {
                smallest = rightChild;
            }
            if (smallest != index) {
                swap(&heap->array[index], &heap->array[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }
}

// 打印最小堆中的元素
void printTopK(MinHeap *heap) {
    printf("Top-K elements: ");
    for (int i = 0; i < heap->size; ++i) {
        printf("%d ", heap->array[i]);
    }
    printf("\n");
}

// 释放最小堆内存
void freeMinHeap(MinHeap *heap) {
    free(heap->array);
    free(heap);
}


TEST(topK, topK测试) {
    // 假设我们要找出数组中最大的3个元素（K=3）
    int k = 3;
    int array[] = {10, 7, 11, 5, 2, 13, 1, 6, 55, 22, 98};
    int arraySize = sizeof(array) / sizeof(array[0]);

    MinHeap *minHeap = initMinHeap(k);

    for (int i = 0; i < arraySize; ++i) {
        insert(minHeap, array[i]);
    }

    printTopK(minHeap);

    freeMinHeap(minHeap);
}