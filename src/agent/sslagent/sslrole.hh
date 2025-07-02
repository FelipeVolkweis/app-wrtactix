#ifndef SSLROLE_HH
#define SSLROLE_HH

enum class SSLRoleType { UNKNOWN, GOALKEEPER, STRIKER };

class SSLRole {
public:
    SSLRole(SSLRoleType type) : type_(type) {};
    SSLRole() : type_(SSLRoleType::UNKNOWN) {};

    SSLRoleType getType() const {
        return type_;
    }

    void setType(SSLRoleType type) {
        type_ = type;
    }

private:
    SSLRoleType type_;
};

#endif