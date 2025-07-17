/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaboom <shaboom@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:28:24 by shaboom           #+#    #+#             */
/*   Updated: 2025/04/17 08:59:40 by shaboom          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/**
 * @brief we can take in args later with a quick function that scans for any other
 * character than digits, use pointer to state that only digits allowed of positive value 
 * show users incorrect output. this will be before class object is created
 * 
 * @return int 
 */
/*int main() {
    std::vector<int> vec = {38, 27, 43, 3, 9, 82, 10};
    std::deque<int> deq = {38, 27, 43, 3, 9, 82, 10};

    std::cout << "Sorting std::vector...\n";
    fordJohnsonSort(vec);
    std::cout << "Sorted std::vector:\n";
    printContainer(vec);

    std::cout << "Sorting std::deque...\n";
    fordJohnsonSort(deq);
    std::cout << "Sorted std::deque:\n";
    printContainer(deq);

    return 0;
}*/


int	main()
{
	//deal with user input
	std::string nums = "10 9 18 7 1 51 4 3 2 0";
	if (nums.find_first_not_of(" 0123456789") != std::string::npos)
	{
		std::cout<<"only positive integers allowed in set\n";
		exit(EXIT_FAILURE);
	}	
	//	throw std::runtime_error("only positive integers allowed in set\n");

	PmergeMe test(nums);
	test.test();
	return 0;
}