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

using namespace std;

class Player {
public:
	int id;
	string m_name;
	int m_elo;
};

class Team {
private:
	set<Player> ids;
	int m_team_elo;

public:
	int var = 0;
	int change_players() {
		//iterate over the players and change their elo accordingly
		//i.e. player1.m_elo += var
		var = 0;
		return 0;
	};
	int elo() { return m_team_elo; };
	set<Player> members() { return ids;};
	int remove(const Player &p) {
		ids.erase(p);
		m_team_elo -= p.m_elo;
		return 0;
	}
	float get_we(Team &opponent) {
		int dr = m_team_elo - opponent.elo();
		return 1/((10^(-dr/400)+1));
	}
	int add(const Player &p) {
		if (ids.size() > 5) 
			return 1;
		else 
			ids.insert(p);
			m_team_elo += p.m_elo;
		return 0;
	};
};

class Game {
private:
	pair<Team,short> TeamA;
	pair<Team, short> TeamB;
	float K = 20;
	float W;
public:
	void set_game(const Team &A,short Ga,const Team &B,short Gb) {
		TeamA.first = A;
		TeamA.second = Ga;
		TeamB.first = B;
		TeamB.second = Gb;

		if (Ga = Gb) W = 0.5;
		else W = Ga > Gb ? 1 : 0;

		short r;
		r = abs(Ga - Gb);
		if (r==2) K*=1.5;
		if (r==3) K*=1.75;
		else if (r>3) K *= 1+0.75+(r-3)/8;		
	};
	pair<Team, Team> teams() {
		return pair<Team,Team> (TeamA.first,TeamB.first);
	};
	int change_elo() {
		TeamA.first.var += K*(W-TeamA.first.get_we(TeamB.first));
		TeamB.first.var += K*((1-W)-TeamB.first.get_we(TeamA.first));
		return 0;
	};
};

void readfile(map<int, Player> &players);
void printplay(const map<int, Player> &players);
void writefile(const map<int, Player> &players);

int main()
{	
	map<int, Player> players;
	Team temp;
	vector<Team> teams;
	Game game;

	teams.reserve(10);

	readfile(players);
	printplay(players);

	string input;
	cout << "To choose the teams type 0. For automatic team generation type 1.\n";
	cin >> input;
	// if (stoi(input)) gen_teams(); // Automatic generation
	
	cout << "New team:\n";
	for (int j = 0; j < 5; ++j) {
		cout << "Player " << j << ": ";
		string input = "";
		cin >> input;
		// Validate input, if not valid --j
		if (1) {
			temp.add(players[stoi(input)]);
			cout << "\n";
		}
	}
	teams.push_back(temp);
		

	cout << "Result:\n";
	// insert results

	// calculations
	writefile(players);

	cout << "Calculation and editing done.\n";
	getchar();
    return 0;
}

void printplay(const map<int, Player> &players) {
	for (auto const &id:players) {
			cout << "Id:" << id.first << ", " << id.second.m_name << ", " << id.second.m_elo << "\n";
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
			temp.id = ++i;
			if (name != "") players.insert(pair<int,Player>(temp.id,temp));
		}
	}
	f.close();
}