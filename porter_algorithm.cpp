#include "porter_algorithm.h"

// Constructor
porter_algorithm::porter_algorithm()
{
	init_rules_1_a();
	init_rules_1_b();
	init_rules_1_b_opt();
	init_rules_2();
	init_rules_3();
	init_rules_4();
}



// Funciones

void porter_algorithm::porter_stem(std::string& in_word)
{
	step_1_a(in_word);
	step_1_b(in_word);
	step_1_c(in_word);
	step_2(in_word);
	step_3(in_word);
	step_4(in_word);
	step_5_a(in_word);
	step_5_b(in_word);
}

void porter_algorithm::debug(std::string in_word)
{
	std::cout << "INCIO: " << in_word << "\n";
	std::cout << "m: " << get_measure(in_word) << "\n";


	step_1_a(in_word);
	std::cout << "step_1_a: " << in_word << "\n";
	step_1_b(in_word);
	std::cout << "step_1_b: " << in_word << "\n";
	step_1_c(in_word);
	std::cout << "step_1_c: " << in_word << "\n";
	step_2(in_word);
	std::cout << "step_2: " << in_word << "\n";
	step_3(in_word);
	std::cout << "step_3: " << in_word << "\n";
	step_4(in_word);
	std::cout << "step_4: " << in_word << "\n";
	step_5_a(in_word);
	std::cout << "step_5_a: " << in_word << "\n";
	step_5_b(in_word);
	std::cout << "step_5_b: " << in_word << "\n";


}

bool porter_algorithm::is_vowel(std::string& str, size_t index)
{
	if (index >= str.size())
		return false;

	char& c = str[index];
	if (c == 'y')
	{
		if (index == 0)
			return true;

		if (!is_vowel(str, index - 1))
			return true;

	}
	else if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
		return true;

	return false;
}

size_t porter_algorithm::get_measure(std::string& in_word)
{
	size_t measure = 0;
	
	for (size_t i = 1; i < in_word.size(); i++)
	{
		if (is_vowel(in_word, i - 1) && !is_vowel(in_word, i))
			measure++;
	}

	return measure;
}

std::string porter_algorithm::get_prefix(std::string& in_word, std::string& suffix)
{
	std::string prefix;
	
	bool valid = true;

	if (in_word.size() > suffix.size())
	{
		size_t pos = in_word.size() - suffix.size();

		for (size_t i = 0; i < suffix.size(); i++, pos++)
		{
			if (in_word[pos] != suffix[i])
			{
				valid = false;
				break;
			}
		}
	}
	else
		valid = false;

	if (valid)
		prefix = in_word.substr(0, in_word.size() - suffix.size());

	return prefix;
}

void porter_algorithm::step_1_a(std::string& in_word)
{
	for (auto& rule : rules_1_a)
	{
		std::string& suffix = rule.s1;
		std::string prefix = get_prefix(in_word, suffix);

		if (prefix.empty())
			continue;

		in_word = prefix + rule.s2;
		return;	
	}
}

void porter_algorithm::step_1_b(std::string& in_word)
{
	for (auto& rule : rules_1_b)
	{
		std::string& suffix = rule.s1;
		std::string prefix = get_prefix(in_word, suffix);


		if (prefix.empty())
			continue;


		if (suffix == "eed" && get_measure(prefix) > 0)
		{
			in_word = prefix + rule.s2;
			return;
		}
		else if (_v_(prefix) && (suffix == "ed" || suffix == "ing"))
		{
			in_word = prefix + rule.s2;


			// 1_B optional
			for (auto& rule_opt : rules_1_b_opt)
			{
				suffix = rule_opt.s1;
				prefix = get_prefix(in_word, suffix);

				if (prefix.empty())
					continue;

				in_word = prefix + rule_opt.s2;
				return;
			}

			if (_d(in_word) && in_word.back() != 'l'
							&& in_word.back() != 's'
							&& in_word.back() != 'z')
			{
				prefix = in_word.substr(0, in_word.size() - 2);
				in_word = prefix + in_word.back();
				return;
			}
			else if ((get_measure(in_word) == 1) && (_o(in_word)))
			{
				prefix = in_word.substr(0, in_word.size() - 3);
				in_word = prefix + "e";
				return;
			}
		}
	}
}

