

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from rti_dds.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#ifndef dds_c_log_impl_h              
#include "dds_c/dds_c_log_impl.h"                                
#endif        

#ifndef cdr_type_h
#include "cdr/cdr_type.h"
#endif    

#ifndef osapi_heap_h
#include "osapi/osapi_heap.h" 
#endif
#else
#include "ndds_standalone_type.h"
#endif

#include "rti_dds.h"

/* ========================================================================= */
const char *Buffer1024TYPENAME = "Buffer1024";

DDS_TypeCode* Buffer1024_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode Buffer1024_g_tc_payload_sequence = DDS_INITIALIZE_SEQUENCE_TYPECODE((1024),NULL);
    static DDS_TypeCode_Member Buffer1024_g_tc_members[4]=
    {

        {
            (char *)"payload",/* Member name */
            {
                0,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"length",/* Member name */
            {
                1,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"msg_id",/* Member name */
            {
                2,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"is_last",/* Member name */
            {
                3,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }
    };

    static DDS_TypeCode Buffer1024_g_tc =
    {{
            DDS_TK_STRUCT,/* Kind */
            DDS_BOOLEAN_FALSE, /* Ignored */
            -1, /*Ignored*/
            (char *)"Buffer1024", /* Name */
            NULL, /* Ignored */      
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            4, /* Number of members */
            Buffer1024_g_tc_members, /* Members */
            DDS_VM_NONE  /* Ignored */         
        }}; /* Type code for Buffer1024*/

    if (is_initialized) {
        return &Buffer1024_g_tc;
    }

    Buffer1024_g_tc_payload_sequence._data._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;

    Buffer1024_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)& Buffer1024_g_tc_payload_sequence;
    Buffer1024_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    Buffer1024_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    Buffer1024_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_boolean;

    is_initialized = RTI_TRUE;

    return &Buffer1024_g_tc;
}

RTIBool Buffer1024_initialize(
    Buffer1024* sample) {
    return Buffer1024_initialize_ex(sample,RTI_TRUE,RTI_TRUE);
}

RTIBool Buffer1024_initialize_ex(
    Buffer1024* sample,RTIBool allocatePointers, RTIBool allocateMemory)
{

    struct DDS_TypeAllocationParams_t allocParams =
    DDS_TYPE_ALLOCATION_PARAMS_DEFAULT;

    allocParams.allocate_pointers =  (DDS_Boolean)allocatePointers;
    allocParams.allocate_memory = (DDS_Boolean)allocateMemory;

    return Buffer1024_initialize_w_params(
        sample,&allocParams);

}

RTIBool Buffer1024_initialize_w_params(
    Buffer1024* sample, const struct DDS_TypeAllocationParams_t * allocParams)
{

    void* buffer = NULL;
    if (buffer) {} /* To avoid warnings */

    if (allocParams) {} /* To avoid warnings */

    if (allocParams->allocate_memory) {
        DDS_OctetSeq_initialize(&sample->payload  );
        if (!DDS_OctetSeq_set_maximum(&sample->payload , (1024))) {
            return RTI_FALSE;
        }
    } else { 
        DDS_OctetSeq_set_length(&sample->payload, 0);
    }

    if (!RTICdrType_initLong(&sample->length)) {
        return RTI_FALSE;
    }     

    if (!RTICdrType_initLong(&sample->msg_id)) {
        return RTI_FALSE;
    }     

    if (!RTICdrType_initBoolean(&sample->is_last)) {
        return RTI_FALSE;
    }     

    return RTI_TRUE;
}

void Buffer1024_finalize(
    Buffer1024* sample)
{

    Buffer1024_finalize_ex(sample,RTI_TRUE);
}

void Buffer1024_finalize_ex(
    Buffer1024* sample,RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParams =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;

    if (sample==NULL) {
        return;
    } 

    deallocParams.delete_pointers = (DDS_Boolean)deletePointers;

    Buffer1024_finalize_w_params(
        sample,&deallocParams);
}

void Buffer1024_finalize_w_params(
    Buffer1024* sample,const struct DDS_TypeDeallocationParams_t * deallocParams)
{

    if (sample==NULL) {
        return;
    }
    if (deallocParams) {} /* To avoid warnings */

    DDS_OctetSeq_finalize(&sample->payload);

}

void Buffer1024_finalize_optional_members(
    Buffer1024* sample, RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParamsTmp =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;
    struct DDS_TypeDeallocationParams_t * deallocParams =
    &deallocParamsTmp;

    if (sample==NULL) {
        return;
    } 
    if (deallocParams) {} /* To avoid warnings */

    deallocParamsTmp.delete_pointers = (DDS_Boolean)deletePointers;
    deallocParamsTmp.delete_optional_members = DDS_BOOLEAN_TRUE;

}

RTIBool Buffer1024_copy(
    Buffer1024* dst,
    const Buffer1024* src)
{

    if (!DDS_OctetSeq_copy(&dst->payload ,
    &src->payload )) {
        return RTI_FALSE;
    }
    if (!RTICdrType_copyLong (
        &dst->length, &src->length)) { 
        return RTI_FALSE;
    }
    if (!RTICdrType_copyLong (
        &dst->msg_id, &src->msg_id)) { 
        return RTI_FALSE;
    }
    if (!RTICdrType_copyBoolean (
        &dst->is_last, &src->is_last)) { 
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'Buffer1024' sequence class.
*/
#define T Buffer1024
#define TSeq Buffer1024Seq
#define T_initialize_w_params Buffer1024_initialize_w_params
#define T_finalize_w_params   Buffer1024_finalize_w_params
#define T_copy       Buffer1024_copy

#ifndef NDDS_STANDALONE_TYPE
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
#else
#include "dds_c_sequence_TSeq.gen"
#include "dds_cpp_sequence_TSeq.gen"
#endif

#undef T_copy
#undef T_finalize_w_params
#undef T_initialize_w_params
#undef TSeq
#undef T

/* ========================================================================= */
const char *Buffer1024kTYPENAME = "Buffer1024k";

DDS_TypeCode* Buffer1024k_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode Buffer1024k_g_tc_payload_sequence = DDS_INITIALIZE_SEQUENCE_TYPECODE((1048576),NULL);
    static DDS_TypeCode_Member Buffer1024k_g_tc_members[4]=
    {

        {
            (char *)"payload",/* Member name */
            {
                0,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"length",/* Member name */
            {
                1,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"msg_id",/* Member name */
            {
                2,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }, 
        {
            (char *)"is_last",/* Member name */
            {
                3,/* Representation ID */          
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL/* Ignored */
        }
    };

    static DDS_TypeCode Buffer1024k_g_tc =
    {{
            DDS_TK_STRUCT,/* Kind */
            DDS_BOOLEAN_FALSE, /* Ignored */
            -1, /*Ignored*/
            (char *)"Buffer1024k", /* Name */
            NULL, /* Ignored */      
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            4, /* Number of members */
            Buffer1024k_g_tc_members, /* Members */
            DDS_VM_NONE  /* Ignored */         
        }}; /* Type code for Buffer1024k*/

    if (is_initialized) {
        return &Buffer1024k_g_tc;
    }

    Buffer1024k_g_tc_payload_sequence._data._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;

    Buffer1024k_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)& Buffer1024k_g_tc_payload_sequence;
    Buffer1024k_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    Buffer1024k_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    Buffer1024k_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_boolean;

    is_initialized = RTI_TRUE;

    return &Buffer1024k_g_tc;
}

RTIBool Buffer1024k_initialize(
    Buffer1024k* sample) {
    return Buffer1024k_initialize_ex(sample,RTI_TRUE,RTI_TRUE);
}

RTIBool Buffer1024k_initialize_ex(
    Buffer1024k* sample,RTIBool allocatePointers, RTIBool allocateMemory)
{

    struct DDS_TypeAllocationParams_t allocParams =
    DDS_TYPE_ALLOCATION_PARAMS_DEFAULT;

    allocParams.allocate_pointers =  (DDS_Boolean)allocatePointers;
    allocParams.allocate_memory = (DDS_Boolean)allocateMemory;

    return Buffer1024k_initialize_w_params(
        sample,&allocParams);

}

RTIBool Buffer1024k_initialize_w_params(
    Buffer1024k* sample, const struct DDS_TypeAllocationParams_t * allocParams)
{

    void* buffer = NULL;
    if (buffer) {} /* To avoid warnings */

    if (allocParams) {} /* To avoid warnings */

    if (allocParams->allocate_memory) {
        DDS_OctetSeq_initialize(&sample->payload  );
        if (!DDS_OctetSeq_set_maximum(&sample->payload , (1048576))) {
            return RTI_FALSE;
        }
    } else { 
        DDS_OctetSeq_set_length(&sample->payload, 0);
    }

    if (!RTICdrType_initLong(&sample->length)) {
        return RTI_FALSE;
    }     

    if (!RTICdrType_initLong(&sample->msg_id)) {
        return RTI_FALSE;
    }     

    if (!RTICdrType_initBoolean(&sample->is_last)) {
        return RTI_FALSE;
    }     

    return RTI_TRUE;
}

void Buffer1024k_finalize(
    Buffer1024k* sample)
{

    Buffer1024k_finalize_ex(sample,RTI_TRUE);
}

void Buffer1024k_finalize_ex(
    Buffer1024k* sample,RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParams =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;

    if (sample==NULL) {
        return;
    } 

    deallocParams.delete_pointers = (DDS_Boolean)deletePointers;

    Buffer1024k_finalize_w_params(
        sample,&deallocParams);
}

void Buffer1024k_finalize_w_params(
    Buffer1024k* sample,const struct DDS_TypeDeallocationParams_t * deallocParams)
{

    if (sample==NULL) {
        return;
    }
    if (deallocParams) {} /* To avoid warnings */

    DDS_OctetSeq_finalize(&sample->payload);

}

void Buffer1024k_finalize_optional_members(
    Buffer1024k* sample, RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParamsTmp =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;
    struct DDS_TypeDeallocationParams_t * deallocParams =
    &deallocParamsTmp;

    if (sample==NULL) {
        return;
    } 
    if (deallocParams) {} /* To avoid warnings */

    deallocParamsTmp.delete_pointers = (DDS_Boolean)deletePointers;
    deallocParamsTmp.delete_optional_members = DDS_BOOLEAN_TRUE;

}

RTIBool Buffer1024k_copy(
    Buffer1024k* dst,
    const Buffer1024k* src)
{

    if (!DDS_OctetSeq_copy(&dst->payload ,
    &src->payload )) {
        return RTI_FALSE;
    }
    if (!RTICdrType_copyLong (
        &dst->length, &src->length)) { 
        return RTI_FALSE;
    }
    if (!RTICdrType_copyLong (
        &dst->msg_id, &src->msg_id)) { 
        return RTI_FALSE;
    }
    if (!RTICdrType_copyBoolean (
        &dst->is_last, &src->is_last)) { 
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'Buffer1024k' sequence class.
*/
#define T Buffer1024k
#define TSeq Buffer1024kSeq
#define T_initialize_w_params Buffer1024k_initialize_w_params
#define T_finalize_w_params   Buffer1024k_finalize_w_params
#define T_copy       Buffer1024k_copy

#ifndef NDDS_STANDALONE_TYPE
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
#else
#include "dds_c_sequence_TSeq.gen"
#include "dds_cpp_sequence_TSeq.gen"
#endif

#undef T_copy
#undef T_finalize_w_params
#undef T_initialize_w_params
#undef TSeq
#undef T

