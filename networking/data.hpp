

#pragma once

#include <deque>
#include <iostream>

namespace networking
{
/**
 * @brief special tokens for specifying different types of data for the network 
 * stream. Uses:
 *  - Headers
 *  - Footers
 */
enum token : uint32_t
{
    invalid = 0x00000000,       // default, invalid token

    begin_stream = 0x11000000,  // begin a stream, expect end_stream 8 times to 
                                // end the stream
    end_stream = 0xff00ff00,    // end a stream if repeated 8 times
    
    begin_chunk2 = 0x00000002,  // begin a chunk, expect a 2 byte integer to 
                                // specify the size of the chunk
    begin_chunk4 = 0x00000004,  // begin a chunk, expect a 4 byte integer to 
                                // specify the size of the chunk
    begin_chunk8 = 0x00000008,  // begin a chunk, expect a 8 byte integer to 
                                // specify the size of the chunk
};

/**
 * @brief A data stream that can be used to serialize then send and receive 
 * data.
 * 
 * @details this implementation uses a deque to store the data. This is a first-
 * in, last-out data structure. Buffers can be chained together.
 */
class buffer
{
public:
    buffer() : len_(), header_(), footer_() {};

    /**
     * @brief Push data into the buffer.
     * 
     * @param data any type of data that is stored contiguously as bytes to be 
     * pushed into the buffer.
     * @warning if the data contains pointers, they may not be valid for the 
     * reciever.
     * 
     * @tparam T the type of data to be pushed into the buffer.
     * 
     * @return a reference to *this so that the function can be chained.
     */
    template <typename T>
    buffer &operator>>(const T &data)
    {
        len_ += sizeof(T);
        const char *ptr = reinterpret_cast<const char *>(&data);
        for (std::size_t i = 0; i < sizeof(T); ++i)
            data_.push_back(ptr[i]);
        return *this;
    }

    /**
     * @brief Pop data into the buffer.
     * 
     * @param data any type of data that is to be retrieved from the buffer.
     * @warning if the data contains pointers, they may not be valid for the
     * sender.
     * @warning the data type should match the type of data that was pushed 
     * into the buffer. Otherwise, the data may be corrupted.
     * 
     * @tparam T the type of data to be retrieved from the buffer.
     * 
     * @return a reference to *this so that the function can be chained.
     */
    template <typename T>
    buffer &operator<<(T &data)
    {
        len_ -= sizeof(T);
        char *ptr = reinterpret_cast<char *>(&data);
        for (std::size_t i = 0; i < sizeof(T); ++i)
        {
            ptr[i] = data_.front();
            data_.pop_front();
        }
        return *this;
    }

    /**
     * @brief get the length of the data in the buffer.
     * @note this length is not the size of the buffer since it does not include
     *  the size of the header and footer, which are four bytes each.
     * 
     * @return the length of the data in the buffer.
     */
    inline std::size_t size() const
    {
        return len_;
    }

    friend std::ostream &operator<<(std::ostream &os, const buffer &buf)
    {
        os << std::hex;
        for (const uint8_t &c : buf.data_)
            os << (const uint_fast16_t)c << " ";
        os << std::dec;
        return os;
    }
private:
    token header_, footer_;
    std::deque<uint8_t> data_;
    std::size_t len_;
};

}