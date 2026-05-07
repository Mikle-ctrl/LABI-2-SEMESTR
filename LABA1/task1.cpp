#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <iomanip>

// функция обмена 
void swap(double &a, double &b) {
    double tmp = a;
    a = b;
    b = tmp;
}

// Шейкерная сортировка 
// Прямой проход (слева направо)
void forward_step(double arr[], unsigned const begin_idx, unsigned const end_idx) {
    for (unsigned i = begin_idx; i < end_idx - 1; ++i) {
        if (arr[i] > arr[i + 1])
            swap(arr[i], arr[i + 1]);
    }
}

// Обратный проход (справа налево)
void backward_step(double arr[], unsigned const begin_idx, unsigned const end_idx) {
    for (unsigned i = end_idx - 1; i > begin_idx; --i) {
        if (arr[i - 1] > arr[i])
            swap(arr[i - 1], arr[i]);
    }
}

// Полная шейкерная сортировка (с подсчётом перестановок)
unsigned shaker_sort(double arr[], unsigned const begin_idx, unsigned const end_idx) {
    unsigned left = begin_idx;
    unsigned right = end_idx;
    bool swapped = true;
    unsigned swap_count = 0;

    while (swapped) {
        swapped = false;
        // Прямой проход
        for (unsigned i = left; i < right - 1; ++i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
                ++swap_count;
            }
        }
        if (!swapped) break;
        --right;

        swapped = false;
        // Обратный проход
        for (unsigned i = right - 1; i > left; --i) {
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);
                swapped = true;
                ++swap_count;
            }
        }
        ++left;
    }
    return swap_count;
}

// ========== Сортировка расчёской ==========
// Один проход с заданным шагом. Возвращает true, если были перестановки,
// и обновляет счётчик перестановок.
bool comb_pass(double arr[], unsigned const begin_idx, unsigned const end_idx,
               unsigned gap, unsigned &swap_count) {
    bool swapped = false;
    for (unsigned i = begin_idx; i + gap < end_idx; ++i) {
        if (arr[i] > arr[i + gap]) {
            swap(arr[i], arr[i + gap]);
            swapped = true;
            ++swap_count;
        }
    }
    return swapped;
}

// Сортировка расчёской (возвращает общее количество перестановок)
unsigned comb_sort(double arr[], unsigned const begin_idx, unsigned const end_idx) {
    unsigned size = end_idx - begin_idx;
    unsigned gap = size;
    bool swapped = true;
    unsigned swap_count = 0;
    const float shrink = 1.3f;

    while (gap > 1 || swapped) {
        if (gap > 1)
            gap = static_cast<unsigned>(static_cast<float>(gap) / shrink);
        swapped = comb_pass(arr, begin_idx, end_idx, gap, swap_count);
    }
    return swap_count;
}

// ========== Генератор случайного массива ==========
double* create_random_array(unsigned N, std::default_random_engine &rng) {
    double *array = new double[N];
    std::uniform_int_distribution<unsigned> dstr(0, N);
    for (unsigned idx = 0; idx < N; ++idx) {
        array[idx] = static_cast<double>(dstr(rng));
    }
    return array;
}

// ========== Тестирование и запись в файл ==========
int main() {
    // Диапазон размеров массивов
    const unsigned sizes[] = {1000, 2000, 3000, 5000, 7000, 10000, 15000, 20000};
    const unsigned num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Генератор случайных чисел (фиксированный seed для воспроизводимости)
    unsigned seed = 676767;
    std::default_random_engine rng(seed);

    // Открываем CSV-файл для записи
    std::ofstream outfile("comb_sort_results.csv");
    outfile << "N,time_ms,swaps\n";

    for (unsigned idx = 0; idx < num_sizes; ++idx) {
        unsigned N = sizes[idx];
        double *arr = create_random_array(N, rng);

        // Измерение времени сортировки расчёской
        auto start = std::chrono::high_resolution_clock::now();
        unsigned swaps = comb_sort(arr, 0, N);
        auto end = std::chrono::high_resolution_clock::now();

        double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

        // Запись в файл
        outfile << N << "," << time_ms << "," << swaps << "\n";

        // Вывод в консоль для контроля
        std::cout << "N = " << std::setw(5) << N
                  << " | time = " << std::setw(8) << time_ms << " ms"
                  << " | swaps = " << swaps << std::endl;

        delete[] arr;
    }

    outfile.close();
    std::cout << "\nРезультаты сохранены в файл comb_sort_results.csv\n";
    return 0;
}
