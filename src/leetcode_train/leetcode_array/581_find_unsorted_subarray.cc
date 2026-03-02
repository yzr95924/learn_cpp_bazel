/**
 * @file 581_find_unsorted_subarray.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief 581. 最短无序连续子数组
 * https://leetcode.cn/problems/shortest-unsorted-continuous-subarray/description/
 * @version 0.1
 * @date 2026-03-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "leetcode_array.h"

static int CmpAscend(const void *a, const void *b)
{
    int latter = *(const int *)a;
    int former = *(const int *)b;
    return latter - former;
}

int findUnsortedSubarray(int *nums, int numsSize)
{
    int *copyArr = (int *)calloc((size_t)numsSize, sizeof(int));
    for (int idx = 0; idx < numsSize; idx++) {
        copyArr[idx] = nums[idx];
    }
    qsort(copyArr, (size_t)numsSize, sizeof(int), CmpAscend);

    int leftIdx = 0;
    while (leftIdx < numsSize - 1 && copyArr[leftIdx] == nums[leftIdx]) {
        leftIdx++;
    }

    int rightIdx = numsSize - 1;
    while (rightIdx >= leftIdx && copyArr[rightIdx] == nums[rightIdx]) {
        rightIdx--;
    }

    if (leftIdx == rightIdx) {
        return 0;
    }
    return rightIdx - leftIdx + 1;
}