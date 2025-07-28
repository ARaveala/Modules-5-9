/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:49:19 by shaboom           #+#    #+#             */
/*   Updated: 2025/07/28 18:26:35 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include "PmergeMe.hpp"
#include <sstream>
#include <chrono>

template<typename Container>
static void printContainer(const Container& cont, std::string type) {
    std::cout << type;
	for (const auto& elem : cont) {
        std::cout <<elem << " ";
    }
    std::cout << std::endl;
}


void PmergeMe::test()
{
	//TimeDiff1
	std::cout<<"insertion sort vecotr\n";
	printContainer(m_vec, " vector::numbers before ");
	printContainer(m_lst, " List::numbers before ");
	auto start_vec = std::chrono::high_resolution_clock::now();
	insertionSortVec();
	auto end_vec = std::chrono::high_resolution_clock::now();
	auto start_lst = std::chrono::high_resolution_clock::now();
	insertionSortList();
	auto end_lst = std::chrono::high_resolution_clock::now();
	printContainer(m_vec, "  after vector sorted");
	printContainer(m_lst,  "  after list sorted");
	
    std::chrono::duration<double, std::micro> duration1 = end_vec - start_vec;
    std::cout << "Sorting took " << duration1.count() << " µs" << std::endl;
    std::chrono::duration<double, std::micro> duration2 = end_lst - start_lst;
    std::cout << "Sorting took " << duration2.count() << " µs" << std::endl;
}

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const std::string& set) {
	fillContainers(set);
}

void PmergeMe::fillContainers(const std::string& set)
{
	std::istringstream tokens(set);
    std::string num;

	int convertedNum = 0;
	
	while (tokens >> num)
	{
		try
		{
			convertedNum = stoi(num);
			// if (convertedNum > or something) throw
			m_lst.push_back(convertedNum);
			m_vec.push_back(convertedNum);
		} catch(const std::exception& e) {
			std::cerr << "stoi failure in construction" << e.what() << '\n';
		}
	}	
}
PmergeMe::~PmergeMe() {}

/**
 * @brief bit shifting here by 1 is the same as / 2
 * 
 * @param vec 
 * @param key 
 * @param left 
 * @param right 
 * @param comp 
 * @return unsigned int 
 */

int PmergeMe::insertPointVec(std::vector<unsigned int>& cont , unsigned int key, int left, int right) {
    while (left <= right) {
		unsigned int mid = left + ((right - left) >> 1);
        if (cont[mid] == key) {
            return mid + 1;	
		}
		else if (cont[mid] < key) {
            left = mid + 1;
		}
        else {
            right = mid - 1;
		}
	}
    return left;
}

/**
 * @brief Insertion sort is an algorithm that builds the sorted array one element at a time 
 * by taking each new element and inserting it into its correct position within the already sorted portion.
 * It repeatedly compares and shifts elements until the new element is in its proper place. Random placement would
 * for example no longer be an insertion sort. 
 * 
 * comparisons are any comparison in the sorting process. 
 * 
 * @param vec 
 * @param left 
 * @param right 

 */

void PmergeMe::insertionSortVec() {
	//int comp = 0;
	std::vector<unsigned int> small, large;
	
	// Step 1: Pair and split
	for (size_t i = 0; i + 1 < m_vec.size(); i += 2) {
	    //comp++;
		if (m_vec[i] < m_vec[i + 1]) {
	        small.push_back(m_vec[i]);
	        large.push_back(m_vec[i + 1]);
	    } else {
	        small.push_back(m_vec[i + 1]);
	        large.push_back(m_vec[i]);
	    }
	}
	if (m_vec.size() % 2 != 0) {
	    small.push_back(small.back());
	}
	std::sort(large.begin(), large.end());
	for (const unsigned int& key: small) {
    	int pos = insertPointVec(large, key, 0, large.size() - 1);
    	large.insert(large.begin() + pos, key);
	}
	m_vec = large;
}


void PmergeMe::insertionSortList() {
	//int comp = 0;
	
    if (m_lst.empty()) {return;} 
	std::list<unsigned int> small, large;
	for (std::list<unsigned int>::iterator it = m_lst.begin(); it != m_lst.end(); it ++) {
		std::list<unsigned int>::iterator next =  std::next(it);
		//comp++;
	    if (next == m_lst.end()) {
	        small.push_back(*it);
	        break;
	    }
		if (*it < *next) {
	        small.push_back(*it);
	        large.push_back(*next);
	    } else {
	        small.push_back(*next);
	        large.push_back(*it);
	    }
		it = m_lst.erase(it);
	}

	large.sort();
    std::list<unsigned int>::iterator it = small.begin();
    for (; it != small.end(); ++it) {
        unsigned int key = *it;
		//comp++;
        std::list<unsigned int>::iterator insertionPoint = large.begin();
        while (insertionPoint != large.end() && *insertionPoint < key) {
			//comp ++;
		    insertionPoint++;
        }
		large.insert(insertionPoint, key);
	}
	m_lst = large;
}

