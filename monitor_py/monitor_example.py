import serial, os
import matplotlib.pyplot as plt
import matplotlib.animation as animation

ser = serial.Serial('COM4', 921600)
z = 0
w, h = 32,32
matrix = [[0 for x in range(w)] for y in range(h)]

def generate_data():
	while not ord(ser.read()) == 0:
		pass
	for y in range(h):
		for x in range(w):
			readByte = ser.read()
			if ord(readByte)==0:
				break
			else:
				matrix[y][x]=ord(readByte)
	os.system('cls')
	print('#'*128)
	print('\n\n'.join([''.join(['{:4}'.format(item) for item in row]) 
    for row in matrix]))
	return matrix

def update(data):
    mat.set_data(data)
    return mat
	
def data_gen():
    while True:
        yield generate_data()
		
fig, ax = plt.subplots()
mat = ax.matshow(generate_data(),  vmin=11, vmax=255)
ax.autoscale(False)
plt.colorbar(mat)
ani = animation.FuncAnimation(fig, update, data_gen)

plt.show()