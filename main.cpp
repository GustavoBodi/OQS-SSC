#include <iostream>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/asn1.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
//#include <oqs/oqs.h>

int main () {
    // O EVP_PKEY e EVP_PKEY_CTX precisam ser inicializados e o CTX em específico com o algoritmo pós-quantum desejado
    EVP_PKEY *key = EVP_PKEY_new();
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

    if (key == NULL) {
        std::cout << "key_new failed" << std::endl;
        std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
        return 1;
    }

    if (ctx == NULL) {
        std::cout << "ctx_new failed" << std::endl;
        std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
        return 1;
    }

    // Inicializamos o Keygen com o CTX do algoritmo pós-quantum desejado
    if (!EVP_PKEY_keygen_init(ctx)) {
        std::cout << "init failed" << std::endl;
        std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
        return 1;
    }

    // Geramos a chave com o CTX do algoritmo pós-quantum desejado e a chave é carregada em key
    if (!EVP_PKEY_keygen(ctx, &key))
    {
        std::cout << "keygen failed" << std::endl;
        std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
        return 1;
    }

    X509* cert = X509_new();
    X509_set_pubkey(cert, key);
    X509_set_version(cert, 2);
    ASN1_INTEGER_set(X509_get_serialNumber(cert), 0);
    X509_gmtime_adj(X509_get_notBefore(cert), 0);
    X509_gmtime_adj(X509_get_notAfter(cert),(long) 60 * 60 * 24 * 10); // Dez dias

    X509_NAME *issuer=NULL;
    issuer = X509_get_subject_name(cert);

    X509_NAME_add_entry_by_txt(issuer, "C", MBSTRING_ASC, "BR", (int)-1, (int)-1, (int)0);
    X509_NAME_add_entry_by_txt(issuer, "S", MBSTRING_ASC, "SC", -1, -1, 0);
    X509_NAME_add_entry_by_txt(issuer, "L", MBSTRING_ASC, "Florianopolis", -1, -1, 0);
    X509_NAME_add_entry_by_txt(issuer, "O", MBSTRING_ASC, "UFSC", -1, -1, 0);
    X509_NAME_add_entry_by_txt(issuer, "OU", MBSTRING_ASC, "LabSEC", -1, -1, 0);
    X509_NAME_add_entry_by_txt(issuer, "CN", MBSTRING_ASC, "OpenSSL Dilithium2", -1, -1, 0);

    X509_set_issuer_name(cert, issuer);
    X509_set_subject_name(cert, issuer);

    X509_sign(cert, key, EVP_sha512());
    X509_print_fp(stdout, cert);

    if (X509_verify(cert, key)) {
      std::cout << "Correctly verified" << std::endl;
    } else {
      std::cout << "Incorrectly verified" << std::endl;
    }

    return 0;
}

