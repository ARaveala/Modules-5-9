/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:24:27 by shaboom           #+#    #+#             */
/*   Updated: 2025/08/11 16:18:36 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <list>
/**
 * @brief do we leave room for 2 fucntions, one to check string clean, then stoi
 * 
 */

class PmergeMe
{
	private:
		std::list<unsigned int> m_lst;
		std::vector<unsigned int> m_vec;
		size_t m_comp_vec = 0;
		size_t m_comp_lst = 0;
		void fillContainers(const std::string& set);
		int insertPointVec(std::vector<std::pair<unsigned int, unsigned int>>& cont, unsigned int key, int left, int right);
		void beginMergeInsertionSortVec();
		void beginMergeInsertionSortlst();
		size_t findAnchorIndexVec(const std::vector<std::pair<unsigned int, unsigned int>>& vec, size_t anchor);
		void insertionSortVec(std::vector<std::pair<unsigned int, unsigned int>>& small, std::vector<std::pair<unsigned int, unsigned int>>& large,  const std::vector<std::pair<unsigned int, unsigned int>>& orgIndex);
		std::vector<std::pair<unsigned int, unsigned int>> splitResultsVec(std::vector<std::pair<unsigned int, unsigned int>>& Originallarge);
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>> splitResultsLst(std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
    int depth);
		void insertionSortLst(
    		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& small,
    		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
			bool recursion
			);
			
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator findInsertPositionLst(
    		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& large,
    		std::list<unsigned int>::iterator anchor
			);
		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator insertPointLst(
    		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>& cont,
    		unsigned int key,
    		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator left,
    		std::list<std::pair<unsigned int, std::list<unsigned int>::iterator>>::iterator right,
			bool recursion
			);
		std::vector<size_t> generateJacobsthalIndices(size_t maxSize);

	public:
		PmergeMe ();
		PmergeMe (const std::string& set);
		PmergeMe (PmergeMe &other) = delete;
		PmergeMe& operator=(PmergeMe &other) = delete;
		~PmergeMe ();			
		void test();
};