#include <iostream>
#include <fstream>
#include <cmath>
#include <numbers>
#include <string>

// 1. Рекурсивное суммирование
float sum_recursive(float const data[], unsigned size)
{
    if (size <= 1)
        return (size == 1) ? data[0] : 0.0f;
    unsigned mid = size / 2;
    return sum_recursive(data, mid) + sum_recursive(data + mid, size - mid);
}

// 2.попарное (без рекурсии)
float sum_pairwise(float const data[], unsigned size)
{
    if (size == 0)
        return 0.0f;
    float *temp = new float[size];
    for (unsigned i = 0; i < size; ++i)
        temp[i] = data[i];

    unsigned n = size;
    while (n > 1)
    {
        for (unsigned i = 0; i < n / 2; ++i)
        {
            temp[i] = temp[2 * i] + temp[2 * i + 1];
        }
        if (n % 2 != 0)
        {
            temp[n / 2] = temp[n - 1];
            n = n / 2 + 1;
        }
        else
        {
            n = n / 2;
        }
    }
    float res = temp[0];
    delete[] temp;
    return res;
}

// 3. Алгоритм Кэхэна
float sum_kahan(float const data[], unsigned size)
{
    float sum = 0.0f, c = 0.0f;
    for (unsigned i = 0; i < size; ++i)
    {
        float y = data[i] - c;
        float t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}

// 4. FMA
float integrate_fma(float T, unsigned N, float dv, float v_min)
{
    float sum = 0.0f;
    const float pi = std::numbers::pi_v<float>;
    for (unsigned i = 0; i < N; ++i)
    {
        float v = v_min + i * dv;
        float pdf = (1.0f / std::sqrt(T * pi)) * std::exp(-(v * v) / T);
        float psi = std::abs(v);
        // Считает (psi * pdf) + sum за один шаг
        sum = std::fma(psi, pdf, sum);
    }
    return sum * dv;
}

// 5. Double
double integrate_double(float T, unsigned N, float dv, float v_min)
{
    double sum = 0.0;
    const double pi = std::numbers::pi_v<double>;
    for (unsigned i = 0; i < N; ++i)
    {
        double v = (double)v_min + (double)i * (double)dv;
        double pdf = (1.0 / std::sqrt((double)T * pi)) * std::exp(-(v * v) / (double)T);
        double psi = std::abs(v);
        sum += psi * pdf;
    }
    return sum * (double)dv;
}

int main()
{
    std::ofstream file("results.csv");
    file << "T,Method,Result,Error\n";

    float Ts[] = {0.1f, 1.0f, 100.0f, 10000.0f};
    const unsigned N = 100000;

    for (float T : Ts)
    {
        float v_min = -10.0f * std::sqrt(T + 1.0f);
        float v_max = 10.0f * std::sqrt(T + 1.0f);
        float dv = (v_max - v_min) / N;
        float ana = std::sqrt(T / std::numbers::pi_v<float>);

        // Подготовка данных для методов 1, 2, 3
        float *p = new float[N];
        for (unsigned i = 0; i < N; ++i)
        {
            float v = v_min + i * dv;
            p[i] = std::abs(v) * ((1.0f / std::sqrt(T * std::numbers::pi_v<float>)) * std::exp(-(v * v) / T));
        }

        // Запись всех методов
        file << T << ",Recursive," << sum_recursive(p, N) * dv << "," << std::abs(sum_recursive(p, N) * dv - ana) << "\n";
        file << T << ",Pairwise," << sum_pairwise(p, N) * dv << "," << std::abs(sum_pairwise(p, N) * dv - ana) << "\n";
        file << T << ",Kahan," << sum_kahan(p, N) * dv << "," << std::abs(sum_kahan(p, N) * dv - ana) << "\n";
        file << T << ",FMA," << integrate_fma(T, N, dv, v_min) << "," << std::abs(integrate_fma(T, N, dv, v_min) - ana) << "\n";
        file << T << ",Double," << integrate_double(T, N, dv, v_min) << "," << std::abs((float)integrate_double(T, N, dv, v_min) - ana) << "\n";

        delete[] p;
    }
    file.close();
    return 0;
}
