import math

topEnd = 100000000000

for i in range(9000, topEnd):
    if not i % 1000:
        print(i) 
    num = i
    prime = True

    sqrt = math.ceil(math.sqrt(num))
    for ii in range(2,sqrt):
        if not (num % ii):
            prime = False
            break
    
    if not prime:
        continue


    index = 0
    for a in range(1,num):
        fPer = True
        seed = 1
        for iii in range(num-1):
            seed = (seed * a) % num
            if seed == 1 and iii != num-2:
                fPer = False
                break
        if fPer:
            index += 1
    
    full = [0] * index
    index = 0
    for a in range(1,num):
        fPer = True
        seed = 1
        for iii in range(num-1):
            seed = (seed * a) % num
            if seed == 1 and iii != num-2:
                fPer = False
                break
        if fPer:
            full[index] = a
            index += 1
    
    if num - 1 in full:
        print(num)
        print(full)
