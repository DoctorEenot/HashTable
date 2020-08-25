#pragma once

#define PLUS_KOEF 100;

void randomise_RandBuf();
unsigned long long Hash(unsigned char* data, size_t size);

struct CellVariable
{
    unsigned char* value;
    size_t size;
};


class Cell {

private:
    CellVariable key;
    CellVariable value;
    bool inited;
public:
    Cell(CellVariable* key, CellVariable* value);
    Cell();
    CellVariable get_key();
    unsigned char* get_value();    
    size_t get_size();   
    bool is_empty();
    void set_value(CellVariable* value);
    void set_key(CellVariable* key);
    void clear();
    bool compare_keys(CellVariable* key);
    void uninit();
    ~Cell();
};



class Bucket {
private:
    size_t number_of_cells;
    size_t actual_size;
    Cell* cells;

public:
    Bucket();
    size_t get_cell_index(CellVariable* key);
    void expand_cells();
    void put(CellVariable* key, CellVariable* value);
    Cell* get(CellVariable* key);
    size_t get_number_of_cells();
    void remove(CellVariable* key);
    bool empty();
    void clear();
    ~Bucket();
};

class Dictionary {
private:
    size_t number_of_buckets;

    Bucket* buckets;

public:
    Dictionary();
    void init(size_t number_of_buckets);
    bool inited();

    size_t get_bucket_index(CellVariable* key);

    void put(CellVariable* key, CellVariable* value);

    Cell* get(CellVariable* key);

};