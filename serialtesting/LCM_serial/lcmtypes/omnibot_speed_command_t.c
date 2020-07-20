// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#include <string.h>
#include "omnibot_speed_command_t.h"

static int __omnibot_speed_command_t_hash_computed;
static uint64_t __omnibot_speed_command_t_hash;

uint64_t __omnibot_speed_command_t_hash_recursive(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for (fp = p; fp != NULL; fp = fp->parent)
        if (fp->v == __omnibot_speed_command_t_get_hash)
            return 0;

    __lcm_hash_ptr cp;
    cp.parent =  p;
    cp.v = (void*)__omnibot_speed_command_t_get_hash;
    (void) cp;

    uint64_t hash = (uint64_t)0x3c5b5136cf7b636aLL
         + __int64_t_hash_recursive(&cp)
         + __float_hash_recursive(&cp)
         + __float_hash_recursive(&cp)
         + __float_hash_recursive(&cp)
        ;

    return (hash<<1) + ((hash>>63)&1);
}

int64_t __omnibot_speed_command_t_get_hash(void)
{
    if (!__omnibot_speed_command_t_hash_computed) {
        __omnibot_speed_command_t_hash = (int64_t)__omnibot_speed_command_t_hash_recursive(NULL);
        __omnibot_speed_command_t_hash_computed = 1;
    }

    return __omnibot_speed_command_t_hash;
}

