/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:55:15 by shaboom           #+#    #+#             */
/*   Updated: 2025/08/11 15:49:40 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout<<"too many arguments \n";
		exit(EXIT_FAILURE);
	}
	try
	{
		RPN test(argv[1]);
		double res = test.calculation();
		std::cout<<res<<std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}