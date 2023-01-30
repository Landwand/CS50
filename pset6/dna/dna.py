import csv
import sys
from sys import argv


def main():

    # fn returns 2 values from command line
    csv_file, txt_file = is_cmd_arg()
    # Nucleotide seq. as List , reader obj
    sequences, csv_data = read_csv(csv_file)
    unknown_dna = read_dna(txt_file)

    dna_fingerprint = {}
    for sequence in sequences:
        # store num of consec seq. found within unknown_dna in dna_fingerprint{} by key-value
        dna_fingerprint[sequence] = consec_counter(sequence, unknown_dna)

    for row in csv_data:
        if find_match(row=row, sequences=sequences, dna_fingerprint=dna_fingerprint):
            print(row['name'])
            # csv_data.close()
            quit()

    print("No match")
    quit()

    # TODO: Read DNA sequence file into a variable

    # TODO: Find longest match of each STR in DNA sequence

    # TODO: Check database for matching profiles

    return 0


def find_match(sequences, dna_fingerprint, row):
    for sequence in sequences:
        if dna_fingerprint[sequence] != int(row[sequence]):
            return False
        else:
            continue

    # print(f"dna fingerprint[sequence] {dna_fingerprint[sequence]}, row[seq] = {row[sequence]}")
    return True


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        substring = ""
        for j in range(4):
            substring.append(sequence[i])

        if sequence == substring:
            longest_run += 1
            # print("Match found, total =  ", longest_run)

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


class Person:
    def __init__(self, name, AGATC, AATG, TATC):
        self.name = name
        self.AGATC = AGATC
        self.AATG = AATG
        self.TATC = TATC


def is_cmd_arg():
    if len(argv) == 3:
        # print('Using database:  ', argv[1])
        return argv[1], argv[2]
    else:
        print('incorrect no. arguments at command-line')
        sys.exit(1)


def read_csv(filename):
    # returns column headers as a list of strings (nucleotide names)
    csv_file = open(filename, "r")
    reader = csv.DictReader(csv_file)
    sequences = reader.fieldnames[1:]
    # csv_file.close()
    return sequences, reader


def read_dna(filename):
    file = open(filename, 'r')
    dna_str = (file.read())
    # print ("len of DNA str ", len(dna_str))
    file.close()
    return dna_str


def consec_counter(sequence, dna):
    # check for the amount of repeats of the 'sequence' within DNA and return
    i = 0
    while sequence * (i+1) in dna:
        i += 1
    return i


def print_people(people):
    for peep in people:
        print(peep.name)
        print(f"AGATC count: {peep.AGATC}")


main()
