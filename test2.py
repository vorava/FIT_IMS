import os
import numpy as np
import matplotlib.pyplot as plt

command = "./bitva --silent"


fig = plt.figure(figsize=(8,10))
ax = fig.add_subplot(2,1,1)
ax.set_ylabel("Vítězství")
ax.set_xlabel("Budoucnost")

budoucnost = list()
win = list()
#vojaci armada 1, pesaci armada 2
for i in range(870,971):
	budoucnost.append(i)
	stream = os.popen(command + " -p 15000 -r 2000 -k 3000 -a 0 -s 0 -d 0 -a " + str(i) + " -s " + str(i) + " -d " + str(i))
	out = stream.read()
	if "Armada 2" in out.split("\n")[1]:
		win.append(1)
	elif "Armada 1" in out.split("\n")[1]:
		win.append(0)
	else:
		print("remiza" + str(i))


budoucnost = np.array(budoucnost, dtype=float)
win = np.array(win)
y_ticks = np.array([0,1])

# Set xtick locations to the values of the array `x_ticks`
ax.set_yticks(y_ticks)


# Get current tick locations and append 271 to this array
x_ticks = np.append([870, 900, 920, 926, 930, 934, 970], 928)

# Set xtick locations to the values of the array `x_ticks`
ax.set_xticks(x_ticks)

ax.plot(budoucnost, win, c="tab:blue")

plt.tight_layout()

plt.show()

