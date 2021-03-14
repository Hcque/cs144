#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

#include <iostream>

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity){
}

size_t ByteStream::write(const string &data) {
    size_t len = data.size();
    if (len > this->remaining_capacity() )
        len = this->remaining_capacity();
    if (this->remaining_capacity() == 0)
        return 0;

    for (size_t i = 0; i < len; i++)
        this->buf.push_front(data[i]);
    this->bytesW += len;
    // DUMMY_CODE(data);
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string ans = "";
    for (auto it = this->buf.end()-1; it != this->buf.end()-1 - len; it--)
        ans += *(it);
    DUMMY_CODE(len);
    return ans;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    for (size_t i = 0; i < len; i++)
       this->buf.pop_back();
    DUMMY_CODE(len);
    this->bytesR += len;
    return;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    int n = len;
    if (len > this->buf.size())
        n = this->buf.size();
    string ans = "";
    ans = this->peek_output(n);
    this->pop_output(n);
    return ans;
}

void ByteStream::end_input() { this->inputended = true; }

bool ByteStream::input_ended() const { return this->inputended ; }

size_t ByteStream::buffer_size() const { return this->buf.size(); }

bool ByteStream::buffer_empty() const { return this->buf.size() == 0; }

bool ByteStream::eof() const { return this->buf.size() == 0 && this->inputended; }

size_t ByteStream::bytes_written() const { return this->bytesW; }

size_t ByteStream::bytes_read() const { return this->bytesR; }

size_t ByteStream::remaining_capacity() const { return this->_capacity - this->buf.size(); }
