import random

def input_massiv(size):

    listString = []
    for count in range(size):
        listEmpty = []
        stringEmpty = ""
        for countTwo in range(size):
            listEmpty.append(str(random.randint(-10, 10)))
        stringEmpty = " ".join(listEmpty)
        listEmpty.clear()
        listString.append(stringEmpty)
    return listString

while True:
    try:
        size = int(input("Введите размер массивов: "))
    except ValueError:
        print("Введите целое число!")
    else:
        break
nameFile = input("Введите название тестового файла: ")

listStringOne = []
listStringOne = input_massiv(size)
listStringTwo = []
listStringTwo = input_massiv(size)
with open(nameFile, "w") as fileWrite:
    fileWrite.write(str(size) + "\n")
    fileWrite.write("\n")
    for string in listStringOne:
        fileWrite.write(string + "\n")
    fileWrite.write("\n")
    for string in listStringTwo:
        fileWrite.write(string + "\n")
    fileWrite.close()
