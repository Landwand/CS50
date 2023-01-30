from cs50 import get_string


# Coleman-Liau index is computed as 0.0588 * L - 0.296 * S - 15.8, where L is the average number of letters per 100 words in the text, 
# and S is the average number of sentences per 100 words in the text.


def print_grade(grade_index):
    if grade_index < 1:
        print('Before Grade 1\n')
    elif grade_index >= 1 and grade_index <= 16:
        print(f'Grade {grade_index}\n')
    else:
        print('Grade 16+\n')


# Coleman-Liau index
def readability(letters, words, sentences):
    L = letters / words * 100
    S = sentences / words * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    rounded_index = round(index)
    return rounded_index


# let CS50 handle this
text = get_string('Text: ')

letters = 0
words = 0
sentences = 0

# checking the type of each character
for word in text:
    for character in word:
        if letters == 1:
            words += 1
        if character.isalpha():
            letters += 1
        elif character.isspace():
            words += 1
        elif character == '.' or character == '?' or character == '!':
            sentences += 1


# find the grade index
grade_index = readability(letters, words, sentences)
print_grade(grade_index)