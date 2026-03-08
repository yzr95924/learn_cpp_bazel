/**
 * @file 75_sort_colors.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief 75. 颜色分类 https://leetcode.cn/problems/sort-colors/description
 * @version 0.1
 * @date 2026-03-09
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "leetcode_array.h"

static void SwapItem(int *item1, int *item2)
{
    int tmp = *item1;
    *item1 = *item2;
    *item2 = tmp;
    return;
}

void sortColors(int *nums, int numsSize)
{
    int curIdx = 0;
    for (int idx = 0; idx < numsSize; idx++) {
        if (nums[idx] == 0) {
            SwapItem(&nums[idx], &nums[curIdx]);
            curIdx++;
        }
    }
    for (int idx = curIdx; idx < numsSize; idx++) {
        if (nums[idx] == 1) {
            SwapItem(&nums[idx], &nums[curIdx]);
            curIdx++;
        }
    }
    return;
}