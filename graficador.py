from wordcloud import WordCloud
import matplotlib.pyplot as plt
import sys

#ruta = "C:\\Users\\diego\\source\\repos\\Proyecto-Eda_Segment-Tree\\Proyecto-Eda_Segment-Tree\\query.txt"

ruta = sys.argv[1]
segundos = float(sys.argv[2])

datos = {}
file = open(ruta, "r")
for line in file:

    partes = line.strip().split()

    if (len(partes) < 2):
        continue

    datos[partes[0]] = int(partes[1])
    
wc = WordCloud(width=800, height=400, background_color="white")

if datos:
    wc.generate_from_frequencies(datos)

    plt.imshow(wc, interpolation="bilinear")
    plt.axis("off")
    plt.draw()

    if segundos == -1:
        plt.show()
    else:
        plt.pause(segundos)
        plt.close()
else:
    print("PYTHON: No hay datos para mostrar\n")