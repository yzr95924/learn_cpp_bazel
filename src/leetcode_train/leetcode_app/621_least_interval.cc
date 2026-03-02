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

static const int kTaskNum = 26;

int leastInterval(char *tasks, int tasksSize, int n)
{
    // 维护nextValid数组、剩余执行任务次数数组
    int restTaskNumArr[kTaskNum];
    int nextValid[kTaskNum];
    int totalRestTaskNum = tasksSize;
    for (int idx = 0; idx < tasksSize; idx++) {
        restTaskNumArr[tasks[idx] - 'A'] = 0;
        nextValid[tasks[idx] - 'A'] = 0;
    }

    for (int idx = 0; idx < tasksSize; idx++) {
        restTaskNumArr[tasks[idx] - 'A']++;
        nextValid[tasks[idx] - 'A'] = 1;
    }

    int time = 0;
    while (totalRestTaskNum > 0) {
        time++;
        int runTaskIdx = -1;
        int curMaxTaskCnt = INT_MIN;
        for (int idx = 0; idx < tasksSize; idx++) {
            int curTaskIdx = tasks[idx] - 'A';
            if (nextValid[curTaskIdx] <= time) {
                if (restTaskNumArr[curTaskIdx] != 0 &&
                    restTaskNumArr[curTaskIdx] >= curMaxTaskCnt) {
                    curMaxTaskCnt = restTaskNumArr[curTaskIdx];
                    runTaskIdx = curTaskIdx;
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