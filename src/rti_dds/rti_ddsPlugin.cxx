

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from rti_dds.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#include <string.h>

#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif

#ifndef osapi_type_h
#include "osapi/osapi_type.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif

#ifndef osapi_utility_h
#include "osapi/osapi_utility.h"
#endif

#ifndef cdr_type_h
#include "cdr/cdr_type.h"
#endif

#ifndef cdr_type_object_h
#include "cdr/cdr_typeObject.h"
#endif

#ifndef cdr_encapsulation_h
#include "cdr/cdr_encapsulation.h"
#endif

#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#include "rti_ddsPlugin.h"

/* ----------------------------------------------------------------------------
*  Type Buffer1024
* -------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------- */

Buffer1024*
Buffer1024PluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params){
    Buffer1024 *sample = NULL;

    RTIOsapiHeap_allocateStructure(
        &sample, Buffer1024);

    if(sample != NULL) {
        if (!Buffer1024_initialize_w_params(sample,alloc_params)) {
            RTIOsapiHeap_freeStructure(sample);
            return NULL;
        }
    }        
    return sample; 
} 

Buffer1024 *
Buffer1024PluginSupport_create_data_ex(RTIBool allocate_pointers){
    Buffer1024 *sample = NULL;

    RTIOsapiHeap_allocateStructure(
        &sample, Buffer1024);

    if(sample != NULL) {
        if (!Buffer1024_initialize_ex(sample,allocate_pointers, RTI_TRUE)) {
            RTIOsapiHeap_freeStructure(sample);
            return NULL;
        }
    }
    return sample; 
}

Buffer1024 *
Buffer1024PluginSupport_create_data(void)
{
    return Buffer1024PluginSupport_create_data_ex(RTI_TRUE);
}

void 
Buffer1024PluginSupport_destroy_data_w_params(
    Buffer1024 *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params) {

    Buffer1024_finalize_w_params(sample,dealloc_params);

    RTIOsapiHeap_freeStructure(sample);
}

void 
Buffer1024PluginSupport_destroy_data_ex(
    Buffer1024 *sample,RTIBool deallocate_pointers) {

    Buffer1024_finalize_ex(sample,deallocate_pointers);

    RTIOsapiHeap_freeStructure(sample);
}

void 
Buffer1024PluginSupport_destroy_data(
    Buffer1024 *sample) {

    Buffer1024PluginSupport_destroy_data_ex(sample,RTI_TRUE);

}

RTIBool 
Buffer1024PluginSupport_copy_data(
    Buffer1024 *dst,
    const Buffer1024 *src)
{
    return Buffer1024_copy(dst,src);
}

void 
Buffer1024PluginSupport_print_data(
    const Buffer1024 *sample,
    const char *desc,
    unsigned int indent_level)
{

    RTICdrType_printIndent(indent_level);

    if (desc != NULL) {
        RTILog_debug("%s:\n", desc);
    } else {
        RTILog_debug("\n");
    }

    if (sample == NULL) {
        RTILog_debug("NULL\n");
        return;
    }

    if (DDS_OctetSeq_get_contiguous_bufferI(&sample->payload) != NULL) {
        RTICdrType_printArray(
            DDS_OctetSeq_get_contiguous_bufferI(&sample->payload),
            DDS_OctetSeq_get_length(&sample->payload),
            RTI_CDR_OCTET_SIZE,
            (RTICdrTypePrintFunction)RTICdrType_printOctet,
            "payload", indent_level + 1);
    } else {
        RTICdrType_printPointerArray(
            DDS_OctetSeq_get_discontiguous_bufferI(&sample->payload),
            DDS_OctetSeq_get_length(&sample->payload ),
            (RTICdrTypePrintFunction)RTICdrType_printOctet,
            "payload", indent_level + 1);
    }

    RTICdrType_printLong(
        &sample->length, "length", indent_level + 1);    

    RTICdrType_printLong(
        &sample->msg_id, "msg_id", indent_level + 1);    

    RTICdrType_printBoolean(
        &sample->is_last, "is_last", indent_level + 1);    

}

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

PRESTypePluginParticipantData 
Buffer1024Plugin_on_participant_attached(
    void *registration_data,
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration,
    void *container_plugin_context,
    RTICdrTypeCode *type_code)
{
    if (registration_data) {} /* To avoid warnings */
    if (participant_info) {} /* To avoid warnings */
    if (top_level_registration) {} /* To avoid warnings */
    if (container_plugin_context) {} /* To avoid warnings */
    if (type_code) {} /* To avoid warnings */

    return PRESTypePluginDefaultParticipantData_new(participant_info);

}

void 
Buffer1024Plugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data)
{

    PRESTypePluginDefaultParticipantData_delete(participant_data);
}

PRESTypePluginEndpointData
Buffer1024Plugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *containerPluginContext)
{
    PRESTypePluginEndpointData epd = NULL;

    unsigned int serializedSampleMaxSize;

    if (top_level_registration) {} /* To avoid warnings */
    if (containerPluginContext) {} /* To avoid warnings */

    epd = PRESTypePluginDefaultEndpointData_new(
        participant_data,
        endpoint_info,
        (PRESTypePluginDefaultEndpointDataCreateSampleFunction)
        Buffer1024PluginSupport_create_data,
        (PRESTypePluginDefaultEndpointDataDestroySampleFunction)
        Buffer1024PluginSupport_destroy_data,
        NULL , NULL );

    if (epd == NULL) {
        return NULL;
    } 

    if (endpoint_info->endpointKind == PRES_TYPEPLUGIN_ENDPOINT_WRITER) {
        serializedSampleMaxSize = Buffer1024Plugin_get_serialized_sample_max_size(
            epd,RTI_FALSE,RTI_CDR_ENCAPSULATION_ID_CDR_BE,0);

        PRESTypePluginDefaultEndpointData_setMaxSizeSerializedSample(epd, serializedSampleMaxSize);

        if (PRESTypePluginDefaultEndpointData_createWriterPool(
            epd,
            endpoint_info,
            (PRESTypePluginGetSerializedSampleMaxSizeFunction)
            Buffer1024Plugin_get_serialized_sample_max_size, epd,
            (PRESTypePluginGetSerializedSampleSizeFunction)
            Buffer1024Plugin_get_serialized_sample_size,
            epd) == RTI_FALSE) {
            PRESTypePluginDefaultEndpointData_delete(epd);
            return NULL;
        }
    }

    return epd;    
}

