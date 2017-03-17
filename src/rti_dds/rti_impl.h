#ifndef RTI_IMPL_H
#define RTI_IMPL_H

#include "rti_interfaces.h"
#include <string>
#include <exception>
#include <sstream>

#define SSTR( x ) static_cast< ostringstream & >( \
        ( ostringstream() << dec << x ) ).str()

using namespace std;

namespace rti {

    class RtiException : public exception {
    private:
        string cause;
    public:
        RtiException(string cause);
        virtual ~RtiException();
        virtual char const * what(); 
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



    class DdsDataSender : public AbstractDds, IDataSink {
    private:
        DDSPublisher *publisher;
        DDSDataWriter *writer;
        Buffer1024DataWriter * Buffer1024_writer;
        Buffer1024 *instance;
        int msgId;
    public:
        DataSender(int domainId, string topic);
        virtual ~DataSender();

        // should always use AUTO_MSG_ID for msgId
        virtual void sink(char* data, int len, int msgId);
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


    class DdsConnection : public AbstractDds {
    private:
        IDataSink* receiver;
        DDSSubscriber *subscriber;
        Buffer1024Listener *reader_listener; 
        DDSDataReader *reader;
        char* buff;
        int buff_len;
        int buff_curr_len;
        int last_msg_id;
    public:
        DdsConnection(IDataSink* receiver, int domainId,
                      string topic_name, int buff_size);
        virtual ~DdsConnection();
        int loop(int wait_time);

        virtual void sink(char* data, int len, int msgId);
    };
    
};

#endif
