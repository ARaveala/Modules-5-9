/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shaboom <shaboom@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:58:06 by shaboom           #+#    #+#             */
/*   Updated: 2025/01/29 12:07:46 by shaboom          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#include <fstream> //file
#include <sstream> 
#include <cmath> //nan


/**
 * @brief constructor sets containers into existenece 
 */
BitcoinExchange::BitcoinExchange () : m_exchangeRates(), m_converstionSheet(){
}

/**
 * @brief non default constructor uses default constructor (one must exits as per subject rules) and fils up multimap (exchange rates)
 * and deque (conversions required), using individual delemeter.
 */
BitcoinExchange::BitcoinExchange(const std::string& rates, const std::string& conversions) : BitcoinExchange()  {
	readToContainer(rates, ',');
	readToContainer(conversions, '|');
}

/**
 * @brief deconstructor , clears our containers
 */
BitcoinExchange::~BitcoinExchange () {
	m_exchangeRates.clear();
	m_converstionSheet.clear();
}

/**
 * @brief year month and date are checked first using substr only if we have enough indexes to fullfill each substr.
 * 
 * Inorder to get mmore direct error messages an elseif forest is used, however it is the simplest way to hanle this
 * style of error handling . 
 */
bool BitcoinExchange::validateFormats(std::string date, double rate, double ammount, int line_num)
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
		std::cout<<"ERROR: bad date format Line number ["<<line_num<<"] ::"<<date<<std::endl;
		return false;
	} 
	std::string year = date.substr(0,4);
	std::string month = date.substr(5,2);
	std::string day = date.substr(8,2);
	if (year.length() != 4 || month.length() != 2 || day.length() != 2) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid date format ::"<<date<<std::endl;
	} else if (date.find_first_not_of("0123456789-") != std::string::npos){
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid date format ::"<<date<<std::endl;
	} else if (std::stoi(year) < 2009 || (std::stoi(year) == 2009 && std::stoi(month) == 1 && std::stoi(day) == 1)) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] no bit exchange history ::"<<date<<std::endl;
	} else if (std::stoi(year) > 2025) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] no bit exchange history ::"<<year<<std::endl;
	} else if (std::stoi(month) < 1 || std::stoi(month) > 12) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid month ::"<<month<<std::endl;
	} else if (std::stoi(day) < 1 || std::stoi(day) > 31) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid day ::"<<day<<std::endl;
	} else if (month == "02" && std::stoi(day) > 29) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid day "<<day<<" for February "<<std::endl;
	} else if (month == "02" && std::stoi(day) == 29 && std::stoi(year) % 4 != 0) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid day "<<day<<" for February "<<std::endl;
	} else if ((month == "04" || month == "06" || month == "09" || month == "11") && std::stoi(day) > 30) {
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid day "<<day<<" for month "<<month<<std::endl;
	} else if (std::isnan(ammount)){
		std::cout<<"ERROR: Line number ["<<line_num<<"] invalid value ::"<<ammount<<std::endl;
	} else if (rate < 0) {
		std::cout<<"ERROR: Line number ["<<line_num<<"]negative rate ::"<<rate<<std::endl;
	} else if (ammount < 0) {
		std::cout<<"ERROR: Line number ["<<line_num<<"]negative ammount ::"<<ammount<<std::endl;
	} else if (ammount > 1000.0) {
		std::cout<<"ERROR: Line number ["<<line_num<<"]ammount too large ::"<<std::to_string(ammount)<<std::endl;
	} else if (rate > 1000.0) {
		std::cout<<"ERROR: Line number ["<<line_num<<"]rate too large ::"<<std::to_string(rate)<<std::endl;
	} else if (ammount == 0) {
		std::cout<<date<<" => "<<rate<<" = "<<ammount <<std::endl; //check this
	}
	else {
		return true;
	}
	return false;
}

/**
 * @brief takes provided file and fills in continaers based on delimeter
 */
void BitcoinExchange::readToContainer(const std::string& filename, char delim)
{
	bool header = true;
	std::string line, date;

	std::ifstream file(filename);
	if (!file)
		throw std::runtime_error(filename + "ERROR:: file could not be found or opened ");
	if (file.peek() == std::ifstream::traits_type::eof()) {
    	throw std::runtime_error(filename + "ERROR:: file is empty");
	}
	while (std::getline(file, line)) {
		double value;
		if (header) {
			// skip the first line
			header = false;
			continue ;
		}
		std::stringstream seperator(line);
		std::getline(seperator, date, delim);
        date.erase(0, date.find_first_not_of(" \t\n\r\f\v"));
        date.erase(date.find_last_not_of(" \t\n\r\f\v") + 1);
		seperator >> value;
		if (seperator.fail()) {
			value = std::nan("");
		}
		if (delim == '|') {
			m_converstionSheet.push_back({date, value});
		} else {
			m_exchangeRates.insert({date, value});
		}
		value = std::nan("");
	}
}

/**
 * @brief this function loops through the provided converstion sheet that has been give as input, and tries to find a matching date from the 
 * exchange rates provided by subject (data.csv). Validation handles format checks and continues to next value in list if format is incorrect.
 * 
 * if validation succeeds , We are assumed to have a valid date, we check to see if that date exists, else we find the previouse date as per subject request.
 * 
 * The validation happens per line as the subject seems to suggest we should output each error but that should not discontinue the ability to keep
 * looking through the file and finding matches
 */
void BitcoinExchange::findMatchingKeys() {

	int i = 0;
	std::deque<std::pair<std::string, double>>::iterator it = m_converstionSheet.begin();
	while (it != m_converstionSheet.end())
	{
		std::multimap<std::string, double>::const_iterator rateIt = m_exchangeRates.lower_bound(it->first);
		if (rateIt == m_exchangeRates.end())  {
			validateFormats(it->first, it->second, it->second, i + 2);
			it++;
			i++;
			continue;

		}
		if (!validateFormats(it->first, rateIt->second, it->second, i + 2)) {
			it++;
			i++;
			continue;
		}
		if (rateIt->first != it->first) {
			if (rateIt != m_exchangeRates.begin()){
				--rateIt; // move to the previous element if it exists
				if (rateIt!= m_exchangeRates.begin()) {
					std::cout<<"No rates for "<<it->first<<" using previous rate ";
				}
			} else {
				std::cout << "No matching date found for " << it->first << std::endl; // bad format
				it++;
				continue; // no matching date found, skip to next evaluation (fail safe)
			}
		}
		printCalculation(rateIt->first, rateIt->second, it->second);
		it++;
		i++;
	}
}
/**
 * @brief checks if value is 0, otherwise we do conversion calculation and display it .
 */
void BitcoinExchange::printCalculation(std::string date, double rate, double ammount)
{
	double result = 0;
	if (rate == 0) {
		std::cout<<0<<std::endl;
	}
	else {
		result = ammount * rate;
		std::cout<<date<<" => "<<rate<<" = "<<result <<std::endl;
	}
}
