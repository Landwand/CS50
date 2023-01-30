from cs50 import get_int

# using CS50's get_int per specs
# blocks must be from 1 to 8, inclusive
while True:
    height = get_int('input height: ')
    if height >= 1 and height <= 8:
        break


# printing pyramid
i = 1
while i < (height + 1):
    if i == 1:
        print((height-1)*' ', '#', '  ', '#')
    else:
        print((height-i)*(' '), i*('#'), '  ', i*('#'))
    i += 1
