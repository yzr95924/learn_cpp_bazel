/**
 * @file 22_generate_parenthesis.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief 22. 括号生成 https://leetcode.cn/problems/generate-parentheses/description/
 * @version 0.1
 * @date 2026-03-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "leetcode_backtrack.h"

typedef struct {
    char **retAns;
    size_t ansCnt;
    size_t maxAnsCnt;
} AnsWrapper;

typedef struct {
    char *curAns;
    size_t curAnsSize;
} BackTrackState;

static bool IsValidParenthesis(char *inputStr, size_t strLen)
{
    int curLeftParenthesisNum = 0;
    for (size_t idx = 0; idx < strLen; idx++) {
        if (inputStr[idx] == '(') {
            curLeftParenthesisNum++;
        } else {
            if (curLeftParenthesisNum == 0) {
                return false;
            }
            curLeftParenthesisNum--;
        }
    }
    if (curLeftParenthesisNum != 0) {
        return false;
    }
    return true;
}

static void BackTrack(AnsWrapper *ansWrap, BackTrackState *btState, size_t targetSize)
{
    char choiceSet[2] = {'(', ')'};
    if (btState->curAnsSize == targetSize) {
        if (IsValidParenthesis(btState->curAns, btState->curAnsSize)) {
            ansWrap->retAns[ansWrap->ansCnt] = (char *)calloc(targetSize + 1, sizeof(char));
            memcpy(ansWrap->retAns[ansWrap->ansCnt], btState->curAns, targetSize);
            ansWrap->ansCnt++;

            if (ansWrap->ansCnt == ansWrap->maxAnsCnt) {
                ansWrap->maxAnsCnt = 2 * ansWrap->maxAnsCnt;
                ansWrap->retAns =
                    (char **)realloc(ansWrap->retAns, ansWrap->maxAnsCnt * sizeof(char *));
            }
        }
        return;
    }

    for (int idx = 0; idx < 2; idx++) {
        btState->curAns[btState->curAnsSize] = choiceSet[idx];
        btState->curAnsSize++;
        BackTrack(ansWrap, btState, targetSize);
        btState->curAnsSize--;
    }
    return;
}

char **generateParenthesis(int n, int *returnSize)
{
    AnsWrapper ansWrap;
    ansWrap.ansCnt = 0;
    ansWrap.maxAnsCnt = (size_t)n * 2;
    ansWrap.retAns = (char **)calloc(ansWrap.maxAnsCnt, sizeof(char *));

    BackTrackState btState;
    size_t targetSize = (size_t)n * 2;
    btState.curAnsSize = 0;
    btState.curAns = (char *)calloc(targetSize, sizeof(char));
    BackTrack(&ansWrap, &btState, targetSize);

    *returnSize = (int)ansWrap.ansCnt;
    free(btState.curAns);
    return ansWrap.retAns;
}