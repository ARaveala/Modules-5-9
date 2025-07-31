/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:24:27 by shaboom           #+#    #+#             */
/*   Updated: 2025/07/31 10:39:47 by araveala         ###   ########.fr       */
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
		void fillContainers(const std::string& set);
		int insertPointVec(std::vector<std::pair<unsigned int, unsigned int>>& cont, unsigned int key, int left, int right);
		void insertionSortVec();
		void insertionSortList();

		void sortLarge(std::vector<std::pair<unsigned int, unsigned int>>& large);
		std::vector<std::pair<unsigned int, unsigned int>> splitResultsVec(std::vector<std::pair<unsigned int, unsigned int>>& Originallarge, int depth);
		// testing
		void insertionSortList(std::vector<std::pair<unsigned int, unsigned int>>& small, std::vector<std::pair<unsigned int, unsigned int>>& large);
		std::vector<size_t> generateJacobsthalIndices(size_t maxSize);
	public:
		PmergeMe ();
		PmergeMe (const std::string& set);
		PmergeMe (PmergeMe &other) = delete;
		PmergeMe& operator=(PmergeMe &other) = delete;
		~PmergeMe ();			
		void test();
		//double calculation();
};