/*
 * Generated by util/mkerr.pl DO NOT EDIT
 * Copyright 1995-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OPENSSL_EVPERR_H
# define OPENSSL_EVPERR_H
# pragma once

# include <openssl/opensslconf.h>
# include <openssl/symhacks.h>
# include <openssl/cryptoerr_legacy.h>



/*
 * EVP reason codes.
 */
# define EVP_R_AES_KEY_SETUP_FAILED                       143
# define EVP_R_ARIA_KEY_SETUP_FAILED                      176
# define EVP_R_BAD_ALGORITHM_NAME                         200
# define EVP_R_BAD_DECRYPT                                100
# define EVP_R_BAD_KEY_LENGTH                             195
# define EVP_R_BUFFER_TOO_SMALL                           155
# define EVP_R_CACHE_CONSTANTS_FAILED                     225
# define EVP_R_CAMELLIA_KEY_SETUP_FAILED                  157
# define EVP_R_CANNOT_GET_PARAMETERS                      197
# define EVP_R_CANNOT_SET_PARAMETERS                      198
# define EVP_R_CIPHER_NOT_GCM_MODE                        184
# define EVP_R_CIPHER_PARAMETER_ERROR                     122
# define EVP_R_COMMAND_NOT_SUPPORTED                      147
# define EVP_R_CONFLICTING_ALGORITHM_NAME                 201
# define EVP_R_COPY_ERROR                                 173
# define EVP_R_CTRL_NOT_IMPLEMENTED                       132
# define EVP_R_CTRL_OPERATION_NOT_IMPLEMENTED             133
# define EVP_R_DATA_NOT_MULTIPLE_OF_BLOCK_LENGTH          138
# define EVP_R_DECODE_ERROR                               114
# define EVP_R_DEFAULT_QUERY_PARSE_ERROR                  210
# define EVP_R_DIFFERENT_KEY_TYPES                        101
# define EVP_R_DIFFERENT_PARAMETERS                       153
# define EVP_R_ERROR_LOADING_SECTION                      165
# define EVP_R_EXPECTING_AN_HMAC_KEY                      174
# define EVP_R_EXPECTING_AN_RSA_KEY                       127
# define EVP_R_EXPECTING_A_DH_KEY                         128
# define EVP_R_EXPECTING_A_DSA_KEY                        129
# define EVP_R_EXPECTING_A_ECX_KEY                        219
# define EVP_R_EXPECTING_A_EC_KEY                         142
# define EVP_R_EXPECTING_A_POLY1305_KEY                   164
# define EVP_R_EXPECTING_A_SIPHASH_KEY                    175
# define EVP_R_FINAL_ERROR                                188
# define EVP_R_GENERATE_ERROR                             214
# define EVP_R_GET_RAW_KEY_FAILED                         182
# define EVP_R_ILLEGAL_SCRYPT_PARAMETERS                  171
# define EVP_R_INACCESSIBLE_DOMAIN_PARAMETERS             204
# define EVP_R_INACCESSIBLE_KEY                           203
# define EVP_R_INITIALIZATION_ERROR                       134
# define EVP_R_INPUT_NOT_INITIALIZED                      111
# define EVP_R_INVALID_CUSTOM_LENGTH                      185
# define EVP_R_INVALID_DIGEST                             152
# define EVP_R_INVALID_IV_LENGTH                          194
# define EVP_R_INVALID_KEY                                163
# define EVP_R_INVALID_KEY_LENGTH                         130
# define EVP_R_INVALID_LENGTH                             221
# define EVP_R_INVALID_NULL_ALGORITHM                     218
# define EVP_R_INVALID_OPERATION                          148
# define EVP_R_INVALID_PROVIDER_FUNCTIONS                 193
# define EVP_R_INVALID_SALT_LENGTH                        186
# define EVP_R_INVALID_SECRET_LENGTH                      223
# define EVP_R_INVALID_SEED_LENGTH                        220
# define EVP_R_INVALID_VALUE                              222
# define EVP_R_KEYMGMT_EXPORT_FAILURE                     205
# define EVP_R_KEY_SETUP_FAILED                           180
# define EVP_R_LOCKING_NOT_SUPPORTED                      213
# define EVP_R_MEMORY_LIMIT_EXCEEDED                      172
# define EVP_R_MESSAGE_DIGEST_IS_NULL                     159
# define EVP_R_METHOD_NOT_SUPPORTED                       144
# define EVP_R_MISSING_PARAMETERS                         103
# define EVP_R_NOT_ABLE_TO_COPY_CTX                       190
# define EVP_R_NOT_XOF_OR_INVALID_LENGTH                  178
# define EVP_R_NO_CIPHER_SET                              131
# define EVP_R_NO_DEFAULT_DIGEST                          158
# define EVP_R_NO_DIGEST_SET                              139
# define EVP_R_NO_IMPORT_FUNCTION                         206
# define EVP_R_NO_KEYMGMT_AVAILABLE                       199
# define EVP_R_NO_KEYMGMT_PRESENT                         196
# define EVP_R_NO_KEY_SET                                 154
# define EVP_R_NO_OPERATION_SET                           149
# define EVP_R_NULL_MAC_PKEY_CTX                          208
# define EVP_R_ONLY_ONESHOT_SUPPORTED                     177
# define EVP_R_OPERATION_NOT_INITIALIZED                  151
# define EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE   150
# define EVP_R_OUTPUT_WOULD_OVERFLOW                      202
# define EVP_R_PARAMETER_TOO_LARGE                        187
# define EVP_R_PARTIALLY_OVERLAPPING                      162
# define EVP_R_PBKDF2_ERROR                               181
# define EVP_R_PKEY_APPLICATION_ASN1_METHOD_ALREADY_REGISTERED 179
# define EVP_R_PRIVATE_KEY_DECODE_ERROR                   145
# define EVP_R_PRIVATE_KEY_ENCODE_ERROR                   146
# define EVP_R_PUBLIC_KEY_NOT_RSA                         106
# define EVP_R_SETTING_XOF_FAILED                         227
# define EVP_R_SET_DEFAULT_PROPERTY_FAILURE               209
# define EVP_R_TOO_MANY_RECORDS                           183
# define EVP_R_UNABLE_TO_ENABLE_LOCKING                   212
# define EVP_R_UNABLE_TO_GET_MAXIMUM_REQUEST_SIZE         215
# define EVP_R_UNABLE_TO_GET_RANDOM_STRENGTH              216
# define EVP_R_UNABLE_TO_LOCK_CONTEXT                     211
# define EVP_R_UNABLE_TO_SET_CALLBACKS                    217
# define EVP_R_UNKNOWN_CIPHER                             160
# define EVP_R_UNKNOWN_DIGEST                             161
# define EVP_R_UNKNOWN_KEY_TYPE                           207
# define EVP_R_UNKNOWN_OPTION                             169
# define EVP_R_UNKNOWN_PBE_ALGORITHM                      121
# define EVP_R_UNSUPPORTED_ALGORITHM                      156
# define EVP_R_UNSUPPORTED_CIPHER                         107
# define EVP_R_UNSUPPORTED_KEYLENGTH                      123
# define EVP_R_UNSUPPORTED_KEY_DERIVATION_FUNCTION        124
# define EVP_R_UNSUPPORTED_KEY_SIZE                       108
# define EVP_R_UNSUPPORTED_KEY_TYPE                       224
# define EVP_R_UNSUPPORTED_NUMBER_OF_ROUNDS               135
# define EVP_R_UNSUPPORTED_PRF                            125
# define EVP_R_UNSUPPORTED_PRIVATE_KEY_ALGORITHM          118
# define EVP_R_UNSUPPORTED_SALT_TYPE                      126
# define EVP_R_UPDATE_ERROR                               189
# define EVP_R_WRAP_MODE_NOT_ALLOWED                      170
# define EVP_R_WRONG_FINAL_BLOCK_LENGTH                   109
# define EVP_R_XTS_DATA_UNIT_IS_TOO_LARGE                 191
# define EVP_R_XTS_DUPLICATED_KEYS                        192

#endif
