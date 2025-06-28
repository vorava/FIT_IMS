import os
import numpy as np
import matplotlib.pyplot as plt

command = "./bitva --external --silent"


fig = plt.figure(figsize=(8,10))
ax = fig.add_subplot(2,1,1)
ax.set_xlabel("Tanky")
ax.set_ylabel("Katapulty")

pesaci = list()
vojaci = list()
win = list()
#vojaci armada 1, pesaci armada 2
for i in range(1,1001):
	pesaci.append(1000-i)
	vojaci.append(i)
	stream = os.popen(command + " -t " + str(i) + " -k 0 -k " + str((1000-i)))
	out = stream.read()
	if "Armada 2" in out.split("\n")[1]:
		win.append(0)
	elif "Armada 1" in out.split("\n")[1]:
		win.append(1)
	else:
		print("remiza" + str(i))


pesaci = np.array(pesaci, dtype=float)
vojaci = np.array(vojaci, dtype=float)
win = np.array(win)


pesaci[win == 0] = np.nan
vojaci[win == 1] = np.nan


for i in range(len(vojaci[win==0])):
    vojaci[i]=1000-i

ax.plot(pesaci, c="tab:red", linewidth=2)
ax.plot(vojaci, linewidth=2)

y=pesaci[win == 1].shape
x=vojaci[win == 0].shape

# Get current tick locations and append 271 to this array
x_ticks = np.append([0,400,600 ,800,1000], x)

# Set xtick locations to the values of the array `x_ticks`
ax.set_xticks(x_ticks)

# Get current tick locations and append 271 to this array
y_ticks = np.append([0,200,400,600,1000], y)

# Set xtick locations to the values of the array `x_ticks`
ax.set_yticks(y_ticks)

ax.plot(x,y, marker="o", c="tab:blue")

plt.tight_layout()

plt.show()

