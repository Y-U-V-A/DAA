#include "post_infix.h"
#include "stack.h"
#include "common.h"

typedef struct string {
    i8 buffer[100];
} string;

void post_infix(const char* post_exp, i8* infix_exp) {

    stack* stk = stack_create(string);

    for (i32 i = 0; post_exp[i] != '\0'; ++i) {
        if ((post_exp[i] >= 'a' && post_exp[i] <= 'z') || (post_exp[i] >= 'A' && post_exp[i] <= 'Z')) {

            string str = {0};
            str.buffer[0] = post_exp[i];
            stack_push(stk, &str);

        } else if (post_exp[i] == '+' || post_exp[i] == '-' || post_exp[i] == '*' || post_exp[i] == '/' || post_exp[i] == '^') {
            string op2 = *(string*)stack_top(stk);
            stack_pop(stk);

            string op1 = *(string*)stack_top(stk);
            stack_pop(stk);

            string str = {0};
            string_format(str.buffer, 100, "(%s %c %s)", op1.buffer, post_exp[i], op2.buffer);

            stack_push(stk, &str);
        }
    }

    string* str = (string*)stack_top(stk);

    for (i32 i = 0; str->buffer[i] != '\0'; ++i) {
        infix_exp[i] = str->buffer[i];
    }

    stack_destroy(stk);
}

void post_infix_run() {

    const char* results[] = {
        "((a + (b * (((c ^ d) - e) ^ (f + (g * h))))) - i)",
        "((p + q) * (m - n))",
        "((a + b) * (c - d))",
        "((A * B) + (C / D))",
        "((x ^ (y + z)) - w)",
        "(((M / N) ^ O) + (P * Q))",
        "(((r * s) + (t / u)) - v)",
        "((X - Y) / (Z + W))",
        "((A + (B * C)) - (D / E))",
        "((f + g) * ((h - i) ^ j))",
        "(((P * Q) / (R + S)) - T)",
        "((U - V) + ((W / X) * Y))",
        "(((k + (l * m)) ^ n) - (o / p))",
        "(((Q ^ R) * (S + T)) / U)",
        "(((a / b) + (c * d)) - e)",
        "((F * (G + H)) / (I - J))",
        "((x - (y * z)) / (w + v))",
        "(((C * (D ^ E)) - F) + (G / H))",
        "(((p + q) * (r - s)) / t)",
        "((K ^ (L - M)) * (N + O))",
        "((a + b) - (((c * d) / e) ^ f))",
        "((((A + B) / C) * D) - E)",
    };

    const char* exps[] = {
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

        i8 buffer[200] = {0};
        post_infix(exps[i], buffer);

        if (string_compare(buffer, results[i]) != 0) {
            LOGE("infix = %s , expected = %s , result = %s ", exps[i], results[i], buffer);
            failed += 1;
        } else {
            LOGI("infix = %s , postfix = %s ", exps[i], buffer);
        }
    }
    LOGT("infix to postfix -> test cases failed = %d", failed);
}
