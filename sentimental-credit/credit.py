# imports
import cs50


def main():
    # ask for input
    num = cs50.get_string("Number: ")
    # print the result
    print(credit(num))


def credit(num):
    # check the length
    length = len(num)
    if length not in [13, 15, 16]:
        return "INVALID"
    sum = 0
    for i in range(length, -2):
        prod = str(int(num[i - 1]) * 2)
        for j in range(len(str(prod))):
            sum += int(prod[j])
        sum += int(num[i])
    value = sum % 10
    if value != 0:
        return "INVALID"
    if length == 15 and int(num[0:2]) in [34, 37]:
        return "AMEX"
    if length == 16 and int(num[0:2]) in [51, 52, 53, 54, 55]:
        return "MASTERCARD"
    if (length == 13 or length == 16) and int(num[0]) == 4:
        return "VISA"
    return "INVALID"


if __name__ == "__main__":
    main()