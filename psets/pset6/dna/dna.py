import csv
import sys

def main():
    
    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("USAGE: python dna.py databases/DATABASENAME.csv sequences/SEQUENCENAME.txt")

    # Read database into memory
    database = []
    db_filename = sys.argv[1]
    with open (db_filename, "r") as db_file:
        for line in csv.DictReader(db_file):
            database.append(line)
    
    # list of STRs found in database
    STRs = [*database[0]]   # Creates list of dictionary keys found in each database profile
    del STRs[0]             # Deletes 'name' key
    
    # Read DNA sequence into memory
    sq_filename = sys.argv[2]
    with open (sq_filename, "r") as sq_file:
        sequence = sq_file.read()
    
    # Create profile from sequence
    sequence_profile = {}
    for STR in STRs:
         sequence_profile[STR] = Max_STR_Repeats(sequence, STR) 
    # Try to match sequence derived profile to a database profile
    for profile in database:
        if(is_match(profile, sequence_profile, STRs)):
            sys.exit(f"{profile['name']}")
    
    # If no match is found
    print("No Match")
            

def Max_STR_Repeats(sequence, STR):
    # Largest number of STR repeats found
    max_repeats = 0
    # Used to count repeats of each STR occurance
    repeats = 0
    # Find first match in sequence (m = string index for first char of STR found in sequence) 
    m = sequence.find(STR)
    # While end of sequence is not reached.
    while m != -1:
        repeats = 1
        while repeats != 0:
            # Index first char after last matched STR
            m += len(STR)
            # If the sequence slice (length of STR) directly after last matched STR is also an STR match: increment repeats   
            if sequence[m:m + len(STR)] == STR:
                repeats += 1
            else:
                if repeats > max_repeats:
                    max_repeats = repeats
                repeats = 0
        # attempt to find next match
        m = sequence.find(STR, m) 
    
    # When end of sequence is reached, return max repeats 
    return max_repeats

def is_match(profile, sequence_profile, STRs):
    # Check if STR values match between profiles
    for STR in STRs:
        if int(profile[STR]) != sequence_profile[STR]:
            return False
    # If all STR values are equal:
    return True

main()

