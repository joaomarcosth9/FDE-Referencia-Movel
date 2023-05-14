# plot graphics comparing the two outputs

import matplotlib.pyplot as plt

if __name__ == "__main__":
    # read files out1 and out2
    input_sizes = []
    out1 = []
    out2 = []
    with open("out1", "r") as f1:
        for line in f1:
            size, time = line.split(',')
            input_sizes.append(int(size))
            out1.append(float(time))
    with open("out2", "r") as f2:
        for line in f2:
            size, time = line.split(',')
            out2.append(float(time))

    percentages = []
    for i in range(len(out1)):
        percentages.append((out2[i] - out1[i]) / out1[i] * 100)
    print(percentages)
    # plot the two outputs
    plt.plot(input_sizes, out1, label="Com referência móvel", color="green", linestyle="solid", marker="")
    plt.plot(input_sizes, out2, label="Sem referência móvel", color="red", linestyle="solid", marker="")
    plt.xticks([i for i in range(1000, 9001, 1000)])
    plt.yticks([i for i in range(0, 150, 10)])
    plt.xlabel("Tamanho da entrada")
    plt.ylabel("Tempo (ms)")
    plt.legend()
    plt.savefig("plot.png")
    plt.show()
