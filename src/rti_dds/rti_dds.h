

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from rti_dds.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef rti_dds_1229006091_h
#define rti_dds_1229006091_h

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#else
#include "ndds_standalone_type.h"
#endif

extern "C" {

    extern const char *Buffer1024TYPENAME;

}

struct Buffer1024Seq;
#ifndef NDDS_STANDALONE_TYPE
class Buffer1024TypeSupport;
class Buffer1024DataWriter;
class Buffer1024DataReader;
#endif

class Buffer1024 
{
  public:
    typedef struct Buffer1024Seq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef Buffer1024TypeSupport TypeSupport;
    typedef Buffer1024DataWriter DataWriter;
    typedef Buffer1024DataReader DataReader;
    #endif

    DDS_OctetSeq  payload ;
    DDS_Long   length ;
    DDS_Long   msg_id ;
    DDS_Boolean   is_last ;

};
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

NDDSUSERDllExport DDS_TypeCode* Buffer1024_get_typecode(void); /* Type code */

DDS_SEQUENCE(Buffer1024Seq, Buffer1024);                                        

NDDSUSERDllExport
RTIBool Buffer1024_initialize(
    Buffer1024* self);

NDDSUSERDllExport
RTIBool Buffer1024_initialize_ex(
    Buffer1024* self,RTIBool allocatePointers,RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool Buffer1024_initialize_w_params(
    Buffer1024* self,
    const struct DDS_TypeAllocationParams_t * allocParams);        

NDDSUSERDllExport
void Buffer1024_finalize(
    Buffer1024* self);

NDDSUSERDllExport
void Buffer1024_finalize_ex(
    Buffer1024* self,RTIBool deletePointers);

NDDSUSERDllExport
void Buffer1024_finalize_w_params(
    Buffer1024* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void Buffer1024_finalize_optional_members(
    Buffer1024* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool Buffer1024_copy(
    Buffer1024* dst,
    const Buffer1024* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif
extern "C" {

    extern const char *Buffer1024kTYPENAME;

}

struct Buffer1024kSeq;
#ifndef NDDS_STANDALONE_TYPE
class Buffer1024kTypeSupport;
class Buffer1024kDataWriter;
class Buffer1024kDataReader;
#endif

class Buffer1024k 
{
  public:
    typedef struct Buffer1024kSeq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef Buffer1024kTypeSupport TypeSupport;
    typedef Buffer1024kDataWriter DataWriter;
    typedef Buffer1024kDataReader DataReader;
    #endif

    DDS_OctetSeq  payload ;
    DDS_Long   length ;
    DDS_Long   msg_id ;
    DDS_Boolean   is_last ;

};
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

NDDSUSERDllExport DDS_TypeCode* Buffer1024k_get_typecode(void); /* Type code */

DDS_SEQUENCE(Buffer1024kSeq, Buffer1024k);                                        

NDDSUSERDllExport
RTIBool Buffer1024k_initialize(
    Buffer1024k* self);

NDDSUSERDllExport
RTIBool Buffer1024k_initialize_ex(
    Buffer1024k* self,RTIBool allocatePointers,RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool Buffer1024k_initialize_w_params(
    Buffer1024k* self,
    const struct DDS_TypeAllocationParams_t * allocParams);        

NDDSUSERDllExport
void Buffer1024k_finalize(
    Buffer1024k* self);

NDDSUSERDllExport
void Buffer1024k_finalize_ex(
    Buffer1024k* self,RTIBool deletePointers);

NDDSUSERDllExport
void Buffer1024k_finalize_w_params(
    Buffer1024k* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void Buffer1024k_finalize_optional_members(
    Buffer1024k* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool Buffer1024k_copy(
    Buffer1024k* dst,
    const Buffer1024k* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* rti_dds */

