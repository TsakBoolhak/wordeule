#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <random>

#define BACKGROUND_DEFAULT "\033[49m"
#define FOREGROUND_DEFAULT "\033[39m"
#define BACKGROUND_GREEN "\033[42m"
#define FOREGROUND_GREEN "\033[32m"
#define BACKGROUND_YELLOW "\033[103m"
#define FOREGROUND_YELLOW "\033[33m"
#define BACKGROUND_GREY "\033[37m"
#define FOREGROUND_GREY "\033[47m"

bool	fillDictionnary( std::unordered_set< std::string > & dictionnary ) {

	std::ifstream	infile;

	infile.open( "words.txt", std::ifstream::in );
	if ( infile.is_open() == false ) {

		std::cerr << "Failure occured when opening the dictionnary file" << std::endl;
		return false;
	}

	std::string str;

	do {

		str.clear();
		getline( infile, str );
		if ( str.empty() == false )
			dictionnary.insert(str);
	} while ( str.empty() == false );

	infile.close();

	return true;
}

#define GREY 0
#define YELLOW 1
#define GREEN 2

void	printWord( std::string const & word, std::string const & answer ) {

	int colors[5] = { 0 };

	int letters_count[5] = { 0 };

	for ( std::string::size_type i = 0 ; i < answer.size() ; i ++) {
	
		for ( std::string::size_type j = 0  ; j < answer.size() ; j++) {

			if (answer[i] == answer[j])
				letters_count[i]++;
		}
	}
	
	for ( std::string::size_type i = 0 ; i < answer.size() ; i ++) {
	
			if (answer[i] == word[i]) {
				letters_count[i]--;
				colors[i] = GREEN;
				for ( std::string::size_type j = i + 1  ; j < answer.size() ; j++) {

					if (answer[i] == answer[j])
						letters_count[j]--;
				}
			}
	}

	for ( std::string::size_type i = 0 ; i < answer.size() ; i++) {
	
		for ( std::string::size_type j = 0  ; j < answer.size() ; j++) {

			if ( word[i] == answer[j] && colors[i] != GREEN && letters_count[i] != 0 ) {
				
				letters_count[i]--;
				colors[i] = YELLOW;
				for ( std::string::size_type j = i + 1  ; j < answer.size() ; j++) {

					if (word[i] == answer[j])
						letters_count[j]--;
				}
			}
		}
	}
	
	for (std::string::size_type i = 0; i < answer.size() ; i++ ) {

		switch (colors[i]) {

			case GREEN :
				std::cout << BACKGROUND_GREEN;
				break;
			case YELLOW :
				std::cout << BACKGROUND_YELLOW;
				break;
			default :
				break;
		}
		std::cout << word[i] << BACKGROUND_DEFAULT;
		if ( i != 4 )
			std::cout << " ";
	}
	std::cout << std::endl;
	return ;
}

void	printGrid( std::vector< std::string > const & grid, std::string const & answer ) {

	for ( std::vector< std::string >::const_iterator it = grid.begin() ; it != grid.end() ; ++it ) {

		printWord( *it, answer );
	}

	return;
}

int	main() {

	std::unordered_set< std::string > dictionnary;

	size_t guess = 1;

	fillDictionnary( dictionnary );

	std::vector< std::string > grid;
	std::string	answer;

	std::random_device	rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<std::mt19937::result_type> dist( 1, dictionnary.size() );
	size_t lol = dist(rng);

	std::cout << BACKGROUND_GREEN << "LOLOLOL" << BACKGROUND_DEFAULT << std::endl;

	std::cout << "lol is " << lol << std::endl;

	{
		
		std::unordered_set<std::string>::iterator it = dictionnary.begin();
		for (size_t i = 1; i < lol ; i++, it++ )
			;
		answer = *it;
	}
	
	std::cout << "answer is " << answer << std::endl;

	for ( size_t i = 0 ; i < 6 ; ++i ) {

		grid.push_back("_____");
	}
	/* testing if parsing of dictionnary went fine
	std::unordered_set<std::string>::iterator it = dictionnary.find("zappy");
	if ( it == dictionnary.end() )
		std::cout << "not ";
	std::cout << "found" << std::endl;
	it = dictionnary.find("asdasdasfdasdfasfa");
	if ( it == dictionnary.end() )
		std::cout << "not ";
	std::cout << "found" << std::endl;
	it = dictionnary.find("");
	if ( it == dictionnary.end() )
		std::cout << "not ";
	std::cout << "found" << std::endl;
	end of test*/

	std::cout << "Total words available: " << dictionnary.size() << std::endl;

	printGrid( grid, answer );

	do {

		std::unordered_set<std::string>::iterator it;
		std::string input;
		std::cout << "input: ";
		do {
			getline( std::cin, input );
			it = dictionnary.find( input );
			if ( std::cin.eof() == 1)
				std::cerr << "End Of File reached" << std::endl;
			else if (input.size() != 5 )
				std::cout << "The word must be five letter long!" << std::endl;
			else if ( it == dictionnary.end() )
				std::cout << "The word is not in the word list" << std::endl;
		} while ( std::cin.eof() == 0 &&  ( input.size() != 5 || it == dictionnary.end() ) );
		grid[guess - 1] = input;
		printGrid(grid, answer);
		guess++;
	} while ( guess < 7 && std::cin.eof() == 0 );

	return 0;
}
