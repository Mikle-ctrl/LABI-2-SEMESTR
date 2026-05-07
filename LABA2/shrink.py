import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv('comb_shrink_study.csv')
df['time_ms*'] = df['time_ms'] ** -1

plt.figure(figsize=(10, 6))
plt.plot(df['shrink'], df['time_ms*'],marker='o' , linestyle='-', color='purple')
    
    

plt.title('Эффективность Comb Sort в зависимости от коэффициента Shrink')
plt.xlabel('Коэффициент Shrink')
plt.ylabel('Время выполнения 1/(мс)')
plt.grid(True, alpha=0.3)
plt.show()
