#include "rti_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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
    // TODO: move Buffer1024klistener inside DdsConnection
    // TODO: should somehow sync ddsconnection loop thread with
    //       dds callback (for Buffer1024kListener) thread, so that
    //       exceptional situations are handled; maybe loop thread
    //       could call sink() as well, to have things simplier
    // TODO: hide sink() in DdsConnection


    RtiException::RtiException(string cause) : cause(cause) {
    }
    RtiException::~RtiException() _GLIBCXX_USE_NOEXCEPT {
    }
    char const * RtiException::what() const throw() {
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

            participant = NULL;
        }
    }

    void AbstractDds::crashdown(string msg) {
        shutdown();
        throw RtiException(msg);
    }

    void AbstractDds::close() {
        shutdown();
    }

    DdsDataSender::DdsDataSender(int domainId, string topic_name) :
        curr_msgId(1) {
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

        type_name = Buffer1024kTypeSupport::get_type_name();
        retcode = Buffer1024kTypeSupport::register_type(
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

        Buffer1024k_writer = Buffer1024kDataWriter::narrow(writer);
        if (Buffer1024k_writer == NULL)
            crashdown("DataWriter narrow error");

        instance = Buffer1024kTypeSupport::create_data();
        if (instance == NULL)
            crashdown("Buffer1024kTypeSupport::create_data error");
        instance->payload.maximum(0);
    }

    DdsDataSender::~DdsDataSender() {
        try {
            shutdown();
        } catch (RtiException& e) {
            cerr << e.what() << endl;
        }
    }
    void DdsDataSender::sink(char* data, int len, int msgId,
        bool isLast) {
        if (!isLast)
            throw RtiException("currently sender-side buffering not supported");
        if (msgId >= 0)
            throw RtiException("currently non-automatic msg_ids are not supported");

        DDS_ReturnCode_t retcode;

        instance->payload.loan_contiguous((DDS_Octet*)data, len, len);
        instance->length = len;
        instance->msg_id = curr_msgId++;
        instance->is_last = isLast;
        retcode = Buffer1024k_writer->write(*instance,
            DDS_HANDLE_NIL);
        instance->payload.unloan();
        if (retcode != DDS_RETCODE_OK)
            throw RtiException(SSTR("write error error" << retcode));
    }

    Buffer1024kListener::Buffer1024kListener(IDataSink* sink) :
        sink(sink) {
    }
    
    void Buffer1024kListener::on_data_available(
        DDSDataReader* reader) {

        Buffer1024kDataReader *Buffer1024k_reader = NULL;
        Buffer1024kSeq data_seq;
        DDS_SampleInfoSeq info_seq;
        DDS_ReturnCode_t retcode;
        int i;

        Buffer1024k_reader = Buffer1024kDataReader::narrow(reader);
        if (Buffer1024k_reader == NULL) {
            printf("DataReader narrow error\n");
            return;
        }

        retcode = Buffer1024k_reader->take(
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
                Buffer1024k* b = &data_seq[i];
                sink->sink((char*)b->payload.get_contiguous_buffer(),
                    b->length, b->msg_id, b->is_last);
            }
        }

        retcode = Buffer1024k_reader->return_loan(data_seq, info_seq);
        if (retcode != DDS_RETCODE_OK) {
            printf("return loan error %d\n", retcode);
        }
    }

    DdsConnection::DdsConnection(IDataSink* rec, int domainId,
                                 string topic_name,
                                 int buff_size) :
        receiver(rec), reader_listener(0),
        buff(new char[buff_size]), buff_len(buff_size),
        buff_curr_len(0), curr_msg_id(-1) {
        
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

        type_name = Buffer1024kTypeSupport::get_type_name();
        retcode = Buffer1024kTypeSupport::register_type(
            participant, type_name);
        if (retcode != DDS_RETCODE_OK)
            crashdown(SSTR("register_type error " << retcode));

        topic = participant->create_topic(
            topic_name.c_str(), type_name, DDS_TOPIC_QOS_DEFAULT,
            NULL, DDS_STATUS_MASK_NONE);
        if (topic == NULL)
            crashdown("create_topic error");
        
        reader_listener = new Buffer1024kListener(this);
        reader = subscriber->create_datareader(
            topic, DDS_DATAREADER_QOS_DEFAULT, reader_listener,
            DDS_STATUS_MASK_ALL);
        if (reader == NULL)
            crashdown("create_datareader error");
    }
    DdsConnection::~DdsConnection() {
        if (reader_listener) {
            DDS_ReturnCode_t retcode = participant->delete_contained_entities();
            if (retcode != DDS_RETCODE_OK) {
                printf("delete_contained_entities error %d\n", retcode);
            }
            retcode = DDSTheParticipantFactory->delete_participant(participant);
            if (retcode != DDS_RETCODE_OK) {
                printf("delete_participant error %d\n", retcode);
            }

            delete reader_listener;
            reader_listener = 0;
        }
    }
    int DdsConnection::loop(int wait_time) {
        if (wait_time < 0) {
            pause();
        } else {
            DDS_Duration_t receive_period = {wait_time, 0};
            NDDSUtility::sleep(receive_period);
        }
        return 0;
    }

    void DdsConnection::sink(char* data, int len, int msgId,
        bool isLast) {
        // no need to sync threads, as at this point everything has
        // been initialized
        if (msgId < 0) {
            printf("bad msgId\n");
            dump_buffer();
            return;
        }

        if (msgId != curr_msg_id) {
            if (buff_curr_len) {
                printf("previous message wasn't marked as last");
                dump_buffer();
            }

            if (isLast) {
                receiver->sink(data, len, msgId, true);
            } else {
                add_to_buffer(data, len, msgId, isLast);
            }
        } else {
            add_to_buffer(data, len, msgId, isLast);
        }
    }

    void DdsConnection::send_curr_buffer() {
        receiver->sink(buff, buff_curr_len, curr_msg_id, true);
        curr_msg_id = -1;
        buff_curr_len = 0;
    }

    void DdsConnection::send_curr_incomplete_buffer() {
        receiver->sink(buff, buff_curr_len, curr_msg_id, false);
        buff_curr_len = 0;
    }
    
    void DdsConnection::dump_buffer() {
        buff_curr_len = 0;
        curr_msg_id = -1;
    }

    void DdsConnection::add_to_buffer(char* data, int len,
        int msgId, bool isLast) {
        curr_msg_id = msgId;

        while (buff_curr_len + len > buff_len) {
            int nl = min(buff_len - buff_curr_len, len);
            memcpy(buff + buff_curr_len, data, nl);
            buff_curr_len += nl;
            send_curr_incomplete_buffer();
            len -= nl;
            data = data + nl;
        }
        // len > 0, according to "while" statement above
        memcpy(buff + buff_curr_len, data, len);
        if (isLast) {
            send_curr_buffer();
        }
    }

}
