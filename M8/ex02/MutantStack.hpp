/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MutantStack.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:23:45 by araveala          #+#    #+#             */
/*   Updated: 2025/07/01 21:25:11 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stack>
#include <deque>
#include <iostream>

template<typename T>
class MutantStack : public std::stack<T>{

	public:
		MutantStack() {};
		~MutantStack() {};
		using iterator = typename std::stack<T>::container_type::iterator;
		iterator begin() { return this->c.begin();};
		iterator end() { return this->c.end(); };
		
		using const_iterator = typename std::stack<T>::container_type::const_iterator;
		const_iterator begin() const { return this->c.begin();};
		const_iterator end() const { return this->c.end(); };

		using reverse_iterator = typename std::stack<T>::container_type::reverse_iterator;
		reverse_iterator rbegin() { return this->c.rbegin(); }
		reverse_iterator rend() { return this->c.rend(); }

		using const_reverse_iterator = typename std::stack<T>::container_type::const_reverse_iterator;
		const_reverse_iterator rbegin() const { return this->c.rbegin(); }
		const_reverse_iterator rend() const { return this->c.rend(); }
};