void 
Buffer1024Plugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data)
{  

    PRESTypePluginDefaultEndpointData_delete(endpoint_data);
}

void    
Buffer1024Plugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024 *sample,
    void *handle)
{

    Buffer1024_finalize_optional_members(sample, RTI_TRUE);

    PRESTypePluginDefaultEndpointData_returnSample(
        endpoint_data, sample, handle);
}

RTIBool 
Buffer1024Plugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024 *dst,
    const Buffer1024 *src)
{
    if (endpoint_data) {} /* To avoid warnings */
    return Buffer1024PluginSupport_copy_data(dst,src);
}

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */
unsigned int 
Buffer1024Plugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

RTIBool 
Buffer1024Plugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const Buffer1024 *sample, 
    struct RTICdrStream *stream,    
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos)
{
    char * position = NULL;
    RTIBool retval = RTI_TRUE;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */

    if(serialize_encapsulation) {
        if (!RTICdrStream_serializeAndSetCdrEncapsulation(stream , encapsulation_id)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }

    if(serialize_sample) {

        if (DDS_OctetSeq_get_contiguous_bufferI(&sample->payload) != NULL) {
            if (!RTICdrStream_serializePrimitiveSequence(
                stream,
                DDS_OctetSeq_get_contiguous_bufferI(&sample->payload),
                DDS_OctetSeq_get_length(&sample->payload),
                (1024),
                RTI_CDR_OCTET_TYPE)) {
                return RTI_FALSE;
            } 
        } else {
            if (!RTICdrStream_serializePrimitivePointerSequence(
                stream,
                (const void **) DDS_OctetSeq_get_discontiguous_bufferI(&sample->payload),
                DDS_OctetSeq_get_length(&sample->payload),
                (1024), 
                RTI_CDR_OCTET_TYPE)) {
                return RTI_FALSE;
            } 
        } 

        if (!RTICdrStream_serializeLong(
            stream, &sample->length)) {
            return RTI_FALSE;
        }

        if (!RTICdrStream_serializeLong(
            stream, &sample->msg_id)) {
            return RTI_FALSE;
        }

        if (!RTICdrStream_serializeBoolean(
            stream, &sample->is_last)) {
            return RTI_FALSE;
        }

    }

    if(serialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return retval;
}

RTIBool 
Buffer1024Plugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024 *sample,
    struct RTICdrStream *stream,   
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos)
{

    char * position = NULL;

    RTIBool done = RTI_FALSE;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */
    if(deserialize_encapsulation) {

        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }
    if(deserialize_sample) {

        Buffer1024_initialize_ex(sample, RTI_FALSE, RTI_FALSE);

        {
            RTICdrUnsignedLong sequence_length;
            if (DDS_OctetSeq_get_contiguous_bufferI(&sample->payload) != NULL) {
                if (!RTICdrStream_deserializePrimitiveSequence(
                    stream,
                    DDS_OctetSeq_get_contiguous_bufferI(&sample->payload),
                    &sequence_length,
                    DDS_OctetSeq_get_maximum(&sample->payload),
                    RTI_CDR_OCTET_TYPE)){
                    goto fin; 
                }
            } else {
                if (!RTICdrStream_deserializePrimitivePointerSequence(
                    stream,
                    (void **) DDS_OctetSeq_get_discontiguous_bufferI(&sample->payload),
                    &sequence_length,
                    DDS_OctetSeq_get_maximum(&sample->payload),
                    RTI_CDR_OCTET_TYPE)){
                    goto fin; 
                }
            }
            if (!DDS_OctetSeq_set_length(&sample->payload, sequence_length)) {
                return RTI_FALSE;
            }

        }
        if (!RTICdrStream_deserializeLong(
            stream, &sample->length)) {
            goto fin; 
        }
        if (!RTICdrStream_deserializeLong(
            stream, &sample->msg_id)) {
            goto fin; 
        }
        if (!RTICdrStream_deserializeBoolean(
            stream, &sample->is_last)) {
            goto fin; 
        }
    }

    done = RTI_TRUE;
  fin:
    if (done != RTI_TRUE && 
    RTICdrStream_getRemainder(stream) >=
    RTI_CDR_PARAMETER_HEADER_ALIGNMENT) {
        return RTI_FALSE;   
    }
    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

RTIBool
Buffer1024Plugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const Buffer1024 *sample)
{
    struct RTICdrStream stream;
    struct PRESTypePluginDefaultEndpointData epd;
    RTIBool result;

    if (length == NULL) {
        return RTI_FALSE;
    }

    epd._maxSizeSerializedSample =
    Buffer1024Plugin_get_serialized_sample_max_size(
        NULL, RTI_TRUE, RTI_CDR_ENCAPSULATION_ID_CDR_NATIVE, 0);

    if (buffer == NULL) {
        *length = 
        Buffer1024Plugin_get_serialized_sample_size(
            (PRESTypePluginEndpointData)&epd,
            RTI_TRUE,
            RTI_CDR_ENCAPSULATION_ID_CDR_NATIVE,
            0,
            sample);

        if (*length == 0) {
            return RTI_FALSE;
        }

        return RTI_TRUE;
    }    

    RTICdrStream_init(&stream);
    RTICdrStream_set(&stream, (char *)buffer, *length);

    result = Buffer1024Plugin_serialize(
        (PRESTypePluginEndpointData)&epd, sample, &stream, 
        RTI_TRUE, RTI_CDR_ENCAPSULATION_ID_CDR_NATIVE, 
        RTI_TRUE, NULL);  

    *length = RTICdrStream_getCurrentPositionOffset(&stream);
    return result;     
}

RTIBool
Buffer1024Plugin_deserialize_from_cdr_buffer(
    Buffer1024 *sample,
    const char * buffer,
    unsigned int length)
{
    struct RTICdrStream stream;

    RTICdrStream_init(&stream);
    RTICdrStream_set(&stream, (char *)buffer, length);

    return Buffer1024Plugin_deserialize_sample( 
        NULL, sample,
        &stream, RTI_TRUE, RTI_TRUE, 
        NULL);
}

RTIBool 
Buffer1024Plugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024 **sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,   
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos)
{

    RTIBool result;
    if (drop_sample) {} /* To avoid warnings */

    stream->_xTypesState.unassignable = RTI_FALSE;
    result= Buffer1024Plugin_deserialize_sample( 
        endpoint_data, (sample != NULL)?*sample:NULL,
        stream, deserialize_encapsulation, deserialize_sample, 
        endpoint_plugin_qos);
    if (result) {
        if (stream->_xTypesState.unassignable) {
            result = RTI_FALSE;
        }
    }

    return result;

}

