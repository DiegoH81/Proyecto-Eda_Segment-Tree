from wordcloud import WordCloud
import matplotlib.pyplot as plt
import sys

#ruta = "C:\\Users\\diego\\source\\repos\\Proyecto-Eda_Segment-Tree\\Proyecto-Eda_Segment-Tree\\query.txt"

ruta = sys.argv[1]
segundos = float(sys.argv[2])

datos = {}
file = open(ruta, "r")
for line in file:
    palabra, repeticiones = line.strip().split()
    datos[palabra] = int(repeticiones)

wc = WordCloud(width=800, height=400, background_color="white")
wc.generate_from_frequencies(datos)

plt.imshow(wc, interpolation="bilinear")
plt.axis("off")
plt.draw()

plt.pause(segundos)
plt.close()