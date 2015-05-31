# The Louvain method for community detection in large networks

Implementation of [Louvain method](https://perso.uclouvain.be/vincent.blondel/research/louvain.html) in C++.

# How to

Copy louvain.h to your project. 

Here is the sample code:

```cpp
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
```

# Reference

Fast unfolding of communities in large networks,   
Vincent D Blondel, Jean-Loup Guillaume, Renaud Lambiotte, Etienne Lefebvre,   
Journal of Statistical Mechanics: Theory and Experiment 2008 (10), P10008 (12pp)  
doi: [10.1088/1742-5468/2008/10/P10008](http://dx.doi.org/10.1088%2F1742-5468%2F2008%2F10%2FP10008). ArXiv: [http://arxiv.org/abs/0803.0476](http://arxiv.org/abs/0803.0476)