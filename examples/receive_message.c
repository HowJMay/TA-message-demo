#include <stdio.h>

#include "client/recv_client.h"
#define HOST "node.deviceproof.org"
#define PORT 14265

#define BUNDLE_HASH                                                            \
  "IFQBPDGIKFTRNKCG9SDEOVNZFDGGKVYQHILFPOFFJHNWKRRH9ESTIKYZNXMJFINPHVBPBSAEC9" \
  "L9DHKIY"
#define CHID                                                                   \
  "SEVXZWWNNPAOXJFRNZVQIZMYOPYYRZHSOKXZZKYYICDAGXFPAIMRKTRVUTESTFUQRKYBNAXIKG" \
  "ODJBUBW"
#define EPID                                                                   \
  "WST9LUPLZMCWYWXJVGLNGDLRA9WQDMS9UCJQSORFDQJXRSKAVCASNJTEDBVQBXPHLCXZUDUQYF" \
  "9WDNSAU"

int main(int ac, char **av) {
  int ret = EXIT_SUCCESS;

  tryte_t bundle_hash[NUM_TRYTES_ADDRESS];
  tryte_t chid[NUM_TRYTES_ADDRESS];
  tryte_t epid[NUM_TRYTES_ADDRESS];

  init_mam_recv_objs();
  gen_keypair(NTRU_NONCE);

  printf("Input bundle hash:");
  fgets((char *)bundle_hash, NUM_TRYTES_ADDRESS + 2, stdin);
  printf("Input chid:");
  fgets((char *)chid, NUM_TRYTES_ADDRESS + 2, stdin);
  printf("Input epid:");
  fgets((char *)epid, NUM_TRYTES_ADDRESS + 2, stdin);

  ret = extra_recv_msg_chid(HOST, PORT, (tryte_t *)CHID, (tryte_t *)EPID);
  if (ret) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
