/**
 * @file 621_least_interval.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief 621. 任务调度器 https://leetcode.cn/problems/task-scheduler/description/
 * @version 0.1
 * @date 2026-03-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "leetcode_app.h"

#define TOTAL_TASK_TYPE_NUM 26

int leastInterval(char *tasks, int tasksSize, int n)
{
    int freqArr[TOTAL_TASK_TYPE_NUM] = {0};
    for (int idx = 0; idx < tasksSize; idx++) {
        int curIdx = tasks[idx] - 'A';
        freqArr[curIdx]++;
    }

    // 维护nextValid数组、剩余执行任务次数数组，按任务种类重新进行压缩
    int restTaskNumArr[TOTAL_TASK_TYPE_NUM] = {0};
    int nextValid[TOTAL_TASK_TYPE_NUM] = {0};
    int curTaskTypeNum = 0;
    for (int idx = 0; idx < TOTAL_TASK_TYPE_NUM; idx++) {
        if (freqArr[idx] > 0) {
            nextValid[curTaskTypeNum] = 1;
            restTaskNumArr[curTaskTypeNum] = freqArr[idx];
            curTaskTypeNum++;
        }
    }

    int time = 0;
    int totalRestTaskNum = tasksSize;
    while (totalRestTaskNum > 0) {
        time++;
        int runTaskIdx = -1;
        int curMaxTaskCnt = INT_MIN;
        for (int idx = 0; idx < curTaskTypeNum; idx++) {
            if (nextValid[idx] <= time) {
                if (restTaskNumArr[idx] != 0 && restTaskNumArr[idx] >= curMaxTaskCnt) {
                    curMaxTaskCnt = restTaskNumArr[idx];
                    runTaskIdx = idx;
                }
            }
        }
        if (runTaskIdx != -1) {
            // has find a valid task
            restTaskNumArr[runTaskIdx]--;
            nextValid[runTaskIdx] = time + n + 1;
            totalRestTaskNum--;
        }
    }
    return time;
}