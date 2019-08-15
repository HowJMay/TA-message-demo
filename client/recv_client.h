#ifndef RECV_CLIENT_H_
#define RECV_CLIENT_H_

#include "core/core.h"

retcode_t init_mam_recv_objs();
retcode_t gen_keypair(char *nonce_str);
retcode_t extra_recv_msg(char *host, int port, tryte_t *bundle_hash,
                         tryte_t *chid, tryte_t *epid);
// TODO: record printed messages. Don't print a message more than once.
retcode_t extra_recv_msg_chid(char *host, int port, tryte_t *chid,
                              tryte_t *epid);

#endif  // RECV_CLIENT_H_