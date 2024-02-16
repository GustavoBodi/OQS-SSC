#include <iostream>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/asn1.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/provider.h>

int main () {
		OSSL_PROVIDER* provider = OSSL_PROVIDER_load(OSSL_LIB_CTX_new(), "oqsprovider");
		EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_name(NULL, "dilithium2", NULL);
		EVP_PKEY *key = EVP_PKEY_new();
		int rc;

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

		X509_NAME *issuer = X509_get_subject_name(cert);

		if (! X509_NAME_add_entry_by_txt(issuer, "C", MBSTRING_ASC, reinterpret_cast<const unsigned char*>("BR"), -1, -1, 0)) {
				std::cout << "Problem on country name" << std::endl;
				std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
				return 1;
		}
		if (! X509_NAME_add_entry_by_txt(issuer, "ST", MBSTRING_ASC, reinterpret_cast<const unsigned char*>("SC"), -1, -1, 0)) {
				std::cout << "Problem on state name" << std::endl;
				std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
				return 1;
		}
		if (! X509_NAME_add_entry_by_txt(issuer, "L", MBSTRING_ASC, reinterpret_cast<const unsigned char*>("Florianopolis"), -1, -1, 0)) {
				std::cout << "Problem on city name" << std::endl;
				std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
				return 1;
		}
		if (! X509_NAME_add_entry_by_txt(issuer, "O", MBSTRING_ASC, reinterpret_cast<const unsigned char*>("UFSC"), -1, -1, 0)) {
				std::cout << "Problem on organization name" << std::endl;
				std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
				return 1;
		}
		if (! X509_NAME_add_entry_by_txt(issuer, "OU", MBSTRING_ASC, reinterpret_cast<const unsigned char*>("LabSEC"), -1, -1, 0)) {
				std::cout << "Problem on organization unit name" << std::endl;
				std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
				return 1;
		}
		if (! X509_NAME_add_entry_by_txt(issuer, "CN", MBSTRING_ASC, reinterpret_cast<const unsigned char*>("OpenSSL Dilithium2"), -1, -1, 0)) {
				std::cout << "Problem on CN name" << std::endl;
				std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
				return 1;
		}

		X509_set_issuer_name(cert, issuer);
		X509_set_subject_name(cert, issuer);

		if (! X509_sign(cert, key, EVP_sha1())) {
			std::cout << "Failed on signing signed" << std::endl;
			std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
			exit(1);
		}

		X509_print_fp(stdout, cert);

		rc = X509_verify(cert, key);
		if (rc == 1) {
			std::cout << "Correctly signed" << std::endl;
		} else if (rc == 0) {
			std::cout << "Incorrectly signed" << std::endl;
			std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
		} else {
			std::cout << "Ill formed signature" << std::endl;
			std::cout << ERR_error_string(ERR_get_error(), NULL) << std::endl;
		}

		return 0;
}

