import math

bottomEnd = 32668
topEnd = 32768

for num in range( bottomEnd, topEnd ):
    prime = True

    sqrt = math.ceil( math.sqrt( num ) ) + 1
    for ii in range( 2, sqrt ):
        if ( num % ii ) == 0:
            prime = False
            break
    
    if prime:
       print( num )

#FOUND PRIME IS 32,749

num = 32749
FPMC = []
for a in range(1,num):
    fPer = True
    seed = 1
    for iii in range(num-1):
        seed = (seed * a) % num
        if seed == 1 and iii != num-2:
            fPer = False
            break
    if fPer and ((num % a) < (num / a)):
        FPMC.append(a)

print(FPMC)
print(len(FPMC))