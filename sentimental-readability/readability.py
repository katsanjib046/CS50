# some imports
import cs50


def main():
    # ask for user input
    text = cs50.get_string("Text: ")
    # calculate the index
    index = index_calc(text)
    # print the grade
    print(grade(index))


def index_calc(text):
    L = 0
    for i in text:
        if i.isalpha():
            L += 1
    S = text.count('?') + text.count('.') + text.count('!')
    W = 1 + text.count(' ')  # addding one for the last word
    L = 100 * (L / W)
    S = 100 * (S / W)
    index = 0.0588 * L - 0.296 * S - 15.8
    return index


def grade(index):
    if 1 < index < 16:
        return f"Grade {round(index)}"
    elif index <= 1:
        return "Before Grade 1"
    else:
        return "Grade 16+"


main()