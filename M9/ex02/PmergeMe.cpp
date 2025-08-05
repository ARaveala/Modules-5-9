/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:49:19 by shaboom           #+#    #+#             */
/*   Updated: 2025/08/05 17:44:27 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include "PmergeMe.hpp"
#include <sstream>
#include <chrono>
#include <limits.h>

template<typename Container>
static void printContainer(const Container& cont, std::string type) {
    std::cout << type;
	unsigned int i = 0;
	for (const auto& elem : cont) {
        std::cout <<elem << " ";
		i++;
	}
    std::cout <<i<<" numbers in set" <<std::endl;
}

template<typename Container>
static void printContainerPair(const Container& cont, const std::string& type) {
    std::cout << type << "\n: ";
    for (const auto& elem : cont) {
        std::cout << "(" << elem.first << ", " << elem.second << ") ";
    }
    std::cout << std::endl;
}

static void checkOrder(std::vector<std::pair<unsigned int, unsigned int>>& m_vec) {
	for (size_t i = 0; i + 1 < m_vec.size(); ++i) {
		if (m_vec[i].first > m_vec[i + 1].first){
			std::cout<<"---error numbers not in order in vector sort at index " << i << " value = " << m_vec[i].first << " next value = " << m_vec[i + 1].first << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

size_t PmergeMe::findAnchorIndexVec(const std::vector<std::pair<unsigned int, unsigned int>>& vec, size_t anchor) {
    for (size_t j = 0; j < vec.size(); ++j) {
        if (vec[j].second == anchor) {
			return j;
		}
    }
    return vec.size(); // fallback
}

void PmergeMe::insertionSortVec(std::vector<std::pair<unsigned int, unsigned int>>& small, 
	std::vector<std::pair<unsigned int, unsigned int>>& large, const std::vector<std::pair<unsigned int, unsigned int>>& orgIndex) {
	
	std::vector<size_t> insertionOrder = generateJacobsthalIndices(small.size());
	int pos = 0;
	std::vector<bool> inserted(small.size(), false);
	for (size_t i : insertionOrder) {
		if (i >= small.size() || inserted[i]) {
			continue;
		}
	    unsigned int key = small[i].first;
		size_t anchor = small[i].second;
		if (large.size() < 2) {
				large.insert(large.begin(), std::make_pair(key, orgIndex[i].second));
				break;
		}	
		anchor = findAnchorIndexVec(large, anchor);
		pos = insertPointVec(large, key, 0, (anchor == 0 ? 0 : anchor - 1));
		if (!orgIndex.empty() && i < orgIndex.size()) {
			large.insert(large.begin() + pos, std::make_pair(key, orgIndex[i].second));
		}
		else {
			large.insert(large.begin() + pos, std::make_pair(key, UINT_MAX));			
		}
		inserted[i] = true; // Mark as inserted
	}
	for (size_t i = 0; i < large.size(); ++i) {
		m_vec[i] = large[i].first;
	}
}

void PmergeMe::test()
{
	//TimeDiff1
	std::cout<<"insertion sort vecotr\n";
	//printContainer(m_vec, " vector::numbers before ");
	printContainer(m_lst, " List::numbers before ");
	auto start_vec = std::chrono::high_resolution_clock::now();
	beginMergeInsertionSortVec();
	auto end_vec = std::chrono::high_resolution_clock::now();
	auto start_lst = std::chrono::high_resolution_clock::now();
	beginMergeInsertionSortlst();
	//insertionSortList();
	auto end_lst = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i + 1 < m_vec.size(); ++i) {
		if (m_vec[i] > m_vec[i + 1]){
			std::cout<<"error numbers not in order in vector sort at index " << i << " value = " << m_vec[i] << " next value = " << m_vec[i + 1] << std::endl;
			return;
		}
	}
	std::list<unsigned int>::iterator it = m_lst.begin();
	while (std::next(it) != m_lst.end()) {
		auto next = std::next(it);
	//for (size_t i = 0; i + 1 < m_lst.size(); ++i) {
		if (*it > *next){
			std::cout<<"error numbers not in order in lst value = " << *it << " next value = " << *next << std::endl;
			return;
		}
		it++;
	}

	printContainer(m_vec, "  after vector sorted");
	
	printContainer(m_lst,  "  after list sorted");
	
    std::chrono::duration<double, std::micro> duration1 = end_vec - start_vec;
    std::cout << "Sorting took " << duration1.count() << " µs" << std::endl;
	std::cout<<"m_comp_vec at end = " << m_comp_vec << std::endl;
    std::chrono::duration<double, std::micro> duration2 = end_lst - start_lst;
    std::cout << "Sorting took " << duration2.count() << " µs" << std::endl;
	std::cout<<"m_comp_lst at end = " << m_comp_lst << std::endl;
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
		try {
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

std::vector<size_t> PmergeMe::generateJacobsthalIndices(size_t maxSize) {
    std::vector<size_t> indices;
    std::vector<bool> seen(maxSize, false);
	
	for (size_t j0 = 0, j1 = 1; ; ) {
        size_t next = (indices.empty()) ? j0 : j1 + 2 * j0;
        if (next >= maxSize) {
			break;
		}
        indices.push_back(next);
        seen[next] = true;
        j0 = j1;
        j1 = next;
    }
	// Collect remaining unseen indices

    std::vector<size_t> remaining;
    for (size_t i = 0; i < maxSize; ++i) {
        if (!seen[i])
            remaining.push_back(i);
    }
	for (size_t i = 0; i < remaining.size() / 2; ++i) {
        indices.push_back(remaining[i]);
        indices.push_back(remaining[remaining.size() - 1 - i]);
    }
    if (remaining.size() % 2) {
        indices.push_back(remaining[remaining.size() / 2]);
	}
	return indices;
}
/**
 * @brief bit shifting here by 1 is the same as / 2
 * 
 * @param vec 
 * @param key 
 * @param left 
 * @param right 
 * @param comp 
 * @return unsigned int 
 * 
 */

