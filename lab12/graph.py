import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('mpi.csv', sep=';')

for c in df.drop('elements', axis=1).columns:
    plt.plot(df['elements'], df[c], label=c)

plt.xscale('symlog')
plt.yscale('symlog')
plt.xlabel('elements')
plt.ylabel('time (s)')
plt.legend()
plt.savefig('graph.png', dpi=300)
