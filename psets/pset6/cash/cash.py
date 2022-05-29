from math import floor

def main():
    change = get_change()
    coins = [25, 10, 5, 1]
    ncoins = 0
    for i in range(len(coins)):
        if change == 0:
            break
        ncoin = floor(change / coins[i])
        change -= ncoin * coins[i]
        ncoins += ncoin
    print(f"{ncoins}")

def get_change():
    while True:
        try:
            change = 100 * float(input("Change owed: "))
            if change >= 0:
               break
            else:
               prompt_valid()
        except ValueError:
            prompt_valid()
    return change

def prompt_valid():
     print("Input non-negative number")

main()
    
