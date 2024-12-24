# Calculations for voltage divider

import matplotlib.pyplot as plt
import numpy as np

# Next steps:
# * Convert to Class, use json to define inputs required

v_s = 3.28
steps = 4096

"""
# Meat sensor
# Inputs
t1 = -10
t2 = 110
r1 = 248.1
r2 = 2.62
#r_suggested = (r1*r2)**0.5
r_suggested = 25

A = 0.8975771660  * 10** -3
B = 2.067767238 * 10** -4
C = 1.742614938  * 10** -7
"""

# ESP32 paramaters
V_low = 0.15
V_high = 3.15
steps = 4076 - 18

# Air Sensor
## Inputs
t1 = 10
t2 = 340
r1 = 82.47
r2 = 0.0313
#r_suggested = (r1*r2)**0.5
r_suggested = 1                     # kOhm
## Steinhart-Hart model coeff.
A = 0.9482846445  * 10** -3
B = 1.952744345 * 10** -4
C = 2.570293116  * 10** -7


def v_o_r(v_s, r_range, r):
    """ Calculates V_o = f(R), from resistance of thermister. """
    return v_s * r / (r_range + r)

def t_o(r_range):
    """ Returns T = f(R) temperature in Celsius from Steinhart-Hart model. """
    r_range = r_range * 1000    # convert from kOhm to Ohm
    t_range = 1/(A + B*np.log(r_range) + C*(np.log(r_range))**3) - 273.15
    return t_range

def t_o_v(v_m, r):
    """ Calculates T = f(V_in), from voltage to input of ADC. """
    r_range = (v_s * r / v_m) - r
    t_range = t_o(r_range)
    return t_range

# Calculations
print(f'ESP32 limits using a {r_suggested} kOhm resistor are {V_low} V: {t_o_v(V_low, r_suggested):.1f} C & {V_high} V: {t_o_v(V_high, r_suggested):.1f} C')
min_step = (t_o_v(V_high, r_suggested) - t_o_v(V_low, r_suggested)) / steps
print(f'Min temperature step is {min_step:.2f} C.')

# Plot
plot_range = np.linspace(r_suggested, r_suggested, 1)

fig, axs = plt.subplots(2, figsize=(8, 6))
fig.subplots_adjust(
    top=0.88,
    bottom=0.11,
    left=0.11,
    right=0.77,
    hspace=0.2,
    wspace=0.2
)

r_range = np.arange(r2, r1, 0.001)  # plot the range that the sensor was calibrated over

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

# Add to the above plot a dot at lower and upper ESP32 limits
axs[1].plot(t_o_v(V_low, r_suggested), V_low, 'o', color='green')
axs[1].plot(t_o_v(V_high, r_suggested), V_high, 'o', color='green')

fig.tight_layout()
plt.show()