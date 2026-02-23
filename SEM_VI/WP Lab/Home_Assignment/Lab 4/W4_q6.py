class Circle:
	def __init__(self, rad):
		self.rad = rad 
	def Area(self):
		area = 3.14 * self.rad * self.rad
		return round(area,2)
	def Perimeter(self):
		peri = 2* 3.14 * self.rad 
		return round(peri,2)

rad = int(input("enter radius"))
obj = Circle(rad)
area = obj.Area()
peri = obj.Perimeter()

print(f"Area: {area} and perimeter: {peri}")

