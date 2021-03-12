#pragma once
#include <string>

namespace {
    //We assume that Dalloc set unused memory to 1
    //Varning, denna typ funkar inte som classer brukar göra!
    //Dhelper d; kommer att ge runtime error!
    struct Dhelper {
        static std::string usedConstr;
        static bool checkDhelper;
        char flag;
        char value;
        int Test() { return 1; }
        const int Test() const { return 2; }
        void IsConstr(bool checkDhelper) {
            assert(!checkDhelper || 1 < flag && flag < 7);
        }
        void IsNotConstr(bool checkDhelper) 
        {
            assert(!checkDhelper || 1 == flag);
        }

        operator char() const { return value; }

        ~Dhelper() {
            IsConstr(true);
            flag = 1;
        }
        Dhelper(bool checkDhelper = true) {
            value = 0;
            usedConstr += "DC";
            IsNotConstr(checkDhelper);
            flag = 2;
        }
        Dhelper(int v, bool checkDhelper = true) {
            value = v;
            usedConstr += "CC";
            IsNotConstr(checkDhelper);
            flag = 3;
        }
        Dhelper(const Dhelper& other, bool checkDhelper = true) {
            value = other.value;
            usedConstr += "CC";
            IsNotConstr(checkDhelper);
            flag = 4;
        }
        Dhelper(Dhelper&& other, bool checkDhelper = true) noexcept {
            value = other.value;
            usedConstr += "MC";
            IsNotConstr(checkDhelper);
            flag = 5; 
        }
        //Not used
        Dhelper& Assign(const Dhelper& other, bool checkDhelper = true) {
            value = other.value;
            usedConstr += "CA";
            IsConstr(checkDhelper);
            flag = 6;
            return *this;
        }
        //Not used
        Dhelper& Assign(char v, bool checkDhelper = true) {
            value = v;
            usedConstr += "CA";
            IsConstr(checkDhelper);
            flag = 6;
            return *this;
        }

        Dhelper& operator=(char v) {
            value = v;
            usedConstr += "CA";
            IsConstr(true);
            flag = 6;
            return *this;
        }
        //Used
        Dhelper& operator=(const Dhelper& other) {
            value = other.value;
            usedConstr += "CA";
            IsConstr(true);
            flag = 6;
            return *this;
        }
        //Not used
        Dhelper& operator=(Dhelper&& other) noexcept {
            value = other.value;
            usedConstr += "MA";
            IsConstr(true);
            flag = 7;
            return *this;
        }
        //Not used
        friend bool operator==(const Dhelper& lhs, const Dhelper& rhs) {
            return lhs.value == rhs.value;
        }
        friend bool operator==(const Dhelper& lhs, char rhs) {
            return lhs.value == rhs;
        }
        friend bool operator!=(const Dhelper& lhs, const Dhelper& rhs) {
            auto x= lhs.value != rhs.value;
            return x;
        }

    };

    std::string Dhelper::usedConstr{};
    bool Dhelper::checkDhelper{};

}

