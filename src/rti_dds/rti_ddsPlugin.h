

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from rti_dds.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef rti_ddsPlugin_1229006091_h
#define rti_ddsPlugin_1229006091_h

#include "rti_dds.h"

struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

extern "C" {

    #define Buffer1024Plugin_get_sample PRESTypePluginDefaultEndpointData_getSample 
    #define Buffer1024Plugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
    #define Buffer1024Plugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 

    #define Buffer1024Plugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
    #define Buffer1024Plugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

    /* --------------------------------------------------------------------------------------
    Support functions:
    * -------------------------------------------------------------------------------------- */

    NDDSUSERDllExport extern Buffer1024*
    Buffer1024PluginSupport_create_data_w_params(
        const struct DDS_TypeAllocationParams_t * alloc_params);

    NDDSUSERDllExport extern Buffer1024*
    Buffer1024PluginSupport_create_data_ex(RTIBool allocate_pointers);

    NDDSUSERDllExport extern Buffer1024*
    Buffer1024PluginSupport_create_data(void);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024PluginSupport_copy_data(
        Buffer1024 *out,
        const Buffer1024 *in);

    NDDSUSERDllExport extern void 
    Buffer1024PluginSupport_destroy_data_w_params(
        Buffer1024 *sample,
        const struct DDS_TypeDeallocationParams_t * dealloc_params);

    NDDSUSERDllExport extern void 
    Buffer1024PluginSupport_destroy_data_ex(
        Buffer1024 *sample,RTIBool deallocate_pointers);

    NDDSUSERDllExport extern void 
    Buffer1024PluginSupport_destroy_data(
        Buffer1024 *sample);

    NDDSUSERDllExport extern void 
    Buffer1024PluginSupport_print_data(
        const Buffer1024 *sample,
        const char *desc,
        unsigned int indent);

    /* ----------------------------------------------------------------------------
    Callback functions:
    * ---------------------------------------------------------------------------- */

    NDDSUSERDllExport extern PRESTypePluginParticipantData 
    Buffer1024Plugin_on_participant_attached(
        void *registration_data, 
        const struct PRESTypePluginParticipantInfo *participant_info,
        RTIBool top_level_registration, 
        void *container_plugin_context,
        RTICdrTypeCode *typeCode);

    NDDSUSERDllExport extern void 
    Buffer1024Plugin_on_participant_detached(
        PRESTypePluginParticipantData participant_data);

    NDDSUSERDllExport extern PRESTypePluginEndpointData 
    Buffer1024Plugin_on_endpoint_attached(
        PRESTypePluginParticipantData participant_data,
        const struct PRESTypePluginEndpointInfo *endpoint_info,
        RTIBool top_level_registration, 
        void *container_plugin_context);

    NDDSUSERDllExport extern void 
    Buffer1024Plugin_on_endpoint_detached(
        PRESTypePluginEndpointData endpoint_data);

    NDDSUSERDllExport extern void    
    Buffer1024Plugin_return_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024 *sample,
        void *handle);    

    NDDSUSERDllExport extern RTIBool 
    Buffer1024Plugin_copy_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024 *out,
        const Buffer1024 *in);

    /* ----------------------------------------------------------------------------
    (De)Serialize functions:
    * ------------------------------------------------------------------------- */

    NDDSUSERDllExport extern RTIBool 
    Buffer1024Plugin_serialize(
        PRESTypePluginEndpointData endpoint_data,
        const Buffer1024 *sample,
        struct RTICdrStream *stream, 
        RTIBool serialize_encapsulation,
        RTIEncapsulationId encapsulation_id,
        RTIBool serialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024Plugin_deserialize_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024 *sample, 
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    Buffer1024Plugin_serialize_to_cdr_buffer(
        char * buffer,
        unsigned int * length,
        const Buffer1024 *sample); 

    NDDSUSERDllExport extern RTIBool 
    Buffer1024Plugin_deserialize(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024 **sample, 
        RTIBool * drop_sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    Buffer1024Plugin_deserialize_from_cdr_buffer(
        Buffer1024 *sample,
        const char * buffer,
        unsigned int length);    

    NDDSUSERDllExport extern RTIBool
    Buffer1024Plugin_skip(
        PRESTypePluginEndpointData endpoint_data,
        struct RTICdrStream *stream, 
        RTIBool skip_encapsulation,  
        RTIBool skip_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024Plugin_get_serialized_sample_max_size_ex(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool * overflow,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);    

    NDDSUSERDllExport extern unsigned int 
    Buffer1024Plugin_get_serialized_sample_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024Plugin_get_serialized_sample_min_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int
    Buffer1024Plugin_get_serialized_sample_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment,
        const Buffer1024 * sample);

    /* --------------------------------------------------------------------------------------
    Key Management functions:
    * -------------------------------------------------------------------------------------- */
    NDDSUSERDllExport extern PRESTypePluginKeyKind 
    Buffer1024Plugin_get_key_kind(void);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024Plugin_get_serialized_key_max_size_ex(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool * overflow,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024Plugin_get_serialized_key_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024Plugin_serialize_key(
        PRESTypePluginEndpointData endpoint_data,
        const Buffer1024 *sample,
        struct RTICdrStream *stream,
        RTIBool serialize_encapsulation,
        RTIEncapsulationId encapsulation_id,
        RTIBool serialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024Plugin_deserialize_key_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024 * sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024Plugin_deserialize_key(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024 ** sample,
        RTIBool * drop_sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    Buffer1024Plugin_serialized_sample_to_key(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024 *sample,
        struct RTICdrStream *stream, 
        RTIBool deserialize_encapsulation,  
        RTIBool deserialize_key, 
        void *endpoint_plugin_qos);

    /* Plugin Functions */
    NDDSUSERDllExport extern struct PRESTypePlugin*
    Buffer1024Plugin_new(void);

    NDDSUSERDllExport extern void
    Buffer1024Plugin_delete(struct PRESTypePlugin *);

    #define Buffer1024kPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample 
    #define Buffer1024kPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
    #define Buffer1024kPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 

    #define Buffer1024kPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
    #define Buffer1024kPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

    /* --------------------------------------------------------------------------------------
    Support functions:
    * -------------------------------------------------------------------------------------- */

    NDDSUSERDllExport extern Buffer1024k*
    Buffer1024kPluginSupport_create_data_w_params(
        const struct DDS_TypeAllocationParams_t * alloc_params);

    NDDSUSERDllExport extern Buffer1024k*
    Buffer1024kPluginSupport_create_data_ex(RTIBool allocate_pointers);

    NDDSUSERDllExport extern Buffer1024k*
    Buffer1024kPluginSupport_create_data(void);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPluginSupport_copy_data(
        Buffer1024k *out,
        const Buffer1024k *in);

    NDDSUSERDllExport extern void 
    Buffer1024kPluginSupport_destroy_data_w_params(
        Buffer1024k *sample,
        const struct DDS_TypeDeallocationParams_t * dealloc_params);

    NDDSUSERDllExport extern void 
    Buffer1024kPluginSupport_destroy_data_ex(
        Buffer1024k *sample,RTIBool deallocate_pointers);

    NDDSUSERDllExport extern void 
    Buffer1024kPluginSupport_destroy_data(
        Buffer1024k *sample);

    NDDSUSERDllExport extern void 
    Buffer1024kPluginSupport_print_data(
        const Buffer1024k *sample,
        const char *desc,
        unsigned int indent);

    /* ----------------------------------------------------------------------------
    Callback functions:
    * ---------------------------------------------------------------------------- */

    NDDSUSERDllExport extern PRESTypePluginParticipantData 
    Buffer1024kPlugin_on_participant_attached(
        void *registration_data, 
        const struct PRESTypePluginParticipantInfo *participant_info,
        RTIBool top_level_registration, 
        void *container_plugin_context,
        RTICdrTypeCode *typeCode);

    NDDSUSERDllExport extern void 
    Buffer1024kPlugin_on_participant_detached(
        PRESTypePluginParticipantData participant_data);

    NDDSUSERDllExport extern PRESTypePluginEndpointData 
    Buffer1024kPlugin_on_endpoint_attached(
        PRESTypePluginParticipantData participant_data,
        const struct PRESTypePluginEndpointInfo *endpoint_info,
        RTIBool top_level_registration, 
        void *container_plugin_context);

    NDDSUSERDllExport extern void 
    Buffer1024kPlugin_on_endpoint_detached(
        PRESTypePluginEndpointData endpoint_data);

    NDDSUSERDllExport extern void    
    Buffer1024kPlugin_return_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024k *sample,
        void *handle);    

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPlugin_copy_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024k *out,
        const Buffer1024k *in);

    /* ----------------------------------------------------------------------------
    (De)Serialize functions:
    * ------------------------------------------------------------------------- */

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPlugin_serialize(
        PRESTypePluginEndpointData endpoint_data,
        const Buffer1024k *sample,
        struct RTICdrStream *stream, 
        RTIBool serialize_encapsulation,
        RTIEncapsulationId encapsulation_id,
        RTIBool serialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPlugin_deserialize_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024k *sample, 
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    Buffer1024kPlugin_serialize_to_cdr_buffer(
        char * buffer,
        unsigned int * length,
        const Buffer1024k *sample); 

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPlugin_deserialize(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024k **sample, 
        RTIBool * drop_sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    Buffer1024kPlugin_deserialize_from_cdr_buffer(
        Buffer1024k *sample,
        const char * buffer,
        unsigned int length);    

    NDDSUSERDllExport extern RTIBool
    Buffer1024kPlugin_skip(
        PRESTypePluginEndpointData endpoint_data,
        struct RTICdrStream *stream, 
        RTIBool skip_encapsulation,  
        RTIBool skip_sample, 
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024kPlugin_get_serialized_sample_max_size_ex(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool * overflow,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);    

    NDDSUSERDllExport extern unsigned int 
    Buffer1024kPlugin_get_serialized_sample_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024kPlugin_get_serialized_sample_min_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int
    Buffer1024kPlugin_get_serialized_sample_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment,
        const Buffer1024k * sample);

    /* --------------------------------------------------------------------------------------
    Key Management functions:
    * -------------------------------------------------------------------------------------- */
    NDDSUSERDllExport extern PRESTypePluginKeyKind 
    Buffer1024kPlugin_get_key_kind(void);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024kPlugin_get_serialized_key_max_size_ex(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool * overflow,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int 
    Buffer1024kPlugin_get_serialized_key_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPlugin_serialize_key(
        PRESTypePluginEndpointData endpoint_data,
        const Buffer1024k *sample,
        struct RTICdrStream *stream,
        RTIBool serialize_encapsulation,
        RTIEncapsulationId encapsulation_id,
        RTIBool serialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPlugin_deserialize_key_sample(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024k * sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    Buffer1024kPlugin_deserialize_key(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024k ** sample,
        RTIBool * drop_sample,
        struct RTICdrStream *stream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool
    Buffer1024kPlugin_serialized_sample_to_key(
        PRESTypePluginEndpointData endpoint_data,
        Buffer1024k *sample,
        struct RTICdrStream *stream, 
        RTIBool deserialize_encapsulation,  
        RTIBool deserialize_key, 
        void *endpoint_plugin_qos);

    /* Plugin Functions */
    NDDSUSERDllExport extern struct PRESTypePlugin*
    Buffer1024kPlugin_new(void);

    NDDSUSERDllExport extern void
    Buffer1024kPlugin_delete(struct PRESTypePlugin *);

}

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* rti_ddsPlugin_1229006091_h */

