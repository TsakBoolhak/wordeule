#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>

#define BACKGROUND_DEFAULT "\033[49m"
#define FOREGROUND_DEFAULT "\033[39m"
#define BACKGROUND_GREEN "\033[42m"
#define FOREGROUND_GREEN "\033[32m"
#define BACKGROUND_YELLOW "\033[103m"
#define FOREGROUND_YELLOW "\033[33m"
#define BACKGROUND_GREY "\033[47m"
#define FOREGROUND_GREY "\033[37m"

bool	fillDictionnary( std::unordered_set< std::string > & dictionnary , int ac, char **av ) {

	std::ifstream	infile;
	std::string str;

	infile.open( "words.txt", std::ifstream::in );
	if ( infile.is_open() == true ) {

		do {

			str.clear();
			getline( infile, str );
			if ( str.size() == 5)
				dictionnary.insert(str);
		} while ( infile );

		infile.close();
	}

	for ( int i = 1 ; i < ac ; ++i ) {

		infile.open( av[i], std::ifstream::in );
		if ( infile.is_open() == true ) {

			do {

				str.clear();
				getline( infile, str );
				if ( str.size() == 5)
					dictionnary.insert(str);
			} while ( infile );

			infile.close();
		}
	}

	if ( dictionnary.empty() )
		return false;
	return true;
}

#define GREY 0
#define YELLOW 1
#define GREEN 2

void	printWord( std::string & word, std::string const & answer ) {

	int colors[5] = { 0 };

	int letters_count[5] = { 0 };

	for ( std::string::size_type i = 0 ; i < answer.size() ; i ++) {
	
		for ( std::string::size_type j = 0  ; j < answer.size() ; j++) {

			if (answer[i] == answer[j])
				letters_count[i]++;
		}
	}
	
/*
	std::cout << "initial count : ";
	for (int i = 0; i < 5 ; i++)
		std::cout << letters_count[i] << " ";
	std::cout << std::endl;
*/
	for ( std::string::size_type i = 0 ; i < answer.size() ; i ++) {
	
			if (answer[i] == word[i]) {
				colors[i] = GREEN;
				for ( std::string::size_type j = 0  ; j < answer.size() ; j++) {

					if (word[i] == answer[j])
						letters_count[j]--;
				}
			}
	}

/*
	std::cout << "intermediate count : ";
	for (int i = 0; i < 5 ; i++)
		std::cout << letters_count[i] << " ";
	std::cout << std::endl;
*/
	for ( std::string::size_type i = 0 ; i < answer.size() ; i++) {
	
		for ( std::string::size_type j = 0  ; j < answer.size() ; j++) {

			if ( word[i] == answer[j] && colors[i] != GREEN && letters_count[j] > 0 ) {
				
				colors[i] = YELLOW;
				letters_count[j]--;
				for ( std::string::size_type k = j + 1  ; k < answer.size() ; k++) {

					if (answer[j] == answer[k] && letters_count[k] > 0 && colors[k] != GREEN )
						letters_count[k]--;
				}
			}
		}
	}

/*
	std::cout << "final count : ";
	for (int i = 0; i < 5 ; i++)
		std::cout << letters_count[i] << " ";
	std::cout << std::endl;
*/
	std::for_each(word.begin(), word.end(), [](char & c) { c = std::toupper(c); } );
	for (std::string::size_type i = 0; i < answer.size() ; i++ ) {

		switch (colors[i]) {

			case GREEN :
				std::cout << FOREGROUND_GREEN;
				break;
			case YELLOW :
				std::cout << FOREGROUND_YELLOW;
				break;
			default :
				std::cout << "\033[90m";
				break;
		}
		std::cout <<  "\033[1m" << word[i] << "\033[0m" << FOREGROUND_GREY << BACKGROUND_DEFAULT;
		if ( i != 4 )
			std::cout << " ";
	}
	std::cout << std::endl;
	std::for_each(word.begin(), word.end(), [](char & c) { c = std::tolower(c); } );
	return ;
}

void	printGrid( std::vector< std::string > & grid, std::string const & answer ) {

	for ( std::vector< std::string >::iterator it = grid.begin() ; it != grid.end() ; ++it ) {

		std::cout << '\t';
		printWord( *it, answer );
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
	std::random_device	rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<std::mt19937::result_type> dist( 1, dictionnary.size() );
	size_t lol = dist(rng);


	std::cout << FOREGROUND_GREY;
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
			if ( std::cin.eof() == 1) {
				std::cerr << "End Of File reached" << std::endl;
				break;
			}
			if (input.size() != 5 ) {

				std::cout << "The word must be five letter long!" << std::endl;
				printGrid( grid, answer );
				std::cout << "input: ";
				continue;
			}
			std::for_each(input.begin(), input.end(), [](char & c) { c = std::tolower(c); } );
			it = dictionnary.find( input );
			if ( it == dictionnary.end() ) {
				std::cout << "The word is not in the word list" << std::endl;
				printGrid( grid, answer );
				std::cout << "input: ";
			}
		} while ( std::cin.eof() == 0 &&  ( input.size() != 5 || it == dictionnary.end() ) );
		if (std::cin.eof() == 1)
			break;
		grid[guess] = input;
		printGrid(grid, answer);
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
