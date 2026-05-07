# import pandas as pd


# df = pd.read_csv('results.csv')
# # Ошибка
# pivot_df = df.pivot(index='T', columns='Method', values='Error')
    
# print("\nСРАВНЕНИЕ ОШИБОК РАЗНЫХ МЕТОДОВ :")
# print(pivot_df.to_string())

# # Cами значения
# pivot_res = df.pivot(index='T', columns='Method', values='Result')
# print("\nПОЛУЧЕННЫЕ ЗНАЧЕНИЯ:")
# print(pivot_res.to_string())

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 1. Загрузка данных (убедитесь, что файл results.csv в той же папке)
try:
    df = pd.read_csv('results.csv')
except FileNotFoundError:
    print("Файл results.csv не найден. Сначала запустите C++ код.")
    exit()

# 2. Расчет относительной ошибки в %: (Результат / Аналитический - 1) * 100
# В вашем C++ коде аналитический ответ можно вычислить обратно из колонки Error
# Но проще рассчитать его заново здесь:
df['Analytical'] = np.sqrt(df['T'] / np.pi)
df['Rel_Error_Pct'] = (df['Result'] / df['Analytical'] - 1) * 100

# 3. Подготовка структуры для графика
methods = df['Method'].unique()
temps = df['T'].unique()

x = np.arange(len(temps))  # Метки для температур
width = 0.15               # Ширина одного столбика

fig, ax = plt.subplots(figsize=(12, 7))

# Рисуем столбики для каждого метода
for i, method in enumerate(methods):
    method_data = df[df['Method'] == method]
    ax.bar(x + i * width, method_data['Rel_Error_Pct'], width, label=method)

# 4. Оформление
ax.set_xlabel('Температура (T)')
ax.set_ylabel('Относительная ошибка (%)')
ax.set_title('Сравнение точности методов суммирования при интегрировании')
ax.set_xticks(x + width * (len(methods) - 1) / 2)
ax.set_xticklabels(temps)
ax.legend()
ax.grid(axis='y', linestyle='--', alpha=0.7)

# Если ошибки слишком маленькие, имеет смысл включить логарифмическую шкалу:
# ax.set_yscale('symlog') 

plt.tight_layout()
plt.show()