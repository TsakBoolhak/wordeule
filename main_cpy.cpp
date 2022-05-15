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
};

bool	fillDictionnary( std::unordered_set< std::string > & dictionnary , int ac, char **av ) {

	std::ifstream	infile;
	std::string str;

	infile.open( "words.txt", std::ifstream::in );
	if ( infile.is_open() == true ) {

		struct stat stats;
		if ( stat( "words.txt", &stats ) != -1 && (stats.st_mode & S_IFMT) == S_IFREG ) {

			do {

				str.clear();
				getline( infile, str );
				if ( str.size() == 5 && std::all_of( str.begin(), str.end(), []( char c ) { return std::isalpha(c); } ) )
					dictionnary.insert(str);
			} while ( infile );
		}
		infile.close();
	}

	for ( int i = 1 ; i < ac ; ++i ) {

		infile.open( av[i], std::ifstream::in );

		if ( infile.is_open() == true ) {

			struct stat stats;
			if ( stat( av[i], &stats ) != -1 && (stats.st_mode & S_IFMT) == S_IFREG ) {

				do {

					str.clear();
					getline( infile, str );
					if ( str.size() == 5 && std::all_of( str.begin(), str.end(), []( char c ) { return std::isalpha(c); } ) )
						dictionnary.insert(str);
				} while ( infile );
			}
			infile.close();
		}
	}

	if ( dictionnary.empty() )
		return false;
	return true;
}

void	setGreen( std::vector< int > & colors, std::string const & word, std::string const & answer ) {

	std::string::const_iterator wordIt = word.begin();
	std::string::const_iterator answerIt = answer.begin();
	std::vector<int>::iterator	colorsIt = colors.begin();
	for ( ; wordIt != word.end() && answerIt != answer.end() && colorsIt != colors.end() ; ++wordIt, ++answerIt, ++colorsIt ) {

		if ( *wordIt == *answerIt )
			*colorsIt = GREEN;
	}
	return;
}

void	setYellow( std::vector< int > & colors, std::string const & word, std::string const & answer ) {

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

void	setColors( std::vector< int > & colors, std::string const & word, std::string const & answer ) {

	setGreen( colors, word, answer );
	setYellow( colors, word, answer );
}

void	printWord( std::vector<int> const & colors, std::string const & word ) {

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

void	printGrid( std::vector< std::vector< int > > const & colorsVec, std::vector< std::string > const & grid ) {

	std::vector< std::vector< int > >::const_iterator colorsVecIt = colorsVec.begin();
	for ( std::vector< std::string >::const_iterator it = grid.begin() ; it != grid.end() ; ++it, ++colorsVecIt ) {

		std::cout << '\t';
		printWord( *colorsVecIt, *it);
		std::cout << std::endl;
	}

	return;
}

int	main(int ac, char **av) {

	std::unordered_set< std::string > dictionnary;

	size_t guess = 0;

	if (fillDictionnary( dictionnary, ac, av ) == false) {

		std::cerr << "Dictionnary file(s) didn't contain any valid word" << std::endl;
		return 1;
	}
	std::vector< std::string > grid;
	std::string	answer;
	RandomGen rGen ( 1, dictionnary.size() );
	size_t lol = rGen.getRandomValue() ;

	{
		
		std::unordered_set<std::string>::iterator it = dictionnary.begin();
		for (size_t i = 1; i < lol ; i++, it++ )
			;
		answer = *it;
	}
	std::vector< std::vector<int> > colorsVec;
	colorsVec.insert( colorsVec.end(), 6, std::vector<int> () );
	for_each( colorsVec.begin(), colorsVec.end(), []( std::vector<int> & vec ) { vec.insert( vec.end(), 5, GREY ); } );

	std::cout << FOREGROUND_WHITE;
	std::cout << "answer is " << answer << std::endl;

	grid.insert( grid.end(), 6, std::string ( "______" ) );

	std::cout << "Total words available: " << dictionnary.size() << std::endl;

	printGrid( colorsVec, grid );

	do {

		std::unordered_set<std::string>::iterator it;
		std::string input;
		std::cout << "input: ";
		do {
			getline( std::cin, input );
			if ( std::cin.eof() == 1) {
				std::cerr << "End Of File reached" << std::endl;
				break;
			}
			if (input.size() != 5 ) {

				std::cout << "The word must be five letter long!" << std::endl;
				printGrid( colorsVec, grid );
				std::cout << "input: ";
				continue;
			}
			std::for_each(input.begin(), input.end(), [](char & c) { c = std::tolower(c); } );
			it = dictionnary.find( input );
			if ( it == dictionnary.end() ) {
				std::cout << "The word is not in the word list" << std::endl;
				printGrid( colorsVec, grid );
				std::cout << "input: ";
			}
		} while ( std::cin.eof() == 0 &&  ( input.size() != 5 || it == dictionnary.end() ) );
		if (std::cin.eof() == 1)
			break;
		grid[guess] = input;
		setColors( *(colorsVec.begin() + guess), grid[guess], answer);
		printGrid(colorsVec, grid );
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
	std::cout << FOREGROUND_DEFAULT;
	return 0;
}
