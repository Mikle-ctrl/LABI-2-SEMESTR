#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>



// Создание массива
double *create_random_array(int N, std::default_random_engine &rng)
{
    double *array = new double[N];
    std::uniform_int_distribution<unsigned> dstr(0, N);
    for (unsigned idx = 0; idx < N; ++idx)
    {
        array[idx] = dstr(rng);
    }
    return array;
}

// Полный перебор
bool perebor(const double *arr, int N, double S)
{
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (arr[i] + arr[j] == S)
                return true;
    return false;
}

// Быстрый поиск
void fast_ser(const double *arr, int N, double S)
{
    int left = 0, right = N - 1;
    while (left < right)
    {
        double sum = arr[left] + arr[right];
        if (sum == S)
        {
            return;
        }
        else if (sum < S)
        {
            ++left;
        }
        else
        {
            --right;
        }
    }
    return;
}

int main()
 {
//     std::ofstream out("results.csv");// создаём файл

//     out << "test_type,N,time_us\n";// заголовок CSV
    
    unsigned seed = 0;
    std::default_random_engine rng(seed);
    double *cur_mas = nullptr;

    /// Замер времени по N для полного перебора
    //out << "      Данные для неотсортированного массива (поиск пробегом по каждому элементу): " << "\n";
    std::cout << "      Данные для неотсортированного массива (поиск пробегом по каждому элементу): " << std::endl;
    for (int N = 100; N <= 100 * 25 * 25 * 25; N *= 5)
    {
        long long total_time = 0;
        const int K = 100;  // количество различных сумм (для усреднения)
        const int REP = 10; // повторений для каждой суммы (для увеличения времени)
        cur_mas = create_random_array(N, rng);

        // Генерируем K случайных сумм в диапазоне [0, 2N]
        std::uniform_int_distribution<unsigned> sum_dist(0, 2 * N);
        double *targets = new double[K];

        for (int t = 0; t < K; ++t)
            targets[t] = sum_dist(rng);

        auto begin = std::chrono::steady_clock::now();
        volatile int found_count = 0;
        for (int rep = 0; rep < REP; ++rep)
        {
            for (int t = 0; t < K; ++t)
            {
                found_count += perebor(cur_mas, N, targets[t]) ? 1 : 0;
            }
        }
        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        total_time = time_span.count();
        double avg_time = total_time / double(K * REP);
        // out << "N = " << N << "  Среднее время (мкс): " << avg_time << "\n";
        std::cout << "N = " << N << "  Среднее время (мкс): " << avg_time << std::endl;
        delete[] cur_mas;
        delete[] targets;
    }

    /// Замер времени по N для упорядоченного списка
   // out << "      Данные для упорядоченного списка : " << "\n";
    std::cout << "      Данные для упорядоченного списка : " << std::endl;
    for (int N = 100; N <= 100 * 25 * 25 * 25; N *= 5)
    {
        long long total_time = 0;
        const int K = 100;
        const int REP = 10;
        cur_mas = create_random_array(N, rng);
        std::sort(cur_mas, cur_mas + N);

        std::uniform_int_distribution<unsigned> sum_dist(0, 2 * N);
        double *targets = new double[K];

        for (int t = 0; t < K; ++t)
            targets[t] = sum_dist(rng);

        auto begin = std::chrono::steady_clock::now();
        for (int rep = 0; rep < REP; ++rep)
        {
            for (int t = 0; t < K; ++t)
            {
                fast_ser(cur_mas, N, targets[t]);
            }
        }
        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        total_time = time_span.count();

        double avg_time = total_time / double(K * REP);
            

        //out << "N = " << N << "  Среднее время (мкс): " << avg_time << "\n";

    
        std::cout << "N = " << N << "  Среднее время (мкс): " << avg_time << std::endl;
        delete[] cur_mas;
        delete[] targets;
    }

    // out << "Данные для неотсортированного массива (полный перебор, наихудший случай):\n";
    std::cout << "Данные для неотсортированного массива (полный перебор, наихудший случай):\n";
    for (int N = 100; N <= 100 * 25 * 25 * 25; N *= 5)
    {
        long long total_time = 0;
        const int REP = (N <= 2500) ? 1000 : (N <= 12500 ? 100 : 10);
        cur_mas = create_random_array(N, rng);
        double S = 2 * N + 1; // отсутствующая сумма

        auto begin = std::chrono::steady_clock::now();
        for (int rep = 0; rep < REP; ++rep)
        {
            volatile bool found = perebor(cur_mas, N, S); // используем результат
        }
        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        total_time = time_span.count();
        double avg_time = total_time / double(REP);
        // out << "N = " << N << "  Среднее время (мкс): " << avg_time << "\n";
        std::cout << "N = " << N << "  Среднее время (мкс): " << avg_time << std::endl;
        delete[] cur_mas;
    }
//out.close();
    return 0;
}
//Честно говоря так и не смог понять как сделать вывод в другой файл