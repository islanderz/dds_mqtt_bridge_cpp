#ifndef RTI_INTERFACES_H
#define RTI_INTERFACES_H

// implies implementation of sink() should assign automatic msgId
// (treat all data passed as a single message)
#define AUTO_MSG_ID -1

namespace rti {

    class IDataSink {
    public:
        // msgId < 0 => automatic msgId
        virtual void sink(char* data, int len, int msgId) = 0;
    };
    
};

#endif
