#include "rti_impl.h"
#include <stdio.h>
#include <stdlib.h>

#include "rti_dds.h"
#include "rti_ddsSupport.h"
#include "ndds/ndds_cpp.h"


namespace rti {

    // TODO: initial msgId
    // TODO: try out USER_QOS_PROFILES; where it should be stored?
    // TODO: try instance register
    // TODO: following should be done from somewhere:
    // // retcode = DDSDomainParticipantFactory::finalize_instance();
    // // if (retcode != DDS_RETCODE_OK) {
    // //     printf("finalize_instance error %d\n", retcode);
    // //     status = -1;
    // // }
    // TODO: move Buffer1024listener inside DdsConnection
    // TODO: should somehow sync ddsconnection loop thread with
    //       dds callback (for Buffer1024Listener) thread, so that
    //       exceptional situations are handled; maybe loop thread
    //       could call sink() as well, to have things simplier
    // TODO: hide sink() in DdsConnection


    RtiException::RtiException(string cause) : cause(cause) {
    }
    RtiException::~RtiException() {
    }
    char const * RtiException::what() const {
        return cause.c_str();
    }

    AbstractDds::AbstractDds() : participant(0) {
    }
    AbstractDds::~AbstractDds() {
    }

    void AbstractDds::shutdown() {
        DDS_ReturnCode_t retcode;
        if (participant != NULL) {
            retcode = participant->delete_contained_entities();
            if (retcode != DDS_RETCODE_OK) {
                throw RtiException
                    (SSTR("delete_contained_entities error " <<
                          retcode));
            }

            retcode = DDSTheParticipantFactory->delete_participant(participant);
            if (retcode != DDS_RETCODE_OK) {
                throw RtiException
                    (SSTR("delete_participant error " <<
                          retcode));
            }

            participant == NULL;
        }
    }

    void AbstractDds::crashdown(string msg) {
        shutdown();
        throw RtiException(msg);
    }

    void AbstractDds::close() {
        shutdown();
    }

    DataSender::DataSender(int domainId, string topic_name) :
        participant(0), msgId(1) {
        DDS_ReturnCode_t retcode;
        const char *type_name;

        participant = DDSTheParticipantFactory->create_participant
            (domainId, DDS_PARTICIPANT_QOS_DEFAULT, 
             NULL, DDS_STATUS_MASK_NONE);
        if (participant == NULL)
            crashdown("create_participant error");

        publisher = participant->create_publisher(
            DDS_PUBLISHER_QOS_DEFAULT, NULL, DDS_STATUS_MASK_NONE);
        if (publisher == NULL)
            crashdown("create_publisher error");

        type_name = Buffer1024TypeSupport::get_type_name();
        retcode = Buffer1024TypeSupport::register_type(
            participant, type_name);
        if (retcode != DDS_RETCODE_OK)
            crashdown(SSTR("register_type error " << retcode));

        topic = participant->create_topic(
            topic_name.c_str(), type_name, DDS_TOPIC_QOS_DEFAULT,
            NULL, DDS_STATUS_MASK_NONE);
        if (topic == NULL)
            crashdown("create_topic error");

        writer = publisher->create_datawriter(
            topic, DDS_DATAWRITER_QOS_DEFAULT, NULL,
            DDS_STATUS_MASK_NONE);
        if (writer == NULL)
            crashdown("create_datawriter error");

        Buffer1024_writer = Buffer1024DataWriter::narrow(writer);
        if (Buffer1024_writer == NULL)
            crashdown("DataWriter narrow error");

        instance = Buffer1024TypeSupport::create_data();
        if (instance == NULL)
            crashdown("Buffer1024TypeSupport::create_data error");
    }
    DataSender::~DataSender() {
        try {
            published_shutdown();
        } catch (RtiException& e) {
            cerr << e.what() << endl;
        }
    }
    void DataSender::sink(char* data, int len, int msgId) {
        DDS_ReturnCode_t retcode;

        instance->payload.loan_contiguous(data, len, len);
        instance->length = len;
        instance->msg_id = msgId;
        retcode = Buffer1024_writer->write(*instance, NULL);
        instance->payload.unloan();
        if (retcode != DDS_RETCODE_OK)
            throw RtiException(SSTR("write error error" << retcode));
    }

