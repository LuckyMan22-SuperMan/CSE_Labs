class String:
	def __init__(self, string):
		self.string = string
	def check(self):
		st = []
		for c in string:
			if c == '(' or c == '[' or c == '{':
				st.append(c)


			elif len(st) != 0 and (
				(c == ')' and st[-1] == '(') or
				(c == ']' and st[-1] == '[') or
				(c == '}' and st[-1] == '{')
				):
				st.pop()


			else:
				return False 


		return len(st) == 0

string = input("enter a string")
obj = String(string)
verdict = obj.check()
if verdict is True:
	print("balanced")
else:
	print("unbalanced")
