/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:23:43 by araveala          #+#    #+#             */
/*   Updated: 2025/07/01 21:24:27 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MutantStack.hpp"
#include <iostream>
#include <list>

void iteratorValues(MutantStack<int>& stack)
{
	stack.push(5);
	stack.push(17);
	std::cout << stack.top() << std::endl;
	stack.pop();
	std::cout << stack.size() << std::endl;
	stack.push(3);
	stack.push(5);
	stack.push(737);
	//[...]
	stack.push(0);
	
}

int main()
{

	MutantStack<int> mstack;
	
	iteratorValues(mstack);
	

	MutantStack<int>::iterator it = mstack.begin();
	MutantStack<int>::iterator ite = mstack.end();
	++it;
	--it;
	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}
	std::cout<<"\n";
	std::stack<int> (lstack);
	{
		std::list<int> lstack;
		lstack.push_back(5);
		lstack.push_back(17);
		std::cout << lstack.back() << std::endl;
		lstack.pop_back();
		std::cout << lstack.size() << std::endl;
		lstack.push_back(3);
		lstack.push_back(5);
		lstack.push_back(737);
		//[...]
		lstack.push_back(0);
		std::list<int>::iterator it = lstack.begin();
		std::list<int>::iterator ite = lstack.end();
		++it;
		--it;
		while (it != ite)
		{
			std::cout << *it << std::endl;
			++it;
		}
		
		
	}

	std::cout<<"reverse test \n";

	MutantStack<int> reverse_mstack;
	
	iteratorValues(reverse_mstack);
	

	MutantStack<int>::reverse_iterator reverse_it = reverse_mstack.rbegin();
	MutantStack<int>::reverse_iterator reverse_ite = reverse_mstack.rend();
	++reverse_it;
	--reverse_it;
	while (reverse_it != reverse_ite)
	{
		std::cout << *reverse_it << std::endl;
		++reverse_it;
	}

	std::cout<<"const tests ---- \n";
	const MutantStack<int> const_mstack = mstack;

	std::cout << const_mstack.top() << std::endl;
	std::cout << const_mstack.size() << std::endl;
	MutantStack<int>::const_iterator const_it = const_mstack.begin();
	MutantStack<int>::const_iterator const_ite = const_mstack.end();
	while (const_it != const_ite)
	{
		std::cout << *const_it << std::endl;
		++const_it;
	}

	std::cout<<"const reverse tests ---- \n";
	const MutantStack<int> const_reverse_mstack = mstack;

	std::cout << const_reverse_mstack.top() << std::endl;
	std::cout << const_reverse_mstack.size() << std::endl;
	MutantStack<int>::const_reverse_iterator const_reverse_it = const_reverse_mstack.rbegin();
	MutantStack<int>::const_reverse_iterator const_reverse_ite = const_reverse_mstack.rend();
	while (const_reverse_it != const_reverse_ite)
	{
		std::cout << *const_reverse_it << std::endl;
		++const_reverse_it;
	}

	return 0;

}
