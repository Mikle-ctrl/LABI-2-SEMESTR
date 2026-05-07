#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <filesystem>

// Вспомогательная функция обмена
inline void swap(double &a, double &b)
{
    double tmp = a;
    a = b;
    b = tmp;
}

// Сортировка расчёской с настраиваемым shrink
void comb_sort_custom(double arr[], unsigned N, float shrink)
{
    unsigned gap = N;
    bool swapped = true;
    while (gap > 1 || swapped)
    {
        if (gap > 1)
        {
            gap = static_cast<unsigned>(static_cast<float>(gap) / shrink);
            if (gap == 0)
                gap = 1;
        }
        swapped = false;
        for (unsigned i = 0; i + gap < N; ++i)
        {
            if (arr[i] > arr[i + gap])
            {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}

int main()
{
    const unsigned N = 100000; // Фиксированный размер для честного теста
    std::default_random_engine rng(12345);
    std::uniform_real_distribution<double> dist(0, 10000);

    std::ofstream out("comb_shrink_study.csv");
    out << "shrink,time_ms\n";

    // Создаем эталонный случайный массив
    double *original = new double[N];
    for (unsigned i = 0; i < N; ++i)
        original[i] = dist(rng);

    double *temp_arr = new double[N];

    std::cout << "Testing shrink factors for Comb Sort (N=" << N << ")..." << std::endl;

    // Перебираем shrink от 1.1 до 2.0
    for (float s = 1.05f; s <= 1.75f; s += 0.05f)
    {
        // Копируем данные для чистоты эксперимента
        std::copy(original, original + N, temp_arr);

        auto start = std::chrono::steady_clock::now();
        comb_sort_custom(temp_arr, N, s);
        auto end = std::chrono::steady_clock::now();

        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
        out << s << "," << elapsed << "\n";

        std::cout << "Shrink: " << s << " | Time: " << elapsed << " ms" << std::endl;

        std::ofstream out("comb_shrink_study.csv");
    }

    delete[] original;
    delete[] temp_arr;
    out.close();
    return 0;
}
