FROM maven:3.9.9-eclipse-temurin-11-alpine AS builder

RUN java -version

RUN apk add --no-cache binutils

RUN jlink \
    --add-modules ALL-MODULE-PATH \
    --output /minimal-jre

COPY . /usr/src/myapp
WORKDIR /usr/src/myapp/
RUN mvn package

FROM alpine:latest
ENV JAVA_HOME=/jre
ENV PATH="${JAVA_HOME}/bin:${PATH}"
RUN apk --no-cache add ca-certificates

ARG USER_NAME="appuser"
ARG USER_ID="1000"
ARG GROUP_NAME="apps"
ARG GROUP_ID="1000"
RUN addgroup --gid $GROUP_ID $GROUP_NAME && \
    adduser --no-create-home --disabled-password --ingroup $GROUP_NAME --uid $USER_ID $USER_NAME

ARG APP_DIR=/app
RUN mkdir $APP_DIR && \
    chown -R $USER_NAME:$GROUP_NAME $APP_DIR
WORKDIR $APP_DIR

COPY --from=builder /minimal-jre $JAVA_HOME

COPY --from=builder --chown=$USER_NAME:$GROUP_NAME /usr/src/myapp/target/memento-1.0* ./app.jar

COPY ./flag.txt /flag.txt

USER $USER_NAME:$GROUP_NAME
EXPOSE 8080
# Original final-stage ENTRYPOINT is wrapped by RSCTF at runtime.

# RSCTF dynamic-flag compatibility layer. The build-time scan records
# exact marker-bearing files; runtime replacement is therefore bounded
# to those files instead of walking the image on every team start.
USER root
COPY rsctf-entrypoint.sh /usr/local/bin/rsctf-entrypoint
RUN set -eu; \
    chmod 0555 /usr/local/bin/rsctf-entrypoint; \
    : > /etc/rsctf-flag-targets; \
    for root in '$APP_DIR/app.jar' /usr/src/myapp /app /src /srv /home /opt /var/www /challenge /chall /ctf /root /usr/src /usr/local/bin /workspace /flag /flag.txt; do \
      if [ -e "$root" ]; then \
        find "$root" -xdev -type f ! -path /usr/local/bin/rsctf-entrypoint -exec grep -lF -e RSCTF_DYNAMIC_FLAG_7788b144854512fd8 -e RSCTF_DYNAMIC_FLAG_7746a {} + >> /etc/rsctf-flag-targets 2>/dev/null || true; \
      fi; \
    done; \
    sort -u /etc/rsctf-flag-targets > /etc/rsctf-flag-targets.sorted; \
    mv /etc/rsctf-flag-targets.sorted /etc/rsctf-flag-targets; \
    while IFS= read -r target; do [ -f "$target" ] && chmod a+w "$target"; done < /etc/rsctf-flag-targets
USER $USER_NAME:$GROUP_NAME
ENTRYPOINT ["/usr/local/bin/rsctf-entrypoint", "java", "-jar", "./app.jar"]
