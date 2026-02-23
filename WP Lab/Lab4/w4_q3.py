
array = []
n = int(input("Enter number of strings: "))
for i in range(0, n):
	st= int(input("Enter string: "))
	array.append(st)
	
	
array.sort()
			
for i in range(0, n):
	print(array[i])
