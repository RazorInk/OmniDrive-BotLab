/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#ifndef __particle_t_hpp__
#define __particle_t_hpp__

#include <lcm/lcm_coretypes.h>

#include "pose_xyt_t.hpp"
#include "pose_xyt_t.hpp"


/**
 * particle_t represents a single particle in the particle filter. A particle is represented
 * as the estimated pose, the pose of the parent sample, i.e. the pose the action model was
 * applied to, and the weight as computed by the sensor model.
 */
class particle_t
{
    public:
        pose_xyt_t pose;

        /// (x,y,theta) pose estimate
        pose_xyt_t parent_pose;

        /// (x,y,theta) of the prior pose the new estimate came from
        double     weight;

    public:
        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline int encode(void *buf, int offset, int maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline int getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to read while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline int decode(const void *buf, int offset, int maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        /**
         * Returns "particle_t"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int particle_t::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int particle_t::decode(const void *buf, int offset, int maxlen)
{
    int pos = 0, thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int particle_t::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t particle_t::getHash()
{
    static int64_t hash = static_cast<int64_t>(_computeHash(NULL));
    return hash;
}

const char* particle_t::getTypeName()
{
    return "particle_t";
}

int particle_t::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = this->pose._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->parent_pose._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->weight, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int particle_t::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = this->pose._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->parent_pose._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->weight, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int particle_t::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->pose._getEncodedSizeNoHash();
    enc_size += this->parent_pose._getEncodedSizeNoHash();
    enc_size += __double_encoded_array_size(NULL, 1);
    return enc_size;
}

uint64_t particle_t::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == particle_t::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, particle_t::getHash };

    uint64_t hash = 0x18016676f01f14e8LL +
         pose_xyt_t::_computeHash(&cp) +
         pose_xyt_t::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

#endif
