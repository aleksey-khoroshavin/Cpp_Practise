#include <iostream>
using namespace std;
#include "Array.h"
/*Задана матрица А размерностью nxm. Записать все элементы матрицы в одномерный массив.*/
int main()
{
    cout << "Enter count of raws: ";
    int raws_count = 0;
    cin >> raws_count;

    cout << "Enter count of columns: ";
    int column_count = 0;
    cin >> column_count;

    Create_matrix(raws_count, column_count);
    return 0;
}
