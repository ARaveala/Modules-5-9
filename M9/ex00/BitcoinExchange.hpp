/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:05:48 by shaboom           #+#    #+#             */
/*   Updated: 2025/07/17 16:28:24 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream> //main
#include <string> //main
#include <deque>
#include <map>
#include <list>
/**
 * @brief use ofunordered_multimap : faster look up, order does not matter in the in
 * the context of this project , since all key values(dates) will be unique, 
 * i do not have to make my own collision handeling for the hash table, unordered unordered_multimap
 * has its own that should suffice here.
 * 
 * @example of a custom hasher function to prevent hash collisions
 * struct CustomHash {
    size_t operator()(const std::string& key) const {
        size_t hash = 0;
        for (char ch : key) {
            hash = hash * 31 + ch;
        }
        return hash;
    
 */
class BitcoinExchange
{
	private:
		unsigned int m_listLength = 0;
		std::multimap<std::string, double> m_exchangeRates;
		std::deque<std::pair<std::string, double>> m_evaluationSheet;
		void readTounordered_multimap(const std::string& filename, char delim);
		//create a file
		void findMatchingKeys();
		void printCalculation(std::string date, double frate, double ammount);
		void adjustLength(char op);
		bool validateFormats(std::string date, double rate, double ammount, int line_num);
	public:
		BitcoinExchange ();
		BitcoinExchange (const std::string& rates, const std::string& conversions);
		BitcoinExchange (BitcoinExchange &other) = delete;
		BitcoinExchange& operator=(BitcoinExchange &other) = delete;
		~BitcoinExchange ();
		unsigned int getevalLength() const;
		void showValues();

		
		
};