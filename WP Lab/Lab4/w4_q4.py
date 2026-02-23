res = []
array = []

def combi(idx: int, n:int, temp: list):
	if idx == n:
		temp.sort()
		res.append(list(temp))
		return;
	temp.append(array[idx])
	combi(idx+1, n, temp)
	temp.pop()
	combi(idx+1, n, temp)
	
	
n = int(input("Enter array length: "))
for i in range(0, n):
	st= int(input("Enter numbers: "))
	array.append(st)
	

combi(0,n, [])
for l in res:
	print(l)
