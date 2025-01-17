#include "libClass.hpp"
#include "libFun.hpp"
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <conio.h>

#pragma warning (disable: 4996)

using namespace std;
class Game;
///////////////////////////
//		class Move		 //
///////////////////////////
/*Klasa przechowuje zapis jednego ruchu, wykonywanego przez gracza w danej rundzie*/
/*Konstruktor domyslny przypisujacy domyslne wartosci do pol nowego obiektu*/
Move::Move() {
	guess[0] = { '\0' };
	correctColour = 0;
	correctPlace = 0;
	next = NULL;
}
/*Konsturtor argumentowy, przekazujacy wartosci z przekazanych argumentow do pol nowego obiektu*/
Move::Move(char *c, unsigned int x, unsigned int y) {
	strcpy(guess, c);
	correctColour = x;
	correctPlace = y;
	next = NULL;
}
/*Konsturtor kopiujacy, przekazujacy wartosci z pol z obiektu m do pol nowego obiektu*/
Move::Move(Move& m) {
	strcpy(guess, m.guess);
	correctColour = m.correctColour;
	correctPlace = m.correctPlace;
	next = NULL;
}
/*Destruktor usuwajacy obiekt alokowany w polu next*/
Move::~Move() {
	delete next;
}
/*Metoda przypisuje ciag znakow z tablicy przekazanej za pomoca parametru c do tablicy zawierajacej strzaly*/
void Move::setGuess(char*c) {
	strcpy(guess, c);
}
/*Metoda zwraca adres poczatku tablicy przechowujacej strzal gracza*/
char* Move::getGuess(void) {
	return guess;
}
/*Metoda przypisuje polu correctPlace wartosc przekazywana w paramencie x*/
void Move::setCorrectPlace(unsigned int x) {
	correctPlace = x;
}
/*Metoda zwraca wartosc pola correctPlace*/
unsigned int Move::getCorrectPlace(void) {
	return correctPlace;
}
/*Metoda przypisuje polu correctColour wartosc przekazywana w paramencie x*/
void Move::setCorrectColour(unsigned int x) {
	correctColour = x;
}
/*Metoda zwraca wartosc pola correctColour*/
unsigned int Move::getCorrectColour(void) {
	return correctColour;
}
/*Metoda sprawdza ile kolorow w kodzie podanym przez gracza znajduje sie na odpowiednim miejscu w kodzie ustalonym podczas tej rozgrywki*/
void Move::checkCorrectPlace(char* code) {
	for (int i = 0; i < CODE_L; ++i) {
		if (guess[i] == code[i])
			correctPlace++;
	}
}
/*Metoda sprawdza ile kolorow w kodzie podanym przez gracza znajduje sie w kodzie ustalonym podczas tej rozgrywki.
*Kolory, ktore znajduja sie na odpowienim miejscu sa pomijane*/
void Move::checkCorrectColour(char* code) {
	for (int i = 0; i < CODE_L; ++i) {
		for (int j = 0; j < CODE_L; ++j) {
			if (guess[j] == code[i]) {
					correctColour++;
					break;
			}
		}
	}
	correctColour -= correctPlace;
}

///////////////////////////
//		class Round		 //
///////////////////////////
/*Klasa przechowuje zapis jednej rundy  rozgrywki, skladajacej sie z kolejnych ruchow*/

