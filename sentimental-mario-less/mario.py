# TODO
# some imports
import cs50


def main():
    # ask user for input
    while True:
        n = cs50.get_int("Height: ")
        if 1 <= n <= 8:
            break
    # call the main function for input
    marioLess(n)


def marioLess(n):
    for i in range(n):
        print(" " * (n-i-1) + "#" * (i + 1))


if __name__ == "__main__":
    main()