# some imports
import cs50


def main():
    # get input from the user
    while True:
        change = cs50.get_float("Change owed: ")
        if change > 0:
            break
    # call the coins function
    # change is in dollar's value, not in cents
    print(coins(change))


def coins(change):
    # change into number of cents
    change = int(change * 100)
    coin = 0
    while change > 0:
        if change >= 25:
            quarters = change // 25
            change -= quarters * 25
            coin += quarters
        elif change >= 10:
            dimes = change // 10
            change = change - dimes * 10
            coin += dimes
        elif change >= 5:
            nickels = change // 5
            change = change - nickels * 5
            coin += nickels
        else:
            coin += change
            change = 0
    return coin


main()