/*Konstruktor domyslny przypisujacy domyslne wartosci do pol nowego obiektu*/
Round::Round() {
	code[0] = { '\0' };
	moveCounter = 0;
	next = NULL;
	firstMove = NULL;
	pointMove = NULL;
	win = false;
}
/*Konsturtor argumentowy, przekazujacy wartosci z przekazanych argumentow do pol nowego obiektu*/
Round::Round(char* c, unsigned int m) {
	strcpy(code, c);
	moveCounter = m;
	next = NULL;
	firstMove = NULL;
	pointMove = NULL;
	win = false;
}
/*Konsturtor kopiujacy, przekazujacy wartosci z pol z obiektu r do pol nowego obiektu*/
Round::Round(Round& r){
	strcpy(code, r.code);
	moveCounter = r.moveCounter;
	next = NULL;
	firstMove = NULL;
	pointMove = NULL;
	win = false;
}
/*Destruktor usuwajacy obiekt alokowany w polu next*/
Round::~Round() {
	delete next;
}
/*Metoda przypisuje ciag znakow z tablicy przekazanej za pomoca parametru c do tablicy zawierajacej kod*/
void Round::setCode(char* c) {
	strcpy(code, c);
}
/*Metoda zwraca adres poczatku tablicy przechowujacej ustalony kod*/
char* Round::getCode(void) {
	return code;
}
/*Metoda przypisuje polu move wartosc przekazywana w paramencie m*/
void Round::setMove(unsigned int m) {
	moveCounter = m;
}
/*Metoda zwraca wartosc pola move*/
unsigned int Round::getMove(void) {
	return  moveCounter;
}
/*Metoda inicjalizuje kolejny ruch w danej rundzie*/
void Round::nextMove(Player* p) {
	if (firstMove == NULL) {
		firstMove = pointMove = new Move();
		allocationCheck<Move*>(firstMove);
	}
	else {
		pointMove->next = new Move();
		allocationCheck<Move*>(pointMove->next);
		pointMove = pointMove->next;
	}
	moveCounter++;
	//podanie strzalu
	cout << ">>>\t";
	p->guessCode(pointMove->guess);
	//sprawdzenie
	pointMove->checkCorrectPlace(code);
	pointMove->checkCorrectColour(code);
	cout << "\n\t[" << pointMove->getCorrectPlace() << "] " << "[" << pointMove->getCorrectColour() << "]\n" << endl;
	if(pointMove->getCorrectPlace() != CODE_L)
		++(*p);
	if (pointMove->getCorrectPlace() == CODE_L) {
		cout << "Bravo! You have guessed the code in " << moveCounter << " moves!\n" << endl;
		win = true;
	}
	else if (moveCounter == MOVES) {
		cout << "Unfortunately you weren't able to guess the code." << endl;
		cout << "The code:\t" << code << endl;
	}
}

//////////////////////////////
//		class Player		//
//////////////////////////////
/*Klasa przechowuje informacje o jednym graczu*/

