#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FOREGROUND_DEFAULT "\033[39m"
#define FOREGROUND_WHITE "\033[37m"
#define FOREGROUND_GREEN "\033[32m"
#define FOREGROUND_YELLOW "\033[33m"
#define FOREGROUND_GREY "\033[90m"
#define FOREGROUND_BOLD "\033[1m"
#define FOREGROUND_UNBOLD "\033[0m"

#define GREY 0
#define YELLOW 1
#define GREEN 2

class RandomGen {

	protected :

		std::random_device	rd;
		std::mt19937		rng;
		std::uniform_int_distribution<std::mt19937::result_type> dist;

	public :

		RandomGen( size_t min = 1, size_t max = 1) : rng ( std::mt19937 ( rd() ) ),  dist (std::uniform_int_distribution<std::mt19937::result_type> ( min, max )) {

			return;
		}


		size_t	getRandomValue() {

			return dist(rng);
		}

		RandomGen &	operator=( const RandomGen & src ) {

			if ( this != &src ) {

				this->rng = src.rng;
				this->dist = src.dist;
			}
			return *this;
		}
};

class Wordle {

	protected :

		bool								play;
		std::unordered_set< std::string >	dictionnary;
		size_t								guess;
		RandomGen							randGen;
		std::vector< std::string >			grid;
		std::string							answer;
		std::vector< std::vector< int > >	colorsVec;

		void	parseFile( char *file );
		bool	fillDictionnary( int ac, char **av );
		void	setGreen( std::vector< int > & colors, std::string const & word );
		void	setYellow( std::vector< int > & colors, std::string const & word );
		void	setColors( std::vector< int > & colors, std::string const & word );
		void	printWord( std::vector<int> const & colors, std::string const & word );
		void	printGrid();
		void	setAnswer();
		void	printHeader();
		void	reset();
		
	public :

		Wordle( int ac = 1, char **av = NULL ) {

			play = fillDictionnary( ac, av );
			if ( play == true )
			{
				guess = 0;
				randGen = RandomGen ( 1, dictionnary.size() );
				grid.insert( grid.end(), 6, std::string ("_____") );
				setAnswer();
				colorsVec.insert( colorsVec.begin(), 6, std::vector<int> () );
				for_each( colorsVec.begin(), colorsVec.end(), []( std::vector<int> & vec ) { vec.insert( vec.end(), 5, GREY ); } );
			}
			return ;
		}

		void	game();
		bool	getPlay() { return play; }

};

void	Wordle::setAnswer() {

	answer.clear();
	size_t lol = randGen.getRandomValue() ;

	{
		std::unordered_set<std::string>::iterator it = dictionnary.begin();
		for (size_t i = 1; i < lol ; i++, it++ )
			;
		answer = *it;
	}
}

void	Wordle::parseFile( char *file ) {

	if ( file == NULL )
		return ;
		
	std::ifstream	infile;
	std::string str;

	infile.open( file, std::ifstream::in );
	if ( infile.is_open() == true ) {

		struct stat stats;
		if ( stat( file, &stats ) != -1 && (stats.st_mode & S_IFMT) == S_IFREG ) {

			do {

				str.clear();
				getline( infile, str );
				if ( str.size() == 5 && std::all_of( str.begin(), str.end(), []( char c ) { return std::isalpha(c); } ) )
					dictionnary.insert(str);
			} while ( infile );
		}
		infile.close();
	}

	return ;
}

bool	Wordle::fillDictionnary( int ac, char **av ) {


	char str[] = "words.txt";

	parseFile( str );

	for ( int i = 1 ; i < ac ; ++i ) {

		parseFile( av[i] );
	}

	if ( dictionnary.empty() )
		return false;
	return true;
}

void	Wordle::setGreen( std::vector< int > & colors, std::string const & word ) {

	std::string::const_iterator wordIt = word.begin();
	std::string::const_iterator answerIt = answer.begin();
	std::vector<int>::iterator	colorsIt = colors.begin();
	for ( ; wordIt != word.end() && answerIt != answer.end() && colorsIt != colors.end() ; ++wordIt, ++answerIt, ++colorsIt ) {

		if ( *wordIt == *answerIt )
			*colorsIt = GREEN;
	}
	return;
}

void	Wordle::setYellow( std::vector< int > & colors, std::string const & word ) {

	std::string::const_iterator answerIt = answer.begin();
	
	
	for ( std::vector< int>::iterator answerColorsIt = colors.begin(); answerIt != answer.end(); ++answerIt, ++answerColorsIt ) {
		
		if ( *answerColorsIt == GREEN)
			continue;
		std::vector< int>::iterator colorsIt = colors.begin();
		for ( std::string::const_iterator wordIt = word.begin() ; wordIt != word.end() && colorsIt != colors.end() ; ++wordIt, ++colorsIt ) {

			if ( *colorsIt == GREY && *answerIt == *wordIt ) {

				*colorsIt = YELLOW;
				break;
			}
		}
		
	}

	return ;
}

