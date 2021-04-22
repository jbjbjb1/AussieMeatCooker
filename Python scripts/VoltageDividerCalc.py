# Calculations for voltage divider

import matplotlib.pyplot as plt
import numpy as np

# Next steps:
# * Convert to Class, use json to define inputs required

# Inputs
t1 = 10
t2 = 340
r1 = 82.47
r2 = 0.0313
#r_suggested = (r1*r2)**0.5
r_suggested = 1
v_s = 3.24
steps = 4096

A = 0.9482846445  * 10** -3
B = 1.952744345 * 10** -4
C = 2.570293116  * 10** -7


def v_o_r(v_s, r_range, r):
    """ Calculates V_o = f(R), from resistance of thermister """
    return v_s * r / (r_range + r)

def t_o(r_range):
    """ Returns temperature in Kelvin from Steinhart-Hart model. """
    r_range = r_range * 1000    # convert from kOhm to Ohm
    return 1/(A + B*np.log(r_range) + C*(np.log(r_range))**3) - 273.15

# Calculations
t_step = (t2 - t1) / steps
print(f'Minimum change in temperature is {t_step:.2f} C.')
t_mid = (t2 - t1) / 2
print(f'Mid temperature point is {t_mid:.2f} C.')
print(f'The low/high voltage reading in will be {t1}C = {v_o_r(v_s, r1, r_suggested):.3f} V & {t2} C = {v_o_r(v_s, r2, r_suggested):.3f} V when using resistance {r_suggested:.3f} kOhm.')

# Plot
plot_range = np.linspace(1, 1, 1)

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

for x in plot_range:
    axs[0].plot(v_o_r(v_s, r_range, x), r_range, label=f'R_o {x} (kOhm)')
axs[0].set_title('Voltage out against resistance')
axs[0].set(xlabel='V_o (V)', ylabel='R0 (kOhm)')
axs[0].legend(loc='upper left', bbox_to_anchor=(1.02, 1))

for x in plot_range:
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