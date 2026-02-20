#include <iostream>
#include <string>
#include <chrono>

#include "IMDBData.h"

// Option 1
void moviesFromActor(IMDBData& data)
{
	std::string aName;
	std::cout << "Enter an actor's name" << std::endl;
	std::cout << "> ";
	std::getline(std::cin, aName);

	const std::vector<std::string>& movieList = data.getMoviesFromActor(aName);

	if (movieList.size() > 0)
	{
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << aName << " has been in " << movieList.size() << " movies: " << std::endl;
		for (std::string s : movieList)
		{
			std::cout << s << std::endl;
		}
		std::cout << "--------------------------------------------------" << std::endl;
	}
	else
	{
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << aName << " has no known movies!" << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;
	}
}

// Option 2
void actorFromMovies(IMDBData& data)
{
	std::string mName;
	std::cout << "Enter a movie title (date)" << std::endl;
	std::cout << "> ";
	std::getline(std::cin, mName);

	const std::vector<std::string>& actorList = data.getActorsFromMovie(mName);

	if (actorList.size() > 0)
	{
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << mName << " had " << actorList.size() << " actors: " << std::endl;
		for (std::string s : actorList)
		{
			std::cout << s << std::endl;
		}
		std::cout << "--------------------------------------------------" << std::endl;
	}
	else
	{
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << mName << " is unknown!" << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;
	}
}

// Option 3
void relationship(IMDBData& data)
{
	std::string firstActor;
	std::string secondActor;

	std::cout << "Enter the first actor's name" << std::endl;
	std::cout << "> ";
	std::getline(std::cin, firstActor);

	std::cout << "Enter the second actor's name" << std::endl;
	std::cout << "> ";
	std::getline(std::cin, secondActor);

	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << data.findRelationship(firstActor, secondActor);
	std::cout << "--------------------------------------------------" << std::endl;
}


int main(int argc, char** argv)
{
	std::cout << "Enter an actor file to load: " << std::endl;
	std::cout << "> ";
	std::string fname;
	std::getline(std::cin, fname);

	std::cout << "Loading..." << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	IMDBData data(fname);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	float seconds = duration / 1000000000.0f;

	std::cout << "Took " << seconds << "s\n";

	std::cout << "Creating graph..." << std::endl;

	start = std::chrono::high_resolution_clock::now();

	data.createGraph();

	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	seconds = duration / 1000000000.0f;

	std::cout << "Took " << seconds << "s\n";

	while (true)
	{
		std::cout << std::endl;
		std::cout << "Main Menu" << std::endl;
		std::cout << "1. Get movies from actor" << std::endl;
		std::cout << "2. Get actors from movies" << std::endl;
		std::cout << "3. Find relationship between actors" << std::endl;
		std::cout << "4. Quit" << std::endl;
		std::cout << "> ";

		std::string option;
		std::getline(std::cin, option);

		if (option == "1")
		{
			moviesFromActor(data);
		}
		else if (option == "2")
		{
			actorFromMovies(data);
		}
		else if (option == "3")
		{
			relationship(data);
		}
		else if (option == "4")
		{
			std::cout << "Goodbye!" << std::endl;
			break;
		}
		else
		{
			std::cout << "Invalid option, please try again." << std::endl;
		}
	}

	return 0;

}
