import pandas as pd


df = pd.read_csv('results.csv')
# Ошибка
pivot_df = df.pivot(index='T', columns='Method', values='Error')
    
print("\nСРАВНЕНИЕ ОШИБОК РАЗНЫХ МЕТОДОВ :")
print(pivot_df.to_string())

# Cами значения
pivot_res = df.pivot(index='T', columns='Method', values='Result')
print("\nПОЛУЧЕННЫЕ ЗНАЧЕНИЯ:")
print(pivot_res.to_string())

