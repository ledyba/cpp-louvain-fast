.PHONY: sample

sample:
	g++ -o sample sample.cpp -lglog -std=c++11
	./sample
	rm sample