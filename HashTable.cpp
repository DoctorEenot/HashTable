

#include <iostream>
#include <ctime>
#include "HashTable.h"

using namespace std;

unsigned char RandBuf[256];


void randomise_RandBuf() {//must be called first
    srand(time(0));
    for (int i = 0; i < 256; i++) {
        RandBuf[i] = rand();
    }
}

unsigned long long Hash(unsigned char* data,size_t size) {
    
    unsigned long long to_return = 0;
    unsigned char h1, h2;
    size_t counter=0;

    h1 = *data; 
    h2 = *data + 1;
    counter++;
    while (counter<size) { 
        h1 = RandBuf[h1 ^ data[counter]];
        h2 = RandBuf[h2 ^ data[counter]];
        to_return += ((unsigned long long)h1 << 54) | (unsigned long long)h2;
        counter++;
    }
    return to_return;
}



Cell::Cell(CellVariable* key, CellVariable* value) {
    this->key.size = (*key).size;
    this->key.value = new unsigned char[this->key.size];
    memcpy(this->key.value, (*key).value, this->key.size);

    this->value.size = (*value).size;
    this->value.value = new unsigned char[this->value.size];
    memcpy(this->value.value, (*value).value, this->value.size);
    inited = true;
}

Cell::Cell() {
    this->key.size = 0;
    this->value.size = 0;
    inited = false;
}

CellVariable Cell::get_key() {
    return this->key;
}
unsigned char* Cell::get_value() {
    return this->value.value;
}
size_t Cell::get_size() {
    return this->value.size;
}
bool Cell::is_empty() {
    return inited;
}

void Cell::set_value(CellVariable* value) {
    if (inited) {
        if ((*value).size > this->value.size) {
            delete[] this->value.value;
            this->value.value = new unsigned char[(*value).size];
        }
    }
    else {
        this->value.value = new unsigned char[(*value).size];
    }
    memcpy(this->value.value, (*value).value,(*value).size);
    this->value.size = (*value).size;
    inited = true;
}

void Cell::set_key(CellVariable* key) {
    if (inited) {
        return;
    }
    this->key.value = new unsigned char[(*key).size];
    memcpy(this->key.value, (*key).value, (*key).size);
    this->key.size = (*key).size;
}

void Cell::clear() {
    delete[] this->key.value;
    this->key.size = 0;
    delete[] this->value.value;
    this->value.size = 0;
    inited = false;
}

bool Cell::compare_keys(CellVariable* key) {
    if (this->key.size != (*key).size) {
        return false;
    }
    for (size_t i = 0; i < this->key.size; i++) {
        if (this->key.value[i] != (*key).value[i]) {
            return false;
        }
    }
    return true;
}

void Cell::uninit() {
    inited = false;
}

Cell::~Cell() {}



Bucket::Bucket() {
    number_of_cells = 0;
    actual_size = PLUS_KOEF;
    cells = new Cell[actual_size];
};

size_t Bucket::get_cell_index(CellVariable* key) {
    size_t ret = 0;
    for (ret; ret < number_of_cells; ret++) {
        if (cells[ret].compare_keys(key)) {
            return ret;
        }
    }
    throw 25;
}

void Bucket::expand_cells() {
    size_t new_size = actual_size + PLUS_KOEF;
    Cell* buf = new Cell[new_size];
    if (number_of_cells != 0) {
        memcpy(buf, cells, sizeof(Cell) * number_of_cells);
        delete[] cells;
    }
    cells = buf;
    actual_size = new_size;
};

void Bucket::put(CellVariable* key, CellVariable* value) {
    size_t cell_number;
    try {
        cell_number = get_cell_index(key);
        cells[cell_number].set_value(value);
    }
    catch (int e) {
        if (number_of_cells == actual_size) {
            expand_cells();
        }
        cells[number_of_cells].set_key(key);
        cells[number_of_cells].set_value(value);
        number_of_cells += 1;
    }
}

Cell* Bucket::get(CellVariable* key) {
    return &cells[get_cell_index(key)];
}

size_t Bucket::get_number_of_cells() {
    return number_of_cells;
}

void Bucket::remove(CellVariable* key) {
    size_t index = get_cell_index(key);
    cells[index].clear();
    memcpy(&cells[index],&cells[index+1],sizeof(Cell)*(actual_size-1-index));
    number_of_cells -= 1;
    cells[number_of_cells].uninit();
            
}

bool Bucket::empty() {
    if (number_of_cells == 0) {
        return true;
    }
    return false;
}
        
void Bucket::clear() {
    size_t index;
    for (number_of_cells; number_of_cells > 0; number_of_cells -= 1) {
        index = number_of_cells - 1;
        cells[index].clear();
        cells[index].uninit();
    }
    delete[] cells;
    actual_size = 0;
}

Bucket::~Bucket() {}



Dictionary::Dictionary(){
    number_of_buckets = 0;
}

void Dictionary::init(size_t number_of_buckets) {
    if (inited()) {
        throw 25;
    }
    buckets = new Bucket[number_of_buckets];
    this->number_of_buckets = number_of_buckets;
}

bool Dictionary::inited() {
    if (this->number_of_buckets != 0) {
        return true;
    }
    return false;
}

size_t Dictionary::get_bucket_index(CellVariable* key) {
    return Hash((*key).value, (*key).size) % number_of_buckets;
}

void Dictionary::put(CellVariable* key, CellVariable* value) {
    size_t bucket_index = get_bucket_index(key);
    buckets[bucket_index].put(key,value);
            
}
        
Cell* Dictionary::get(CellVariable* key) {
    size_t bucket_index = get_bucket_index(key);
    return buckets[bucket_index].get(key);
}