int __omnibot_speed_command_t_encode_array(void *buf, int offset, int maxlen, const omnibot_speed_command_t *p, int elements)
{
    int pos = 0, element;
    int thislen;

    for (element = 0; element < elements; element++) {

        thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].timestamp), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __float_encode_array(buf, offset + pos, maxlen - pos, &(p[element].v_x), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __float_encode_array(buf, offset + pos, maxlen - pos, &(p[element].v_y), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __float_encode_array(buf, offset + pos, maxlen - pos, &(p[element].w_z), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int omnibot_speed_command_t_encode(void *buf, int offset, int maxlen, const omnibot_speed_command_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __omnibot_speed_command_t_get_hash();

    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    thislen = __omnibot_speed_command_t_encode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int __omnibot_speed_command_t_encoded_array_size(const omnibot_speed_command_t *p, int elements)
{
    int size = 0, element;
    for (element = 0; element < elements; element++) {

        size += __int64_t_encoded_array_size(&(p[element].timestamp), 1);

        size += __float_encoded_array_size(&(p[element].v_x), 1);

        size += __float_encoded_array_size(&(p[element].v_y), 1);

        size += __float_encoded_array_size(&(p[element].w_z), 1);

    }
    return size;
}

int omnibot_speed_command_t_encoded_size(const omnibot_speed_command_t *p)
{
    return 8 + __omnibot_speed_command_t_encoded_array_size(p, 1);
}

size_t omnibot_speed_command_t_struct_size(void)
{
    return sizeof(omnibot_speed_command_t);
}

int omnibot_speed_command_t_num_fields(void)
{
    return 4;
}

int omnibot_speed_command_t_get_field(const omnibot_speed_command_t *p, int i, lcm_field_t *f)
{
    if (0 > i || i >= omnibot_speed_command_t_num_fields())
        return 1;
    
    switch (i) {
    
        case 0: {
            f->name = "timestamp";
            f->type = LCM_FIELD_INT64_T;
            f->typestr = "int64_t";
            f->num_dim = 0;
            f->data = (void *) &p->timestamp;
            return 0;
        }
        
        case 1: {
            f->name = "v_x";
            f->type = LCM_FIELD_FLOAT;
            f->typestr = "float";
            f->num_dim = 0;
            f->data = (void *) &p->v_x;
            return 0;
        }
        
        case 2: {
            f->name = "v_y";
            f->type = LCM_FIELD_FLOAT;
            f->typestr = "float";
            f->num_dim = 0;
            f->data = (void *) &p->v_y;
            return 0;
        }
        
        case 3: {
            f->name = "w_z";
            f->type = LCM_FIELD_FLOAT;
            f->typestr = "float";
            f->num_dim = 0;
            f->data = (void *) &p->w_z;
            return 0;
        }
        
        default:
            return 1;
    }
}

const lcm_type_info_t *omnibot_speed_command_t_get_type_info(void)
{
    static int init = 0;
    static lcm_type_info_t typeinfo;
    if (!init) {
        typeinfo.encode         = (lcm_encode_t) omnibot_speed_command_t_encode;
        typeinfo.decode         = (lcm_decode_t) omnibot_speed_command_t_decode;
        typeinfo.decode_cleanup = (lcm_decode_cleanup_t) omnibot_speed_command_t_decode_cleanup;
        typeinfo.encoded_size   = (lcm_encoded_size_t) omnibot_speed_command_t_encoded_size;
        typeinfo.struct_size    = (lcm_struct_size_t)  omnibot_speed_command_t_struct_size;
        typeinfo.num_fields     = (lcm_num_fields_t) omnibot_speed_command_t_num_fields;
        typeinfo.get_field      = (lcm_get_field_t) omnibot_speed_command_t_get_field;
        typeinfo.get_hash       = (lcm_get_hash_t) __omnibot_speed_command_t_get_hash;
    }
    
    return &typeinfo;
}
int __omnibot_speed_command_t_decode_array(const void *buf, int offset, int maxlen, omnibot_speed_command_t *p, int elements)
{
    int pos = 0, thislen, element;

    for (element = 0; element < elements; element++) {

        thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].timestamp), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __float_decode_array(buf, offset + pos, maxlen - pos, &(p[element].v_x), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __float_decode_array(buf, offset + pos, maxlen - pos, &(p[element].v_y), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __float_decode_array(buf, offset + pos, maxlen - pos, &(p[element].w_z), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int __omnibot_speed_command_t_decode_array_cleanup(omnibot_speed_command_t *p, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __int64_t_decode_array_cleanup(&(p[element].timestamp), 1);

        __float_decode_array_cleanup(&(p[element].v_x), 1);

        __float_decode_array_cleanup(&(p[element].v_y), 1);

        __float_decode_array_cleanup(&(p[element].w_z), 1);

    }
    return 0;
}

int omnibot_speed_command_t_decode(const void *buf, int offset, int maxlen, omnibot_speed_command_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __omnibot_speed_command_t_get_hash();

    int64_t this_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (this_hash != hash) return -1;

    thislen = __omnibot_speed_command_t_decode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int omnibot_speed_command_t_decode_cleanup(omnibot_speed_command_t *p)
{
    return __omnibot_speed_command_t_decode_array_cleanup(p, 1);
}

int __omnibot_speed_command_t_clone_array(const omnibot_speed_command_t *p, omnibot_speed_command_t *q, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __int64_t_clone_array(&(p[element].timestamp), &(q[element].timestamp), 1);

        __float_clone_array(&(p[element].v_x), &(q[element].v_x), 1);

        __float_clone_array(&(p[element].v_y), &(q[element].v_y), 1);

        __float_clone_array(&(p[element].w_z), &(q[element].w_z), 1);

    }
    return 0;
}

omnibot_speed_command_t *omnibot_speed_command_t_copy(const omnibot_speed_command_t *p)
{
    omnibot_speed_command_t *q = (omnibot_speed_command_t*) malloc(sizeof(omnibot_speed_command_t));
    __omnibot_speed_command_t_clone_array(p, q, 1);
    return q;
}

void omnibot_speed_command_t_destroy(omnibot_speed_command_t *p)
{
    __omnibot_speed_command_t_decode_array_cleanup(p, 1);
    free(p);
}

int omnibot_speed_command_t_publish(lcm_t *lc, const char *channel, const omnibot_speed_command_t *p)
{
      int max_data_size = omnibot_speed_command_t_encoded_size (p);
      uint8_t *buf = (uint8_t*) malloc (max_data_size);
      if (!buf) return -1;
      int data_size = omnibot_speed_command_t_encode (buf, 0, max_data_size, p);
      if (data_size < 0) {
          free (buf);
          return data_size;
      }
      int status = lcm_publish (lc, channel, buf, data_size);
      free (buf);
      return status;
}

struct _omnibot_speed_command_t_subscription_t {
    omnibot_speed_command_t_handler_t user_handler;
    void *userdata;
    lcm_subscription_t *lc_h;
};
static
void omnibot_speed_command_t_handler_stub (const lcm_recv_buf_t *rbuf,
                            const char *channel, void *userdata)
{
    int status;
    omnibot_speed_command_t p;
    memset(&p, 0, sizeof(omnibot_speed_command_t));
    status = omnibot_speed_command_t_decode (rbuf->data, 0, rbuf->data_size, &p);
    if (status < 0) {
        fprintf (stderr, "error %d decoding omnibot_speed_command_t!!!\n", status);
        return;
    }

    omnibot_speed_command_t_subscription_t *h = (omnibot_speed_command_t_subscription_t*) userdata;
    h->user_handler (rbuf, channel, &p, h->userdata);

    omnibot_speed_command_t_decode_cleanup (&p);
}

omnibot_speed_command_t_subscription_t* omnibot_speed_command_t_subscribe (lcm_t *lcm,
                    const char *channel,
                    omnibot_speed_command_t_handler_t f, void *userdata)
{
    omnibot_speed_command_t_subscription_t *n = (omnibot_speed_command_t_subscription_t*)
                       malloc(sizeof(omnibot_speed_command_t_subscription_t));
    n->user_handler = f;
    n->userdata = userdata;
    n->lc_h = lcm_subscribe (lcm, channel,
                                 omnibot_speed_command_t_handler_stub, n);
    if (n->lc_h == NULL) {
        fprintf (stderr,"couldn't reg omnibot_speed_command_t LCM handler!\n");
        free (n);
        return NULL;
    }
    return n;
}

int omnibot_speed_command_t_subscription_set_queue_capacity (omnibot_speed_command_t_subscription_t* subs,
                              int num_messages)
{
    return lcm_subscription_set_queue_capacity (subs->lc_h, num_messages);
}

int omnibot_speed_command_t_unsubscribe(lcm_t *lcm, omnibot_speed_command_t_subscription_t* hid)
{
    int status = lcm_unsubscribe (lcm, hid->lc_h);
    if (0 != status) {
        fprintf(stderr,
           "couldn't unsubscribe omnibot_speed_command_t_handler %p!\n", hid);
        return -1;
    }
    free (hid);
    return 0;
}

