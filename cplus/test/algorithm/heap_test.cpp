//
// Created by 宋博文 on 2024/5/8.
// https://www.cnblogs.com/lanhaicode/p/10546257.html
//

#include "gtest/gtest.h"

#define MAX_HEAP_SIZE 100

// 定义一个全局数组作为小堆，每次插入元素到最后
int heap[MAX_HEAP_SIZE];
int heapSize = 0;

// 获取父节点索引
int getParentIndex(int childIndex) {
    return (childIndex - 1) / 2;
}

// 获取左子节点索引
int getLeftChildIndex(int parentIndex) {
    return 2 * parentIndex + 1;
}

// 获取右子节点索引
int getRightChildIndex(int parentIndex) {
    return 2 * parentIndex + 2;
}

// 上移操作（向上调整小堆）
void siftUp(int index) {
    while (index > 0 && heap[index] < heap[getParentIndex(index)]) {
        // 如果当前节点小于其父节点，则交换它们的值
        int temp = heap[index];
        heap[index] = heap[getParentIndex(index)];
        heap[getParentIndex(index)] = temp;
        // 更新当前节点的索引为其父节点的索引，继续上移操作
        index = getParentIndex(index);
    }
}

// 下移操作（向下调整小堆）
void siftDown(int index) {
    int minIndex = index;
    int leftChildIndex = getLeftChildIndex(index);
    int rightChildIndex = getRightChildIndex(index);
    // 找到当前节点、左子节点和右子节点中的最小值
    if (leftChildIndex < heapSize && heap[leftChildIndex] < heap[minIndex]) {
        minIndex = leftChildIndex;
    }
    if (rightChildIndex < heapSize && heap[rightChildIndex] < heap[minIndex]) {
        minIndex = rightChildIndex;
    }
    // 如果最小值不是当前节点，则交换它们的值并继续下移操作
    if (index != minIndex) {
        int temp = heap[index];
        heap[index] = heap[minIndex];
        heap[minIndex] = temp;
        siftDown(minIndex);
    }
}

// 插入元素到小堆中
void insert(int value) {
    if (heapSize >= MAX_HEAP_SIZE) {
        printf("Heap is full\n");
        return;
    }
    heap[heapSize] = value;
    heapSize++;
    siftUp(heapSize - 1);
}

// 删除堆顶元素
void removeMin() {
    if (heapSize <= 0) {
        printf("Heap is empty\n");
        return;
    }
    heap[0] = heap[heapSize - 1];
    heapSize--;
    siftDown(0);
}

// 打印堆中的元素
void printHeap() {
    printf("Heap: ");
    for (int i = 0; i < heapSize; i++) {
        printf("%d ", heap[i]);
    }
    printf("\n");
}

TEST(heap_test, 模拟堆结构) {
    insert(5);
    insert(3);
    insert(8);
    insert(1);
    insert(2);

    printf("Original heap:\n");
    printHeap();

    removeMin();

    printf("Heap after removing min element:\n");
    printHeap();
}


