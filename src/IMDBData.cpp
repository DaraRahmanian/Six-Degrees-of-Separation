#include "IMDBData.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <queue>
#include <sstream>

IMDBData::IMDBData(const std::string& fileName)
{

	// declarations
	std::string line; // holds the line that we read in
	std::string actor; // name of the actor that we are on when parsing through file
	std::string movie; // name of movie that we are on when parsing through file
	std::vector<std::string> movies; // vector of movies

	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw std::invalid_argument("Error: Invalid Argument");
	}
	while (!file.eof())
	{


		// read file and store it in variable line
		std::getline(file, line);

		// Check if the line we read was empty()
		if (line.empty())
		{
			// Go back to the beginning of the loop to read in a new line
			continue;
		}

		// If the first character of line is not '|' 
		// Then we are at an actor name line

		
		if (line.front() != '|')
		{
			

			//If actor name is not empty
			if (!actor.empty())
			{
				// Add key and value pair {actor, movies} to hashMap
				mActorsToMoviesMap[actor] = movies;

				// Call reverseMap to fill mMvoiesToActorsMap
				reverseMap(actor, mActorsToMoviesMap[actor]);
			}

			// Set the actor variable to equal line
			actor = line;
			
			// Clear movies vector
			movies.clear();
		}

		// Otherwise we are at a movie name line
		else
		{
			//Set variable movie with line from index 1 to end 
			movie = line.substr(1);
			
			//push back string movie to vectors movies
			movies.push_back(movie);
		}
	}

	// Finished parsing through file

	// Add key and value pair {actor, movies} to hashMap for the last actor
	mActorsToMoviesMap[actor] = movies;

	// Call reverseMap to fill mMvoiesToActorsMap
	reverseMap(actor, mActorsToMoviesMap[actor]);
}

void IMDBData::reverseMap(const std::string& actorName, const std::vector<std::string>& movies)
{	
	// Iterate through map of movies
	for (int i = 0; i < movies.size(); i++)
	{
		// Create a vector and push back actorName to it
		// This will be used when creating an element in hashmap

		//actors.push_back(actorName);

		// 1. If mMoviesToActorsMap already contains the movie in question, you need to add
		//    actorName to that movie’s associated vector
		if (mMoviesToActorsMap.find(movies[i]) != mMoviesToActorsMap.end())
		{
			mMoviesToActorsMap[movies[i]].push_back(actorName);
		}

	// 2. Otherwise, you need to add an entry in mMoviesToActorsMap that associates the movie
	//    with a vector that, for now, only contains actorName

		else
		{
			// Create an element in hashmap mMoviesToActorsMap with key as movie and value as actors
			std::vector<std::string> actors;
			actors.push_back(actorName);
			mMoviesToActorsMap[movies[i]] = actors;
		}
	}
}

const std::vector<std::string>& IMDBData::getMoviesFromActor(const std::string& actorName)
{
	if (mActorsToMoviesMap.find(actorName) != mActorsToMoviesMap.end())
	{
		return mActorsToMoviesMap[actorName];
	}
	return sEmptyVector;
	
}

const std::vector<std::string>& IMDBData::getActorsFromMovie(const std::string& movieName)
{
	if (mMoviesToActorsMap.find(movieName) != mMoviesToActorsMap.end())
	{
		return mMoviesToActorsMap[movieName];
	}
	return sEmptyVector;
}

void IMDBData::createGraph()
{

	// Double for loop (inner and outer) to create an edge between each actor in a movie
	
	// Begin by iterating through the mActorsToMoviesMap hashmap
	// This is to go through all the actors for each movie
	for (const auto& item : mMoviesToActorsMap)
	{
		// Name of the movie stored in "movie"
		const std::string& movie = item.first;

		// vector of actors
		const std::vector<std::string>& actors = item.second;
		
		// Iterate through the vector
		
		for (int i = 0; i < item.second.size(); i++)
		{
			// Get the first actor
			ActorNode* actor1 = mGraph.getActorNode(item.second[i]);
			//const std::string& actor1 = actors[i];

			// Iterate through the vector from 1 + i to the end of the vector
			for (int j = i + 1; j < item.second.size(); j++)
			{
				// Get the second node
				ActorNode* actor2 = mGraph.getActorNode(item.second[j]);
				//const std::string& actor2 = actors[j];

				// Create edge, pass in two actor nodes and the movie name they share
				mGraph.createActorEdge(actor1, actor2, item.first);
			}
		}
	}

}

