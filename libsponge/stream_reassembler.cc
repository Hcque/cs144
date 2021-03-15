#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;
#include <list>

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof)
        this->eof_idx = index + data.size() ;
    // insert each new data 
    bool insert = true;
    int idx = static_cast<int> (index);
    list<int> toerase = {};
    for (auto i: this->wait){
        // 'bc' 'bcd'
        if (i.first == idx && i.second.size() >= data.size()) 
            insert = false;
        // 'bcd' 'c'
        if (i.first < idx && i.first + i.second.size() >= idx + data.size())
            insert = false;
        // 'bcd' 'c'
        if (i.first > idx && i.first + i.second.size() <= idx + data.size()){
            toerase.push_back(i.first);
        }
    }
    for (auto i : toerase)
        this->wait.erase(i);
    if (insert)
        this->wait[index] = data;

    string writebuf = "";
    while (this->wait.size()) {
        int min_idx = (*(this->wait.begin())).first;
        string min_data = (*(this->wait.begin())).second;
        if (min_idx > this->need) break;
        addtobuf(writebuf, min_data, min_idx);
    }
    this->_output.write(writebuf);
}

size_t StreamReassembler::unassembled_bytes() const {
    size_t ans = 0;
    for (auto idx: this->wait){
        ans += idx.second.size();
    }
    return ans;
}

bool StreamReassembler::empty() const { return wait.size() == 0 ; }

void StreamReassembler::addtobuf(string& writebuf, string& data, int index){
    int len = data.size();
    string elem = "";
    // overlap
    if (index < this->need){
        if (index + len <= this->need ){
             // discard overlap string
        }
        else 
            elem = data.substr(this->need - index);
    }
    if (index == this->need){ // write in immedately
        elem = data;
    }
    this->need += elem.size();
    if (this->need == this->eof_idx ) 
        this->_output.end_input();
    this->wait.erase(index);
    writebuf += elem;

}