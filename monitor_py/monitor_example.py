import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

ser = serial.Serial('COM4', 921600)

w, h = 16,16
matrix = [[0 for x in range(w)] for y in range(h)]

def generate_data():
	while not ord(ser.read()) == 0:
		pass
	for y in range(h):
		for x in range(w):
			readByte = ser.read()
			if  ord(readByte)==0:
				break
			else:
				matrix[y][x]=ord(readByte)
	return matrix

def update(data):
    mat.set_data(data)
    return mat
	
def data_gen():
    while True:
        yield generate_data()
		
fig, ax = plt.subplots()
mat = ax.matshow(generate_data(),  vmin=0, vmax=100)
ax.autoscale(False)
plt.colorbar(mat)
ani = animation.FuncAnimation(fig, update, data_gen)

plt.show()