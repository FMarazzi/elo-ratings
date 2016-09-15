// Elo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <cmath>
#include<algorithm>
using namespace std;

class Player {
public:
	int m_id;
	string m_name;
	int m_elo;
};

class Team {
private:
	vector<int> ids;
	int m_team_elo;
public:
	int var = 0;
	int change_players(map<int,Player> &players) {
		for (int i : ids) //iterate over the players and change their elo accordingly
			players[i].m_elo += var;
		var = 0;
		return 0;
	};
	int elo() { return m_team_elo; };
	vector<int> members() { return ids;};
	float get_we(Team &opponent) {
		int dr = m_team_elo - opponent.elo();
		float v;
		v = 1 / ((10 ^ (-dr / 400) + 1));
		return v;
	};
	int remove_p(const Player &p) {
		ids.erase(std::remove(ids.begin(), ids.end(), p.m_id), ids.end());
		m_team_elo -= p.m_elo;
		return 0;
	};
	int add_p(const Player &p) {
		for (int i : ids)
			if (i == p.m_id) return 1;
		if (ids.size() > 5) return 1;
		else {
			ids.push_back(p.m_id);
			m_team_elo += p.m_elo;
		}
		return 0;
	};
};

class Game {
private:
	pair<Team,short> TeamA;
	pair<Team, short> TeamB;
	float K = 20, W, Wea;
public:
	int set_game(Team &A,short Ga,Team &B,short Gb) {
		for (int i : A.members())
			for (int j : B.members())
				if (i == j) return 1;

		TeamA.first = A;
		TeamA.second = Ga;
		TeamB.first = B;
		TeamB.second = Gb;

		W = Ga > Gb ? 1 : Ga = Gb ? 0.5 : 0;
		short r;
		r = abs(Ga - Gb);
		K *= r < 2 ? 1 : r == 2 ? 1.5 : r == 3 ? 1.75 : 1 + 0.75 + (r - 3) / 8;
		return 0;
	};
	pair<Team, Team> teams() {
		return pair<Team,Team> (TeamA.first,TeamB.first);
	};
	int change_elo(map<int, Player> &players) {
		// Change the "var" parameter in each team
		Wea = TeamA.first.get_we(TeamB.first);
		TeamA.first.var += K*(W - Wea);
		TeamB.first.var -= TeamA.first.var;
		cout << "Change1 expected: " << TeamA.first.var << " Change2 expected: " << TeamB.first.var;
		// Change the ELO of the players
		TeamA.first.change_players(players);
		TeamB.first.change_players(players);
		return 0;
	};
};

void readfile(map<int, Player> &players);
void printplay(const map<int, Player> &players);
void writefile(const map<int, Player> &players);
Team createteam(map<int, Player> &players);

int main()
{	
	map<int, Player> players;
	readfile(players); // Read data from file
	printplay(players); // Print data out

	// Choices for the user
	string input;
	cout << "To choose the teams type 0. For automatic team generation type 1.\n";
	cin >> input;
	// if (stoi(input)) gen_teams(); // Automatic generation
	
	// Create teams and add them to the vector
	vector<Team> teams;
	cout << "First team:\n";
	teams.push_back(createteam(players));
	cout << "Second team:\n";
	teams.push_back(createteam(players));

	Game game; // Create game
	short g1, g2;
	cout << "First team goals:\n";
	cin >> g1;
	cout << "Second team goals:\n";
	cin >> g2;
	if (game.set_game(teams[0], g1, teams[1], g2)) cout << "Fail setting up the game"; // Set game's parameters (team1, goal1, team2, goal2)
	else {
		cout << "Wanna change? (1 for yes)";
		cin >> g1;
		if (g1 == 1) game.change_elo(players); // change team's elo
	}

	writefile(players); // write out changes

	cout << "Calculation and editing done.\n";
	getchar();
    return 0;
}

Team createteam(map<int, Player> &players) {
	Team temp;
	for (int j = 0; j < 5; ++j) {
		cout << "Player " << j << ": ";
		string input = "";
		cin >> input;
		// validate input, if not valid --j
		if (1) {
			if (temp.add_p(players[stoi(input)])) {
				cout << "Player already present";
				--j;
			}
			cout << "\n";
		}
	}
	return temp;
}

void printplay(const map<int, Player> &players) {
	for (auto const &id:players) {
			cout << "Id:" << id.second.m_id << ", " << id.second.m_name << ", " << id.second.m_elo << "\n";
	}
}

void writefile(const map<int, Player> &players) {
	ofstream f;
	f.open("ELO.txt");
	if (f.is_open()) {
		for (auto const &id : players) {
			f << id.second.m_name << "\n" << id.second.m_elo << "\n";
		}
	}
	f.close();
}

void readfile(map<int, Player> &players) {
	Player temp;
	ifstream f;
	f.open("ELO.txt");
	if (f.is_open()) {
		int i = 0;
		string name;
		string elo;
		while (!f.eof()) {
			getline(f, name);
			getline(f, elo);
			temp.m_name = name;
			temp.m_elo = stoi(elo);
			temp.m_id = ++i;
			if (name != "") players.insert(pair<int,Player>(temp.m_id,temp));
		}
	}
	f.close();
}