/* coding: utf-8 */
/**
 * cpp-louvain-fast
 *
 * Copyright 2015, PSI
 */
// sample.
// This program creates the dummy friendship graph
// and then, clustering by this library.

#include "louvain.h"
#include <vector>
#include <iostream>

// Data structure that stick to each node.
struct Person {
	int id;
};

struct Merger {
	// It is called when the algorithm merge the nodes into the cluster.
	Person operator()(std::vector<louvain::Node<Person> > const& nodes) const{
		// Select the most popular person
		louvain::Node<Person> const* most_popular = &nodes.front();
		for(auto it = nodes.begin(); it != nodes.end(); ++it){
			auto next = *it;
			if(most_popular->degree() < it->degree()){
				most_popular = &*it;
			}
		}
		return most_popular->payload();
	}
};

int main(int argc, char** argv){
	std::mt19937 mt((std::random_device()()));
	// make 100 persons
	std::vector<louvain::Node<Person>> persons(100);
	int totalLinks = 0;
	// connect the friends
	for(int i=0;i<100;++i){
		persons[i].payload().id = i;
		int from = mt() % persons.size();
		int to = mt() % persons.size();
		int weight = mt() % 100;
		if(from == to){
			persons[from].selfLoops(persons[from].selfLoops()+weight);
		}else{
			persons[from].neighbors().push_back(std::pair<int,int>(to,weight));
		}
		totalLinks += weight;
	}
	// clustering hierarchically
	louvain::Graph<Person, Merger> graph(totalLinks, std::move(persons));
	for(int i=0;i<10;++i){
		const size_t nedges = graph.edges();
		const size_t nnodes = graph.nodes().size();
		std::cout << "Edges: " << nedges << " / Nodes: " << nnodes << std::endl;
		graph = graph.nextLevel();
		// exit if it converged
		if( graph.edges() == nedges && graph.nodes().size() == nnodes ) {
			break;
		}
	}
	return 0;
}
