#ifndef RTI_INTERFACES_H
#define RTI_INTERFACES_H

// implies implementation of sink() should assign automatic msgId
// (treat all data passed as a single message)
#define AUTO_MSG_ID -1

namespace rti {

    class IDataSink {
    public:
        // contract:
        // - msgId < 0 => automatic msgId; this implies receiver can
        //   assign message ids itself, if necessary (can be
        //   omitted if not required)
        // - ownership of data is not given away with this call
        // - once method returns, it can be called again immediately
        // - presence of theLast implies that receiver is able to
        //   to buffer data if that is required (in some scenarios
        //   both parties can assume it never happens)
        // - nothing known about threads, syncing happens as required
        virtual void sink(char* data, int len, int msgId,
            bool isLast) = 0;
    };
    
};

#endif