/*Konstruktor domyslny przypisujacy domyslne wartosci do pol nowego obiektu*/
Player::Player() {
	strcpy(nick, "PLAYER");
	points = 0;
	isComputer = 0;
}
/*Konsturtor argumentowy, przekazujacy wartosci z przekazanych argumentow do pol nowego obiektu*/
Player::Player(const char* n, unsigned int p, int i) {
	strcpy(nick, n);
	points = p;
	isComputer = i;
}
/*Konsturtor kopiujacy, przekazujacy wartosci z pol z obiektu p do pol nowego obiektu*/
Player::Player(Player&p) {
	strcpy(nick, p.nick);
	points = p.points;
	isComputer = p.isComputer;
}
/*Metoda przypisujaca polu nick zawartosc argumentu n*/
void Player::setNick(char*n) {
	strcpy(nick, n);
}
/*Metoda zwracajaca adres poczatku tablicy zawierajacej nick*/
char* Player::getNick(void) {
	return nick;
}
/*Metoda przypisujaca polu points wartosc argumentu p*/
void Player::setPoints(unsigned int p) {
	points = p;
}
/*Metoda zwracajaca wartosc pola points*/
unsigned int Player::getPoints(void) {
	return points;
}
/*Metoda przypisujaca polu isComputer wartosc argumentu i*/
void Player::setIsComputer(int i) {
	isComputer = i;
}
/*Metoda zwracajaca wartosc pola isComputer*/
int Player::getIsComputer(void) {
	return isComputer;
}
/*Metoda polimorficzna, pobierajaca od uzytkownika 4-znakowy kod, wypisujac go w konsoli w postaci XXXX.
* Przeprowadzajaca kontrole poprawnosci danych i umozliwiajaca poprawe bldednego kodu od momentu wystapienia bledu.
* Zwraca adres poczatku tablicy zawierajacej poprawny, 4-znakowy kod.*/
void Player::setCode(char* buff) {
	for (int i = 0; i < CODE_L; ++i) {
		char c = getch();
		c = toupper(c);
		while (c != red && c != green && c != blue && c != orange && c != purple && c != yellow) {
			if (isspace(c)) {
				cout << "\nYou have entered wrong character. Try again from the" <<  i + 1 << (i+1 == 1 ? "st": i+1 == 2 ? "nd": i+1 == 3 ? "rd": "nd") << " character." << endl;
				cout << "\n>>>\t";
				for (int j = 0; j < i; ++j) {
					cout.put('X');
				}
				c = getch();
				c = toupper(c);
				continue;
			}
			else {
				cout << "\nYou have entered wrong code. Try again from the" << i + 1 << (i + 1 == 1 ? "st" : i + 1 == 2 ? "nd" : i + 1 == 3 ? "rd" : "nd") << " character." << endl;
				cout << "\n>>>\t";
				for (int j = 0; j < i; ++j) {
					cout.put('X');
				}
				c = getch();
				c = toupper(c);
				continue;
			}
		}
		cout.put('X');
		buff[i] = c;
	}
	while (getchar() != '\n')
		;
}
/*Metoda pobierajaca od uzytkownika 4-znakowy kod.
* Przeprowadzajaca kontrole poprawnosci danych i umozliwiajaca poprawe bldednego kodu od momentu wystapienia bledu.
* Zwraca adres poczatku tablicy zawierajacej poprawny, 4-znakowy kod.*/
void Player::guessCode(char * buff) {
	for (int i = 0; i < CODE_L; ++i) {
		char c = getchar();
		c = toupper(c);
		while (c != red && c != green && c != blue && c != orange && c != purple && c != yellow) {
			if (isspace(c)) {
				cout << "\nYou have entered wrong character. Try again from the" << i + 1 << (i + 1 == 1 ? "st" : i + 1 == 2 ? "nd" : i + 1 == 3 ? "rd" : "nd") << " character." << endl;
				cout << "\n>>>\t";
				for (int j = 0; j <= i; ++j) {
					cout.put(buff[j]);
				}	
				while (getchar() != '\n')
					;
				c = getchar();
				c = toupper(c);
				continue;
			}
			cout << "\nYou have entered wrong code. Try again from the" << i + 1 << (i + 1 == 1 ? "st" : i + 1 == 2 ? "nd" : i + 1 == 3 ? "rd" : "nd") << " character." << endl;
			cout << "\n>>>\t";
			for (int j = 0; j <= i; ++j) {
				cout.put(buff[j]);
			}
			while (getchar() != '\n')
				;
			c = getchar();
			c = toupper(c);
		}
		buff[i] = c;
	}
	while (getchar() != '\n')
		;
}
/*Przeciazaony operator ++, ktory zwieksza liczbe punktow gracza*/
const Player& Player::operator++(void) {
	points += 1;
	return *this;
}
/*Metoda polimorficzna, inicjalizujaca kolejna runde rozgrwyki*/
void Player::nextRound(Game* g, Player* player) {
	char buff[CODE_L];
	if (g->firstRound == NULL) {
		g->firstRound = g->pointRound = new Round();
		allocationCheck(g->firstRound);
	}
	else {
		g->pointRound->next = new Round();
		allocationCheck(g->pointRound->next);
		g->pointRound = g->pointRound->next;
	}
	cout << "Player " << getNick() << " is putting up a code." << endl;
	//ustalenie kodu
	cout << "[R] - red, [G] - green, [B] - blue, [O] - orange, [P] - purple, [Y] - yellow" << endl;
	cout << "\n>>>\t";
	setCode(g->pointRound->getCode());
	cout << "\nGracz " << player->getNick() << " odgaduje kod.\n" << endl;
	for (int i = 0; i < MOVES && !g->pointRound->win; ++i) {
		g->pointRound->nextMove(player);
	}
}

//////////////////////////////
//		class Computer		//
//////////////////////////////
/*Klasa stosowana podczas rozgrywki gracz kontra komputer, oznaczajaca komputer*/

