import csv
import sys

teams = []
# TODO: Read teams into memory from file
with open(sys.argv[1]) as file:
    reader = csv.DictReader(file)
    for line in reader:
        team = {"team":line["team"] , "rating" : int(line["rating"])}
        teams.append(team)
print(teams)