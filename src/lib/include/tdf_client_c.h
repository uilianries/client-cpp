/*
* Copyright 2022 Virtru Corporation
*
* SPDX - License Identifier: BSD-3-Clause-Clear
*
*/

#ifndef TDF_CLIENT_C_H
#define TDF_CLIENT_C_H

#include "tdf_constants_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Hack to allow .NET P/Invoke to free native memory for a random pointer
/// \param vMemoryPtr - The malloc'd memory to be freed.
DLL_PUBLIC TDF_STATUS TDFFreeMemory(void *memoryPtr);

/// Destruct a credentials instance created with TDFCreateCredentialXXX
DLL_PUBLIC void TDFDestroyCredential(TDFCredsPtr creds);

/// Create a new Credential instance configured for PKI authentication.
/// \param oidcEndpoint - The OIDC server url
/// \param clientId - The client id
/// \param clientKeyFileName - The name of the file containing the client key
/// \param clientCertFileName - The name of the file containing the client certificate
/// \param certificateAuthority - The certificate authority to be used
/// \param organizationName - The OIDC realm or organization the client belongs to
DLL_PUBLIC TDFCredsPtr TDFCreateCredentialPKI(const char *oidcEndpoint,
                                              const char *clientId,
                                              const char *clientKeyFileName,
                                              const char *clientCertFileName,
                                              const char *sdkConsumerCertAuthority,
                                              const char *organizationName);

/// Create a new Credential instance configured for Client Credential authentication.
/// \param oidcEndpoint - The OIDC server url
/// \param clientId - The client id
/// \param clientSecret - The client secret
/// \param organizationName - The OIDC realm or organization the client belongs to
DLL_PUBLIC TDFCredsPtr TDFCreateCredentialClientCreds(const char *oidcEndpoint,
                                                      const char *clientId,
                                                      const char *clientSecret,
                                                      const char *organizationName);

/// Create a new Credential instance configured for Client Credential authentication.
/// \param oidcEndpoint - The OIDC server url
/// \param clientId - The client id
/// \param clientSecret - The client secret
/// \param externalExchangeToken - An external token from the to be exchanged during auth via OIDC Token Exchange
/// \param organizationName - The OIDC realm or organization the client belongs to
DLL_PUBLIC TDFCredsPtr TDFCreateCredentialTokenExchange(const char *oidcEndpoint,
                                                        const char *clientId,
                                                        const char *clientSecret,
                                                        const char *externalExchangeToken,
                                                        const char *organizationName);

/// Create a new TDF client using provided credentials object
/// \param credsPtr - Creds object created by calling TDFCreateCredentialXXX
/// \param kasURL - URL of a key access service
/// NOTE: On failure returns NULL ptr.
DLL_PUBLIC TDFClientPtr TDFCreateClient(TDFCredsPtr credsPtr, const char *kasUrl);

/// Destruct the Virtru client instance.
/// \param clientPtr - The pointer to Virtru client opaque object.
DLL_PUBLIC void TDFDestroyClient(TDFClientPtr clientPtr);

/// Add a data attribute in URL format to the data being encrypted
/// \param dataAttribute - Attribute string in format: "https://example.com/attr/Classification/value/Alpha"
/// \param kasURL - URL of a key access service TODO currently ignored
DLL_PUBLIC TDF_STATUS TDFAddDataAttribute(TDFClientPtr clientPtr, const char *dataAttribute, const char *kasUrl);

/// Enable the internal logger class to write logs to the console for given LogLevel.
/// The default logLevel is 'Warn'
DLL_PUBLIC TDF_STATUS TDFEnableConsoleLogging(TDFClientPtr clientPtr, TDFLogLevel logLevel);

/// Encrypt the contents of the input file into a TDF. In the process of encryption, a policy is
/// associated with the TDF. The policy has a unique id which can be used to identify the TDF policy.
/// \param vClientPtr - The pointer to Virtru client opaque object.
/// \param tdfStorageType - The type of the tdf.
/// \param outFilepath - The file path for tdf after successful encryption
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
DLL_PUBLIC TDF_STATUS TDFEncryptFile(TDFClientPtr clientPtr,  TDFStorageTypePtr storageTypePtr, const char *outFilepath);

/// Decrypt the contents of the TDF file into its original content.
/// \param vClientPtr - The pointer to Virtru client opaque object.
/// \param tdfStorageType - The type of the tdf.
/// \param outFilepath - The file path of the original content after successful decryption
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
DLL_PUBLIC TDF_STATUS TDFDecryptFile(TDFClientPtr clientPtr, TDFStorageTypePtr storageTypePtr, const char *outFilepath);

