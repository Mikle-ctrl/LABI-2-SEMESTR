#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>

// ========== Swap ==========
inline void swap(double &a, double &b)
{
    double tmp = a;
    a = b;
    b = tmp;
}

// ========== Элементарные проходы для пузырьков ==========
void forward_step(double arr[], unsigned begin_idx, unsigned end_idx)
{
    for (unsigned i = begin_idx; i < end_idx - 1; ++i)
        if (arr[i] > arr[i + 1])
            swap(arr[i], arr[i + 1]);
}

void backward_step(double arr[], unsigned begin_idx, unsigned end_idx)
{
    for (unsigned i = end_idx - 1; i > begin_idx; --i)
        if (arr[i - 1] > arr[i])
            swap(arr[i - 1], arr[i]);
}

// ========== 1. Прямой пузырёк ==========
void bubble_sort_forward(double arr[], unsigned begin_idx, unsigned end_idx)
{
    bool swapped;
    while (swapped)
    {
        swapped = false;
        for (unsigned i = begin_idx; i < end_idx - 1; ++i)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    }
}

// ========== 2. Обратный пузырёк ==========
void bubble_sort_backward(double arr[], unsigned begin_idx, unsigned end_idx)
{
    bool swapped;
    while (swapped)
    {
        swapped = false;
        for (unsigned i = end_idx - 1; i > begin_idx; --i)
        {
            if (arr[i - 1] > arr[i])
            {
                swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
    }
}

// ========== 3. Шейкерная сортировка ==========
void shaker_sort(double arr[], unsigned begin_idx, unsigned end_idx)
{
    unsigned left = begin_idx, right = end_idx;
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (unsigned i = left; i < right - 1; ++i)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
        --right;
        swapped = false;
        for (unsigned i = right - 1; i > left; --i)
        {
            if (arr[i - 1] > arr[i])
            {
                swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
        ++left;
    }
}

// ========== 4. Сортировка расчёской ==========
bool comb_pass(double arr[], unsigned begin_idx, unsigned end_idx, unsigned gap)
{
    bool swapped = false;
    for (unsigned i = begin_idx; i + gap < end_idx; ++i)
    {
        if (arr[i] > arr[i + gap])
        {
            swap(arr[i], arr[i + gap]);
            swapped = true;
        }
    }
    return swapped;
}

void comb_sort(double arr[], unsigned begin_idx, unsigned end_idx)
{
    unsigned size = end_idx - begin_idx;
    unsigned gap = size;
    bool swapped = true;
    const float shrink = 1.3f;
    while (gap > 1 || swapped)
    {
        if (gap > 1)
            gap = static_cast<unsigned>(static_cast<float>(gap) / shrink);
        swapped = comb_pass(arr, begin_idx, end_idx, gap);
    }
}

// ========== Генератор случайного массива ==========
double *create_random_array(unsigned N, std::default_random_engine &rng)
{
    double *arr = new double[N];
    std::uniform_int_distribution<unsigned> dstr(0, N);
    for (unsigned i = 0; i < N; ++i)
        arr[i] = static_cast<double>(dstr(rng));
    return arr;
}

// ========== Тестирование ==========
int main()
{
    const unsigned sizes[] = {1000, 2000, 3000, 5000, 7000, 10000, 15000, 20000};
    const unsigned num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    unsigned seed = 12345;
    std::default_random_engine rng(seed);

    std::ofstream out("all_sorts_comparison.csv");
    out << "N,time_forward_ms,time_backward_ms,time_shaker_ms,time_comb_ms\n";

    std::cout << "Сравнение 4 сортировок (прямой, обратный, шейкер, расчёска)\n";

    for (unsigned idx = 0; idx < num_sizes; ++idx)
    {
        unsigned N = sizes[idx];
        double *original = create_random_array(N, rng);

        // Копии
        double *arr_fwd = new double[N];
        double *arr_bwd = new double[N];
        double *arr_shaker = new double[N];
        double *arr_comb = new double[N];
        std::copy(original, original + N, arr_fwd);
        std::copy(original, original + N, arr_bwd);
        std::copy(original, original + N, arr_shaker);
        std::copy(original, original + N, arr_comb);

        // Прямой
        auto start = std::chrono::steady_clock::now();
        bubble_sort_forward(arr_fwd, 0, N);
        auto end = std::chrono::steady_clock::now();
        double t_fwd = std::chrono::duration<double, std::milli>(end - start).count();

        // Обратный
        start = std::chrono::steady_clock::now();
        bubble_sort_backward(arr_bwd, 0, N);
        end = std::chrono::steady_clock::now();
        double t_bwd = std::chrono::duration<double, std::milli>(end - start).count();

        // Шейкер
        start = std::chrono::steady_clock::now();
        shaker_sort(arr_shaker, 0, N);
        end = std::chrono::steady_clock::now();
        double t_shaker = std::chrono::duration<double, std::milli>(end - start).count();

        // Расчёска
        start = std::chrono::steady_clock::now();
        comb_sort(arr_comb, 0, N);
        end = std::chrono::steady_clock::now();
        double t_comb = std::chrono::duration<double, std::milli>(end - start).count();

        out << N << "," << t_fwd << "," << t_bwd << "," << t_shaker << "," << t_comb << "\n";

        std::cout << "N = " << std::setw(5) << N
                  << " | forward = " << std::setw(8) << t_fwd << " ms"
                  << " | backward = " << std::setw(8) << t_bwd << " ms"
                  << " | shaker  = " << std::setw(8) << t_shaker << " ms"
                  << " | comb    = " << std::setw(8) << t_comb << " ms\n";

        delete[] original;
        delete[] arr_fwd;
        delete[] arr_bwd;
        delete[] arr_shaker;
        delete[] arr_comb;
    }
    out.close();

    return 0;
}