void porter_algorithm::step_1_c(std::string& in_word)
{
	
	std::string suffix = "y";
	std::string prefix = get_prefix(in_word, suffix);;
	

	if (!prefix.empty())
	{

		if (_v_(prefix))
			in_word = prefix + "i";
	}
}

void porter_algorithm::step_2(std::string& in_word)
{
	for (auto& rule : rules_2)
	{
		std::string& suffix = rule.s1;
		std::string prefix = get_prefix(in_word, suffix);

		if (prefix.empty())
			continue;

		if (get_measure(prefix) > 0)
		{
			in_word = prefix + rule.s2;
			return;
		}
	}
}

void porter_algorithm::step_3(std::string& in_word)
{
	for (auto& rule : rules_3)
	{
		std::string& suffix = rule.s1;
		std::string prefix = get_prefix(in_word, suffix);

		if (prefix.empty())
			continue;

		if (get_measure(prefix) > 0)
		{
			in_word = prefix + rule.s2;
			return;
		}
	}
}

void porter_algorithm::step_4(std::string& in_word)
{
	for (auto& rule : rules_4)
	{
		std::string& suffix = rule.s1;
		std::string prefix = get_prefix(in_word, suffix);

		if (prefix.empty())
			continue;

		if (get_measure(prefix) > 1)
		{
			bool meets_condition = false;
			if (suffix == "ion")
			{
				if (prefix.back() == 's' || prefix.back() == 't')
					meets_condition = true;
			}
			else
				meets_condition = true;

			if (meets_condition)
			{
				in_word = prefix + rule.s2;
				return;
			}
		}
	}
}

void porter_algorithm::step_5_a(std::string& in_word)
{
	std::string suffix = "e";
	std::string prefix = get_prefix(in_word, suffix);

	if (!prefix.empty())
	{
		size_t measure = get_measure(prefix);

		if ((measure > 1) || (measure == 1 && !_o(prefix)))
			in_word = prefix;
	}
}

void porter_algorithm::step_5_b(std::string& in_word)
{
	if ((get_measure(in_word) > 1) && _d(in_word) && in_word.back() == 'l')
		in_word = in_word.substr(0, in_word.size() - 1);
}

// Condiciones
bool porter_algorithm::_s(std::string& in_word) // Ends with s
{
	if (in_word.size() >= 1)
		return in_word.back() == 's';

	return false;
}

bool porter_algorithm::_v_(std::string& in_word) // Has a vowel
{
	if (in_word.size() >= 1)
	{
		for (size_t i = 0; i < in_word.size(); i++)
			if (is_vowel(in_word, i))
				return true;
	}

	return false;
}

bool porter_algorithm::_d(std::string& in_word) // Ends with double consonant
{
	if (in_word.size() >= 2)
	{
		size_t a = in_word.size() - 1;
		size_t b = in_word.size() - 2;

		if (!is_vowel(in_word, a) && !is_vowel(in_word, b) && (in_word[a] == in_word[b]))
			return true;
	}

	return false;
}

bool porter_algorithm::_o(std::string& in_word) // Ends with cvc, where the second c is not W, X or Y
{
	if (in_word.size() >= 3)
	{
		size_t a = in_word.size() - 1;
		size_t b = in_word.size() - 2;
		size_t c = in_word.size() - 3;

		if (!is_vowel(in_word, a) && is_vowel(in_word, b) && !is_vowel(in_word, c))
		{
			if (a != 'w' && a != 'x' && a != 'y')
				return true;
		}
	}
	return false;
}

