#include "proto.h"

namespace Proto {

using namespace Protocol;

ProtoBuf::ProtoBuf(Protocol::FrameType frame, const uint8_t* payload,
                   size_t size) {
    prepareBuf(frame, payload, size);
}

void ProtoBuf::prepareBuf(Protocol::FrameType frame, const uint8_t* payload,
                          size_t size) {
    Head head;
    head.frameType = frame;
    head.len = size;
    sz = sizeof(Head) + size;
    memcpy(arr.data(), &head, sizeof(Head));
    if (payload) {
        memcpy(arr.data() + sizeof(Head), payload, size);
    }
}

const uint8_t* ProtoBuf::data() const { return arr.data(); }
size_t ProtoBuf::size() const { return sz; }

} // namespace Proto
