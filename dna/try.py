import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    # first list will represent the keys
    dataD = []
    with open(sys.argv[1]) as data:
        reader = csv.reader(data)
        for line in reader:
            dataD.append(line)
    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as seq:
        seqReader = csv.reader(seq)
        for line in seqReader:
            sequence = line[0]

    # TODO: Find longest match of each STR in DNA sequence
    seqLengths = []
    for subsequence in dataD[0][1:]:
        seqLengths.append(str(longest_match(sequence, subsequence)))

    # TODO: Check database for matching profiles
    for i in range(1, len(dataD)):
        if dataD[i][1:] == seqLengths:
            return dataD[i][0]

    return "No match"


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize Variables
    longest_length = 0
    sequence_length = len(sequence)
    subsequence_length = len(subsequence)

    # checking for the longest consecutive run
    for i in range(0, sequence_length - subsequence_length):
        count = 0
        while True:
            # choose a starting point
            start = i + count * subsequence_length
            # choose a ending point
            end = start + subsequence_length
            if sequence[start : end] == subsequence:
                count += 1
            else:
                break
        # update if this consecutive count is larger than earlier
        longest_length = max(longest_length, count)

    return longest_length



print(main())