    Buffer1024Listener::Buffer1024Listener(IDataSink* sink) :
        sink(sink) {
    }
    
    void Buffer1024Listener::on_data_available(
        DDSDataReader* reader) {
        
        Buffer1024DataReader *Buffer1024_reader = NULL;
        Buffer1024Seq data_seq;
        DDS_SampleInfoSeq info_seq;
        DDS_ReturnCode_t retcode;
        int i;

        Buffer1024_reader = Buffer1024DataReader::narrow(reader);
        if (Buffer1024_reader == NULL) {
            printf("DataReader narrow error\n");
            return;
        }

        retcode = Buffer1024_reader->take(
            data_seq, info_seq, DDS_LENGTH_UNLIMITED,
            DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE,
            DDS_ANY_INSTANCE_STATE);

        if (retcode == DDS_RETCODE_NO_DATA) {
            return;
        } else if (retcode != DDS_RETCODE_OK) {
            printf("take error %d\n", retcode);
            return;
        }

        for (i = 0; i < data_seq.length(); ++i) {
            if (info_seq[i].valid_data) {
                Buffer1024* b = &data_seq[i];
                sink->sink(b->payload.get_contiguous_buffer(),
                    b->length, b->msg_id);
            }
        }

        retcode = Buffer1024_reader->return_loan(data_seq, info_seq);
        if (retcode != DDS_RETCODE_OK) {
            printf("return loan error %d\n", retcode);
        }
    }

    DdsConnection::DdsConnection(IDataSink* rec, int domainId,
                                 string topic_name,
                                 int buff_size) :
        receiver(rec), participant(0), reader_listener(0),
        buff(new char[buff_size]), buff_len(buff_size),
        buff_curr_len(0), last_msg_id(-1) {
        
        DDS_ReturnCode_t retcode;
        const char *type_name;

        DDS_ReturnCode_t retcode;
        const char *type_name;

        participant = DDSTheParticipantFactory->create_participant(
            domainId, DDS_PARTICIPANT_QOS_DEFAULT, 
            NULL, DDS_STATUS_MASK_NONE);
        if (participant == NULL)
            crashdown("create_participant error");

        subscriber = participant->create_subscriber(
            DDS_SUBSCRIBER_QOS_DEFAULT, NULL,
            DDS_STATUS_MASK_NONE);
        if (subscriber == NULL)
            crashdown("create_participant error");

        type_name = Buffer1024TypeSupport::get_type_name();
        retcode = Buffer1024TypeSupport::register_type(
            participant, type_name);
        if (retcode != DDS_RETCODE_OK)
            crashdown(SSTR("register_type error " << retcode));

        topic = participant->create_topic(
            topic_name.c_str(), type_name, DDS_TOPIC_QOS_DEFAULT,
            NULL, DDS_STATUS_MASK_NONE);
        if (topic == NULL)
            crashdown("create_topic error");
        
        reader_listener = new Buffer1024Listener(this);
        reader = subscriber->create_datareader(
            topic, DDS_DATAREADER_QOS_DEFAULT, reader_listener,
            DDS_STATUS_MASK_ALL);
        if (reader == NULL)
            crashdown("create_datareader error");
    }
    DdsConnection::~DdsConnection() {
        if (reader_listener) {
            delete reader_listener;
            reader_listener = 0;
        }
    }
    int DdsConnection::loop(int wait_time) {
        DDS_Duration_t receive_period = {wait_time, 0};
        NDDSUtility::sleep(receive_period);
        return 0;
    }

    void DdsConnection::sink(char* data, int len, int msgId) {
        if (last_msg_id == -1) {
        } else if (msgId != last_msg_id) {
        }
    }

}
