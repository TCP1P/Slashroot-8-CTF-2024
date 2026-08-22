# Use OpenJDK 17 as the base image for building
FROM openjdk:17-slim

# Print Java version
RUN java -version

# Install Maven directly from the image
RUN apt update && apt install -y maven binutils --no-install-recommends

# Copy source code and build with Maven
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp/
RUN mvn package

# Set up application directory
ARG APP_DIR=/app
RUN mkdir -p $APP_DIR

# Copy the application JAR to the app directory
RUN cp /usr/src/myapp/target/memento-1.0-SNAPSHOT-jar-with-dependencies.jar $APP_DIR/app.jar

# Copy any additional necessary files
COPY ./flag.txt $APP_DIR/flag.txt

# Expose the port for the application
EXPOSE 8080

# Set the working directory and entry point for running the app
WORKDIR $APP_DIR
# Original final-stage ENTRYPOINT is wrapped by RSCTF at runtime.

# RSCTF dynamic-flag compatibility layer. The build-time scan records
# exact marker-bearing files; runtime replacement is therefore bounded
# to those files instead of walking the image on every team start.
USER root
COPY rsctf-entrypoint.sh /usr/local/bin/rsctf-entrypoint
RUN set -eu; \
    chmod 0555 /usr/local/bin/rsctf-entrypoint; \
    : > /etc/rsctf-flag-targets; \
    for root in /usr/src/myapp /app /src /srv /home /opt /var/www /challenge /chall /ctf /root /usr/src /usr/local/bin /workspace /flag /flag.txt; do \
      if [ -e "$root" ]; then \
        find "$root" -xdev -type f ! -path /usr/local/bin/rsctf-entrypoint -exec grep -lF -e RSCTF_DYNAMIC_FLAG_a9c8ee4becbf73755 -e RSCTF_DYNAMIC_FLAG_070ad {} + >> /etc/rsctf-flag-targets 2>/dev/null || true; \
      fi; \
    done; \
    sort -u /etc/rsctf-flag-targets > /etc/rsctf-flag-targets.sorted; \
    mv /etc/rsctf-flag-targets.sorted /etc/rsctf-flag-targets; \
    while IFS= read -r target; do [ -f "$target" ] && chmod a+w "$target"; done < /etc/rsctf-flag-targets
ENTRYPOINT ["/usr/local/bin/rsctf-entrypoint", "java", "-jar", "./app.jar"]
