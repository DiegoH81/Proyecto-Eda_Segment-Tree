#include "porter_algorithm.h"

// Constructor
porter_algorithm::porter_algorithm()
{
	init_rules_1_a();
	init_rules_2();
	init_rules_3();
	init_rules_4();
}

// Methods

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
	std::cout << "m: " << get_measure_limits(in_word, in_word.size()) << "\n";


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

bool porter_algorithm::ends_with(std::string& word, const std::string& suffix)
{
	size_t word_len = word.size();
	size_t suffix_len = suffix.size();
	
	if (word_len < suffix_len)
		return false;
	
	size_t start = word_len - suffix_len;
	for (size_t i = 0; i < suffix_len; i++)
	{
		if (word[start + i] != suffix[i])
			return false;
	}
	return true;
}

bool porter_algorithm::prefix_has_vowel(std::string& word, size_t prefix_len)
{
	for (size_t i = 0; i < prefix_len; i++)
		if (is_vowel(word, i))
			return true;
	return false;
}

size_t porter_algorithm::get_measure_limits(std::string& word, size_t length)
{
	size_t measure = 0;

	for (size_t i = 1; i < length; i++)
	{
		if (is_vowel(word, i - 1) && !is_vowel(word, i))
			measure++;
	}

	return measure;
}

void porter_algorithm::step_1_a(std::string& in_word)
{
	for (auto& rule : rules_1_a)
	{
		if (ends_with(in_word, rule.s1))
		{
			in_word.resize(in_word.size() - rule.s1.size());
			in_word += rule.s2;
			return;
		}
	}
}

void porter_algorithm::step_1_b(std::string& in_word)
{
	// A
	if (ends_with(in_word, "eed"))
	{
		if (get_measure_limits(in_word, in_word.size() - 3))
		{
			in_word.pop_back();
			return;
		}
	}

	//B
	bool modified = false;
	if (ends_with(in_word, "ed") && _v_(in_word, in_word.size() - 2))
	{
		in_word.resize(in_word.size() - 2);
		modified = true;
	}
	// C
	else if (ends_with(in_word, "ing") && _v_(in_word, in_word.size() - 3))
	{
		in_word.resize(in_word.size() - 3);
		modified = true;
	}

	// Rules OPT
	if (modified)
	{

		if (ends_with(in_word, "at"))
			in_word += "e";
		else if (ends_with(in_word, "bl"))
			in_word += "e";
		else if (ends_with(in_word, "iz"))
			in_word += "e";
		else if (_d(in_word) && in_word.back() != 'l'
							 && in_word.back() != 's'
							 && in_word.back() != 'z')
			in_word.pop_back();
		else if (get_measure_limits(in_word, in_word.size()) && _o(in_word, in_word.size()))
			in_word += 'e';
	}

}

void porter_algorithm::step_1_c(std::string& in_word)
{
	if (ends_with(in_word, "y"))
	{
		if (_v_(in_word, in_word.length() - 1))
			in_word.back() = 'i';
	}

}

void porter_algorithm::step_2(std::string& in_word)
{
	for (auto& rule : rules_2)
	{
		if (ends_with(in_word, rule.s1))
		{
			size_t prefix_len = in_word.length() - rule.s1.length();
			if (get_measure_limits(in_word, prefix_len) > 0)
			{
				in_word.resize(prefix_len);
				in_word += rule.s2;
				return;
			}
		}

	}
}

void porter_algorithm::step_3(std::string& in_word)
{
	for (auto& rule : rules_3)
	{
		if (ends_with(in_word, rule.s1))
		{
			size_t prefix_len = in_word.length() - rule.s1.length();

			if (get_measure_limits(in_word, prefix_len) > 0)
			{
				in_word.resize(prefix_len);
				in_word += rule.s2;
				return;
			}
		}
	}
}

void porter_algorithm::step_4(std::string& in_word)
{
	for (auto& rule : rules_4)
	{
		if (ends_with(in_word, rule.s1))
		{
			
			size_t prefix_len = in_word.length() - rule.s1.length();

			if (get_measure_limits(in_word, prefix_len) > 1)
			{
				bool meets_condition = false;
				if (rule.s1 == "ion")
				{
					if(prefix_len > 0 && (in_word[prefix_len - 1] == 's' ||
						in_word[prefix_len - 1] == 't'))
						meets_condition = true;
				}
				else
					meets_condition = true;

				if (meets_condition)
				{
					in_word.resize(prefix_len);
					in_word += rule.s2;
					return;
				}
			}

		}
	}
}

void porter_algorithm::step_5_a(std::string& in_word)
{
	size_t prefix_len = in_word.length() - 1;

	if (ends_with(in_word, "e"))
	{
		size_t measure = get_measure_limits(in_word, prefix_len);

		if ((measure > 1) || (measure == 1 && !_o(in_word, prefix_len)))
			in_word.pop_back();
	}
}

void porter_algorithm::step_5_b(std::string& in_word)
{
	if ((get_measure_limits(in_word, in_word.size()) > 1) && _d(in_word) && in_word.back() == 'l')
		in_word.pop_back();
}

// Conditionals
bool porter_algorithm::_s(std::string& in_word) // Ends with s
{
	if (in_word.size() >= 1)
		return in_word.back() == 's';

	return false;
}

bool porter_algorithm::_v_(std::string& in_word, size_t length) // Has a vowel
{
	if (length >= 1)
	{
		for (size_t i = 0; i < length; i++)
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

bool porter_algorithm::_o(std::string& in_word, size_t length) // Ends with cvc, where the second c is not W, X or Y
{
	if (length >= 3)
	{
		size_t a = length - 1;
		size_t b = length - 2;
		size_t c = length - 3;

		if (!is_vowel(in_word, a) && is_vowel(in_word, b) && !is_vowel(in_word, c))
		{
			if (in_word[a] != 'w' && in_word[a] != 'x' && in_word[a] != 'y')
				return true;
		}
	}
	return false;
}

// Init
void porter_algorithm::init_rules_1_a()
{
	rules_1_a.reserve(4);

	rules_1_a.push_back(rule_porter("sses", "ss"));
	rules_1_a.push_back(rule_porter("ies", "i"));
	rules_1_a.push_back(rule_porter("ss", "ss"));
	rules_1_a.push_back(rule_porter("s", ""));

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
	rules_4.push_back(rule_porter("iti", ""));
	rules_4.push_back(rule_porter("ous", ""));
	rules_4.push_back(rule_porter("ive", ""));
	rules_4.push_back(rule_porter("ize", ""));
}