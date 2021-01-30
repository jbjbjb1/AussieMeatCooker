# This script plots the temperatures from app 'Serial USB terminal'

import matplotlib.dates as mdates
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

class BbqPlot:
    def __init__(self, file):
        # Convert datafile into pandas dataframe
        data = []
        with open(file, 'r') as f:
            for line in f:
                time = pd.to_datetime(line[:12], format='%H:%M:%S.%f')
                air = float(line.split('C')[0].split(':')[-1].strip())
                prb1 = float(line.split(':')[-1][:-2].strip())
                data.append({'Time': time, 'Air': air, 'Prb1': prb1})
                #print({'Time': time, 'Air': air, 'Prb1': prb1})
        self.df = pd.DataFrame(data)

    def plot(self):
        fig, ax = plt.subplots()
        ax.plot(self.df['Time'], self.df['Air'], 'r')
        ax.plot(self.df['Time'], self.df['Prb1'], 'b')
        fig.autofmt_xdate()
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
        ax.set_title('Time Temperature Plot')
        plt.show()



a = BbqPlot(r"Python scripts\sample data\serial_20210130_085518.txt")
a.plot()
