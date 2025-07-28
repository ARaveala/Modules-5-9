/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:24:27 by shaboom           #+#    #+#             */
/*   Updated: 2025/07/28 18:25:59 by araveala         ###   ########.fr       */
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
		void fillContainers(const std::string& set);
		int insertPointVec(std::vector<unsigned int>& cont, unsigned int key, int left, int right);
		void insertionSortVec();
		void insertionSortList();

	public:
		PmergeMe ();
		PmergeMe (const std::string& set);
		PmergeMe (PmergeMe &other) = delete;
		PmergeMe& operator=(PmergeMe &other) = delete;
		~PmergeMe ();
		void test();
		//double calculation();
};