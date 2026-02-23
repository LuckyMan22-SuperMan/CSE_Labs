class RevString:
		
	def rev(self,sent):
		self.sent = sent
		words = self.sent.split()
		words.reverse()
		return " ".join(words)


sent = input("enter a sentence")
obj = RevString()
res = obj.rev(sent)

print(f"final stirng {res}")