RTIBool Buffer1024Plugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream,   
    RTIBool skip_encapsulation,
    RTIBool skip_sample, 
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    RTIBool done = RTI_FALSE;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */

    if(skip_encapsulation) {
        if (!RTICdrStream_skipEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }

    if (skip_sample) {

        {
            RTICdrUnsignedLong sequence_length;
            if (!RTICdrStream_skipPrimitiveSequence(
                stream,
                &sequence_length,
                RTI_CDR_OCTET_TYPE)){
                goto fin; 
            }
        }
        if (!RTICdrStream_skipLong (stream)) {
            goto fin; 
        }
        if (!RTICdrStream_skipLong (stream)) {
            goto fin; 
        }
        if (!RTICdrStream_skipBoolean (stream)) {
            goto fin; 
        }
    }

    done = RTI_TRUE;
  fin:
    if (done != RTI_TRUE && 
    RTICdrStream_getRemainder(stream) >=
    RTI_CDR_PARAMETER_HEADER_ALIGNMENT) {
        return RTI_FALSE;   
    }
    if(skip_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

unsigned int 
Buffer1024Plugin_get_serialized_sample_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */ 
    if (overflow) {} /* To avoid warnings */

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment +=RTICdrType_getPrimitiveSequenceMaxSizeSerialized(
        current_alignment,(1024),RTI_CDR_OCTET_TYPE) ;

    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);

    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);

    current_alignment +=RTICdrType_getBooleanMaxSizeSerialized(
        current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return  current_alignment - initial_alignment;
}

unsigned int 
Buffer1024Plugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{
    unsigned int size;
    RTIBool overflow = RTI_FALSE;

    size = Buffer1024Plugin_get_serialized_sample_max_size_ex(
        endpoint_data,&overflow,include_encapsulation,encapsulation_id,current_alignment);

    if (overflow) {
        size = RTI_CDR_MAX_SERIALIZED_SIZE;
    }

    return size;
}

unsigned int 
Buffer1024Plugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */ 

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment +=    RTICdrType_getPrimitiveSequenceMaxSizeSerialized(
        current_alignment,0, RTI_CDR_OCTET_TYPE);
    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment +=RTICdrType_getBooleanMaxSizeSerialized(
        current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return  current_alignment - initial_alignment;
}

/* Returns the size of the sample in its serialized form (in bytes).
* It can also be an estimation in excess of the real buffer needed 
* during a call to the serialize() function.
* The value reported does not have to include the space for the
* encapsulation flags.
*/
unsigned int
Buffer1024Plugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const Buffer1024 * sample) 
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */ 
    if (sample==NULL) {
        return 0;
    }

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment += RTICdrType_getPrimitiveSequenceSerializedSize(
        current_alignment, 
        DDS_OctetSeq_get_length(&sample->payload),
        RTI_CDR_OCTET_TYPE);
    current_alignment += RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment += RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment += RTICdrType_getBooleanMaxSizeSerialized(
        current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return current_alignment - initial_alignment;
}

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */

PRESTypePluginKeyKind 
Buffer1024Plugin_get_key_kind(void)
{
    return PRES_TYPEPLUGIN_NO_KEY;
}

RTIBool 
Buffer1024Plugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const Buffer1024 *sample, 
    struct RTICdrStream *stream,    
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    if(serialize_encapsulation) {
        if (!RTICdrStream_serializeAndSetCdrEncapsulation(stream , encapsulation_id)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }

    if(serialize_key) {

        if (!Buffer1024Plugin_serialize(
            endpoint_data,
            sample,
            stream,
            RTI_FALSE, encapsulation_id,
            RTI_TRUE,
            endpoint_plugin_qos)) {
            return RTI_FALSE;
        }

    }

    if(serialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

RTIBool Buffer1024Plugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024 *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */

    if(deserialize_encapsulation) {

        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }
    if (deserialize_key) {

        if (!Buffer1024Plugin_deserialize_sample(
            endpoint_data, sample, stream, 
            RTI_FALSE, RTI_TRUE, 
            endpoint_plugin_qos)) {
            return RTI_FALSE;
        }
    }

    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

RTIBool Buffer1024Plugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024 **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos)
{
    RTIBool result;
    if (drop_sample) {} /* To avoid warnings */
    stream->_xTypesState.unassignable = RTI_FALSE;
    result= Buffer1024Plugin_deserialize_key_sample(
        endpoint_data, (sample != NULL)?*sample:NULL, stream,
        deserialize_encapsulation, deserialize_key, endpoint_plugin_qos);
    if (result) {
        if (stream->_xTypesState.unassignable) {
            result = RTI_FALSE;
        }
    }

    return result;    

}

unsigned int
Buffer1024Plugin_get_serialized_key_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */
    if (overflow) {} /* To avoid warnings */

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment += Buffer1024Plugin_get_serialized_sample_max_size_ex(
        endpoint_data, overflow,RTI_FALSE, encapsulation_id, current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return current_alignment - initial_alignment;
}

unsigned int
Buffer1024Plugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{
    unsigned int size;
    RTIBool overflow = RTI_FALSE;

    size = Buffer1024Plugin_get_serialized_key_max_size_ex(
        endpoint_data,&overflow,include_encapsulation,encapsulation_id,current_alignment);

    if (overflow) {
        size = RTI_CDR_MAX_SERIALIZED_SIZE;
    }

    return size;
}

RTIBool 
Buffer1024Plugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024 *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    RTIBool done = RTI_FALSE;
    RTIBool error = RTI_FALSE;

    if (stream == NULL) {
        error = RTI_TRUE;
        goto fin;
    }
    if(deserialize_encapsulation) {
        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }
        position = RTICdrStream_resetAlignment(stream);
    }

    if (deserialize_key) {

        if (!Buffer1024Plugin_deserialize_sample(
            endpoint_data, sample, stream, RTI_FALSE, 
            RTI_TRUE, endpoint_plugin_qos)) {
            return RTI_FALSE;
        }

    }

    done = RTI_TRUE;
  fin:
    if(!error) {
        if (done != RTI_TRUE && 
        RTICdrStream_getRemainder(stream) >=
        RTI_CDR_PARAMETER_HEADER_ALIGNMENT) {
            return RTI_FALSE;   
        }
    } else {
        return error;
    }       

    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

