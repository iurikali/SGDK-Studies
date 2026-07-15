# Atalhos para o SGDK via Docker
alias sgdk-build='docker run --rm -v "$PWD":/src -u $(id -u):$(id -g) ghcr.io/stephane-d/sgdk:latest release'
alias sgdk-clean='docker run --rm -v "$PWD":/src -u $(id -u):$(id -g) ghcr.io/stephane-d/sgdk:latest clean'
alias sgdk-run='blastem out/rom.bin'