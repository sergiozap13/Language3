/*
 * Metodología de la Programación: Language3
 * Curso 2022/2023
 */

/** 
 * @file BigramFreq.cpp
 * @author estudiante1: Zapata De la Hoz, Sergio
 * Created on 12 February 2023, 10:40
 */

#include <string>

#include "BigramFreq.h"

using namespace std;

BigramFreq::BigramFreq():_bigram("__"), _frequency(0) {
}

const Bigram& BigramFreq::getBigram() const {
    return _bigram;
}
int BigramFreq::getFrequency() const {
    return _frequency;
}

void BigramFreq::setBigram(const Bigram& bigram) {
    this->_bigram = bigram;
}

void BigramFreq::setFrequency(int frequency) {
    if(frequency<0){
        throw  std::out_of_range(string("void BigramFreq::setFrequency(int frequency): ") +
                "invalid frecuency " + to_string(frequency));
    }
    this->_frequency = frequency;
}

string BigramFreq::toString() const {
    return _bigram.toString() + " " + to_string(_frequency);
}


