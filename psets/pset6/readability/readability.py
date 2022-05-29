from sys import exit
# Define key chars
END_PUNC = ['.', '!', '?']

sample = input("Text: ")
 # Count letters, words, and sentences
nLetters = 0
nWords = 0
nSentences = 0
# Cycle through each char
for i in range(len(sample)):
    # Count letter if char is alpha 
    if sample[i].isalpha():
        nLetters += 1
    # Count word if char is a space or end of string, and there isn't already a space before it 
    if sample[i] == ' ' or i == len(sample) - 1 and sample[i-1] != ' ':
        nWords += 1
    # Count sentence if char is a sentence ending punctuation
    if sample[i] in END_PUNC and sample[i-1] not in END_PUNC:
        nSentences += 1
# Calculate L and S
L = 100 * nLetters / nWords
S = 100 * nSentences / nWords
# Calculate Coleman-Liau Index
index = round(0.0588 * L - 0.296 * S - 15.8)
# Print grade level
if index < 1:
    exit("Before Grade 1")
elif index > 16:
    exit("Grade 16+")
print(f"Grade {index}")