/* ------------------------------------------------------------------------
* Plug-in Installation Methods
* ------------------------------------------------------------------------ */
struct PRESTypePlugin *Buffer1024Plugin_new(void) 
{ 
    struct PRESTypePlugin *plugin = NULL;
    const struct PRESTypePluginVersion PLUGIN_VERSION = 
    PRES_TYPE_PLUGIN_VERSION_2_0;

    RTIOsapiHeap_allocateStructure(
        &plugin, struct PRESTypePlugin);
    if (plugin == NULL) {
        return NULL;
    }

    plugin->version = PLUGIN_VERSION;

    /* set up parent's function pointers */
    plugin->onParticipantAttached =
    (PRESTypePluginOnParticipantAttachedCallback)
    Buffer1024Plugin_on_participant_attached;
    plugin->onParticipantDetached =
    (PRESTypePluginOnParticipantDetachedCallback)
    Buffer1024Plugin_on_participant_detached;
    plugin->onEndpointAttached =
    (PRESTypePluginOnEndpointAttachedCallback)
    Buffer1024Plugin_on_endpoint_attached;
    plugin->onEndpointDetached =
    (PRESTypePluginOnEndpointDetachedCallback)
    Buffer1024Plugin_on_endpoint_detached;

    plugin->copySampleFnc =
    (PRESTypePluginCopySampleFunction)
    Buffer1024Plugin_copy_sample;
    plugin->createSampleFnc =
    (PRESTypePluginCreateSampleFunction)
    Buffer1024Plugin_create_sample;
    plugin->destroySampleFnc =
    (PRESTypePluginDestroySampleFunction)
    Buffer1024Plugin_destroy_sample;

    plugin->serializeFnc =
    (PRESTypePluginSerializeFunction)
    Buffer1024Plugin_serialize;
    plugin->deserializeFnc =
    (PRESTypePluginDeserializeFunction)
    Buffer1024Plugin_deserialize;
    plugin->getSerializedSampleMaxSizeFnc =
    (PRESTypePluginGetSerializedSampleMaxSizeFunction)
    Buffer1024Plugin_get_serialized_sample_max_size;
    plugin->getSerializedSampleMinSizeFnc =
    (PRESTypePluginGetSerializedSampleMinSizeFunction)
    Buffer1024Plugin_get_serialized_sample_min_size;

    plugin->getSampleFnc =
    (PRESTypePluginGetSampleFunction)
    Buffer1024Plugin_get_sample;
    plugin->returnSampleFnc =
    (PRESTypePluginReturnSampleFunction)
    Buffer1024Plugin_return_sample;

    plugin->getKeyKindFnc =
    (PRESTypePluginGetKeyKindFunction)
    Buffer1024Plugin_get_key_kind;

    /* These functions are only used for keyed types. As this is not a keyed
    type they are all set to NULL
    */
    plugin->serializeKeyFnc = NULL ;    
    plugin->deserializeKeyFnc = NULL;  
    plugin->getKeyFnc = NULL;
    plugin->returnKeyFnc = NULL;
    plugin->instanceToKeyFnc = NULL;
    plugin->keyToInstanceFnc = NULL;
    plugin->getSerializedKeyMaxSizeFnc = NULL;
    plugin->instanceToKeyHashFnc = NULL;
    plugin->serializedSampleToKeyHashFnc = NULL;
    plugin->serializedKeyToKeyHashFnc = NULL;    
    plugin->typeCode =  (struct RTICdrTypeCode *)Buffer1024_get_typecode();

    plugin->languageKind = PRES_TYPEPLUGIN_CPP_LANG;

    /* Serialized buffer */
    plugin->getBuffer = 
    (PRESTypePluginGetBufferFunction)
    Buffer1024Plugin_get_buffer;
    plugin->returnBuffer = 
    (PRESTypePluginReturnBufferFunction)
    Buffer1024Plugin_return_buffer;
    plugin->getSerializedSampleSizeFnc =
    (PRESTypePluginGetSerializedSampleSizeFunction)
    Buffer1024Plugin_get_serialized_sample_size;

    plugin->endpointTypeName = Buffer1024TYPENAME;

    return plugin;
}

void
Buffer1024Plugin_delete(struct PRESTypePlugin *plugin)
{
    RTIOsapiHeap_freeStructure(plugin);
} 

/* ----------------------------------------------------------------------------
*  Type Buffer1024k
* -------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------- */

Buffer1024k*
Buffer1024kPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params){
    Buffer1024k *sample = NULL;

    RTIOsapiHeap_allocateStructure(
        &sample, Buffer1024k);

    if(sample != NULL) {
        if (!Buffer1024k_initialize_w_params(sample,alloc_params)) {
            RTIOsapiHeap_freeStructure(sample);
            return NULL;
        }
    }        
    return sample; 
} 

