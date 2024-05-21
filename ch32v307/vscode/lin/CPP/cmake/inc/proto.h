#ifndef PROTO_H
#define PROTO_H

#include "protocol.h"
#include <array>
#include <string.h>
// #include "short_alloc.h"
// #include <vector>

namespace Proto {

// template <typename T = uint8_t>
// using vByte = std::vector<T, short_alloc<uint8_t, BUFSZ, alignof(T)>>;

// class VectorByte {
//   public:
//     VectorByte(Protocol::FrameType frame, uint8_t* payload, size_t size);
//     void prepareBuf(Protocol::FrameType frame, uint8_t* payload, size_t
//     size); uint8_t* data(); size_t size();
//
//   private:
//     vByte<uint8_t>::allocator_type::arena_type a;
//     vByte<uint8_t> vbyte{a};
//
//     VectorByte(const VectorByte&) = delete;
//     VectorByte(VectorByte&&) = delete;
//     VectorByte& operator=(const VectorByte&) = delete;
//     VectorByte& operator=(VectorByte&&) = delete;
// };

class ProtoBuf {
  public:
    ProtoBuf(Protocol::FrameType frame, const uint8_t* payload, size_t size);
    const uint8_t* data() const;
    size_t size() const;

  private:
    void prepareBuf(Protocol::FrameType frame, const uint8_t* payload,
                    size_t size);
    size_t sz = 0;
    static constexpr int BUFSZ = 64;
    std::array<uint8_t, BUFSZ> arr{0};
    ProtoBuf(const ProtoBuf&) = delete;
    ProtoBuf(ProtoBuf&&) = delete;
    ProtoBuf& operator=(const ProtoBuf&) = delete;
    ProtoBuf& operator=(ProtoBuf&&) = delete;
};

} // namespace Proto

#endif // PROTO_H
