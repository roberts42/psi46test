stop
trc 50
tct 25   24@40MHz, 21@20MHz, 20@10MHz
ttk 30
wbc 21
cc   1

pixe : 0 0
dclear
mdelay 100

seq b1000
run
udelay 100

cald
cal 0 0 0
cal 1 0 0
cal 2 0 0
cal 3 0 0
cal 4 0 0
cal 5 0 0
cal 6 0 0
udelay 500
flush

seq b0110
run
udelay 200
seq b0110
run
udelay 200

seq b0001

dtrig
run
udelay 500
dreadd

dtrig
run
udelay 500
dreadd

flush