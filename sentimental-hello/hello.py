# TODO
def main():
    # calling a function that asks for user input and prints the hello thing
    hello()


def hello():
    """This function does exactly that"""
    name = input("What's your name?\n")
    print(f"hello, {name}")


if __name__ == "__main__":
    main()