with open("file.txt", "r") as inn:
    data = inn.read()

with open("output.txt", "w") as outt:
    outt.write(data[::-1])
