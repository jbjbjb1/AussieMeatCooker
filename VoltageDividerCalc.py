# Calculations for voltage divider

import matplotlib.pyplot as plt

# Inputs
t1 = -10
t2 = 110
r1 = 221.3
r2 = 1.870
vin = 5
steps = 1024

# Calculations
t_step = (t2 - t1) / steps
print(f'Minimum change in temperature is {t_step:.2f} C.')
t_mid = (t2 - t1) / 2
print(f'Mid temperature point is {t_mid:.2f} C.')

# Plot
def v_out(vin, r_range):
    pass
    return 0

r_range = range(r2, r1, 1)

plt.plot(r_range, v_out(vin, r_range))
plt.show()