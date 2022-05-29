def main():
    global height
    height = get_height()
    for level in range(height):
        print_spaces(level)
        print_blocks(level)
        print("  ", end="")
        print_blocks(level)
        print_spaces(level)
        print()

def get_height():
    while True:
        try:
            n = int(input("Height: "))
            if n > 0 and n < 9:
                break
            prompt_valid()
        except ValueError:
            prompt_valid()
    return n

def prompt_valid():
    print ("INVALID: Height must be a positive integer less than 9.")

def print_spaces(level):
    print(" " * ((height - 1) - level), end="") # (height - 1) = number of spaces at level 0

def print_blocks(level):
    print("#" * (1 + level), end="") # 1 = number of blocks at level 0

main()
