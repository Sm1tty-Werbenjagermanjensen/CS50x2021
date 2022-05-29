from math import log10
from sys import exit
# Indecies for leading digit(s) of valid cards.
VISA = 4
MC = [51, 52, 53, 54, 55]
AMEX = [34, 37]

# Indecies for valid card length (number of digits).
VISA_LEN = [13, 16]
MC_LEN = 16
AMEX_LEN = 15

def main():
    n = get_credit_number() # Also creates global variable: n_len  (number of digits in n) 
    if not luhn_check(n):
        invalid()
    # Check if n qualifies as a valid: Visa, Mastercard, or American Express.
    n_leading_digit = get_digit(n, n_len - 1) # leading digit of n
    if n_leading_digit == VISA and n_len in VISA_LEN:
        print("VISA")
        exit()
    n_leading_2_digits = n_leading_digit * 10 + get_digit(n, n_len - 2) # leading 2 digits of n
    if n_leading_2_digits in MC and n_len == MC_LEN:
        print("MASTERCARD")
        exit()
    elif n_leading_2_digits in AMEX and n_len == AMEX_LEN:
        print("AMEX")
        exit()
    else:
        invalid()

def get_credit_number():
    while True:
        try:
            n = int(input("Number: "))
            global n_len
            n_len = int(log10(n)) + 1
            if n_len == 13 or 15 or 16:
                break
            else:
                prompt_reinput()
        except ValueError:
            prompt_reinput()
    return n

def prompt_reinput():
    print("Card number must be 13, 15, or 16 decimal digits.")
    
def luhn_check(n): # Checks if n passes Luhn's Algorithim
    sum = 0
    # Step 1:
    for i in range(1, n_len, 2):
        x = get_digit(n, i) * 2
        sum += (x % 10) + (x // 10)
    # Step 2:
    for i in range(0, n_len, 2):
        sum += get_digit(n, i)
    # Step 3:
    if sum % 10 == 0:
        return True
    else:
        return False

def get_digit(n, d): # "d" is the dth digit in "number" (d == 0 is the 10^0 digit & d == (n_len - 1) is the leading digit) 
    
    return n // 10**d % 10

def invalid():
    print("INVALID")
    exit()
main()