std::string IMDBData::findRelationship(const std::string& fromActor, const std::string& toActor)
{
	// TODO: Complete function, fix return value

	// Check that the two actors passed to findRelationship are in the graph

	// Create two bool variables determining whether the fromActor and toActor are nodes

	std::ostringstream ss;


	// If fromActor is not a node
	if (!mGraph.containsActor(fromActor))
	{
		// Output error message

		ss << fromActor << " is unknown!" << std::endl;
	}
	
	// If toActor is not a node
	if (!mGraph.containsActor(toActor))
	{
		// Output error message
		ss << toActor << " is unknown!" << std::endl;
	}
	
	if (mGraph.containsActor(fromActor) && mGraph.containsActor(toActor))
	{
		// Create node variables for fromActor and toActor
		ActorNode* beginNode = mGraph.getActorNode(fromActor);
		ActorNode* targetNode = mGraph.getActorNode(toActor);


		// 1. Create a queue of ActorNodes*, and enqueue the node we’re starting from…
		std::queue<ActorNode*> bfsQueue;
		bfsQueue.push(beginNode);

		// 2. While the BFS queue is not empty…
		while (!bfsQueue.empty())
		{

			// a) Dequeue the front ITPGraphNode*, and save in currentNode
			ActorNode* currentNode = bfsQueue.front();
			bfsQueue.pop();

			// b) If currentNode == targetNode, we found a path!
			if (currentNode == targetNode)
			{
				ss << "Found a path! (" << currentNode->mPath.size() << " hops)";
				ss << std::endl << fromActor << " was in..." << std::endl;
				for (int i = 0; i < currentNode->mPath.size(); i++)
				{		
					if (i < targetNode->mPath.size() - 1)
					{
						// If it isn't the last path, output this way
						ss << targetNode->mPath[i].getMovieName() << " with " << targetNode->mPath[i].getActorName() << " who was in..." << std::endl;
					}
					else
					{
						// If it is the last path, output this way
						ss << targetNode->mPath[i].getMovieName() << " with " << targetNode->mPath[currentNode->mPath.size() - 1].getActorName() << std::endl;
					}	
				}

				// Call the clearVisited() function for mGraph to set all mVisited back to false
				mGraph.clearVisited();

				// Return the string stream as w string
				return ss.str();
			}

			// c) Otherwise if currentNode’s visited bool is false…
			else if (currentNode->mIsVisited == false)
			{

				// 1. Visit currentNode


				// 2. Set currentNode visited to true
				currentNode->mIsVisited = true;
				// 3. Loop through currentNode’s adjacency list and if the visited flag is false…
				for (auto& adjEdge : currentNode->mEdges)
				{
					ActorNode* adjNode = adjEdge.mOtherActor;
					//a) enqueue the adjacent node
					bfsQueue.push(adjNode);
					//b) If the adjacent node path has a size of 0…
					if (adjNode->mPath.size() == 0)
					{
						//a) Set the adjacent node’s path equal to currentNode’s path
						adjNode->mPath = currentNode->mPath;
						//b) Push_back adjacent nodes’s relevant path information(destination name and label in ITPPathInfo struct)
						adjNode->mPath.push_back(PathPair(adjEdge.mMovieName, adjEdge.mOtherActor->mName));
					}
					//ss << adjNode->mPath.size();

				}



			}
		}
		
			
			//3. If we made it here, we didn’t find a path
		ss << "Didn't find a path." << std::endl;
	}


	// Call the clearVisited() function for mGraph to set all mVisited back to false
	mGraph.clearVisited();

	// Return the string stream as a string
	return ss.str();
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!! Leave this here, DO NOT REMOVE THIS LINE !!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
std::vector<std::string> IMDBData::sEmptyVector;