/*Konstruktor domyslny przypisujacy domyslne wartosci do pol obiektu klasy Player*/
Computer::Computer(): Player(){}
/*Konsturtor argumentowy, przekazujacy wartosci z przekazanych argumentow do pol obiektu klasy Player*/
Computer::Computer(char* n, unsigned int p, int i): Player(n, p, i){}
/*Konsturtor kopiujacy, przekazujacy wartosci z pol z obiektu p do pol obiektu klasy Player*/
Computer::Computer(Player& p) : Player(p) {}
/*Metoda generujaca pseudolosowy 4-znakowy kod
* arg buff adres poczatku tablicy, do ktorej zostanie zapisany 4-znakowy kod.*/
void Computer::setCode(char* buff){
	for (int i = 0; i < CODE_L; ++i) {
		int c = rand() % 6;
		switch (c){
		case 0:
			buff[i] = red;
			break;
		case 1:
			buff[i] = green;
			break;
		case 2:
			buff[i] = blue;
			break;
		case 3:
			buff[i] = orange;
			break;
		case 4:
			buff[i] = purple;
			break;
		case 5:
			buff[i] = yellow;
			break;
		default:
			break;
		}
	}
	
}
/*Metoda inicjalizuje kolejna runde rozgrywki.*/
void Computer::nextRound(Game* g, Player* p) {
	if (g->firstRound == NULL) {
		g->firstRound = g->pointRound = new Round();
		allocationCheck(g->firstRound);
	}
	else {
		g->pointRound->next = new Round();
		allocationCheck(g->pointRound->next);
		g->pointRound = g->pointRound->next;
	}
	setCode(g->pointRound->getCode());
	cout << "\nTry to guess the code!\n" << endl;
	for (int i = 0; i < MOVES && !g->pointRound->win; ++i) {
		g->pointRound->nextMove(p);
	}
}
//////////////////////////
//		class Game		//
//////////////////////////
/*Klasa przechowywuje zapis gry, skladajacej sie z rund*/

