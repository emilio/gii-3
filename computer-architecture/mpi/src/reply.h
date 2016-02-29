#ifndef REPLY_H
#define REPLY_H

#include <cstring>

enum class ReplyType {
    FAILURE,
    SUCCESS
};

class Reply {
    ReplyType m_type;
protected:
    explicit constexpr Reply(ReplyType type): m_type(type) {}
public:
    ReplyType type() { return m_type; }
};

class FailureReply: public Reply {
public:
    explicit constexpr FailureReply(): Reply(ReplyType::FAILURE) {}
};

class SuccessReply: public Reply {
    // TODO: Don't use a fixed size, probably requires some `Channel` support.
    char m_decrypted[100];
public:
    explicit SuccessReply(const char* decrypted): Reply(ReplyType::SUCCESS) {
        memcpy(m_decrypted, decrypted, std::min(strlen(decrypted), sizeof(decrypted)));
    }
};

#endif
