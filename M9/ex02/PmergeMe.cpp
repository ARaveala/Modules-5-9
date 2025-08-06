/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:49:19 by shaboom           #+#    #+#             */
/*   Updated: 2025/08/06 16:16:15 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include "PmergeMe.hpp"
#include <sstream>
#include <chrono>
#include <limits.h>
#include <exception>
/**
 * @brief
 *  vector based : “I’m implementing Ford-Johnson with the bounded insertion optimization.”
 * list based : “I’m using the Ford-Johnson insertion schedule, but with unbounded scans.”
That’s totally fair — and often more robust, especially if the anchor logic is fragile.
 * 
 * @tparam Container 
 * @param cont 
 * @param type 
 */
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

static void checkOrderVecPair(std::vector<std::pair<unsigned int, unsigned int>>& vec, const std::string& msg) {
	for (size_t i = 0; i + 1 < vec.size(); ++i) {
		if (vec[i].first > vec[i + 1].first){
			throw std::runtime_error("ERROR @( " + msg + " ) value = " + std::to_string(vec[i].first) + "is bigger than next value = " + std::to_string(vec[i + 1].first) + "\n");

		}
	}
	std::cout<<" vec values ordered @("<<msg<<") current size of large = "<<vec.size()<<"\n";

}

static void checkOrderLstPair(std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& lst, const std::string& msg) {
	for (auto it = lst.begin(); std::next(it) != lst.end(); it++)
	{
		if (it->first > std::next(it)->first) {
			throw std::runtime_error("ERROR @( " + msg + " ) value = " + std::to_string(it->first) + "is bigger than next value = " + std::to_string(std::next(it)->first) + "\n");
		}
	}
	std::cout<<" list values ordered @("<<msg<<") current size of large = "<<lst.size()<<"\n";
}

static void checkFinalOrderVec(std::vector<unsigned int>& vec, const std::string& msg, const size_t& prevSize) {
	if (prevSize != vec.size()) {
		throw std::runtime_error("ERROR @( " + msg + " ) prev size = " + std::to_string(prevSize) + "not equal to current size = " + std::to_string(vec.size()) + "\n");
	}
	std::vector<unsigned int>::iterator it = vec.begin();
	while (std::next(it) != vec.end()) {
		auto next = std::next(it);
		if (*it > *next){
			throw std::runtime_error("ERROR @( " + msg + " ) value = " + std::to_string(*it) + "is bigger than next value = " + std::to_string(*next) + "\n");
		}
		it++;
	}
	std::cout<<" vector values ordered @("<<msg<<") \n";
}

static void checkFinalOrderLst(std::list<unsigned int>& lst, const std::string& msg, const size_t& prevSize) {
	if (prevSize != lst.size()) {
		throw std::runtime_error("ERROR @( " + msg + " ) prev size = " + std::to_string(prevSize) + "not equal to current size = " + std::to_string(lst.size()) + "\n");
	}
	std::list<unsigned int>::iterator it = lst.begin();
	while (std::next(it) != lst.end()) {
		auto next = std::next(it);
		if (*it > *next){
			throw std::runtime_error("ERROR @( " + msg + " ) value = " + std::to_string(*it) + "is bigger than next value = " + std::to_string(*next) + "\n");
		}
		it++;
	}
	std::cout<<" list values ordered @("<<msg<<") \n";
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
	size_t numCountLst = m_lst.size();
	size_t numCountVec = m_vec.size();
	
	auto start_vec = std::chrono::high_resolution_clock::now();
	beginMergeInsertionSortVec();
	auto end_vec = std::chrono::high_resolution_clock::now();
	auto start_lst = std::chrono::high_resolution_clock::now();
	beginMergeInsertionSortlst();
	//insertionSortList();
	auto end_lst = std::chrono::high_resolution_clock::now();
	checkFinalOrderVec(m_vec, "final order for vector", numCountVec);
	checkFinalOrderLst(m_lst, "on final check ", numCountLst);

	//printContainer(m_vec, "  after vector sorted");
	
	//printContainer(m_lst,  "  after list sorted");
	
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
#include <set>
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
	if (m_lst.size() == 1) {
		throw std::runtime_error("need more values than 1 ");		
	}
	std::set<unsigned int> dups(m_lst.begin(), m_lst.end());
	if (m_lst.size() != dups.size()) {
		throw std::runtime_error("no duplicates allowed in number set ");
	}
}
PmergeMe::~PmergeMe() {}

