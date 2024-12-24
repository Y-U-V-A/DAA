#include "infix_post.h"
#include "stack.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

i32 get_priority_post(char op) {

    if (op == '+' || op == '-') {
        return 0;
    }
    if (op == '*' || op == '/') {
        return 1;
    }
    if (op == '^') {
        return 2;
    }

    return -1;
}

void infix_post(const char* infix_exp, char* post_exp) {

    i32 j = 0;
    stack* stk = stack_create(char);

    for (i32 i = 0; infix_exp[i] != '\0'; ++i) {
        if ((infix_exp[i] >= 'a' && infix_exp[i] <= 'z') || (infix_exp[i] >= 'A' && infix_exp[i] <= 'Z')) {

            post_exp[j++] = infix_exp[i];

        } else if (infix_exp[i] == '(') {
            stack_push(stk, &infix_exp[i]);

        } else if (infix_exp[i] == '+' || infix_exp[i] == '-' || infix_exp[i] == '*' || infix_exp[i] == '/' || infix_exp[i] == '^') {

            while (stack_length(stk) != 0 && get_priority_post(*(char*)stack_top(stk)) >= get_priority_post(infix_exp[i])) {
                post_exp[j++] = *(char*)stack_top(stk);
                stack_pop(stk);
            }
            stack_push(stk, &infix_exp[i]);

        } else if (infix_exp[i] == ')') {

            while (stack_length(stk) != 0 && *(char*)stack_top(stk) != '(') {
                post_exp[j++] = *(char*)stack_top(stk);
                stack_pop(stk);
            }
            stack_pop(stk);
        }
    }

    while (stack_length(stk) != 0) {
        post_exp[j++] = *(char*)stack_top(stk);
        stack_pop(stk);
    }

    stack_destroy(stk);
}

void infix_post_run() {

    static const char* exps[] = {
        "a + b * (c ^ d - e) ^ (f + g * h) - i",
        "(p + q) * (m - n)",
        "(a + b) * (c - d)",
        "A * B + C / D",
        "x ^ (y + z) - w",
        "(M / N) ^ O + P * Q",
        "r * s + t / u - v",
        "(X - Y) / (Z + W)",
        "A + (B * C) - D / E",
        "(f + g) * (h - i) ^ j",
        "(P * Q) / (R + S) - T",
        "U - V + (W / X) * Y",
        "(k + l * m) ^ n - o / p",
        "Q ^ R * (S + T) / U",
        "(a / b) + (c * d) - e",
        "F * (G + H) / (I - J)",
        "(x - y * z) / (w + v)",
        "C * (D ^ E) - F + G / H",
        "(p + q) * (r - s) / t",
        "K ^ (L - M) * (N + O)",
        "a + b - (c * d / e) ^ f",
        "(A + B) / C * D - E",
    };

    static const char* results[] = {
        "abcd^e-fgh*+^*+i-",
        "pq+mn-*",
        "ab+cd-*",
        "AB*CD/+",
        "xyz+^w-",
        "MN/O^PQ*+",
        "rs*tu/+v-",
        "XY-ZW+/",
        "ABC*+DE/-",
        "fg+hi-j^*",
        "PQ*RS+/T-",
        "UV-WX/Y*+",
        "klm*+n^op/-",
        "QR^ST+*U/",
        "ab/cd*+e-",
        "FGH+*IJ-/",
        "xyz*-wv+/",
        "CDE^*F-GH/+",
        "pq+rs-*t/",
        "KLM-^NO+*",
        "ab+cd*e/f^-",
        "AB+C/D*E-",
    };

    i32 exps_len = sizeof(exps) / sizeof(exps[0]);
    i32 results_len = sizeof(results) / sizeof(results[0]);

    if (exps_len != results_len) {
        LOGE("results are not eqqual to expressions");
        return;
    }

    i32 failed = 0;
    for (i32 i = 0; i < exps_len; ++i) {

        char buffer[200] = {0};
        infix_post(exps[i], buffer);

        if (string_compare(buffer, results[i]) != 0) {
            LOGE("infix = %s , expected = %s , result = %s ", exps[i], results[i], buffer);
            failed += 1;
        } else {
            LOGI("infix = %s , postfix = %s ", exps[i], buffer);
        }
    }
    LOGT("infix to postfix -> test cases failed = %d", failed);
}
