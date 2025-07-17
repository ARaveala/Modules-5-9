/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:57:21 by shaboom           #+#    #+#             */
/*   Updated: 2025/07/17 18:26:32 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main (int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <conversions_file>" << std::endl;
		return 1;
	}
	
	std::string ratesFile = "cpp_09/data.csv"; // Default rates file
	std::string conversionsFile = argv[1];
	if (ratesFile.empty() || conversionsFile.empty()) {
		std::cerr << "Error: File names cannot be empty." << std::endl;
		return 1;
	}
	try {
		BitcoinExchange test(ratesFile, conversionsFile);
		test.showValues();
	} catch(const std::exception& e){
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}