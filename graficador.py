import time
import os
from wordcloud import WordCloud
import matplotlib.pyplot as plt


ruta = "C:\\Users\\diego\\source\\repos\\Proyecto-Eda_Segment-Tree\\Proyecto-Eda_Segment-Tree\\query.txt"

mod_time = 0
plt.ion()

fig, ax = plt.subplots()

while True:
    mod = os.path.getmtime(ruta)

    if (mod != mod_time):
        datos = {}
        file = open(ruta, "r")
        for line in file:
            palabra, repeticiones = line.strip().split()
            datos[palabra] = int(repeticiones)

        wc = WordCloud(width=800, height=400, background_color="white")
        wc.generate_from_frequencies(datos)

        ax.clear()
        ax.imshow(wc, interpolation="bilinear")
        ax.axis("off")
        plt.draw()
        plt.pause(0.1)

        plt.show()

        mod_time = mod