/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:49:19 by shaboom           #+#    #+#             */
/*   Updated: 2025/07/31 16:20:21 by araveala         ###   ########.fr       */
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
	for (const auto& elem : cont) {
        std::cout <<elem << " ";
    }
    std::cout << std::endl;
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
void PmergeMe::insertionSortList(std::vector<std::pair<unsigned int, unsigned int>>& small, std::vector<std::pair<unsigned int, unsigned int>>& large) {
	//std::cout<<"insertion sort list\n";
	/*for (auto it = m_lst.begin(); it != m_lst.end(); ++it) {
		auto key = *it;
		auto j = it;
		while (j != m_lst.begin() && *(--j) > key) {
			m_comp_vec++;
			//std::cout<<"m_comp_vec = " << m_comp_vec << std::endl;
			//std::cout<<"key = " << key << " j = " << *j << std::endl;
			*(++j) = *it; // Shift larger element to the right
			it = j; // Move iterator to the position of the shifted element
		}
		*(++j) = key; // Insert the key in its correct position
	}*/
std::vector<size_t> insertionOrder = generateJacobsthalIndices(small.size());
	//printContainer(insertionOrder, "Insertion Order: ");
	std::vector<bool> inserted(small.size(), false);
	for (size_t i : insertionOrder) {
		if (i >= small.size() || inserted[i]) {
			//std::cout << "Skipping index " << i << " (out of bounds or already inserted)\n";
			continue; // Skip duplicates
		}

	    unsigned int key = small[i].first;
		//std::cout<<" is key "<<key <<" defeated is "<<defeated[i].second<<"\n";
		//std::cout<<"check defeated winner" << defeated[i].first << std::endl;
		//std::cout<<"check large i " << large[i].first << std::endl;
		//std::cout<<"i is = "<< i <<" check large.second = " << large[i].second << std::endl;
		
		size_t anchor = small[i].second;
		for (size_t j = 0; j < large.size(); ++j) {
			if (large[j].second == anchor) {
				anchor = j;
				//break;
			}
		}
		//std::cout<<"anchor = " << anchor << std::endl;
		int pos = insertPointVec(large, key, 0, anchor - 1);
		//size_t test = large[anchor].second + i;
//		large.insert(large.begin() + pos, std::make_pair(key, test));
		//printContainerPair(large, "Large after insert: ");
		large.insert(large.begin() + pos, std::make_pair(key, UINT_MAX));
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
	printContainer(m_vec, " vector::numbers before ");
	printContainer(m_lst, " List::numbers before ");
	auto start_vec = std::chrono::high_resolution_clock::now();
	insertionSortVec();
	auto end_vec = std::chrono::high_resolution_clock::now();
	auto start_lst = std::chrono::high_resolution_clock::now();
	insertionSortList();
	auto end_lst = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i + 1 < m_vec.size(); ++i) {
		if (m_vec[i] > m_vec[i + 1]){
			std::cout<<"error numbers not in order in vector sort at index " << i << " value = " << m_vec[i] << " next value = " << m_vec[i + 1] << std::endl;
			return;
		}
	}
	printContainer(m_vec, "  after vector sorted");
	printContainer(m_lst,  "  after list sorted");
	
    std::chrono::duration<double, std::micro> duration1 = end_vec - start_vec;
    std::cout << "Sorting took " << duration1.count() << " µs" << std::endl;
	std::cout<<"m_comp_vec at end = " << m_comp_vec << std::endl;
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

std::vector<size_t> PmergeMe::generateJacobsthalIndices(size_t maxSize) {
    std::vector<size_t> indices;
    std::vector<bool> seen(maxSize, false);
	
	size_t j0 = 0, j1 = 1;
    
	if (j0 < maxSize && !seen[j0]) {
//		std::cout << "j0 = " << j0 << std::endl;
        indices.push_back(j0);
        seen[j0] = true;
    }

	while (true) {
        size_t next = j1 + 2 * j0;
        if (next >= maxSize)
            break;
//		std::cout << "next = " << next << std::endl;
        indices.push_back(next);
        j0 = j1;
        j1 = next;
    }

    // Fill in remaining missing indices in order
    
    for (size_t idx : indices)
        seen[idx] = true;
    for (size_t i = 0; i < maxSize; ++i)
        if (!seen[i]) {
//			std::cout << "i = " << i << std::endl;
            indices.push_back(i);
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
 * @return unsigned int 	//std::cout << "Binary insert key: " << key 
     //     << " [left=" << left << ", right=" << right << "]\n";
	//if (right >=  static_cast<int>(cont.size())) {
		//int test =  
	//	right = std::min(right, static_cast<int>(cont.size()) - 1);
		//right = (right * cont.size() / 25);
	//}
	//left = std::max(0, left);
	//std::cout << "Binary insert key after min max: " << key 
    //      << " [left=" << left << ", right=" << right << "]\n";
 */

int PmergeMe::insertPointVec(std::vector<std::pair<unsigned int, unsigned int>>& cont , unsigned int key, int left, int right) {
	//(void) left;
	if (cont.empty()) return 0;
	/*int adjustedLeft  = right / 2;
	if (adjustedLeft < 0)
	{
		adjustedLeft = 0;
	}
	std::cout<<"left = "<<adjustedLeft<<" and righyt = "<<right<<"\n";
	while (adjustedLeft <= right) {
		unsigned int mid = adjustedLeft + ((right - adjustedLeft) >> 1);
		//std::cout<<"mid = " << mid <<"leftr "<< left << " right = " << right << " cont[mid].first = " << cont[mid].first << std::endl;
		m_comp_vec++;
		if (cont[mid].first < key ) {
			adjustedLeft = mid + 1;
		}
        else {
            right = mid - 1;
		}
	}
    return adjustedLeft;*/
	
	while (left <= right) {
		unsigned int mid = left + ((right - left) >> 1);
		//std::cout<<"mid = " << mid <<"leftr "<< left << " right = " << right << " cont[mid].first = " << cont[mid].first << std::endl;
		m_comp_vec++;
		if (cont[mid].first < key ) {
			left = mid + 1;
		}
        else {
            right = mid - 1;
		}
	}
    return left;
}
/*int findAnchorIndex(const std::vector<std::pair<unsigned int, unsigned int>>& cont, unsigned int originalIndex) {
    int low = 0, high = cont.size() - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (cont[mid].second == originalIndex) return mid;
        else if (cont[mid].second < originalIndex) low = mid + 1;
        else high = mid - 1;
    }
    return 0; // fallback
}*/

void PmergeMe::sortLarge(std::vector<std::pair<unsigned int, unsigned int>>& large) {
    for (size_t i = 1; i < large.size(); ++i) {
		std::pair<unsigned int, unsigned int> key_pair = large[i];
        size_t j = i;

        while (j > 0 && large[j - 1].first > key_pair.first) {
	        m_comp_vec ++;
			//std::cout<<" sortlarge m_comp_vec = " << m_comp_vec << std::endl;
			large[j] = large[j - 1];		
            --j;
        }
        large[j] = key_pair;
    }
}

std::vector<std::pair<unsigned int, unsigned int>>  PmergeMe::splitResultsVec(std::vector<std::pair<unsigned int, unsigned int>>& Originallarge, int depth) {
	std::cout<<"Original large size = " << Originallarge.size() << std::endl;
	//printContainerPair(Originallarge, "large during recursive large sort: ");
		if (Originallarge.size() <= 1) {
		/*std::cout << "Returning from base case: ";
		for (const auto& p : Originallarge) {
	   		 std::cout << "(" << p.first << ", " << p.second << ") @ " << &p << "\n";
		}*/
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
				std::cout<<Originallarge[i].first<<" lost to "<<Originallarge[i + 1].first<<"\n";
				smallIndex.push_back(std::make_pair(Originallarge[i].first, Originallarge[i + 1].second));

			} else {
				small.push_back(Originallarge[i + 1]);
    			large.push_back(Originallarge[i]);
				std::cout<<Originallarge[i + 1].first<<" lost to "<<Originallarge[i].first<<"\n";

				smallIndex.push_back(std::make_pair(Originallarge[i + 1].first, Originallarge[i].second));
			}

		}
		if (Originallarge.size() % 2 != 0) {
			small.push_back(std::make_pair((Originallarge.back().first), Originallarge.back().second));
			smallIndex.push_back(std::make_pair((Originallarge.back().first), UINT_MAX));
			std::cout<<Originallarge.back().first<<"got leftout \n";

		}
		std::vector<std::pair<unsigned int, unsigned int>> sortedLarge = splitResultsVec(large, depth + 1);

		std::cout << "After recursion sortedLarge: ";
		std::cout<<"depth = " << depth << " sortedLarge size = " << sortedLarge.size() << std::endl;
		printContainerPair(small, "smalls being handled  are == ");
		/*for (const auto& p : sortedLarge) {
		    std::cout << "(" << p.first << ", " << p.second << ") @ " << &p << "\n";
		}*/
		
		printContainerPair(sortedLarge, "Sorted output: ");
		//5printContainerPair(Originallarge, "original large after winner/looser split: ");
		/*for (size_t i = 0; i < sortedLarge.size(); ++i) {
			for (size_t x = 0; x < Originallarge.size(); ++x) {
				if (sortedLarge[i].second == sortedLarge[x].second && i != x) {
					std::cout<<"error in sortedLarge second index = " << sortedLarge[i].second << " at i = " << i << " and x = " << x << std::endl;
					std::cout<<"sortedLarge[i].first = " << sortedLarge[i].first << " sortedLarge[x].first = " << sortedLarge[x].first << std::endl;
				}
			}			
		}*/
		//printContainerPair(sortedLarge, "Large after split second time: ");
		//printContainerPair(small, "Small before adding to sorted: ");
		std::vector<size_t> insertionOrder = generateJacobsthalIndices(small.size());
		//unsigned int prevPos = 0;
		//printContainer(insertionOrder, "Insertion Order: ");
		//size_t leftBound = 0;
    	std::vector<bool> inserted(small.size(), false);
		unsigned int pos = 0;
    	for (size_t i : insertionOrder) {
    	    if (i >= small.size() || inserted[i]) {
				std::cout << "Skipping index " << i << " (out of bounds or already inserted)\n";
				continue;
			}
			std::cout<<"debugging 666. what is size of sorted \n";
    	    unsigned int key = smallIndex[i].first;
    	    size_t anchor = smallIndex[i].second;
			
			size_t insertPos = sortedLarge.size();
			//size_t test = smallIndex[i].second;
			std::cout<<"does the index = "<<smallIndex[i].first<<" match table above? and whats the key "<<key<<" \n";
			//bool anchorFound = false;
    	    // 👀 Find anchor position
			
			/*for (size_t i = 0; i < smallIndex.size(); ++i) {
			    int anchor = smallIndex[i].second;
			    for (size_t j = 0; j < sortedLarge.size(); ++j) {
			        if (sortedLarge[j].second == anchor) {
			            // You've found where the winner ended up
			            // Place the loser appropriately based on j
			            break;
			        }
			    }
			}*/

			
    	     for (size_t j = 0; j < sortedLarge.size(); ++j) {
				//std::cout<<"here is the small = "<<small[i].second<<" looking at ijndexes of sortedlarge "<<sortedLarge[j].second<<"\n";
				if (sortedLarge[j].second == anchor) {
					std::cout<<"NOTICE BE THIS SHOULD HAPPEN ATLEAST A FEW TIMES WHATWHTANHWYAHTWHATH\n";
					insertPos = j; //test
					break;
    	        }
    	    }

			std::cout<<"anchor = " << anchor << std::endl;
			//std::cout<<"smallIndex + " <<test<<"\n";
			std::cout<<"inserPos" <<insertPos<<"\n";
			//std::cout<<"debugging 666. what is size of sorted "<<sortedLarge.size()<<"\n";
			//std::cout<<"debugging 666. what is size of sorted "<<large.size()<<"\n";

		// this removes 1 comparison only
			/*if (sortedLarge.size() < 2) {
				std::cout<<"--------debugging 666. sortedLarge size is less than 2 INSERT\n";
			    sortedLarge.insert(sortedLarge.begin(),  std::make_pair(key, small[i].second));
				inserted[i] = true;
				//break;
				continue;			
			}*/
	
			size_t leftBound = (insertPos >= 4) ? insertPos / 2: 0;
			std::cout<<"debugging 777 what is insertPos "<<insertPos<<" and left bound "<<leftBound<<"\n";
    	  	pos = insertPointVec(sortedLarge, key, 0, (insertPos == 0 ? 0 : insertPos - 1));
			std::cout<<"pos = "<<pos<<"\n";
			//leftBound = pos;
			//std::cout<<"prevPos = " << prevPos << " Pos = " << pos << std::endl;
    	    //unsigned int pos = findAnchorIndex(sortedLarge, anchor);
			//prevPos = pos;
			//std::cout<<"debugging 888 checking pos "<<pos<<" and sortedLarge.size = "<< sortedLarge.size() <<"\n";

			//if (pos < sortedLarge.size() - 5) {
			//	std::cout<<"-----------debugging 999. INSERTING \n";
			sortedLarge.insert(sortedLarge.begin() + pos, std::make_pair(key, small[i].second));
			//}
			/*for (size_t i = 0; i < sortedLarge.size(); ++i) {
				for (size_t x = 0; x < sortedLarge.size(); ++x) {
					if (sortedLarge[i].second == sortedLarge[x].second && i != x) {
						std::cout<<"error in sortedLarge second index = " << sortedLarge[i].second << " at i = " << i << " and x = " << x << std::endl;
						std::cout<<"sortedLarge[i].first = " << sortedLarge[i].first << " sortedLarge[x].first = " << sortedLarge[x].first << std::endl;
					}
				}			
			}*/
			inserted[i] = true;
			//std::cout<<"debugging 999\n";

		}
		
    	// 📝 Final output
		//printContainerPair(sortedLarge, "Sorted Large: ");
		//checkOrder(sortedLarge);
		/*for (size_t i = 0; i < inserted.size(); ++i) {
    		if (!inserted[i]) std::cout << "⚠️ Missing insertion at index: " << i << "\n";
		}*/

    	Originallarge.resize(sortedLarge.size());
    	for (size_t i = 0; i < sortedLarge.size(); ++i) {
			std::pair<unsigned int, unsigned int> key_pair = sortedLarge[i];
    	    Originallarge[i] = key_pair;
    	}
		std::cout << "✅ Final sortedLarge size: " << sortedLarge.size() << "\n";
		std::cout << "✅ Final originakl size: " << Originallarge.size() << "\n";
		
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
#include <limits.h>
void PmergeMe::insertionSortVec() {
	//int comp = 0;
	std::vector<std::pair<unsigned int, unsigned int>> small;
	std::vector<std::pair<unsigned int, unsigned int>> large;
	//std::vector<std::pair<size_t, size_t>> defeated;
	//std::cout<<"m_vec size = " << m_vec.size() << std::endl;
	// Step 1: Pair and split
	for (size_t i = 0; i + 1 < m_vec.size(); i += 2) {
		m_comp_vec++;
		//size_t defeated_idx = defeated.size();
		//std::cout<<" a < b m_comp_vec = " << m_comp_vec << std::endl;
		if (m_vec[i] < m_vec[i + 1] ) {
	        small.push_back(std::make_pair((m_vec[i]), i + 1));
			//std::cout << "Inserted into large: (" << m_vec[i + 1] << ", " << i + 1 << ")\n";
	        large.push_back(std::make_pair((m_vec[i + 1]), i + 1));
			//defeated.push_back(std::make_pair(m_vec[i + 1], m_vec[i]));
		} else {
	        small.push_back(std::make_pair((m_vec[i + 1]), i));
			//std::cout << "Inserted into large: (" << m_vec[i] << ", " << i << ")\n";
	        large.push_back(std::make_pair((m_vec[i]), i));
			//defeated.push_back(std::make_pair(m_vec[i], m_vec[i + 1]));

		}
	}
	printContainerPair(large, "Large before sorted: ");
	if (m_vec.size() % 2 != 0) {
	    small.push_back(small.back());
	}
	std::vector<std::pair<unsigned int, unsigned int>> new_large = splitResultsVec(large, 0);
	printContainerPair(new_large, "New Large after split: ");
	checkOrder(new_large);
	
	//sortLarge(large);
	//checkOrder(large);
	
	//printContainerPair(new_large, "Large sorted: ");
	//std::cout<<"check size of new_large = " << new_large.size() << std::endl;
	//std::cout<<"check large.second = " << large[3].second << std::endl;
	insertionSortList(small, large);
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