/// Encrypt the plain data into a TDF. In the process of encryption, a policy is
/// associated with the TDF. The policy has a unique id which can be used to identify the TDF policy.
/// \param vClientPtr - The pointer to Virtru client opaque object.
/// \param tdfStorageType - The type of the tdf.
/// \param outBytesPtr  - On success, it contains the encrypted tdf data.
/// \param outBytesLength  - On success, it is length of the encrypted tdf data.
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
/// NOTE: The caller of the api should free outBytesPtr.
DLL_PUBLIC TDF_STATUS TDFEncryptString(TDFClientPtr clientPtr,
                                       TDFStorageTypePtr storageTypePtr,
                                       TDFBytesPtr *outBytesPtr,
                                       TDFBytesLength *outBytesLength);


/// Gets the JSON policy (as string) of a string-encoded TDF payload
/// \param vClientPtr - The pointer to Virtru client opaque object.
/// \param tdfStorageType - The type of the tdf.
/// \param outBytesPtr  - On success, it contains the TDF policy as a JSON-encoded string.
/// \param outBytesLength  - On success, it is length of the policy string.
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
/// NOTE: The caller of the api should free outBytesPtr.
DLL_PUBLIC TDF_STATUS TDFGetPolicy(TDFClientPtr clientPtr,
                                   TDFStorageTypePtr storageTypePtr,
                                   TDFBytesPtr *outBytesPtr,
                                   TDFBytesLength *outBytesLength);

/// Decrypt the TDF data
/// \param vClientPtr - The pointer to Virtru client opaque object.
/// \param tdfStorageType - The type of the tdf.
/// \param outBytesPtr  - On success, it contains the decrypted tdf data.
/// \param outBytesLength  - On success, it is length of the decrypted tdf data.
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
/// NOTE: The caller of the api should free outBytesPtr.
DLL_PUBLIC TDF_STATUS TDFDecryptString(TDFClientPtr clientPtr,
                                       TDFStorageTypePtr storageTypePtr,
                                       TDFBytesPtr *outBytesPtr,
                                       TDFBytesLength *outBytesLength);

/// \param clientPtr - The pointer to Virtru client opaque object.
/// \param tdfStorageType - The type of the tdf.
/// \param offset - The offset within the plaintext to return
/// \param length - The length of the plaintext to return
/// \param outBytesPtr  - On success, it contains the meta data string.
/// \param outBytesLength  - On success, it is length of the meta data string.
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
/// NOTE: The caller of the api should free outBytesPtr.
DLL_PUBLIC TDF_STATUS TDFDecryptDataPartial(TDFClientPtr clientPtr,
                                            TDFStorageTypePtr storageTypePtr,
                                            TDFBytesLength offset,
                                            TDFBytesLength length,
                                            TDFBytesPtr *outBytesPtr,
                                            TDFBytesLength *outBytesLength);

/// Assign the metadata that will be encrypted and stored in
/// the TDF, separately from the data.
/// \param vClientPtr - The pointer to Virtru client opaque object.
/// \param inBytesPtr  - Pointer to buffer containing the meta data.
/// \param inBytesLength  - Length of buffer containing the meta data.
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
DLL_PUBLIC TDF_STATUS TDFSetEncryptedMetadata(TDFClientPtr clientPtr,
                                       TDFCBytesPtr inBytesPtr,
                                       TDFBytesLength inBytesLength);


/// Decrypt and return TDF metadata as a string. If the TDF content has
/// no encrypted metadata, will return an empty string.
/// \param clientPtr - The pointer to Virtru client opaque object.
/// \param tdfStorageType - The type of the tdf.
/// \param outBytesPtr  - On success, it contains the meta data string.
/// \param outBytesLength  - On success, it is length of the meta data string.
/// \return TDF_STATUS - VSTATUS_SUCCESS on success
/// NOTE: The caller of the api should free outBytesPtr.
DLL_PUBLIC TDF_STATUS TDFGetEncryptedMetadata(TDFClientPtr clientPtr,
                                              TDFStorageTypePtr storageTypePtr,
                                              TDFBytesPtr *outBytesPtr,
                                              TDFBytesLength *outBytesLength);

/// Create a TDF Storage Type instance of type file
/// \param filePath - The path to the file
DLL_PUBLIC TDFStorageTypePtr TDFCreateTDFStorageFileType(const char *filePath);

/// Create a TDF Storage Type instance of type string
/// \param buffer - The buffer for the data
DLL_PUBLIC TDFStorageTypePtr TDFCreateTDFStorageStringType(const char *buffer);

/// Create a TDF Storage Type instance of type S3
/// \param S3Url - https-prefixed URL to the object to be read
/// \param awsAccessKeyID - Access Key ID for the AWS credentials
/// \param awsSecretAccessKey - Secret access key for AWS credentials
/// \param awsRegionName - Region name for AWS credentials
DLL_PUBLIC TDFStorageTypePtr TDFCreateTDFStorageS3Type(const char *S3Url,
                                                       const char *awsAccessKeyId,
                                                       const char *awsSecretAccessKey,
                                                       const char *awsRegionName);

#ifdef __cplusplus
}
#endif

#endif // TDF_CLIENT_C_H
