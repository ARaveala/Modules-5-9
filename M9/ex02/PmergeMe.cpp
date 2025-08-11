/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:49:19 by shaboom           #+#    #+#             */
/*   Updated: 2025/08/11 16:34:43 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include "PmergeMe.hpp"
#include <sstream>
#include <chrono>
#include <limits.h>
#include <exception>
#include <set>

/**
 * @brief
 *  vector based : im implementing Ford-Johnson with the bounded insertion optimization.
 * list based : im using the Ford-Johnson insertion schedule, but with unbounded scans.
 * 
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

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const std::string& set) {
	fillContainers(set);
}

/**
 * @brief This function converst the number and puts the number into both deque and list.
 * By copying list into a set we can compare length of each to see if there where duplicate values
 */
void PmergeMe::fillContainers(const std::string& set)
{
	std::istringstream tokens(set);
    std::string num;

	int convertedNum = 0;
	while (tokens >> num)
	{
		try {
			convertedNum = stoi(num);
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

void PmergeMe::test()
{
	std::cout<<"insertion sort vecotr\n";
	printContainer(m_lst, " List::numbers before ");
	size_t numCountLst = m_lst.size();
	size_t numCountVec = m_vec.size();
	
	auto start_vec = std::chrono::high_resolution_clock::now();
	beginMergeInsertionSortVec();
	auto end_vec = std::chrono::high_resolution_clock::now();
	auto start_lst = std::chrono::high_resolution_clock::now();
	beginMergeInsertionSortlst();
	auto end_lst = std::chrono::high_resolution_clock::now();
	checkFinalOrderVec(m_vec, "final order for vector", numCountVec);
	checkFinalOrderLst(m_lst, "on final check ", numCountLst);
    std::chrono::duration<double, std::micro> duration1 = end_vec - start_vec;
    std::cout << "Sorting took " << duration1.count() << " µs" << std::endl;
	std::cout<<"m_comp_vec at end = " << m_comp_vec << std::endl;
    std::chrono::duration<double, std::micro> duration2 = end_lst - start_lst;
    std::cout << "Sorting took " << duration2.count() << " µs" << std::endl;
	std::cout<<"m_comp_lst at end = " << m_comp_lst << std::endl;
}

/**
 * @brief 
 * 
 * @param maxSize 
 * @return std::vector<size_t> 
 * Although the sorting is performed on a std::list, the Jacobsthal insertion schedule is index-based.
 * Since std::list lacks random access, I use a std::vector<size_t> to generate and store the insertion order.
 * This allows me to traverse the list using std::next() and insert elements in the correct sequence.
 * The vector is not used to manipulate the list directly, only to guide the insertion logic.”
 */

std::vector<size_t> PmergeMe::generateJacobsthalIndices(size_t maxSize) {
    std::vector<size_t> indices;
    std::vector<bool> seen(maxSize, false);
	
	for (size_t j0 = 0, j1 = 1; ; ) {
		size_t next = j1 + 2 * j0;
        if (next >= maxSize) {
			break;
		}
        indices.push_back(next);
        seen[next] = true;
        j0 = j1;
        j1 = next;
    }
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


size_t PmergeMe::findAnchorIndexVec(const std::vector<std::pair<unsigned int, unsigned int>>& vec, size_t anchor) {
    for (size_t j = 0; j < vec.size(); ++j) {
        if (vec[j].second == anchor) {
			return j;
		}
    }
    return vec.size(); // fallback
}

std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator
PmergeMe::findInsertPositionLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
    std::list<unsigned int>::iterator anchor
) {
    for (auto it = large.begin(); it != large.end(); ++it) {
        if (it->second == anchor|| *(it->second) == *anchor) {
            return it;
        }
    }
    return large.end(); // insert at end if no match
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

/**
 * @brief bit shifting here by 1 is the same as / 2
 * 
 * @param cont
 * @param key 
 * @param left 
 * @param right  
 * @return a predicted position 
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

std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator PmergeMe::insertPointLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& cont,
    unsigned int key,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator left,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator  right,
	bool recursion
) {

	auto it = std::next(right);
	if (recursion && key < it->first) {
	    while (it != cont.begin()) {
	        --it;
	        m_comp_lst++;
	        if (it->first < key) {
				++it;
	            return it;
	        }
	    }
	    return cont.begin(); // insert at front
	}
	
	it = left;
	// Otherwise, scan forward
	while (it != cont.end()) {
	    m_comp_lst++;
	    if (it->first > key) {
			return it;
	    }
	    ++it;
	}
	return cont.end();

}

std::vector<std::pair<unsigned int, unsigned int>>  PmergeMe::splitResultsVec(std::vector<std::pair<unsigned int, unsigned int>>& Originallarge) {
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
		std::vector<std::pair<unsigned int, unsigned int>> sortedLarge = splitResultsVec(large);
		insertionSortVec(smallIndex, sortedLarge, small);
	return sortedLarge;
}

std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> PmergeMe::splitResultsLst(
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& originalLarge,
    int depth
) {
	
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> large;
	std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> small;
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> smallAnchors;
 	if (originalLarge.size() <= 1) {
		return originalLarge;
	}

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
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> sortedLarge = splitResultsLst(large, depth + 1);
	insertionSortLst(smallAnchors, sortedLarge, false);
    return sortedLarge;
}
/**
 * @brief Insertion sort is an algorithm that builds the sorted array one element at a time 
 * by taking each new element and inserting it into its correct position within the already sorted portion.
 * It repeatedly compares and shifts elements until the new element is in its proper place. Random placement would
 * for example no longer be an insertion sort. 
 * 
 * comparisons are any comparison in the sorting process. 
 * These index values are used to pick which loser to insert next not where to insert.
 * 
 * 
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
	std::vector<std::pair<unsigned int, unsigned int>> new_large = splitResultsVec(large);
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
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> sortedLarge = splitResultsLst(large, 0);
	checkOrderLstPair(sortedLarge, " sortedLarge ");
    insertionSortLst(small, sortedLarge, true);
    m_lst.clear();
	for (const auto& entry : sortedLarge) {
    	m_lst.push_back(entry.first);
	}
}

void PmergeMe::insertionSortLst(
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& small,
    std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
	bool recursion

) {
    std::vector<size_t> insertionOrder = generateJacobsthalIndices(small.size());
    std::vector<bool> inserted(small.size(), false);
	std::list<std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator> iteratorLookup;
	for (auto it = small.begin(); it != small.end(); ++it) {
    	iteratorLookup.push_back(it);
	}
	for (size_t i : insertionOrder) {
        if (i >= iteratorLookup.size() || inserted[i]) {continue;}
		auto it = std::next(iteratorLookup.begin(), i);
		auto smallIt = *it;
        unsigned int key = smallIt->first;
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator anchor = findInsertPositionLst(large, smallIt->second);//smallIt->second;
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator insertPos = {};
		if (recursion) {
			insertPos = insertPointLst(large, key, large.begin(), anchor, false);
		} else {
			insertPos = insertPointLst(large, key, large.begin(), anchor, true);
		}
		large.splice(insertPos, small, smallIt);
		inserted[i] = true;
	}
}

PmergeMe::~PmergeMe() {}