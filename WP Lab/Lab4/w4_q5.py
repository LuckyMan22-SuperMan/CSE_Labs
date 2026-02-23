array = []
n = int(input("Enter array length: "))
print("Enter numbers of the array: ")
for i in range(0, n):
	st= int(input())
	array.append(st)
	
dict ={}
res=[]

sum1 = int(input("Enter sum: "))

for i in range(n):
	sec = sum1 - array[i]
	if sec in dict:
		res.append({array.index(dict.get(sec)), array.index(sec)})
	else:
		dict[array[i]] = sec
		
		
for val1, val2 in res:
	print(f"pair is: {val1} and {val2}") 
