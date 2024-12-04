#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// تعريف هيكل المصفوفة
typedef struct {
    unsigned int num_rows;       // عدد الصفوف
    unsigned int num_columns;    // عدد الأعمدة
    double *elements;            // العناصر
} Matrix;

// دالة لحساب العنصر ل[س, ص] بناءً على الحالتين
double compute_l_element(Matrix L, Matrix A, unsigned int i, unsigned int j) {
    double sum = 0.0;

    // حساب مجموع القيم السابقة
    for (unsigned int k = 0; k < j; k++) {
        sum += L.elements[i * L.num_columns + k] * L.elements[j * L.num_columns + k];
    }

    // حالة العنصر القطري (س == ص)
    if (i == j) {
        return sqrt(A.elements[i * A.num_columns + i] - sum);
    } 
    // حالة العنصر غير القطري (س != ص)
    else {
        return (1.0 / L.elements[j * L.num_columns + j]) * (A.elements[i * A.num_columns + j] - sum);
    }
}

// دالة لتحليل شولسكي
Matrix cholesky_decomposition(Matrix A) {
    unsigned int n = A.num_rows; // عدد الصفوف (أو الأعمدة لأن المصفوفة مربعة)
    Matrix L;

    // تخصيص الذاكرة للمصفوفة ل
    L.num_rows = n;
    L.num_columns = n;
    L.elements = (double *)malloc(n * n * sizeof(double));

    // تعبئة المصفوفة ل بالقيم الصفرية
    for (unsigned int i = 0; i < n * n; i++) {
        L.elements[i] = 0.0;
    }

    // تنفيذ التحليل الثلاثي
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j <= i; j++) {
            // [س, ص]حساب العنصر ل 
            L.elements[i * n + j] = compute_l_element(L, A, i, j);
        }
    }

    return L;
}

// دالة لإنشاء مصفوفة إيجابية التعريف
Matrix create_positive_definite_matrix(unsigned int num_rows, unsigned int num_columns) {
    Matrix M;
    M.num_rows = num_rows;
    M.num_columns = num_columns;
    unsigned int size = num_rows * num_columns;
    M.elements = (double *)malloc(size * sizeof(double));

    // تعبئة العناصر بقيم عشوائية
    for (unsigned int i = 0; i < size; i++) {
        M.elements[i] = ((double)rand() / (double)RAND_MAX) - 0.5;
    }

    // جعل المصفوفة متماثلة
    for (unsigned int i = 0; i < num_rows; i++) {
        for (unsigned int j = 0; j < num_columns; j++) {
            if (i != j) {
                M.elements[i * num_columns + j] += M.elements[j * num_columns + i];
            }
        }
    }

    // جعل القيم القطرية أكبر من مجموع القيم الأخرى
    for (unsigned int i = 0; i < num_rows; i++) {
        M.elements[i * num_columns + i] += 0.5 * num_rows;
    }

    return M;
}
