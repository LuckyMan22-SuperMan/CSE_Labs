print("enter dim for A: ")
m = int(input("m: "))
n = int(input("n: "))

print("enter elements A: ")
A = [[int(input()) for j in range(n)] for i in range(m)]

print("enter dim for B")
m = int(input("m: "))
n = int(input("n: "))

print("enter elements B: ")
B = [[int(input()) for j in range(n)] for i in range(m)]



m = len(A)
n = len(A[0])
p = len(B[0])


C = [[0 for _ in range(p)] for _ in range(m)]


for i in range(m):
    for j in range(p):
        for k in range(n):
            C[i][j] += A[i][k] * B[k][j]


print("Resultant Matrix:")
for row in C:
    print(row)