n = int(input("enter size of list: "))
lis = []

for i in range(n):
    lis.append(int(input()))

x = lis[0]
for i in range(1, n):
    x = min(x, lis[i])

print(f"smallest found: {x}")