void	Wordle::setColors( std::vector< int > & colors, std::string const & word ) {

	setGreen( colors, word );
	setYellow( colors, word );
}

void	Wordle::printWord( std::vector<int> const & colors, std::string const & word ) {

	std::string	tmp (word);
	for_each( tmp.begin(), tmp.end(), [](char & c) {c = std::toupper(c);} );
	std::vector<int>::const_iterator colorsIt = colors.begin();
	for (std::string::iterator it = tmp.begin() ; it != tmp.end() && colorsIt != colors.end(); ++it, ++colorsIt) {

		switch ( *colorsIt ) {

			case GREEN :
				std::cout << FOREGROUND_GREEN;
				break;
			case YELLOW :
				std::cout << FOREGROUND_YELLOW;
				break;
			default :
				std::cout << FOREGROUND_GREY;
				break;
		}
		std::cout << FOREGROUND_BOLD  << *it << FOREGROUND_UNBOLD << FOREGROUND_WHITE;
		if ( it + 1 != word.end() )
			std::cout << " ";
	}
	std::cout << std::endl;
	return ;
}

void	Wordle::printGrid() {

	std::vector< std::vector< int > >::const_iterator colorsVecIt = colorsVec.begin();
	for ( std::vector< std::string >::const_iterator it = grid.begin() ; it != grid.end() ; ++it, ++colorsVecIt ) {

		std::cout << '\t';
		printWord( *colorsVecIt, *it);
		std::cout << std::endl;
	}

	return;
}

void	Wordle::reset() {

	play = true;
	guess = 0;
	grid.clear();
	grid.insert( grid.end(), 6, std::string ("_____") );
	setAnswer();
	for_each( colorsVec.begin(), colorsVec.end(), []( std::vector<int> & vec ) { vec.clear(); } );
	for_each( colorsVec.begin(), colorsVec.end(), []( std::vector<int> & vec ) { vec.insert( vec.end(), 5, GREY ); } );
}

void	Wordle::printHeader() {

	if ( play == false ) {

		std::cerr << "Dictionnary file(s) didn't contain any valid word" << std::endl;
		return ;
	}
	std::cout << FOREGROUND_WHITE;
	std::cout << "Total words available: " << dictionnary.size() << std::endl;
	printGrid( );
}

void	Wordle::game() {

	if ( play == true )
		reset();
	printHeader();
	if ( play == false )
		return ;
	std::string input;
	do {

		std::unordered_set<std::string>::iterator it;
		std::cout << "input: ";
		do {
			getline( std::cin, input );
			if ( std::cin.eof() == 1) {
				std::cerr << "End Of File reached" << std::endl;
				break;
			}
			if (input.size() != 5 ) {

				std::cout << "The word must be five letter long!" << std::endl;
				printGrid();
				std::cout << "input: ";
				continue;
			}
			std::for_each(input.begin(), input.end(), [](char & c) { c = std::tolower(c); } );
			it = dictionnary.find( input );
			if ( it == dictionnary.end() ) {
				std::cout << "The word is not in the word list" << std::endl;
				printGrid();
				std::cout << "input: ";
			}
		} while ( std::cin.eof() == 0 &&  ( input.size() != 5 || it == dictionnary.end() ) );
		if (std::cin.eof() == 1)
			break;
		grid[guess] = input;
		setColors( *(colorsVec.begin() + guess), grid[guess] );
		printGrid();
		if (input == answer)
			break;
		guess++;
	} while ( guess < 6 && std::cin.eof() == 0 );

	std::for_each(answer.begin(), answer.end(), [](char & c) { c = std::toupper(c); } );
	if (guess < 6 && std::cin.eof() == 0) {

		std::cout << "Congratulation you found the word " << answer << " in " << guess + 1 << " guess";
		if ( guess > 0) 
			std::cout << "es";
		std::cout << std::endl;
	}
	else if (std::cin.eof() == 0) {

		std::cout << "Too bad you lost! The word was " << answer << std::endl;
	}
	else {

		std::cout << FOREGROUND_DEFAULT;
		return ;
	}
	do {

		input.clear();
		std::cout << "Do you want to play again? (Y / N) " << std::endl;
		getline( std::cin, input );
		if (input.size() > 0)
			*(input.begin()) = tolower(*(input.begin()));
	}while ( input != "y" && input != "n" && std::cin.eof() == 0);

	std::cout << FOREGROUND_DEFAULT;

	if ( std::cin.eof() != 0 || input == "n" )
		play = false;
}

int	main(int ac, char **av) {

	Wordle	wordle(ac, av);

	do {

		wordle.game();
	} while (wordle.getPlay() == true && std::cin.eof() == 0 );
	std::cout << "Good Bye!" << std::endl;
	return 0;
}
