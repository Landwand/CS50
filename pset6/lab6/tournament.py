# Simulate a sports tournament

import csv
import sys
import random
from tempfile import template

# Number of simluations to run
N = 1000
#N = 31


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")
    else:
        filename = check_args(sys.argv[1])
        
    teams = []

    # Read teams into memory from file
    teams = load_teams(filename)

    counts = {}
    # Simulate N tournaments and keep track of win count
    counter = N

    for i in range(N):
        round_winner = simulate_tournament(teams)

        if round_winner in counts:
            #print('existing winner found')
            counts[round_winner] += 1
            #print(f'increment winner {round_winner} : {counts[round_winner]}')
        else:
            #print('new winner added - ', round_winner)
            counts[round_winner] = 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def check_args(argmnt):
    arg = argmnt.lower()
    if arg == '2018m.csv':
        return '2018m.csv'
    elif arg == '2019w.csv':
        return '2019w.csv'
    else:
        #print("Error: bad args")
        sys.exit("Usage: python tournament.py FILENAME")
    

def load_teams(filename):

    teams = []
    with open(filename, "r") as file:
        reader = csv.DictReader(file)
        #counter = 0
        for row in reader:
            dict_team = {}  # defined IN this for loop as it initiates a brand new dict per run
            team_name = row["team"]
            team_rating = int(row["rating"])       
            dict_team = {
                'team': team_name,
                'rating': team_rating
            }     
            teams.append(dict_team)
            #counter += 1
    return teams


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    # Simulate games for all pairs of teams

    winners = []

    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    # 16 teams each file - each pair has 1 winner .. /2 

    team_count = len(teams)
    current_winners = teams

    while team_count > 1:
        current_winners = simulate_round(current_winners)
        team_count /= 2

    tourny_winner = current_winners[0]['team']
    #print("current winners = ", current_winners[0]['team'])

    return tourny_winner


if __name__ == "__main__":
    main()
