FROM alpine:latest AS rsctf-flag-builder
RUN apk add --no-cache build-base
COPY rsctf-flag-reader.c /tmp/rsctf-flag-reader.c
RUN cc -static -Os -s -o /execute_me /tmp/rsctf-flag-reader.c

FROM alpine:latest AS build

FROM pwn.red/jail
COPY --from=build --chmod=755 / /srv

COPY --chmod=755 chall /srv/app/run
COPY --chmod=755 execute_me /srv/execute_me

ENV JAIL_MEM=20M
COPY --from=rsctf-flag-builder --chmod=755 /execute_me /srv/execute_me

# RSCTF dynamic-flag compatibility layer. The build-time scan records
# exact marker-bearing files; runtime replacement is therefore bounded
# to those files instead of walking the image on every team start.
USER root
COPY rsctf-entrypoint.sh /usr/local/bin/rsctf-entrypoint
COPY rsctf-prepare-flag.sh /usr/local/bin/rsctf-prepare-flag
RUN set -eu; \
    chmod 0555 /usr/local/bin/rsctf-entrypoint; \
    : > /etc/rsctf-flag-targets; \
    for root in /srv/execute_me /srv/app/run /tmp/rsctf-flag-reader.c /app /src /srv /home /opt /var/www /challenge /chall /ctf /root /usr/src /usr/local/bin /workspace /flag /flag.txt; do \
      if [ -e "$root" ]; then \
        find "$root" -xdev -type f ! -path /usr/local/bin/rsctf-entrypoint -exec grep -lF -e RSCTF_DYNAMIC_FLAG_e4eeefc04c816a6485cd91cdfaa1a885818246e6a1ec00367be -e RSCTF_DYNAMIC_FLAG_36c79d59627b0f4519f0d93ce65ddc007198f12 {} + >> /etc/rsctf-flag-targets 2>/dev/null || true; \
      fi; \
    done; \
    sort -u /etc/rsctf-flag-targets > /etc/rsctf-flag-targets.sorted; \
    mv /etc/rsctf-flag-targets.sorted /etc/rsctf-flag-targets; \
    while IFS= read -r target; do [ -f "$target" ] && chmod a+w "$target"; done < /etc/rsctf-flag-targets; \
    chmod 0555 /usr/local/bin/rsctf-prepare-flag
ENTRYPOINT ["/usr/local/bin/rsctf-entrypoint"]
