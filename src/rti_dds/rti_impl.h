#ifndef RTI_IMPL_H
#define RTI_IMPL_H

#include "rti_interfaces.h"
#include <string>
#include <exception>
#include <sstream>

#include "rti_dds.h"
#include "rti_ddsSupport.h"
#include "ndds/ndds_cpp.h"

#define SSTR( x ) static_cast< ostringstream & >( \
        ( ostringstream() << dec << x ) ).str()

using namespace std;

namespace rti {

    // TODO: abstract away from Buffer1024 with help of templates
    // TODO: SSTR could be made better by including file + line number
    //       for errors

    class RtiException : public exception {
    private:
        string cause;
    public:
        RtiException(string cause);
        virtual ~RtiException() _GLIBCXX_USE_NOEXCEPT;
        virtual char const * what() const throw(); 
    };

    class AbstractDds {
    protected:
        DDSDomainParticipant *participant;
        DDSTopic *topic;
    protected:
        AbstractDds();
        virtual ~AbstractDds();
        void shutdown();
        void crashdown(string msg);
    public:
        virtual void close();
    };



    class DdsDataSender : public AbstractDds, public IDataSink {
    private:
        DDSPublisher *publisher;
        DDSDataWriter *writer;
        Buffer1024DataWriter * Buffer1024_writer;
        Buffer1024 *instance;
        int curr_msgId;
    public:
        DdsDataSender(int domainId, string topic);
        virtual ~DdsDataSender();

        // should always use AUTO_MSG_ID for msgId
        // isLast should always be true
        virtual void sink(char* data, int len, int msgId,
            bool isLast);
    };

    class Buffer1024Listener : public DDSDataReaderListener {
    private:
        IDataSink* sink;
    public:
        Buffer1024Listener(IDataSink* sink);
        virtual void on_requested_deadline_missed(
            DDSDataReader* /*reader*/,
            const DDS_RequestedDeadlineMissedStatus& /*status*/) {}

        virtual void on_requested_incompatible_qos(
            DDSDataReader* /*reader*/,
            const DDS_RequestedIncompatibleQosStatus& /*status*/) {}

        virtual void on_sample_rejected(
            DDSDataReader* /*reader*/,
            const DDS_SampleRejectedStatus& /*status*/) {}

        virtual void on_liveliness_changed(
            DDSDataReader* /*reader*/,
            const DDS_LivelinessChangedStatus& /*status*/) {}

        virtual void on_sample_lost(
            DDSDataReader* /*reader*/,
            const DDS_SampleLostStatus& /*status*/) {}

        virtual void on_subscription_matched(
            DDSDataReader* /*reader*/,
            const DDS_SubscriptionMatchedStatus& /*status*/) {}

        virtual void on_data_available(DDSDataReader* reader);
    };


    class DdsConnection : public AbstractDds, IDataSink {
    private:
        IDataSink* receiver;
        DDSSubscriber *subscriber;
        Buffer1024Listener *reader_listener; 
        DDSDataReader *reader;

        // idea is that connection attempts to buffer incoming
        // data until full message is ready (when data with
        // isLast flag comes in, it is considered as ready);
        // however, it is possible that unready messages are sent
        // to receiver, if buffer is full
        char* buff;
        int buff_len;
        int buff_curr_len;
        int curr_msg_id;

        void dump_buffer();
        void add_to_buffer(char* data, int len, int msgId,
            bool isLast);
        void send_curr_buffer();
        void send_curr_incomplete_buffer();

    public:
        DdsConnection(IDataSink* receiver, int domainId,
                      string topic_name, int buff_size);
        virtual ~DdsConnection();
        int loop(int wait_time);

        virtual void sink(char* data, int len, int msgId,
            bool isLast);
    };
    
};

#endif
