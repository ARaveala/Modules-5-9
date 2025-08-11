/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:28:24 by shaboom           #+#    #+#             */
/*   Updated: 2025/08/11 16:24:24 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include "PmergeMe.hpp"


std::string generateUniqueNumbers(size_t count, int maxValue = 100000000) {
    std::set<int> unique; //ensures no doubles
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, maxValue);

    while (unique.size() < count) {
        unique.insert(dist(gen));
    }
	std::string numSet;
	for (int num : unique) {
		numSet += std::to_string(num) + " ";
	}
	std::istringstream iss(numSet);
	std::set<int> seen;
	int val;
	while (iss >> val) {
	    if (!seen.insert(val).second) {
	        std::cout << "⚠️ Duplicate found in generated string: " << val << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return numSet;
}

// Dump failing input to file
void dumpToFile(const std::string& data, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for (char num : data) {
        out << num << " ";
    }
    out << std::endl;
    out.close();
    std::cout << "❌ Failure dumped to " << filename << std::endl;
}

int main(int argc, char** argv) {
    
	if (argc == 2) {
		std::string arg = argv[1];
		if (arg.find_first_not_of(" 0123456789") != std::string::npos)
		{
			std::cout<<"only positive integers allowed in set\n";
			exit(EXIT_FAILURE);
		}
        try {
			PmergeMe test(arg);		
		} catch(const std::exception& e) {
			std::cerr << e.what() << '\n';
			exit(EXIT_FAILURE);
		}
		

	}
	else {
		std::cout<<"running manual tests  \n";
		
	}
	const int testRuns = 20;       //  Number of test iterations
    const int numCount = 1000;      // Numbers per test
    int failures = 0;

    for (int i = 0; i < testRuns; ++i) {
        std::string numSet = generateUniqueNumbers(numCount);
        try {
            PmergeMe test(numSet);
            test.test();
        } catch (const std::exception& e) {
            ++failures;
            std::cerr << "Test #" << i + 1 << " failed: " << e.what() << std::endl;

            // Save the failing input
            std::string filename = "failed_test_" + std::to_string(i + 1) + ".txt";
            dumpToFile(numSet, filename);
        }
		numSet ="";
    }

    std::cout << "\n✅ Completed " << testRuns << " tests with " << failures << " failures.\n";
    return 0;
}
