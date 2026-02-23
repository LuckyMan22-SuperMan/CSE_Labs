class laddu:
	def __init__ (self):
		print("initialised")
	
	def getladdu(self,x:int,n:int):
		d=x**n
		print(f"Your power ans is {d}")
	
haha=laddu()
x=int(input("Enter base: "))
y=int(input("Enter power to be raised: "))
haha.getladdu(x,y) 
	
