#ifndef PORTER_ALGORITHM_H
#define PORTER_ALGORITHM_H

#include <string>
#include <vector>
#include <iostream>

#include "rule_porter.h"

class porter_algorithm
{
public:
    // Constructor
    porter_algorithm(); 

    void porter_stem(std::string& in_word);

    void debug(std::string in_word);

private:

    std::vector<rule_porter> rules_1_a,
                             rules_2,
                             rules_3,
                             rules_4;

    // Methods
    void step_1_a(std::string& in_word);
    void step_1_b(std::string& in_word);
    void step_1_c(std::string& in_word);
    void step_2(std::string& in_word);
    void step_3(std::string& in_word);
    void step_4(std::string& in_word);
    void step_5_a(std::string& in_word);
    void step_5_b(std::string& in_word);


    bool is_vowel(std::string& str, size_t index);
    bool prefix_has_vowel(std::string& word, size_t prefix_len);

    size_t get_measure_limits(std::string& word, size_t length);
    bool ends_with(std::string& word, const std::string& suffix);

    
    // Conditionals
    bool _s(std::string& in_word); // Ends with s
    bool _v_(std::string& in_word, size_t length); // Has a vowel
    bool _d(std::string& in_word); // Ends with double consonant
    bool _o(std::string& in_word, size_t length); // Ends with cvc, where the second c is not W, X or Y

    // Init
    void init_rules_1_a();
    void init_rules_2();
    void init_rules_3();
    void init_rules_4();
};

#endif