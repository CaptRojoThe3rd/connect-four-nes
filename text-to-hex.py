LETTERS = list("ACEGILMNOPRSTUVWY.'!12")
data = {}
for i in range(len(LETTERS)):
    data[LETTERS[i]] = 1 + i
while True:
    text = list(str(input("ENTER TEXT: ")))
    output = ""
    for i in text:
        if i == " ":
            output = f"{output}, 0"
        else:
            output = f"{output}, {hex(data[i])}"
    print(output)