Buffer1024k *
Buffer1024kPluginSupport_create_data_ex(RTIBool allocate_pointers){
    Buffer1024k *sample = NULL;

    RTIOsapiHeap_allocateStructure(
        &sample, Buffer1024k);

    if(sample != NULL) {
        if (!Buffer1024k_initialize_ex(sample,allocate_pointers, RTI_TRUE)) {
            RTIOsapiHeap_freeStructure(sample);
            return NULL;
        }
    }
    return sample; 
}

Buffer1024k *
Buffer1024kPluginSupport_create_data(void)
{
    return Buffer1024kPluginSupport_create_data_ex(RTI_TRUE);
}

void 
Buffer1024kPluginSupport_destroy_data_w_params(
    Buffer1024k *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params) {

    Buffer1024k_finalize_w_params(sample,dealloc_params);

    RTIOsapiHeap_freeStructure(sample);
}

void 
Buffer1024kPluginSupport_destroy_data_ex(
    Buffer1024k *sample,RTIBool deallocate_pointers) {

    Buffer1024k_finalize_ex(sample,deallocate_pointers);

    RTIOsapiHeap_freeStructure(sample);
}

void 
Buffer1024kPluginSupport_destroy_data(
    Buffer1024k *sample) {

    Buffer1024kPluginSupport_destroy_data_ex(sample,RTI_TRUE);

}

RTIBool 
Buffer1024kPluginSupport_copy_data(
    Buffer1024k *dst,
    const Buffer1024k *src)
{
    return Buffer1024k_copy(dst,src);
}

void 
Buffer1024kPluginSupport_print_data(
    const Buffer1024k *sample,
    const char *desc,
    unsigned int indent_level)
{

    RTICdrType_printIndent(indent_level);

    if (desc != NULL) {
        RTILog_debug("%s:\n", desc);
    } else {
        RTILog_debug("\n");
    }

    if (sample == NULL) {
        RTILog_debug("NULL\n");
        return;
    }

    if (DDS_OctetSeq_get_contiguous_bufferI(&sample->payload) != NULL) {
        RTICdrType_printArray(
            DDS_OctetSeq_get_contiguous_bufferI(&sample->payload),
            DDS_OctetSeq_get_length(&sample->payload),
            RTI_CDR_OCTET_SIZE,
            (RTICdrTypePrintFunction)RTICdrType_printOctet,
            "payload", indent_level + 1);
    } else {
        RTICdrType_printPointerArray(
            DDS_OctetSeq_get_discontiguous_bufferI(&sample->payload),
            DDS_OctetSeq_get_length(&sample->payload ),
            (RTICdrTypePrintFunction)RTICdrType_printOctet,
            "payload", indent_level + 1);
    }

    RTICdrType_printLong(
        &sample->length, "length", indent_level + 1);    

    RTICdrType_printLong(
        &sample->msg_id, "msg_id", indent_level + 1);    

    RTICdrType_printBoolean(
        &sample->is_last, "is_last", indent_level + 1);    

}

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

PRESTypePluginParticipantData 
Buffer1024kPlugin_on_participant_attached(
    void *registration_data,
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration,
    void *container_plugin_context,
    RTICdrTypeCode *type_code)
{
    if (registration_data) {} /* To avoid warnings */
    if (participant_info) {} /* To avoid warnings */
    if (top_level_registration) {} /* To avoid warnings */
    if (container_plugin_context) {} /* To avoid warnings */
    if (type_code) {} /* To avoid warnings */

    return PRESTypePluginDefaultParticipantData_new(participant_info);

}

void 
Buffer1024kPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data)
{

    PRESTypePluginDefaultParticipantData_delete(participant_data);
}

PRESTypePluginEndpointData
Buffer1024kPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *containerPluginContext)
{
    PRESTypePluginEndpointData epd = NULL;

    unsigned int serializedSampleMaxSize;

    if (top_level_registration) {} /* To avoid warnings */
    if (containerPluginContext) {} /* To avoid warnings */

    epd = PRESTypePluginDefaultEndpointData_new(
        participant_data,
        endpoint_info,
        (PRESTypePluginDefaultEndpointDataCreateSampleFunction)
        Buffer1024kPluginSupport_create_data,
        (PRESTypePluginDefaultEndpointDataDestroySampleFunction)
        Buffer1024kPluginSupport_destroy_data,
        NULL , NULL );

    if (epd == NULL) {
        return NULL;
    } 

    if (endpoint_info->endpointKind == PRES_TYPEPLUGIN_ENDPOINT_WRITER) {
        serializedSampleMaxSize = Buffer1024kPlugin_get_serialized_sample_max_size(
            epd,RTI_FALSE,RTI_CDR_ENCAPSULATION_ID_CDR_BE,0);

        PRESTypePluginDefaultEndpointData_setMaxSizeSerializedSample(epd, serializedSampleMaxSize);

        if (PRESTypePluginDefaultEndpointData_createWriterPool(
            epd,
            endpoint_info,
            (PRESTypePluginGetSerializedSampleMaxSizeFunction)
            Buffer1024kPlugin_get_serialized_sample_max_size, epd,
            (PRESTypePluginGetSerializedSampleSizeFunction)
            Buffer1024kPlugin_get_serialized_sample_size,
            epd) == RTI_FALSE) {
            PRESTypePluginDefaultEndpointData_delete(epd);
            return NULL;
        }
    }

    return epd;    
}

void 
Buffer1024kPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data)
{  

    PRESTypePluginDefaultEndpointData_delete(endpoint_data);
}

void    
Buffer1024kPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024k *sample,
    void *handle)
{

    Buffer1024k_finalize_optional_members(sample, RTI_TRUE);

    PRESTypePluginDefaultEndpointData_returnSample(
        endpoint_data, sample, handle);
}

