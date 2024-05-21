#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

constexpr int PAYLOAD_MAX_SIZE = 1408 - 4;

constexpr int PICTURE_CHUNK_SIZE = 256;

// 2 bytes FrameType, 2 bytes payload length
namespace Protocol {
enum FrameType : uint16_t {
    minBound,

    cmdAction,
    ansAction,

    getSetPositionLift,
    setSetPositionLift,
    getSetPositionRotation,
    setSetPositionRotation,
    getSetPositionPress,
    setSetPositionPress,
    getSetSpeedLift,
    setSetSpeedLift,
    getSetSpeedRotation,
    setSetSpeedRotation,
    getSetSpeedPress,
    setSetSpeedPress,
    getSetAccelerationLift,
    setSetAccelerationLift,
    getSetAccelerationRotation,
    setSetAccelerationRotation,
    getSetAccelerationPress,
    setSetAccelerationPress,
    getSetWordControl,
    setSetWordControl,
    getGetSpeedLift,
    getGetSpeedRotation,
    getGetSpeedPress,
    getGetSettedMoment,
    getGetCurrentMoment,
    getGetPositionLift,
    getGetPositionRotation,
    getGetPositionPress,
    getGetWordStatus,
    setGetWordStatus,
    getMotorsInfo,
    getAllPositionsAndStatus,

    getLatticeState,
    getFilter,

    flashRemoteLeft,  // payload - RemoteFlashPacket,
    flashRemoteRight, // reply payload - RemoteFlashAckReply

    maxBound
};

enum Actions : uint8_t {
    ButLightPressed,
    ButLightReleased,
    ButLeftPressed,
    ButLeftReleased,
    ButRightPressed,
    ButRightReleased,
    ButUpPressed,
    ButUpReleased,
    ButDownPressed,
    ButDownReleased,
    ButRotateLeft45Clicked,
    ButRotateTo0Clicked,
    ButRotateRight45Clicked,
    ButMassUpPressed,
    ButMassUpReleased,
    ButMassDownPressed,
    ButMassDownReleased,
    ButAngleUpPressed,
    ButAngleUpReleased,
    ButAngleDownPressed,
    ButAngleDownReleased,
    PedalUpPressed,
    PedalUpReleased,
    PedalDownPressed,
    PedalDownReleased,
    EncoderPlus,
    EncoderMinus,
    LatticeStart,
    LatticeStop,
    StartAxisCalibration,
};

enum Filters : uint8_t {
    FILTER_NONE,
    FILTER_AL,
    FILTER_CU
};

enum RemotePictures : uint8_t {
    MainScreen,
    //----- splash screen ---------
    logo0,
    logo1,
    logo2,
    logo3,
    logo4,
    logo5,
    logo6,
    logo7,
    logo8,
    logo9,
    logo10,
    //----- digits  ---------------
    grey0,
    grey1,
    grey2,
    grey3,
    grey4,
    grey5,
    grey6,
    grey7,
    grey8,
    grey9,
    black0,
    black1,
    black2,
    black3,
    black4,
    black5,
    black6,
    black7,
    black8,
    black9,
    green0,
    green1,
    green2,
    green3,
    green4,
    green5,
    green6,
    green7,
    green8,
    green9,
    red0,
    red1,
    red2,
    red3,
    red4,
    red5,
    red6,
    red7,
    red8,
    red9,
    //----- symbols  --------------
    ang_selected,
    ang_unselected,
    kg_selected,
    kg_unselected,
    empty_indicators,
    emergency,
    error,
    light_on,
    light_off
};

#pragma pack(push, 1)
//-------- frame header -------------------------------------------------------
struct Head {
    FrameType frameType;
    uint16_t len;
};
//-------- payloads -----------------------------------------------------------
struct Action {
    Actions action;
};

struct Motors2RegVal {
    float value;
};

struct MotorsRegVal {
    int16_t value;
};

struct MotorsAllRegs {
    int16_t SetPositionLift;
    int16_t SetPositionRotation;
    int16_t SetPositionPress;
    int16_t SetSpeedLift;
    int16_t SetSpeedRotation;
    int16_t SetSpeedPress;
    int16_t SetAccelerationLift;
    int16_t SetAccelerationRotation;
    int16_t SetAccelerationPress;

    int16_t GetSpeedLift;
    int16_t GetSpeedRotation;
    int16_t GetSpeedPress;
    int16_t GetSettedMoment;
    int16_t GetCurrentMoment;
    int16_t GetPositionLift;
    int16_t GetPositionRotation;
    int16_t GetPositionPress;
    int16_t GetWordStatus;
    // uint16_t SetWordControl;
};

struct MotorsAllPositionsAndStatus {
    int16_t GetCurrentMoment;
    int16_t GetPositionLift;
    int16_t GetPositionRotation;
    int16_t GetPositionPress;
    int16_t GetWordStatus;
};

struct FilterState {
    Filters currentFilter;
};
struct BoolState {
    bool state;
};
struct RemoteFlashPacket {
    RemotePictures picNum;
    uint16_t size;
    uint16_t num;
    uint8_t arr[PICTURE_CHUNK_SIZE];
};
struct RemoteFlashAckReply {
    RemotePictures picNum;
    uint16_t size;
    uint16_t num;
};
#pragma pack(pop) 

} // namespace Protocol

#endif // PROTOCOL_H
