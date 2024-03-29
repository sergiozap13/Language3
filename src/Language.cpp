/*
 * Metodología de la Programación: Language3
 * Curso 2022/2023
 */

/** 
 * @file Language.cpp
 * @author estudiante1: Zapata De la Hoz, Sergio
 * Created on 12 February 2023, 10:40
 */

#include "Language.h"

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const string Language::MAGIC_STRING_T="MP-LANGUAGE-T-1.0";

Language::Language(){
    this->_languageId = "unknown";
    this->_size = 0;
}

Language::Language(int numberBigrams){
    if(numberBigrams > DIM_VECTOR_BIGRAM_FREQ || numberBigrams < 0){
        throw std::out_of_range("const char Bigram::at(int index) invalid position (" + std::to_string(numberBigrams) + ")"
                    + " and it can only be 0 or 1");
    }
    
    this->_size = numberBigrams;
    this->_languageId = "unknown";
    for(int i = 0; i < numberBigrams; i++){
        Bigram b;
        this->_vectorBigramFreq[i].setBigram(b);
        this->_vectorBigramFreq[i].setFrequency(0);
    }
}

const std::string& Language::getLanguageId() const{
    return this->_languageId;
}

void Language::setLanguageId(std::string id){
    this->_languageId = id;
}

const BigramFreq& Language::at(int index) const{
    if(index < 0 || index > DIM_VECTOR_BIGRAM_FREQ){
        throw std::out_of_range("const char Bigram::at(int index) invalid position (" + std::to_string(index) + ")"
                    + " and it can only be 0 or 1");
    }
    return this->_vectorBigramFreq[index];
}

BigramFreq& Language::at(int index){
    if(index < 0 || index > DIM_VECTOR_BIGRAM_FREQ){
        throw std::out_of_range("const char Bigram::at(int index) invalid position (" + std::to_string(index) + ")"
                    + " and it can only be 0 or 1");
    }
    return this->_vectorBigramFreq[index];
}

int Language::getSize() const{
    return this->_size;
}

double Language::getDistance(const Language &otherLanguage) const{
    double distance = 0.0; 
    // Para todos los bigramas de L1 (this), debo coger su posición en L1, ver si está en L2 y recoger su posicion (otherLanguage)
    int posL1 = 0, posL2 = 0 ,denominador = this->getSize() * this->getSize();
    double numerador = 0.0;
   
    for(int i = 0; i < this->getSize(); i++){
        posL1 = i;
        // Si está, su rank tiene el valor de la posicion en L2 en la que se encuentra
        posL2 = otherLanguage.findBigram(this->_vectorBigramFreq[i].getBigram());
        // Si no está toma el valor de size(L1) this.getSize();
        if(posL2 == -1){
            posL2 = this->getSize();
        }
        
        numerador += abs(posL1 - posL2);  
    }
    
    distance = numerador / denominador;
    
    return distance; // debe devolver un valor entre 0 y 1
}

int Language::findBigram(const Bigram &bigram) const{
    
    for(int i = 0; i < getSize(); i++){
        if(at(i).getBigram().getText() == bigram.getText()){
           return i; 
        }
    }
    return -1;
}

std::string Language::toString() const {
    string result;
    result += to_string(this->_size);
    result += "\n";
    result += this->_languageId;
    result += "\n";
    for(int i = 0; i < this->_size; i++){
        result += _vectorBigramFreq[i].getBigram().getText() + " " + to_string(_vectorBigramFreq[i].getFrequency());
        result += "\n";
    }
    return result;
}

void Language::sort(){

    int max_index = 0;
    for (int i = 0; i < getSize(); i++){
        max_index = i;
        for (int j = i +1; j < getSize(); j++){
            if(at(j).getFrequency() > at(max_index).getFrequency())
                max_index = j;
            else if(at(j).getFrequency() == at(max_index).getFrequency() &&
                        at(j).getBigram().getText() < at(max_index).getBigram().getText())
                max_index = j;
        }
        swapElements(i, max_index);
    }
}

void Language::save(const char fileName[]) const{
    ofstream fo;
    fo.open(fileName);
    
    string s(fileName);
    if(!fo){
        throw std::ios_base::failure("void Language::save(char fileName[]) const: the file (" + s + ")"
                    + " can't be opened or an error occurs while writing to the file"); 
    }
    fo << Language::MAGIC_STRING_T << endl;
    
    fo << this->toString();
    
    fo.close();
    
}

void Language::load(const char fileName[]){
    ifstream fi;
    fi.open(fileName);
    
    string s(fileName);
    if(!fi){
        throw std::ios_base::failure("void Language::load(char fileName[]): the file (" + s + ")"
                    + " can't be opened or an error occurs while reading to the file");
    }
    string magic_s = "";
    string language = "";
    int freq = 0;
    fi >> magic_s;
    
    if(magic_s != Language::MAGIC_STRING_T ){
        throw std::invalid_argument("void Language::load(char fileName[]): an invalid magic string is found in: " + s);
    }
    fi >> language;
    this->setLanguageId(language);
    fi >> freq;
    
    if(freq > Language::DIM_VECTOR_BIGRAM_FREQ){
        throw std::out_of_range("void Language::load(char fileName[]): the number of bigrams in the given file, cannot be allocated in this Language because it exceeds the maximum capacity");
    }
  
    this->_size = freq;
    string bigram_text_aux = "";
    int freq_aux = 0;
    
    for(int i = 0; i < freq; i++){
        fi >> bigram_text_aux;
        fi >> freq_aux;
        Bigram aux(bigram_text_aux);
        BigramFreq bf_aux;
        bf_aux.setBigram(aux);
        bf_aux.setFrequency(freq_aux);
        this->at(i) = bf_aux;
    }
    
    fi.close();
}

void Language::append(const BigramFreq &bigramFreq){
    int pos = findBigram(bigramFreq.getBigram());
    if(pos != -1)
        _vectorBigramFreq[pos].setFrequency(_vectorBigramFreq[pos].getFrequency() + bigramFreq.getFrequency());
    else {
        //Bigram b;
        //int primer_bigram_vacio = findBigram(b);
        
        if(this->_size == DIM_VECTOR_BIGRAM_FREQ )
            throw std::out_of_range("void Language::append( BigramFreq &bigramFreq): the array is full, and no more elements can be appended to the array");
        this->_size++;
        _vectorBigramFreq[this->_size - 1].setBigram(bigramFreq.getBigram());
        _vectorBigramFreq[this->_size - 1].setFrequency(bigramFreq.getFrequency());
    }
}

void Language::join(const Language &language){
    for (int i = 0; i < language.getSize(); i++){
        append(language.at(i));
    }
}

void Language::swapElements(int first, int second){
    
    if(first < getSize() && second < getSize() && first >= 0 && second >= 1 && first != second){
        BigramFreq temp = at(first);
        at(first) = at(second);
        at(second) = temp;
    }
}




//