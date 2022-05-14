#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <random>

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

void	printGrid( std::vector< std::string > const & grid ) {

	for ( std::vector< std::string >::const_iterator it = grid.begin() ; it != grid.end() ; ++it ) {

		std::cout << *it << std::endl;
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

	std::cout << "lol is " << lol << std::endl;

	{
		
		std::unordered_set<std::string>::iterator it = dictionnary.begin();
		for (size_t i = 1; i < lol ; i++, it++ )
			;
		answer = *it;
	}
	
	std::cout << "answer is " << answer << std::endl;

	for ( size_t i = 0 ; i < 6 ; ++i ) {

		grid.push_back("_ _ _ _ _");
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

	printGrid( grid );

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
//		actualizeGrid( answer, input, grid[guess - 1] );
		guess++;
	} while ( guess < 7 && std::cin.eof() == 0 );

	return 0;
}