/**
 * @brief 
 * 
 * @param maxSize 
 * @return std::vector<size_t> 
 * “Although the sorting is performed on a std::list, the Jacobsthal insertion schedule is index-based.
 * Since std::list lacks random access, I use a std::vector<size_t> to generate and store the insertion order.
 * This allows me to traverse the list using std::next() and insert elements in the correct sequence.
 * The vector is not used to manipulate the list directly — only to guide the insertion logic.”
 */



std::vector<size_t> PmergeMe::generateJacobsthalIndices(size_t maxSize) {
    std::vector<size_t> indices;
    std::vector<bool> seen(maxSize, false);
	
	for (size_t j0 = 0, j1 = 1; ; ) {
		size_t next = j1 + 2 * j0;
        //size_t next = (indices.empty()) ? j0 : j1 + 2 * j0;
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

//#include <iterator>
std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator PmergeMe::insertPointLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& cont,
    unsigned int key,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator left,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator  right,
	bool recursion
) {
	//(void) right;
	//(void) recursion;
    //(void) cont;
	//(void) key;

	auto it = std::next(right); // try just right if fail found via anchor

// If recursion is active and key < it->first, scan backward
if (recursion && key < it->first) {
    while (it != cont.begin()) {
        --it;
        m_comp_lst++;
        if (it->first < key) {
			++it;
			//std::cout << "Scanning at: " << it->first << std::endl;
            return it; // insert after this
        }
    }
//	std::cout<<"returning cont begin\n";
    return cont.begin(); // insert at front
}

it = left;
// Otherwise, scan forward
while (it != cont.end()) {
    m_comp_lst++;
    if (it->first > key) {
//		std::cout<<"returning it forwards\n";
		return it;
    }
    ++it;
}
//std::cout<<"returning cont end\n";
return cont.end(); // insert at end*/

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
	checkOrderVecPair(new_large, "large vec sorted "); //testing
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

//	std::cout << "Large befor recursion: ";
//	for (const auto& p : large) std::cout << p.first << " ";//<<"( "<< p.second <<" )";
//	std::cout << std::endl;
    // Now recurse on large
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> sortedLarge = splitResultsLst(large, 0);
	// first time anything should be sorted, only large
	checkOrderLstPair(sortedLarge, " sortedLarge ");

	//    std::cout << "sortedLarge after recursion: ";
//	for (const auto& p : sortedLarge) std::cout << p.first << " ";
//	std::cout << std::endl;

	/*auto test = sortedLarge.begin();
	while (std::next(test) != sortedLarge.end()) {
	    auto next = std::next(test);
	    if (test->first > next->first) {
	        std::cout << "------error numbers not in order in lst value = " << test->first << " next value = " << next->first << std::endl;
	        return;
	    }
	    ++test;
	}*/
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
	
   
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> large;
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> small;
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> smallAnchors;
	/*std::cout << "Depth: " << depth << ", originalLarge.size(): " << originalLarge.size()
          << ", large.size(): " << originalLarge.size() << std::endl;*/
 	if (originalLarge.size() <= 1) {
		//std::cout<<"returned when originaLarge.size() <= 1\n"; 
		return originalLarge;

	}


	auto it = originalLarge.begin();
	 while (it != originalLarge.end()) {
    	auto firstIt = it;
    	auto secondIt = std::next(it);

   		if (secondIt == originalLarge.end()) {
        	large.splice(large.end(), originalLarge, firstIt);
			//smallAnchors.emplace_back(firstIt->first, firstIt->second);
       		//smallAnchors.emplace_back(secondIt->first, firstIt->second);
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

	/*std::cout << "Large inside recursion: ";
	for (const auto& p : large) std::cout << p.first << " ";
	std::cout << std::endl;*/
	/*std::cout << "Inserting key: " << key << " with anchor value: " << *anchor << std::endl;
		for (std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator it = large.begin(); it != large.end(); ++it) {
		    std::cout << "Candidate in large: " << it->first << " with anchor value: " << *(it->second) << std::endl;
		}*/
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> sortedLarge = splitResultsLst(large, depth + 1);
	/*std::cout << "Depth: " << depth << ", sortedLarge.size(): " << sortedLarge.size()
          << ", small.size(): " << small.size()  << ", large.size(): " << originalLarge.size() << std::endl;*/
	//std::cout<<"did we segvc beofre insertionsort\n";
	/*std::cout << "Large after recursion before insertionSortLst: ";
	for (const auto& p : large) std::cout << p.first << " ";
	std::cout << std::endl;*/

	insertionSortLst(smallAnchors, sortedLarge, small);
	/*std::cout << "smallanchors after recursion before insertionSortLst: ";
	for (const auto& p : smallAnchors) std::cout << p.first << " ";
	std::cout << std::endl;
	std::cout << "small after recursion before insertionSortLst: ";
	for (const auto& p : small) std::cout << p.first << " ";
	std::cout << std::endl;*/
	//std::cout<<"did we segvc after insertionsort\n";

    return sortedLarge;
}

void PmergeMe::insertionSortLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& small,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& anchors
) {
	//std::cout << "smallsize = "<<small.size()<<": ";
    std::vector<size_t> insertionOrder = generateJacobsthalIndices(small.size());
    std::vector<bool> inserted(small.size(), false);
	std::list<std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator> iteratorLookup;
	for (auto it = small.begin(); it != small.end(); ++it) {
    	iteratorLookup.push_back(it);
	}
	
	(void)anchors;
	//std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator lastInsert = large.end();
    /*std::cout << "inserted vector: ";
	for (bool b : inserted) std::cout << b << " ";
		std::cout << std::endl;

    std::cout << "insertion order: ";
	for (size_t b : insertionOrder) std::cout << b << " ";
		std::cout << std::endl;*/

	for (size_t i : insertionOrder) {
        if (i >= iteratorLookup.size() || inserted[i]) {continue;}
       //auto anchorIt = std::next(anchors.begin(), i);
		auto it = std::next(iteratorLookup.begin(), i);
		auto smallIt = *it;
        unsigned int key = smallIt->first;
		//std::cout << "siz eof small "<<small.size()<<"key value of = (" << key << ") at i = "<<i << std::endl;
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator anchor = findInsertPositionLst(large, smallIt->second);//smallIt->second;

		//auto prevAnchor = (i > 0) ? findInsertPositionLst(large, std::prev(smallIt)->second) : large.begin();
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator insertPos = {};
		//auto left = anchors.empty() ? lastInsert : large.begin();
		if (anchors.empty())
			insertPos = insertPointLst(large, key, large.begin(), anchor, false);
		else
			insertPos = insertPointLst(large, key, large.begin(), anchor, true);
		//std::cout << "intention to splice ------------------------ " << std::endl;
		large.splice(insertPos, small, smallIt);
		//std::cout << "Size after inserting value of = (" << key << "): " << large.size() << std::endl;
		inserted[i] = true;
		
	}


}


std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator
PmergeMe::findInsertPositionLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
    std::list<unsigned int>::iterator anchor
) {
    for (auto it = large.begin(); it != large.end(); ++it) {
        if (it->second == anchor|| *(it->second) == *anchor) {
            return it; // ✅ correct insertion point
        }
    }
    return large.end(); // insert at end if no match
}