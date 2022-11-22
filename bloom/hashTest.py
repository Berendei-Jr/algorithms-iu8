primes = [2, 3]
Mer = 2147483647
#set 1000 0.274172

key = 14678372060444760201
size = 2693

for i in range(2):
    print((((i+1)%Mer*key%Mer+primes[i]%Mer)%Mer)%size)