RTIBool 
Buffer1024kPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024k *dst,
    const Buffer1024k *src)
{
    if (endpoint_data) {} /* To avoid warnings */
    return Buffer1024kPluginSupport_copy_data(dst,src);
}

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */
unsigned int 
Buffer1024kPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

RTIBool 
Buffer1024kPlugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const Buffer1024k *sample, 
    struct RTICdrStream *stream,    
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos)
{
    char * position = NULL;
    RTIBool retval = RTI_TRUE;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */

    if(serialize_encapsulation) {
        if (!RTICdrStream_serializeAndSetCdrEncapsulation(stream , encapsulation_id)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }

    if(serialize_sample) {

        if (DDS_OctetSeq_get_contiguous_bufferI(&sample->payload) != NULL) {
            if (!RTICdrStream_serializePrimitiveSequence(
                stream,
                DDS_OctetSeq_get_contiguous_bufferI(&sample->payload),
                DDS_OctetSeq_get_length(&sample->payload),
                (1048576),
                RTI_CDR_OCTET_TYPE)) {
                return RTI_FALSE;
            } 
        } else {
            if (!RTICdrStream_serializePrimitivePointerSequence(
                stream,
                (const void **) DDS_OctetSeq_get_discontiguous_bufferI(&sample->payload),
                DDS_OctetSeq_get_length(&sample->payload),
                (1048576), 
                RTI_CDR_OCTET_TYPE)) {
                return RTI_FALSE;
            } 
        } 

        if (!RTICdrStream_serializeLong(
            stream, &sample->length)) {
            return RTI_FALSE;
        }

        if (!RTICdrStream_serializeLong(
            stream, &sample->msg_id)) {
            return RTI_FALSE;
        }

        if (!RTICdrStream_serializeBoolean(
            stream, &sample->is_last)) {
            return RTI_FALSE;
        }

    }

    if(serialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return retval;
}

RTIBool 
Buffer1024kPlugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024k *sample,
    struct RTICdrStream *stream,   
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos)
{

    char * position = NULL;

    RTIBool done = RTI_FALSE;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */
    if(deserialize_encapsulation) {

        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }
    if(deserialize_sample) {

        Buffer1024k_initialize_ex(sample, RTI_FALSE, RTI_FALSE);

        {
            RTICdrUnsignedLong sequence_length;
            if (DDS_OctetSeq_get_contiguous_bufferI(&sample->payload) != NULL) {
                if (!RTICdrStream_deserializePrimitiveSequence(
                    stream,
                    DDS_OctetSeq_get_contiguous_bufferI(&sample->payload),
                    &sequence_length,
                    DDS_OctetSeq_get_maximum(&sample->payload),
                    RTI_CDR_OCTET_TYPE)){
                    goto fin; 
                }
            } else {
                if (!RTICdrStream_deserializePrimitivePointerSequence(
                    stream,
                    (void **) DDS_OctetSeq_get_discontiguous_bufferI(&sample->payload),
                    &sequence_length,
                    DDS_OctetSeq_get_maximum(&sample->payload),
                    RTI_CDR_OCTET_TYPE)){
                    goto fin; 
                }
            }
            if (!DDS_OctetSeq_set_length(&sample->payload, sequence_length)) {
                return RTI_FALSE;
            }

        }
        if (!RTICdrStream_deserializeLong(
            stream, &sample->length)) {
            goto fin; 
        }
        if (!RTICdrStream_deserializeLong(
            stream, &sample->msg_id)) {
            goto fin; 
        }
        if (!RTICdrStream_deserializeBoolean(
            stream, &sample->is_last)) {
            goto fin; 
        }
    }

    done = RTI_TRUE;
  fin:
    if (done != RTI_TRUE && 
    RTICdrStream_getRemainder(stream) >=
    RTI_CDR_PARAMETER_HEADER_ALIGNMENT) {
        return RTI_FALSE;   
    }
    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

RTIBool
Buffer1024kPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const Buffer1024k *sample)
{
    struct RTICdrStream stream;
    struct PRESTypePluginDefaultEndpointData epd;
    RTIBool result;

    if (length == NULL) {
        return RTI_FALSE;
    }

    epd._maxSizeSerializedSample =
    Buffer1024kPlugin_get_serialized_sample_max_size(
        NULL, RTI_TRUE, RTI_CDR_ENCAPSULATION_ID_CDR_NATIVE, 0);

    if (buffer == NULL) {
        *length = 
        Buffer1024kPlugin_get_serialized_sample_size(
            (PRESTypePluginEndpointData)&epd,
            RTI_TRUE,
            RTI_CDR_ENCAPSULATION_ID_CDR_NATIVE,
            0,
            sample);

        if (*length == 0) {
            return RTI_FALSE;
        }

        return RTI_TRUE;
    }    

    RTICdrStream_init(&stream);
    RTICdrStream_set(&stream, (char *)buffer, *length);

    result = Buffer1024kPlugin_serialize(
        (PRESTypePluginEndpointData)&epd, sample, &stream, 
        RTI_TRUE, RTI_CDR_ENCAPSULATION_ID_CDR_NATIVE, 
        RTI_TRUE, NULL);  

    *length = RTICdrStream_getCurrentPositionOffset(&stream);
    return result;     
}

RTIBool
Buffer1024kPlugin_deserialize_from_cdr_buffer(
    Buffer1024k *sample,
    const char * buffer,
    unsigned int length)
{
    struct RTICdrStream stream;

    RTICdrStream_init(&stream);
    RTICdrStream_set(&stream, (char *)buffer, length);

    return Buffer1024kPlugin_deserialize_sample( 
        NULL, sample,
        &stream, RTI_TRUE, RTI_TRUE, 
        NULL);
}

RTIBool 
Buffer1024kPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024k **sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,   
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos)
{

    RTIBool result;
    if (drop_sample) {} /* To avoid warnings */

    stream->_xTypesState.unassignable = RTI_FALSE;
    result= Buffer1024kPlugin_deserialize_sample( 
        endpoint_data, (sample != NULL)?*sample:NULL,
        stream, deserialize_encapsulation, deserialize_sample, 
        endpoint_plugin_qos);
    if (result) {
        if (stream->_xTypesState.unassignable) {
            result = RTI_FALSE;
        }
    }

    return result;

}