// Inicializadores
void porter_algorithm::init_rules_1_a()
{
	rules_1_a.reserve(4);

	rules_1_a.push_back(rule_porter("sses", "ss"));
	rules_1_a.push_back(rule_porter("ies", "i"));
	rules_1_a.push_back(rule_porter("ss", "ss"));
	rules_1_a.push_back(rule_porter("s", ""));

}

void porter_algorithm::init_rules_1_b()
{
	rules_1_b.reserve(3);

	rules_1_b.push_back(rule_porter("eed", "ee"));
	rules_1_b.push_back(rule_porter("ed", ""));
	rules_1_b.push_back(rule_porter("ing", ""));
}

void porter_algorithm::init_rules_1_b_opt()
{
	rules_1_b_opt.reserve(3);

	rules_1_b_opt.push_back(rule_porter("at", "ate"));
	rules_1_b_opt.push_back(rule_porter("bl", "ble"));
	rules_1_b_opt.push_back(rule_porter("iz", "ize"));
}

void porter_algorithm::init_rules_2()
{
	rules_2.reserve(20);

	rules_2.push_back(rule_porter("ational", "ate"));
	rules_2.push_back(rule_porter("tional", "tion"));
	rules_2.push_back(rule_porter("enci", "ence"));
	rules_2.push_back(rule_porter("anci", "ance"));
	rules_2.push_back(rule_porter("izer", "ize"));
	rules_2.push_back(rule_porter("abli", "able"));
	rules_2.push_back(rule_porter("alli", "al"));
	rules_2.push_back(rule_porter("entli", "ent"));
	rules_2.push_back(rule_porter("eli", "e"));
	rules_2.push_back(rule_porter("ousli", "ous"));
	rules_2.push_back(rule_porter("ization", "ize"));
	rules_2.push_back(rule_porter("ation", "ate"));
	rules_2.push_back(rule_porter("ator", "ate"));
	rules_2.push_back(rule_porter("alism", "al"));
	rules_2.push_back(rule_porter("iveness", "ive"));
	rules_2.push_back(rule_porter("fulness", "ful"));
	rules_2.push_back(rule_porter("ousness", "ous"));
	rules_2.push_back(rule_porter("aliti", "al"));
	rules_2.push_back(rule_porter("iviti", "ive"));
	rules_2.push_back(rule_porter("biliti", "ble"));
}

void porter_algorithm::init_rules_3()
{
	rules_3.reserve(7);

	rules_3.push_back(rule_porter("icative", "ic"));
	rules_3.push_back(rule_porter("ative", ""));
	rules_3.push_back(rule_porter("alize", "al"));
	rules_3.push_back(rule_porter("iciti", "ic"));
	rules_3.push_back(rule_porter("ical", "ic"));
	rules_3.push_back(rule_porter("ful", ""));
	rules_3.push_back(rule_porter("ness", ""));
}

void porter_algorithm::init_rules_4()
{
	rules_4.reserve(19);

	rules_4.push_back(rule_porter("al", ""));
	rules_4.push_back(rule_porter("ance", ""));
	rules_4.push_back(rule_porter("ence", ""));
	rules_4.push_back(rule_porter("er", ""));
	rules_4.push_back(rule_porter("ic", ""));
	rules_4.push_back(rule_porter("able", ""));
	rules_4.push_back(rule_porter("ible", ""));
	rules_4.push_back(rule_porter("ant", ""));
	rules_4.push_back(rule_porter("ement", ""));
	rules_4.push_back(rule_porter("ment", ""));
	rules_4.push_back(rule_porter("ent", ""));
	rules_4.push_back(rule_porter("ion", ""));
	rules_4.push_back(rule_porter("ou", ""));
	rules_4.push_back(rule_porter("ism", ""));
	rules_4.push_back(rule_porter("ate", ""));
	rules_4.push_back(rule_porter("ati", ""));
	rules_4.push_back(rule_porter("ous", ""));
	rules_4.push_back(rule_porter("ive", ""));
	rules_4.push_back(rule_porter("ize", ""));
}