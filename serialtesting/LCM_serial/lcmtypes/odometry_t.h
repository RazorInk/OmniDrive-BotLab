// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#include <stdint.h>
#include <stdlib.h>
#include <lcm/lcm_coretypes.h>
#include <lcm/lcm.h>

#ifndef _odometry_t_h
#define _odometry_t_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _odometry_t odometry_t;
struct _odometry_t
{
    int64_t    utime;
    float      x;
    float      y;
    float      psi;
    float      v_x;
    float      v_y;
    float      w_z;
};

/**
 * Create a deep copy of a odometry_t.
 * When no longer needed, destroy it with odometry_t_destroy()
 */
odometry_t* odometry_t_copy(const odometry_t* to_copy);

/**
 * Destroy an instance of odometry_t created by odometry_t_copy()
 */
void odometry_t_destroy(odometry_t* to_destroy);

/**
 * Identifies a single subscription.  This is an opaque data type.
 */
typedef struct _odometry_t_subscription_t odometry_t_subscription_t;

/**
 * Prototype for a callback function invoked when a message of type
 * odometry_t is received.
 */
typedef void(*odometry_t_handler_t)(const lcm_recv_buf_t *rbuf,
             const char *channel, const odometry_t *msg, void *userdata);

/**
 * Publish a message of type odometry_t using LCM.
 *
 * @param lcm The LCM instance to publish with.
 * @param channel The channel to publish on.
 * @param msg The message to publish.
 * @return 0 on success, <0 on error.  Success means LCM has transferred
 * responsibility of the message data to the OS.
 */
int odometry_t_publish(lcm_t *lcm, const char *channel, const odometry_t *msg);

/**
 * Subscribe to messages of type odometry_t using LCM.
 *
 * @param lcm The LCM instance to subscribe with.
 * @param channel The channel to subscribe to.
 * @param handler The callback function invoked by LCM when a message is received.
 *                This function is invoked by LCM during calls to lcm_handle() and
 *                lcm_handle_timeout().
 * @param userdata An opaque pointer passed to @p handler when it is invoked.
 * @return 0 on success, <0 if an error occured
 */
odometry_t_subscription_t* odometry_t_subscribe(lcm_t *lcm, const char *channel, odometry_t_handler_t handler, void *userdata);

/**
 * Removes and destroys a subscription created by odometry_t_subscribe()
 */
int odometry_t_unsubscribe(lcm_t *lcm, odometry_t_subscription_t* hid);

/**
 * Sets the queue capacity for a subscription.
 * Some LCM providers (e.g., the default multicast provider) are implemented
 * using a background receive thread that constantly revceives messages from
 * the network.  As these messages are received, they are buffered on
 * per-subscription queues until dispatched by lcm_handle().  This function
 * how many messages are queued before dropping messages.
 *
 * @param subs the subscription to modify.
 * @param num_messages The maximum number of messages to queue
 *  on the subscription.
 * @return 0 on success, <0 if an error occured
 */
int odometry_t_subscription_set_queue_capacity(odometry_t_subscription_t* subs,
                              int num_messages);

/**
 * Encode a message of type odometry_t into binary form.
 *
 * @param buf The output buffer.
 * @param offset Encoding starts at this byte offset into @p buf.
 * @param maxlen Maximum number of bytes to write.  This should generally
 *               be equal to odometry_t_encoded_size().
 * @param msg The message to encode.
 * @return The number of bytes encoded, or <0 if an error occured.
 */
int odometry_t_encode(void *buf, int offset, int maxlen, const odometry_t *p);

/**
 * Decode a message of type odometry_t from binary form.
 * When decoding messages containing strings or variable-length arrays, this
 * function may allocate memory.  When finished with the decoded message,
 * release allocated resources with odometry_t_decode_cleanup().
 *
 * @param buf The buffer containing the encoded message
 * @param offset The byte offset into @p buf where the encoded message starts.
 * @param maxlen The maximum number of bytes to read while decoding.
 * @param msg Output parameter where the decoded message is stored
 * @return The number of bytes decoded, or <0 if an error occured.
 */
int odometry_t_decode(const void *buf, int offset, int maxlen, odometry_t *msg);

/**
 * Release resources allocated by odometry_t_decode()
 * @return 0
 */
int odometry_t_decode_cleanup(odometry_t *p);

/**
 * Check how many bytes are required to encode a message of type odometry_t
 */
int odometry_t_encoded_size(const odometry_t *p);
size_t odometry_t_struct_size(void);
int  odometry_t_num_fields(void);
int  odometry_t_get_field(const odometry_t *p, int i, lcm_field_t *f);
const lcm_type_info_t *odometry_t_get_type_info(void);

// LCM support functions. Users should not call these
int64_t __odometry_t_get_hash(void);
uint64_t __odometry_t_hash_recursive(const __lcm_hash_ptr *p);
int     __odometry_t_encode_array(void *buf, int offset, int maxlen, const odometry_t *p, int elements);
int     __odometry_t_decode_array(const void *buf, int offset, int maxlen, odometry_t *p, int elements);
int     __odometry_t_decode_array_cleanup(odometry_t *p, int elements);
int     __odometry_t_encoded_array_size(const odometry_t *p, int elements);
int     __odometry_t_clone_array(const odometry_t *p, odometry_t *q, int elements);

#ifdef __cplusplus
}
#endif

#endif
