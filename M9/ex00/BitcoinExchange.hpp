/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:05:48 by shaboom           #+#    #+#             */
/*   Updated: 2025/07/18 12:04:50 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream> //main
#include <string> //main
#include <deque>
#include <map>
#include <list>
/**
 * @brief use of multimap for exchange rates is effecient for look up and allows for duplicate keys.
 * The multimap could be adjusted to have date as key, and a pair for holding time and value, for this project
 * however it is out of scope. 
 * 
 * use of deque pair for converstion rates allows us to loop thorugh in order of input from given file,
 * this allows for inputting line number where error occured in an easy manner. It was assumed that 
 * it would be easier for user to check output against a text.txt file . 
 * 
 */
class BitcoinExchange
{
	private:
		unsigned int m_listLength = 0;
		std::multimap<std::string, double> m_exchangeRates;
		std::deque<std::pair<std::string, double>> m_evaluationSheet;
		void readToContainer(const std::string& filename, char delim);
		void printCalculation(std::string date, double frate, double ammount);
		bool validateFormats(std::string date, double rate, double ammount, int line_num);
	public:
		BitcoinExchange ();
		BitcoinExchange (const std::string& rates, const std::string& conversions);
		BitcoinExchange (BitcoinExchange &other) = delete;
		BitcoinExchange& operator=(BitcoinExchange &other) = delete;
		~BitcoinExchange ();
		void findMatchingKeys();
};