int PmergeMe::insertPointVec(std::vector<std::pair<unsigned int, unsigned int>>& cont , unsigned int key, int left, int right) {
    while (left <= right) {
        int mid  = left + ((right - left) >> 1);
        m_comp_vec++;
        if (cont[mid].first < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return left;
}
#include <iterator>
std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator PmergeMe::insertPointLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& cont,
    unsigned int key,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator left,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator  right
) {
	//(void) right;
    //(void) cont;
	auto it = left;
	size_t how_many = 0;
	//auto it = cont.begin();
	/*while (it != cont.end()) {
	    m_comp_lst++;
	    if (it->first > key) {
	        return it;
	    }
	    ++it;
	}*/
/*	if (right == cont.end())
	{
		it = cont.end();
		while (it != cont.begin()) {
    	    --it;
    	    m_comp_lst++;
    	    how_many++;
    	    if (it->first < key) {
    	        std::cout << "Reverse scan comparisons: " << how_many << "\n";
    	        return ++it; // insert after the smaller/equal element
    	    }
    	}
		std::cout << "Reverse scan comparisons: " << how_many << "\n";
		right = ++it;
		//return cont.begin(); // key is smallest
		std::cout<<"catch me if you can \n";
	}*/
	
	while (it != cont.end()){// && it != right) {
		if (how_many == 0)
			std::cout << "Distance to right: " << std::distance(it, right) << "\n";
        m_comp_lst++;
		how_many++;
        if (it->first > key) {
			
			//std::cout<<"returned by actually seeing thaty forst is bigger than key  = "<< how_many<<"\n";
            
			return it;
        }
        ++it;
    }
//	how_many = m_comp_lst;

	std::cout<<"how many comps did we do = "<< how_many<<"\n";
    return it;
}

std::vector<std::pair<unsigned int, unsigned int>>  PmergeMe::splitResultsVec(std::vector<std::pair<unsigned int, unsigned int>>& Originallarge, int depth) {
		if (Originallarge.size() <= 1) {
        return Originallarge;
    }
	std::vector<std::pair<unsigned int, unsigned int>> small;
	std::vector<std::pair<unsigned int,unsigned int>> smallIndex;
	std::vector<std::pair<unsigned int, unsigned int>> large;
	
	for (size_t i = 0; i + 1 < Originallarge.size(); i += 2) {
			m_comp_vec++;	
			if (Originallarge[i].first < Originallarge[i + 1].first ) {
				small.push_back(Originallarge[i]);
			    large.push_back(Originallarge[i + 1]);
				smallIndex.push_back(std::make_pair(Originallarge[i].first, Originallarge[i + 1].second));

			} else {
				small.push_back(Originallarge[i + 1]);
    			large.push_back(Originallarge[i]);
				smallIndex.push_back(std::make_pair(Originallarge[i + 1].first, Originallarge[i].second));
			}
		}
		if (Originallarge.size() % 2 != 0) {
			large.push_back(std::make_pair((Originallarge.back().first), Originallarge.back().second));
		}		
		std::vector<std::pair<unsigned int, unsigned int>> sortedLarge = splitResultsVec(large, depth + 1);
		insertionSortVec(smallIndex, sortedLarge, small);
	return sortedLarge;
}	
/**
 * @brief Insertion sort is an algorithm that builds the sorted array one element at a time 
 * by taking each new element and inserting it into its correct position within the already sorted portion.
 * It repeatedly compares and shifts elements until the new element is in its proper place. Random placement would
 * for example no longer be an insertion sort. 
 * 
 * comparisons are any comparison in the sorting process. 
 * These index values are used to pick which loser to insert next — not where to insert, but when.
 * 
 * 
 * must use Using Jacobsthal Numbers
 * @param vec 
 * @param left 
 * @param right 

 */

void PmergeMe::beginMergeInsertionSortVec() {
	std::vector<std::pair<unsigned int, unsigned int>> small;
	std::vector<std::pair<unsigned int, unsigned int>> large;

	for (size_t i = 0; i + 1 < m_vec.size(); i += 2) {
		m_comp_vec++;
		if (m_vec[i] < m_vec[i + 1] ) {
	        small.push_back(std::make_pair((m_vec[i]), i + 1));
	        large.push_back(std::make_pair((m_vec[i + 1]), i + 1));
		} else {
	        small.push_back(std::make_pair((m_vec[i + 1]), i));
	        large.push_back(std::make_pair((m_vec[i]), i));

		}
	}
	if (m_vec.size() % 2 != 0) {
	    small.push_back(small.back());
	}
	std::vector<std::pair<unsigned int, unsigned int>> new_large = splitResultsVec(large, 0);
	checkOrder(new_large); //testing
	insertionSortVec(small, new_large, {});
}



/// @brief ////////////////////////
void PmergeMe::beginMergeInsertionSortlst() {
    if (m_lst.empty()) return;

	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> small;
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> large;
	std::list<unsigned int>::iterator it = m_lst.begin();
        while (it != m_lst.end()) {
        auto first = it;
        auto second = std::next(it);

        if (second == m_lst.end()) {
            // Odd element left over — move it to small
            small.emplace_back(*first, second);
            break;
        }

        m_comp_lst++;
        if (*first < *second) {
            small.emplace_back(*first, second);
            large.emplace_back(*second, second);
        } else {
            small.emplace_back(*second, first);
            large.emplace_back(*first, first);
        }

        std::advance(it, 2);
    }

	std::cout << "Large befor recursion: ";
	for (const auto& p : large) std::cout << p.first << " ";//<<"( "<< p.second <<" )";
	std::cout << std::endl;
    // Now recurse on large
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> sortedLarge = splitResultsLst(large, 0);
    std::cout << "sortedLarge after recursion: ";
	for (const auto& p : sortedLarge) std::cout << p.first << " ";
	std::cout << std::endl;

	auto test = sortedLarge.begin();
	while (std::next(test) != sortedLarge.end()) {
	    auto next = std::next(test);
	    if (test->first > next->first) {
	        std::cout << "------error numbers not in order in lst value = " << test->first << " next value = " << next->first << std::endl;
	        return;
	    }
	    ++test;
	}
	static std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> emptyAnchors;
    // Insert small into sortedLarge
    insertionSortLst(small, sortedLarge, emptyAnchors);

    // Copy back to m_lst
    m_lst.clear();
	for (const auto& entry : sortedLarge) {
    	m_lst.push_back(entry.first);
	}
    //m_lst.insert(m_lst.end(), sortedLarge.begin(), sortedLarge.end());
}

std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> PmergeMe::splitResultsLst(
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& originalLarge,
    int depth
) {
	std::cout << "Depth: " << depth << ", originalLarge.size(): " << originalLarge.size()
          << ", large.size(): " << originalLarge.size() << std::endl;
    if (originalLarge.size() <= 1) {std::cout<<"returned when originaLarge.size() <= 1\n"; return originalLarge;}

	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> large;
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> small;
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> smallAnchors;



	auto it = originalLarge.begin();
	 while (it != originalLarge.end()) {
    auto firstIt = it;
    auto secondIt = std::next(it);

    if (secondIt == originalLarge.end()) {
        large.splice(large.end(), originalLarge, firstIt);
        break;
    }

    // Advance iterator BEFORE splicing
    std::advance(it, 2);

    m_comp_lst++;

    if (firstIt->first < secondIt->first) {
        small.splice(small.end(), originalLarge, firstIt);
        large.splice(large.end(), originalLarge, secondIt);
        smallAnchors.emplace_back(firstIt->first, secondIt->second);
    } else {
        small.splice(small.end(), originalLarge, secondIt);
        large.splice(large.end(), originalLarge, firstIt);
        smallAnchors.emplace_back(secondIt->first, firstIt->second);
    }
}
	/*while (it != originalLarge.end()) {
	    auto nextIt = std::next(it);
	    if (nextIt == originalLarge.end()) {
	        // Odd element left over
	        large.push_back(*it);
	        break;
	    }
	
	    m_comp_lst++;
	    if (it->first < nextIt->first) {
	        small.push_back(*it);
	        large.push_back(*nextIt);
	        smallAnchors.push_back({it->first, nextIt->second});
	    } else {
	        small.push_back(*nextIt);
	        large.push_back(*it);
	        smallAnchors.push_back({nextIt->first, it->second});
	    }
	
	    std::advance(it, 2);
	}*/

	std::cout << "Large inside recursion: ";
	for (const auto& p : large) std::cout << p.first << " ";
	std::cout << std::endl;
	/*std::cout << "Inserting key: " << key << " with anchor value: " << *anchor << std::endl;
		for (std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator it = large.begin(); it != large.end(); ++it) {
		    std::cout << "Candidate in large: " << it->first << " with anchor value: " << *(it->second) << std::endl;
		}*/
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> sortedLarge = splitResultsLst(large, depth + 1);
	//std::cout<<"did we segvc beofre insertionsort\n";
	/*std::cout << "Large after recursion before insertionSortLst: ";
	for (const auto& p : large) std::cout << p.first << " ";
	std::cout << std::endl;*/

	insertionSortLst(smallAnchors, sortedLarge, small);
	std::cout << "smallanchors after recursion before insertionSortLst: ";
	for (const auto& p : smallAnchors) std::cout << p.first << " ";
	std::cout << std::endl;
	std::cout << "small after recursion before insertionSortLst: ";
	for (const auto& p : small) std::cout << p.first << " ";
	std::cout << std::endl;
	//std::cout<<"did we segvc after insertionsort\n";

    return sortedLarge;
}

void PmergeMe::insertionSortLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& small,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& anchors
) {
     std::vector<size_t> insertionOrder = generateJacobsthalIndices(small.size());
    std::vector<bool> inserted(small.size(), false);
	(void)anchors;
	//std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator lastInsert = large.begin();
     for (size_t i : insertionOrder) {
        if (i >= small.size() || inserted[i]) continue;
       //auto anchorIt = std::next(anchors.begin(), i);

		auto smallIt = std::next(small.begin(), i);
        unsigned int key = smallIt->first;
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator anchor = findInsertPositionLst(large, smallIt->second);//smallIt->second;

		
		
		
		std::cout << "Key: " << key << ", Anchor points to: ";
		if (anchor == large.end()) std::cout << "end()\n";
		else std::cout << anchor->first << "\n";


        auto insertPos = insertPointLst(large, key, large.begin(), anchor);
		//auto insertPos = (anchor == large.end())
	    //? insertPointLst(large, key, lastInsert, large.end())
    	//: insertPointLst(large, key, large.begin(), anchor);
		//anchorIt->second = smallIt->second;
		/*if (!anchors.empty()) {
			
			std::cout << "Insert position before key " << key << ": ";
			if (insertPos == large.end()) std::cout << "end()\n";
			else std::cout << insertPos->first << "\n";
			std::cout << "smallit points at  " << *smallIt->second << " and anchorIt points at: "<<*anchorIt->second<<"\n";
			smallIt->second = large->second;
						//large.splice(insertPos, small, smallIt);
				
		}*/
		//else {
			large.splice(insertPos, small, smallIt);
			
//		}
		//smallIt->second = anchorIt->second;
		//std::cout << "Insert position before key " << key << ": ";
		//if (insertPos == large.end()) std::cout << "end()\n";
		//else std::cout << insertPos->first << "\n";
       
		//large.insert(insertPos, std::make_pair(key, anchorIt->second));
		//lastInsert = insertPos;
		inserted[i] = true;
	}
}


std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator
PmergeMe::findInsertPositionLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
    std::list<unsigned int>::iterator anchor
) {
    for (auto it = large.begin(); it != large.end(); ++it) {
		//for (auto it = large.begin(); it != large.end(); ++it) {
		    /*m_comp_lst++;
			if (it->first == key) {
		        return it;
		    }
		}*/
        if (it->second == anchor|| *(it->second) == *anchor) {
            return it; // ✅ correct insertion point
        }
    }
    return large.end(); // insert at end if no match
}