RTIBool Buffer1024kPlugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream,   
    RTIBool skip_encapsulation,
    RTIBool skip_sample, 
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    RTIBool done = RTI_FALSE;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */

    if(skip_encapsulation) {
        if (!RTICdrStream_skipEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }

    if (skip_sample) {

        {
            RTICdrUnsignedLong sequence_length;
            if (!RTICdrStream_skipPrimitiveSequence(
                stream,
                &sequence_length,
                RTI_CDR_OCTET_TYPE)){
                goto fin; 
            }
        }
        if (!RTICdrStream_skipLong (stream)) {
            goto fin; 
        }
        if (!RTICdrStream_skipLong (stream)) {
            goto fin; 
        }
        if (!RTICdrStream_skipBoolean (stream)) {
            goto fin; 
        }
    }

    done = RTI_TRUE;
  fin:
    if (done != RTI_TRUE && 
    RTICdrStream_getRemainder(stream) >=
    RTI_CDR_PARAMETER_HEADER_ALIGNMENT) {
        return RTI_FALSE;   
    }
    if(skip_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

unsigned int 
Buffer1024kPlugin_get_serialized_sample_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */ 
    if (overflow) {} /* To avoid warnings */

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment +=RTICdrType_getPrimitiveSequenceMaxSizeSerialized(
        current_alignment,(1048576),RTI_CDR_OCTET_TYPE) ;

    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);

    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);

    current_alignment +=RTICdrType_getBooleanMaxSizeSerialized(
        current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return  current_alignment - initial_alignment;
}

unsigned int 
Buffer1024kPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{
    unsigned int size;
    RTIBool overflow = RTI_FALSE;

    size = Buffer1024kPlugin_get_serialized_sample_max_size_ex(
        endpoint_data,&overflow,include_encapsulation,encapsulation_id,current_alignment);

    if (overflow) {
        size = RTI_CDR_MAX_SERIALIZED_SIZE;
    }

    return size;
}

unsigned int 
Buffer1024kPlugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */ 

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment +=    RTICdrType_getPrimitiveSequenceMaxSizeSerialized(
        current_alignment,0, RTI_CDR_OCTET_TYPE);
    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment +=RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment +=RTICdrType_getBooleanMaxSizeSerialized(
        current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return  current_alignment - initial_alignment;
}

/* Returns the size of the sample in its serialized form (in bytes).
* It can also be an estimation in excess of the real buffer needed 
* during a call to the serialize() function.
* The value reported does not have to include the space for the
* encapsulation flags.
*/
unsigned int
Buffer1024kPlugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const Buffer1024k * sample) 
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */ 
    if (sample==NULL) {
        return 0;
    }

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment += RTICdrType_getPrimitiveSequenceSerializedSize(
        current_alignment, 
        DDS_OctetSeq_get_length(&sample->payload),
        RTI_CDR_OCTET_TYPE);
    current_alignment += RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment += RTICdrType_getLongMaxSizeSerialized(
        current_alignment);
    current_alignment += RTICdrType_getBooleanMaxSizeSerialized(
        current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return current_alignment - initial_alignment;
}

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */

PRESTypePluginKeyKind 
Buffer1024kPlugin_get_key_kind(void)
{
    return PRES_TYPEPLUGIN_NO_KEY;
}

RTIBool 
Buffer1024kPlugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const Buffer1024k *sample, 
    struct RTICdrStream *stream,    
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    if(serialize_encapsulation) {
        if (!RTICdrStream_serializeAndSetCdrEncapsulation(stream , encapsulation_id)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }

    if(serialize_key) {

        if (!Buffer1024kPlugin_serialize(
            endpoint_data,
            sample,
            stream,
            RTI_FALSE, encapsulation_id,
            RTI_TRUE,
            endpoint_plugin_qos)) {
            return RTI_FALSE;
        }

    }

    if(serialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

RTIBool Buffer1024kPlugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024k *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    if (endpoint_data) {} /* To avoid warnings */
    if (endpoint_plugin_qos) {} /* To avoid warnings */

    if(deserialize_encapsulation) {

        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }

        position = RTICdrStream_resetAlignment(stream);
    }
    if (deserialize_key) {

        if (!Buffer1024kPlugin_deserialize_sample(
            endpoint_data, sample, stream, 
            RTI_FALSE, RTI_TRUE, 
            endpoint_plugin_qos)) {
            return RTI_FALSE;
        }
    }

    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

RTIBool Buffer1024kPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024k **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos)
{
    RTIBool result;
    if (drop_sample) {} /* To avoid warnings */
    stream->_xTypesState.unassignable = RTI_FALSE;
    result= Buffer1024kPlugin_deserialize_key_sample(
        endpoint_data, (sample != NULL)?*sample:NULL, stream,
        deserialize_encapsulation, deserialize_key, endpoint_plugin_qos);
    if (result) {
        if (stream->_xTypesState.unassignable) {
            result = RTI_FALSE;
        }
    }

    return result;    

}

unsigned int
Buffer1024kPlugin_get_serialized_key_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{

    unsigned int initial_alignment = current_alignment;

    unsigned int encapsulation_size = current_alignment;

    if (endpoint_data) {} /* To avoid warnings */
    if (overflow) {} /* To avoid warnings */

    if (include_encapsulation) {

        if (!RTICdrEncapsulation_validEncapsulationId(encapsulation_id)) {
            return 1;
        }
        RTICdrStream_getEncapsulationSize(encapsulation_size);
        encapsulation_size -= current_alignment;
        current_alignment = 0;
        initial_alignment = 0;
    }

    current_alignment += Buffer1024kPlugin_get_serialized_sample_max_size_ex(
        endpoint_data, overflow,RTI_FALSE, encapsulation_id, current_alignment);

    if (include_encapsulation) {
        current_alignment += encapsulation_size;
    }
    return current_alignment - initial_alignment;
}

