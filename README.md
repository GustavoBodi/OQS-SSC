# Self Signed Certificate with Post-quantum cryptography
## Description
This program creates, signs and verifies a self signed certificate. It uses the
OQS-provider for interacting with openssl and give support to Post-quantum
cryptograhy. For building it uses CMake and the docker file in the repository
installs (by compiling) all the dependencies.

## How to use
Using it is quite simple, clone the repository and run the docker file:

```bash
git clone https://github.com/GustavoBodi/OQS-SSC/
cd OQS-SSC
docker build -t oqs .
```

## Cleaning
For cleaning after running the program, delete the repository and the docker image
```bash
rm -rf OQS-SSC
docker image rm oqs
```
