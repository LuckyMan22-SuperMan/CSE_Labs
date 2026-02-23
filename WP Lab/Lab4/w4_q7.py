class StringClass:
	def __init__(self):
		print("initialised")
		
	def Get_String(self):
		self.ha = input("Enter a string: ")
		
	def Print_String(self):
		print(f"initialised string:{self.ha.upper()}")
		
		
yolo = StringClass()
yolo.Get_String()
yolo.Print_String()