unsigned int
Buffer1024kPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment)
{
    unsigned int size;
    RTIBool overflow = RTI_FALSE;

    size = Buffer1024kPlugin_get_serialized_key_max_size_ex(
        endpoint_data,&overflow,include_encapsulation,encapsulation_id,current_alignment);

    if (overflow) {
        size = RTI_CDR_MAX_SERIALIZED_SIZE;
    }

    return size;
}

RTIBool 
Buffer1024kPlugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    Buffer1024k *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos)
{
    char * position = NULL;

    RTIBool done = RTI_FALSE;
    RTIBool error = RTI_FALSE;

    if (stream == NULL) {
        error = RTI_TRUE;
        goto fin;
    }
    if(deserialize_encapsulation) {
        if (!RTICdrStream_deserializeAndSetCdrEncapsulation(stream)) {
            return RTI_FALSE;
        }
        position = RTICdrStream_resetAlignment(stream);
    }

    if (deserialize_key) {

        if (!Buffer1024kPlugin_deserialize_sample(
            endpoint_data, sample, stream, RTI_FALSE, 
            RTI_TRUE, endpoint_plugin_qos)) {
            return RTI_FALSE;
        }

    }

    done = RTI_TRUE;
  fin:
    if(!error) {
        if (done != RTI_TRUE && 
        RTICdrStream_getRemainder(stream) >=
        RTI_CDR_PARAMETER_HEADER_ALIGNMENT) {
            return RTI_FALSE;   
        }
    } else {
        return error;
    }       

    if(deserialize_encapsulation) {
        RTICdrStream_restoreAlignment(stream,position);
    }

    return RTI_TRUE;
}

/* ------------------------------------------------------------------------
* Plug-in Installation Methods
* ------------------------------------------------------------------------ */
struct PRESTypePlugin *Buffer1024kPlugin_new(void) 
{ 
    struct PRESTypePlugin *plugin = NULL;
    const struct PRESTypePluginVersion PLUGIN_VERSION = 
    PRES_TYPE_PLUGIN_VERSION_2_0;

    RTIOsapiHeap_allocateStructure(
        &plugin, struct PRESTypePlugin);
    if (plugin == NULL) {
        return NULL;
    }

    plugin->version = PLUGIN_VERSION;

    /* set up parent's function pointers */
    plugin->onParticipantAttached =
    (PRESTypePluginOnParticipantAttachedCallback)
    Buffer1024kPlugin_on_participant_attached;
    plugin->onParticipantDetached =
    (PRESTypePluginOnParticipantDetachedCallback)
    Buffer1024kPlugin_on_participant_detached;
    plugin->onEndpointAttached =
    (PRESTypePluginOnEndpointAttachedCallback)
    Buffer1024kPlugin_on_endpoint_attached;
    plugin->onEndpointDetached =
    (PRESTypePluginOnEndpointDetachedCallback)
    Buffer1024kPlugin_on_endpoint_detached;

    plugin->copySampleFnc =
    (PRESTypePluginCopySampleFunction)
    Buffer1024kPlugin_copy_sample;
    plugin->createSampleFnc =
    (PRESTypePluginCreateSampleFunction)
    Buffer1024kPlugin_create_sample;
    plugin->destroySampleFnc =
    (PRESTypePluginDestroySampleFunction)
    Buffer1024kPlugin_destroy_sample;

    plugin->serializeFnc =
    (PRESTypePluginSerializeFunction)
    Buffer1024kPlugin_serialize;
    plugin->deserializeFnc =
    (PRESTypePluginDeserializeFunction)
    Buffer1024kPlugin_deserialize;
    plugin->getSerializedSampleMaxSizeFnc =
    (PRESTypePluginGetSerializedSampleMaxSizeFunction)
    Buffer1024kPlugin_get_serialized_sample_max_size;
    plugin->getSerializedSampleMinSizeFnc =
    (PRESTypePluginGetSerializedSampleMinSizeFunction)
    Buffer1024kPlugin_get_serialized_sample_min_size;

    plugin->getSampleFnc =
    (PRESTypePluginGetSampleFunction)
    Buffer1024kPlugin_get_sample;
    plugin->returnSampleFnc =
    (PRESTypePluginReturnSampleFunction)
    Buffer1024kPlugin_return_sample;

    plugin->getKeyKindFnc =
    (PRESTypePluginGetKeyKindFunction)
    Buffer1024kPlugin_get_key_kind;

    /* These functions are only used for keyed types. As this is not a keyed
    type they are all set to NULL
    */
    plugin->serializeKeyFnc = NULL ;    
    plugin->deserializeKeyFnc = NULL;  
    plugin->getKeyFnc = NULL;
    plugin->returnKeyFnc = NULL;
    plugin->instanceToKeyFnc = NULL;
    plugin->keyToInstanceFnc = NULL;
    plugin->getSerializedKeyMaxSizeFnc = NULL;
    plugin->instanceToKeyHashFnc = NULL;
    plugin->serializedSampleToKeyHashFnc = NULL;
    plugin->serializedKeyToKeyHashFnc = NULL;    
    plugin->typeCode =  (struct RTICdrTypeCode *)Buffer1024k_get_typecode();

    plugin->languageKind = PRES_TYPEPLUGIN_CPP_LANG;

    /* Serialized buffer */
    plugin->getBuffer = 
    (PRESTypePluginGetBufferFunction)
    Buffer1024kPlugin_get_buffer;
    plugin->returnBuffer = 
    (PRESTypePluginReturnBufferFunction)
    Buffer1024kPlugin_return_buffer;
    plugin->getSerializedSampleSizeFnc =
    (PRESTypePluginGetSerializedSampleSizeFunction)
    Buffer1024kPlugin_get_serialized_sample_size;

    plugin->endpointTypeName = Buffer1024kTYPENAME;

    return plugin;
}

void
Buffer1024kPlugin_delete(struct PRESTypePlugin *plugin)
{
    RTIOsapiHeap_freeStructure(plugin);
} 

