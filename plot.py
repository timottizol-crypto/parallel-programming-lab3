import pandas as pd
import matplotlib.pyplot as plt
import os

# Создаем папку для картинок
os.makedirs('img', exist_ok=True)
data = {
    'N': [200]*4 + [400]*4 + [800]*4 + [1200]*4 + [1600]*4 + [2000]*4,
    'P': [1, 2, 4, 8] * 6,
    'time': [
        0.013470, 0.007290, 0.004220, 0.003290,
        0.110064, 0.057217, 0.030700, 0.016908,
        0.563287, 0.287196, 0.175030, 0.152368,
        1.906070, 0.962661, 0.716750, 0.545851,
        8.316404, 4.230258, 3.361641, 1.846467,
        20.626423, 10.281187, 5.421312, 3.023195
    ]
}
df = pd.DataFrame(data)

# Считаем ускорение и эффективность
speedup_data = []
eff_data = []
for n in df['N'].unique():
    subset = df[df['N'] == n]
    t1 = subset[subset['P'] == 1]['time'].values[0]
    for _, row in subset.iterrows():
        s = t1 / row['time']
        e = s / row['P']
        speedup_data.append({'N': n, 'P': row['P'], 'S': s})
        eff_data.append({'N': n, 'P': row['P'], 'E': e})
df_s = pd.DataFrame(speedup_data)
df_e = pd.DataFrame(eff_data)

# Настройка стиля
plt.style.use('seaborn-v0_8-whitegrid')

# График 1: Время (лог-лог)
plt.figure(figsize=(10, 6))
for p in sorted(df['P'].unique()):
    subset = df[df['P'] == p]
    plt.plot(subset['N'], subset['time'], marker='o', linewidth=2, label=f'{p} процессов')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Размер матрицы N', fontsize=12)
plt.ylabel('Время выполнения, сек', fontsize=12)
plt.title('Зависимость времени выполнения от размера матрицы', fontsize=14)
plt.legend()
plt.tight_layout()
plt.savefig('img/time_plot.png', dpi=300)
plt.close()

# График 2: Ускорение
plt.figure(figsize=(10, 6))
for n in sorted(df_s['N'].unique()):
    subset = df_s[df_s['N'] == n]
    plt.plot(subset['P'], subset['S'], marker='s', linewidth=2, label=f'N={n}')
# Идеальное ускорение
plt.plot([1, 8], [1, 8], 'k--', alpha=0.5, label='Идеальное ускорение')
plt.xlabel('Число процессов P', fontsize=12)
plt.ylabel('Ускорение S = T1 / Tp', fontsize=12)
plt.title('Ускорение от числа процессов', fontsize=14)
plt.legend()
plt.tight_layout()
plt.savefig('img/speedup_plot.png', dpi=300)
plt.close()

# График 3: Эффективность
plt.figure(figsize=(10, 6))
for n in sorted(df_e['N'].unique()):
    subset = df_e[df_e['N'] == n]
    plt.plot(subset['P'], subset['E'], marker='^', linewidth=2, label=f'N={n}')
plt.axhline(y=1.0, color='k', linestyle='--', alpha=0.5, label='Идеал E=1')
plt.xlabel('Число процессов P', fontsize=12)
plt.ylabel('Эффективность E = S / P', fontsize=12)
plt.title('Эффективность распараллеливания', fontsize=14)
plt.ylim(0, 1.1)
plt.legend()
plt.tight_layout()
plt.savefig('img/efficiency_plot.png', dpi=300)
plt.close()

print("Графики успешно созданы в папке img/")