/*Konstruktor domyslny przypisujacy domyslne wartosci do pol nowego obiektu*/
Game::Game() {
	roundCounter = 0;
	firstRound = NULL;
	pointRound = NULL;
	playerOne = playerTwo = NULL;
}
/*Konsturtor argumentowy, przekazujacy wartosci z przekazanych argumentow do pol nowego obiektu*/
Game::Game(unsigned int c, Player *p1, Player *p2, Round* r){
	roundCounter = c;
	firstRound = pointRound = r;
	playerOne = p1;
	playerTwo = p2;
}
/*Konsturtor kopiujacy, przekazujacy wartosci z pol z obiektu g do pol nowego obiektu*/
Game::Game(const Game& g) {
	roundCounter = g.roundCounter;
	firstRound = pointRound = g.firstRound;
	playerOne = g.playerOne;
	playerTwo = g.playerTwo;
}
/*Destruktor usuwajacy obiekty dynamiczne*/
Game::~Game() {
	delete firstRound;
	delete playerOne;
	delete playerTwo;
}
/*Metoda przypisujaca polu roundCounter wartosc argumentu c*/
void Game::setRoundCounter(unsigned int c) {
	roundCounter = c;
}
/*Metoda zwracajaca wartosc pola roundCounter*/
unsigned int Game::getRoundCounter(void) {
	return roundCounter;
}
/*Metoda pobiera od uzytkownika informacje o trybie przeprowadzanej gry*/
int Game::gameMode(void) {
	cout << "\nChoose the gameplay mode:\n" << endl;
	cout << "\t[1] - Player vs Player" << endl;
	cout << "\t[2] - Player vs Computer" << endl;
	cout << "\n>>>\t";
	char buff[40];
	int type = getTwoOptions<int>();
	switch (type) {
	case 1:
		cout << "\nEnter the 1st player's nick: " << endl;
		cout << "\n>>>\t";
		cin >> buff;
		playerOne = new Player();
		allocationCheck(playerOne);
		playerOne->setNick(buff);
		cout << "\nEnter the 2nd player's nick: " << endl;
		cout << "\n>>>\t";
		cin >> buff;
		playerTwo = new Player();
		allocationCheck(playerTwo);
		playerTwo->setNick(buff);
		break;
	case 2:
		cout << "\nEnter your nick:" << endl;
		cout << "\n>>>\t";
		cin >> buff;
		while (getchar() != '\n')
			;
		playerOne = new Player();
		allocationCheck(playerOne);
		playerOne->setNick(buff);
		playerOne->setIsComputer(0);
		char c[] = PC;
		playerTwo = new Computer(c, 0, 1);
		allocationCheck<Player*>(playerTwo);
		break;
	}
	return type;
}
/*Metoda rozpoczyna gre trybu Gracz kontra gracz*/
void Game::gameStartPVP(void) {
	cout.put('\n');
	cout <<"\nPLAYER: "<< playerOne->getNick() << " STARTS THE GAME" << endl;
	int i = roundCounter;
	for (i; i < ROUNDS; ++i) {
		cout << "\nROUND " << ++roundCounter <<"\n"<< endl;
		playerOne->nextRound(this, playerTwo);
		playerTwo->nextRound(this, playerOne);
		//przerwanie i zczytanie do pliku
		if (roundCounter != ROUNDS) {
			cout << "\nChoose:\n" << endl;
			cout << "\t[1] - continue" << endl;
			cout << "\t[2] - save and quit" << endl;
			cout << "\n>>>\t";
			int m = getTwoOptions<int>();
			switch (m) {
			case 1:
				cout.put('\n');
				break;
			case 2:
				game2file();
				cout << "\nGameplay has been saved" << endl;
				cout << "See you next time!" << endl;
				exit(0);
				break;
			}
		}
	}
}
/*Metoda rozpoczyna gre trybu Gracz kontra komputer*/
void Game::gameStartPVC(void) {
	cout.put('\n');
	int i = roundCounter;
	for (i; i < ROUNDS; ++i) {
		cout << "ROUND " << ++roundCounter << endl;
		playerTwo->nextRound(this, playerOne);
		//przerwanie i zczytanie do pliku
		if (roundCounter != ROUNDS) {
			cout << "\nChoose:\n" << endl;
			cout << "\t[1] - continue" << endl;
			cout << "\t[2] - save and quit" << endl;
			cout << "\n>>>\t";
			int m = getTwoOptions<int>();
			switch (m) {
			case 1:
				cout.put('\n');
				break;
			case 2:
				game2file();
				cout << "\nGameplay has been saved" << endl;
				cout << "See you next time!" << endl;
				exit(0);
				break;
			}
		}
	}
}
/*Metoda wyswietla informacje na koniec gry w trybie gracz kontra gracz*/
void Game::gameEndPvP(void) {
	if (playerOne->getPoints() < playerTwo->getPoints()) {
		cout << "Game has been ended!" << endl;
		cout << "Congratulations! Player " << playerOne->getNick() << " won by gaining" << playerOne->getPoints() << " points" << endl;
		cout << "Player " << playerTwo->getNick() << " gains " << playerTwo->getPoints() << " points.\n" << endl;
	}
	else if(playerOne->getPoints() > playerTwo->getPoints()) {
		cout << "Game has been ended!" << endl;
		cout << "Congratulations! Player " << playerTwo->getNick() << " won by gaining" << playerTwo->getPoints() << " points" << endl;
		cout << "Player " << playerOne->getNick() << " gains " << playerOne->getPoints() << " points.\n" << endl;
	}
	else {
		cout << "Game has been ended!" << endl;
		cout << "Draw! Each of the players gained" << playerOne->getPoints() << " points\n" << endl;
	}
	saveHistory(*this);
}
/*Metoda wyswietla informacje na koniec gry w trybie gracz kontra komputer*/
void Game::gameEndPvC(void) {
	cout << "Game has been ended!" << endl;
	cout << "You have gained " << playerOne->getPoints() << " points.\n" << endl;
	saveHistory(*this);
}
/*Metoda zapisuje postep gry do pliku*/
void Game::game2file(void) {
	FILE* stream;
	stream = fopen(SAVE_NAME, "w");
	allocationCheck<FILE*>(stream);
	fprintf(stream, "%s\t%d\t%d\n", playerOne->getNick(), playerOne->getPoints(), playerOne->getIsComputer());
	fprintf(stream, "%s\t%d\t%d\n", playerTwo->getNick(), playerTwo->getPoints(), playerTwo->getIsComputer());
	fprintf(stream, "%d\n", roundCounter);
	fclose(stream);
}
/*Przeciazony operator =*/
const Game& Game::operator=(const Game& g) {
	roundCounter = g.roundCounter;
	pointRound = g.pointRound;
	playerOne = new Player(g.playerOne->getNick(), g.playerOne->getPoints(), 0);
	if (!g.playerTwo->getIsComputer())
		playerTwo = new Player(g.playerTwo->getNick(), g.playerTwo->getPoints(), 0);
	else
		playerTwo = new Computer(g.playerTwo->getNick(), g.playerTwo->getPoints(), 1);
	return *this;
}
