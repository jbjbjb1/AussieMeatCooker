# Calculations for voltage divider

import matplotlib.pyplot as plt
import numpy as np

# Next steps:
# * Convert to Class, use json to define inputs required

# Inputs
t1 = -10
t2 = 110
r1 = 221.3
r2 = 1.870
v_s = 5
steps = 1024

A = 1.839472271  * 10** -3
B = 0.6807883650 * 10** -4
C = 6.022892289  * 10** -7

# Calculations
t_step = (t2 - t1) / steps
print(f'Minimum change in temperature is {t_step:.2f} C.')
t_mid = (t2 - t1) / 2
print(f'Mid temperature point is {t_mid:.2f} C.')

# Plot
def v_o_r(v_s, r_range, r):
    """ Calculates V_o = f(R), from resistance of thermister """
    return v_s * r / (r_range + r)

def t_o(r_range):
    """ Returns temperature in Kelvin from Steinhart-Hart model. """
    r_range = r_range * 1000    # convert from kOhm to Ohm
    return 1/(A + B*np.log(r_range) + C*(np.log(r_range))**3) - 273.15

fig, axs = plt.subplots(2, figsize=(8, 6))
fig.subplots_adjust(
    top=0.88,
    bottom=0.11,
    left=0.11,
    right=0.77,
    hspace=0.2,
    wspace=0.2
)

r_range = np.arange(r2, r1, 1)

for x in range(10, 60, 10):
    axs[0].plot(v_o_r(v_s, r_range, x), r_range, label=f'R_o {x} (kOhm)')
axs[0].set_title('Voltage out against resistance')
axs[0].set(xlabel='V_o (V)', ylabel='R0 (kOhm)')
axs[0].legend(loc='upper left', bbox_to_anchor=(1.02, 1))

for x in range(10, 60, 10):
    axs[1].plot(t_o(r_range), v_o_r(v_s, r_range, x), label=f'R_o {x} (kOhm)')
axs[1].set_title('Temperature against voltage out')
axs[1].set(xlabel='Temp (C)', ylabel='V_o (V)')
axs[1].legend(loc='upper left', bbox_to_anchor=(1.02, 1))

"""
axs[2].plot(t_o(r_range), r_range)
axs[2].set_title('Temperature vs thermister resistance')
axs[2].set(xlabel='Temp (C)', ylabel='R0 (kOhm)')
"""

fig.tight_layout()
plt.show()